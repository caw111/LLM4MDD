#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// ---- Pin assignments (authoritative; use UNO digital pin numbers) ----
static const uint8_t PIN_BUTTON      = 2;  // D2
static const uint8_t PIN_DHT          = 7;  // D7
static const uint8_t PIN_TM1637_CLK   = 4;  // D4
static const uint8_t PIN_TM1637_DIO   = 5;  // D5

// ---- DHT11 ----
#define DHTTYPE DHT11
static DHT dht(PIN_DHT, DHTTYPE);

// ---- TM1637 ----
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// TM1637 settings (typical)
static const uint8_t TM1637_BRIGHTNESS = 7;

// ---- Serial/menu protocol ----
// Commands accepted from serial lines (line or idle timeout):
//   TEMP  -> display temperature
//   HUMI  -> display humidity
//   BOTH  -> alternate temperature/humidity display
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// Serial output switch controlled by button D2:
// When enabled, print readings/menu status.
static bool serialOutputEnabled = true;

// ---- Display mode ----
enum DisplayMode : uint8_t {
  MODE_TEMP = 0,
  MODE_HUMI  = 1,
  MODE_BOTH  = 2
};
static DisplayMode mode = MODE_BOTH;

// ---- Timing for BOTH mode ----
static const uint32_t BOTH_SWITCH_PERIOD_MS = 2000;
static uint32_t lastBothSwitchMs = 0;
static bool showTempInBoth = true;

// ---- Button debounce ----
static bool lastButtonReading = HIGH;
static bool buttonState = HIGH;
static uint32_t lastDebounceMs = 0;
static const uint32_t DEBOUNCE_MS = 50;

// ---- Serial line buffer ----
static char lineBuf[24];
static uint8_t lineLen = 0;
static uint32_t lastCharMs = 0;

static void resetLine() {
  lineLen = 0;
  lineBuf[0] = '\0';
}

static void handleCommand(const char *cmdRaw) {
  // Make uppercase copy and trim spaces/newlines
  char cmd[24];
  uint8_t i = 0;
  const char *p = cmdRaw;

  // Skip leading whitespace
  while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;

  // Copy up to whitespace/end
  while (*p && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n' && i < sizeof(cmd) - 1) {
    char c = *p++;
    if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
    cmd[i++] = c;
  }
  cmd[i] = '\0';

  if (cmd[0] == '\0') return;

  if (strcmp(cmd, "TEMP") == 0) {
    mode = MODE_TEMP;
  } else if (strcmp(cmd, "HUMI") == 0) {
    mode = MODE_HUMI;
  } else if (strcmp(cmd, "BOTH") == 0) {
    mode = MODE_BOTH;
  }
}

static void pollSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    uint32_t now = millis();
    lastCharMs = now;

    // Build line until newline or buffer full
    if (c == '\n' || c == '\r') {
      if (lineLen > 0) {
        lineBuf[lineLen] = '\0';
        handleCommand(lineBuf);
      }
      resetLine();
      continue;
    }

    // Accept only ASCII printable
    if (c >= 32 && c <= 126) {
      if (lineLen < sizeof(lineBuf) - 1) {
        lineBuf[lineLen++] = c;
        lineBuf[lineLen] = '\0';
      }
    }
  }

  // Idle timeout framing (line_or_idle)
  if (lineLen > 0) {
    uint32_t now = millis();
    if ((now - lastCharMs) > SERIAL_IDLE_TIMEOUT_MS) {
      lineBuf[lineLen] = '\0';
      handleCommand(lineBuf);
      resetLine();
    }
  }
}

// ---- Read sensors and display ----
static float lastTempC = NAN;
static float lastHumPct = NAN;
static uint32_t lastReadMs = 0;
static const uint32_t DHT_READ_PERIOD_MS = 1000;

static void updateReadings() {
  uint32_t now = millis();
  if (now - lastReadMs < DHT_READ_PERIOD_MS) return;
  lastReadMs = now;

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (!isnan(h)) lastHumPct = h;
  if (!isnan(t)) lastTempC = t;
}

// ---- TM1637 helper: render 4-digit xx.x using showNumberDecEx ----
// Map:
// digits: [d0 d1 d2 d3] from left to right. dp (tenths) shown on d2 by dots mask 0x04.
// This matches typical TM1637Display showNumberDecEx dots bit mapping (bit0 = leftmost).
static void showXXDotX(float value) {
  if (isnan(value)) {
    // library in this version doesn't have showString; use numeric placeholder "----" as 8888 with all dots off.
    tm1637.showNumberDecEx(9999, 0, false);
    return;
  }

  bool neg = value < 0.0f;
  if (neg) value = -value;

  long scaled = (long)lround(value * 10.0); // one decimal
  if (scaled > 9999) scaled = 9999;

  // If negative, attempt to use minus on leftmost digit: showNumberDecEx expects an integer.
  // We'll display signed by providing negative value; TM1637Display typically handles it.
  long disp = neg ? -scaled : scaled;

  // Show one decimal point at digit position d2 (mask 0x04)
  // Keep leading zeros allowed? Use false for right alignment? Library signature: showNumberDecEx(num, dots, leadingZero).
  // We'll set leadingZero = false to avoid clutter.
  tm1637.showNumberDecEx((int)disp, 0x04, false);
}

static void showValueOnDisplay(float tempC, float humPct, bool showTemp) {
  float v = showTemp ? tempC : humPct;
  showXXDotX(v);
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  dht.begin();

  tm1637.setBrightness(TM1637_BRIGHTNESS);

  // Show init indicator using methods available in TM1637 library version:
  // No showString/showFloat available, so use numeric indicator.
  tm1637.showNumberDecEx(0, 0, false);

  Serial.begin(SERIAL_BAUD);

  resetLine();
  lastCharMs = millis();
  lastBothSwitchMs = millis();
  lastReadMs = 0;
}

void loop() {
  // --- Poll serial commands (TEMP/HUMI/BOTH) ---
  pollSerial();

  // --- Button toggles serial output enable ---
  uint32_t now = millis();
  bool reading = digitalRead(PIN_BUTTON);

  if (reading != lastButtonReading) {
    lastDebounceMs = now;
    lastButtonReading = reading;
  }

  if ((now - lastDebounceMs) > DEBOUNCE_MS) {
    if (reading != buttonState) {
      buttonState = reading;
      // Detect press: INPUT_PULLUP -> pressed = LOW
      if (buttonState == LOW) {
        serialOutputEnabled = !serialOutputEnabled;

        if (serialOutputEnabled) {
          Serial.print(F("SERIAL_OUTPUT "));
          Serial.println(serialOutputEnabled ? "ON" : "OFF");
        }
      }
    }
  }

  // --- Sensor read and display update ---
  updateReadings();

  // Switch display in BOTH mode
  if (mode == MODE_BOTH) {
    if (millis() - lastBothSwitchMs >= BOTH_SWITCH_PERIOD_MS) {
      lastBothSwitchMs = millis();
      showTempInBoth = !showTempInBoth;
    }
  } else {
    showTempInBoth = (mode == MODE_TEMP);
  }

  showValueOnDisplay(lastTempC, lastHumPct, showTempInBoth);

  // --- Serial output combining local display system ---
  if (serialOutputEnabled) {
    // Print after each DHT update (synchronized by lastReadMs)
    if (millis() - lastReadMs < 50) {
      Serial.print(F("MODE="));
      if (mode == MODE_TEMP) Serial.print(F("TEMP"));
      else if (mode == MODE_HUMI) Serial.print(F("HUMI"));
      else Serial.print(F("BOTH"));

      Serial.print(F(" TEMP_C="));
      if (isnan(lastTempC)) Serial.print(F("NaN"));
      else Serial.print(lastTempC, 1);

      Serial.print(F(" HUMI="));
      if (isnan(lastHumPct)) Serial.print(F("NaN"));
      else Serial.print(lastHumPct, 1);

      Serial.print(F(" SHOW="));
      Serial.println(showTempInBoth ? F("TEMP") : F("HUMI"));
    }
  }
}
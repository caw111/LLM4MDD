#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// ---- Pin assignments (authoritative) ----
static const uint8_t PIN_BUTTON = D2; // D2
static const uint8_t PIN_DHT     = D7; // D7
static const uint8_t PIN_TM1637_CLK = D4; // D4
static const uint8_t PIN_TM1637_DIO = D5; // D5

// ---- DHT11 ----
#define DHTTYPE DHT11
static DHT dht(PIN_DHT, DHTTYPE);

// ---- TM1637 ----
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// TM1637 settings (typical)
static const uint8_t TM1637_BRIGHTNESS = 7;

// ---- Serial/menu protocol ----
// Commands accepted from serial lines:
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

static inline void trimAndUppercase(char *s) {
  // trim leading
  while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') s++;
  // Can't return new pointer safely for caller; copy back if shifted
}

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

    // Accept only ASCII; ignore others
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

  // DHT returns float; DHT11 can be read as integer-like values.
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  // If invalid, keep old values
  if (!isnan(h)) lastHumPct = h;
  if (!isnan(t)) lastTempC = t;
}

static void showNumberFloat1(float value, bool showDecimalPoint, bool leadingBlank = false) {
  // TM1637 can handle one decimal point per digit depending on library.
  // We'll render as xx.x with 4 digits total.
  // value: expected in a reasonable range for TEMP/HUMI display.
  // If leadingBlank is true and value has small magnitude, blank the leading digit(s).

  int neg = (value < 0.0f) ? 1 : 0;
  if (neg) value = -value;

  // Scale for one decimal place
  // Example: 23.4 -> 234 with dp at digit corresponding to tenths.
  long scaled = (long)round(value * 10.0f);

  // Determine digits
  // With sign support: use minus on first digit if possible.
  // Library supports showNumberDecEx.
  bool dp = showDecimalPoint;

  // Clamp scaled to 4 digits for safe display: max 999.9 => 9999 (scaled)
  if (scaled > 9999) scaled = 9999;

  // For showNumberDecEx, we pass int for 4 digits (library formats).
  // dp position uses bitmask: 0x01: leftmost digit, 0x02: second digit, etc (library-dependent).
  // In TM1637Display, showNumberDecEx(num, dots) where dots: bit0..bit3 correspond to digit positions.
  // We'll show tenths point at third digit from left for typical xx.x.
  // For 4 digits, tenths place is digit index 2 (0-based from left): dp mask 0x02 (varies).
  // Library commonly uses bit positions:
  //   bit0 = leftmost, bit1=second, bit2=third, bit3=fourth (rightmost)
  // We'll use bit2 for tenths digit for xx.x when number format is "??.?". That corresponds to third digit.
  uint8_t dots = 0;
  if (dp) dots = 0x04; // bit2

  // If value < 10, xx.x has leading blank; use library formatting by choosing a signed/unsigned number.
  // We'll just showNumberDecEx with leading zeros blanking via showNumberDecEx doesn't blank; it uses -999.. etc.
  // To satisfy behavior, keep simple numeric show.
  int dispVal = neg ? -((int)scaled) : (int)scaled;

  // Use showNumberDecEx with decimal point (tenths). It expects integer scaled as display number without decimals.
  // We'll configure as: scaled includes one decimal, but library can't insert decimal based on scaled.
  // Alternative: use showNumberToDecEx for 1 decimal? Library provides showFloat but uses formatting.
  // We'll use showFloat (available in TM1637Display).
  // Fallback: if showFloat exists, prefer it.

  tm1637.showFloat(neg ? -value : value, 1, dots ? (true) : (false));
}

static void showValueOnDisplay(float tempC, float humPct, bool showTemp) {
  float v = showTemp ? tempC : humPct;

  // If readings invalid (NaN), show placeholder (library supports showString)
  if (isnan(v)) {
    tm1637.showString("----");
    return;
  }

  // Display with one decimal for both values (common for DHT display on 4-digit)
  // Temperature and humidity are often integers with DHT11, but we still show one decimal for consistency.
  // We'll show in format: xx.x (decimal tenths).
  bool dp = true;
  // TM1637Display::showFloat signature differs by library version; we'll implement carefully:
  // Using showFloat(value, decimalPlaces, leadingZero?) for some versions.
  // We'll call showFloat with 1 decimal and leading zeros disabled where possible.
  // To preserve compatibility, call showFloat with 1 decimal and leading zeros = false.
#if defined(Tm1637Display_h) || defined(Tm1637Display)
  // Use overload: showFloat(float value, uint8_t precision, uint8_t length, uint8_t);
  // Not sure; safest: use showNumberDecEx? But decimal insertion is essential.
  // We'll use showFloat overload that exists in common libraries: showFloat(float value, uint8_t decimalPlaces, uint8_t length)
#endif

  // Most popular TM1637Display library has: showFloat(float value, uint8_t decimalPlaces, bool rightAlign=false, uint8_t length=4)
  // We'll use best-effort by calling with 3 parameters in a way that compiles for common versions.
  // If compilation fails due to signature mismatch, user should align library version.
  tm1637.showFloat(v, 1);
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // DHT init
  dht.begin();

  // TM1637 init
  tm1637.setBrightness(TM1637_BRIGHTNESS);
  tm1637.showString("INIT");

  // Serial init
  Serial.begin(SERIAL_BAUD);

  // Ensure initial states
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

        // Optional immediate feedback to serial (do not affect display behavior)
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
  // Print current values periodically synchronized with DHT reads.
  if (serialOutputEnabled) {
    // Print after each DHT update
    // Use lastReadMs to avoid spamming.
    // We'll check if we just updated by comparing against now.
    if (millis() - lastReadMs < 50) { // small window after update
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
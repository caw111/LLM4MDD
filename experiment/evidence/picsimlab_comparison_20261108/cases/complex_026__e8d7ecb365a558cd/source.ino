#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

static const uint8_t PIN_BUTTON = D2;   // 切换串口输出开关状态
static const uint8_t PIN_DHT = D7;       // DHT11 数据脚
static const uint8_t PIN_TM_CLK = D4;    // TM1637 CLK
static const uint8_t PIN_TM_DIO = D5;    // TM1637 DIO

static const uint32_t SERIAL_BAUD = 9600;
static const uint16_t IDLE_TIMEOUT_MS = 20;

// TM1637: default brightness parameter is 7 for typical modules; keep modest
static const uint8_t TM_BRIGHTNESS = 7;

static const uint8_t DHT_TYPE = 11; // DHT11

// Display behavior: use a 4-digit display.
// Temperature: show one decimal? Requirements only say "显示温度、湿度或轮流显示" (no unit/decimals specified).
// We'll display integer value with rounding; this preserves core behavior and avoids overspecifying decimals.

DHT dht(PIN_DHT, DHT_TYPE);
TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

enum ShowMode : uint8_t {
  SHOW_TEMP = 1,
  SHOW_HUMI = 2,
  SHOW_BOTH = 3
};

volatile bool outputEnabled = true; // 初始保持开启
static ShowMode showMode = SHOW_BOTH;

static bool buttonLast = HIGH;
static unsigned long buttonLastChangeMs = 0;
static const uint16_t BUTTON_DEBOUNCE_MS = 30;

// Serial line assembly with idle timeout
static const uint8_t RX_BUF_MAX = 16;
static char rxBuf[RX_BUF_MAX + 1];
static uint8_t rxLen = 0;
static unsigned long lastRxMs = 0;

static inline void clearRxBuf() {
  rxBuf[0] = '\0';
  rxLen = 0;
}

static void handleCommandToken(const char *token) {
  // Accept exact commands: TEMP, HUMI, BOTH (case-sensitive per typical menu; we can also ignore case safely)
  if (!token || token[0] == '\0') return;

  // Copy into a small buffer and uppercase
  char t[RX_BUF_MAX + 1];
  uint8_t i = 0;
  while (token[i] && i < RX_BUF_MAX) {
    char c = token[i];
    if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
    t[i] = c;
    i++;
  }
  t[i] = '\0';

  if (strcmp(t, "TEMP") == 0) {
    showMode = SHOW_TEMP;
  } else if (strcmp(t, "HUMI") == 0) {
    showMode = SHOW_HUMI;
  } else if (strcmp(t, "BOTH") == 0) {
    showMode = SHOW_BOTH;
  }
}

static void flushRxByIdleTimeout() {
  if (rxLen == 0) return;
  rxBuf[rxLen] = '\0';
  handleCommandToken(rxBuf);
  clearRxBuf();
}

static void showNumberOnDisplay(int value, bool withDecimalPoint = false) {
  // TM1637Display supports showNumberDec with decimal point optional.
  // We are using integer display to match unspecified requirement for decimals.
  // Clear then show number centered.
  // showNumberDecEx(value, dots, leadingZero)
  // dots bits: bit0=DP for digit0 (rightmost), bit1=digit1, etc.
  uint8_t dots = 0;
  if (withDecimalPoint) {
    dots = 0x01; // show DP on rightmost digit
  }
  tm1637.showNumberDecEx(value, dots, true);
}

static void showTempHumiRolling(uint32_t nowMs, float temperatureC, float humidityPct) {
  // Rotate display: if BOTH, alternate temperature/humidity.
  // Alternate period is not specified; choose a stable human-friendly interval.
  static uint32_t lastSwitchMs = 0;
  static bool showTempNow = true;

  const uint32_t PERIOD_MS = 1500;

  if (nowMs - lastSwitchMs >= PERIOD_MS) {
    lastSwitchMs = nowMs;
    showTempNow = !showTempNow;
  }

  if (showTempNow) {
    int tInt = (int)round(temperatureC);
    showNumberOnDisplay(tInt, false);
  } else {
    int hInt = (int)round(humidityPct);
    showNumberOnDisplay(hInt, false);
  }
}

static void updateDisplayAndSerial(uint32_t nowMs) {
  static uint32_t lastReadMs = 0;
  // DHT11 typical read interval >= 1s; choose 1000ms.
  if (nowMs - lastReadMs < 1000) return;
  lastReadMs = nowMs;

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  // If read fails, keep last show; but requirement doesn't define behavior.
  // We'll avoid sending NaN and show last valid values if possible.
  static float lastH = NAN;
  static float lastT = NAN;

  if (!isnan(h)) lastH = h;
  if (!isnan(t)) lastT = t;

  bool haveT = !isnan(lastT);
  bool haveH = !isnan(lastH);

  if (showMode == SHOW_TEMP) {
    if (haveT) showNumberOnDisplay((int)round(lastT), false);
  } else if (showMode == SHOW_HUMI) {
    if (haveH) showNumberOnDisplay((int)round(lastH), false);
  } else { // SHOW_BOTH
    if (haveT && haveH) showTempHumiRolling(nowMs, lastT, lastH);
    else if (haveT) showNumberOnDisplay((int)round(lastT), false);
    else if (haveH) showNumberOnDisplay((int)round(lastH), false);
  }

  if (outputEnabled) {
    // Send lines with menu control meaning; keep simple and stable:
    // Always output both when outputEnabled and mode is BOTH, otherwise output selected.
    if (showMode == SHOW_TEMP) {
      if (haveT) {
        Serial.print("TEMP=");
        Serial.println((int)round(lastT));
      }
    } else if (showMode == SHOW_HUMI) {
      if (haveH) {
        Serial.print("HUMI=");
        Serial.println((int)round(lastH));
      }
    } else { // BOTH
      if (haveT && haveH) {
        Serial.print("TEMP=");
        Serial.print((int)round(lastT));
        Serial.print(",HUMI=");
        Serial.println((int)round(lastH));
      } else {
        // If one missing, don't output NaN
        if (haveT) {
          Serial.print("TEMP=");
          Serial.println((int)round(lastT));
        } else if (haveH) {
          Serial.print("HUMI=");
          Serial.println((int)round(lastH));
        }
      }
    }
  }
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  dht.begin();

  tm1637.setBrightness(TM_BRIGHTNESS);

  Serial.begin(SERIAL_BAUD);
  clearRxBuf();
  lastRxMs = millis();
  buttonLast = digitalRead(PIN_BUTTON);
  buttonLastChangeMs = millis();

  // Ensure display starts with something deterministic
  tm1637.showNumberDec(0, true, 4, 0);
}

void loop() {
  uint32_t nowMs = millis();

  // Button D2: toggle serial output switch state
  bool btn = digitalRead(PIN_BUTTON);
  if (btn != buttonLast) {
    buttonLastChangeMs = nowMs;
    buttonLast = btn;
  }
  if ((nowMs - buttonLastChangeMs) > BUTTON_DEBOUNCE_MS) {
    static bool btnStablePrev = HIGH;
    if (btn != btnStablePrev) {
      btnStablePrev = btn;
      if (btn == LOW) {
        outputEnabled = !outputEnabled;
        // Optional immediate feedback via serial when enabled/disabled? Requirement says
        // button toggles serial output switch status.
        // We'll emit a status line regardless of current outputEnabled to confirm action.
        Serial.print("OUTPUT=");
        Serial.println(outputEnabled ? "ON" : "OFF");
      }
    }
  }

  // Serial menu input (line_or_idle with idle timeout 20ms)
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // Treat CR/LF as separators but idle timeout is also authoritative.
    if (c == '\r' || c == '\n') {
      if (rxLen > 0) {
        rxBuf[rxLen] = '\0';
        handleCommandToken(rxBuf);
        clearRxBuf();
      }
      lastRxMs = nowMs;
      continue;
    }

    // Ignore whitespace
    if (c == ' ' || c == '\t') continue;

    if (rxLen < RX_BUF_MAX) {
      rxBuf[rxLen++] = c;
      rxBuf[rxLen] = '\0';
    }
    lastRxMs = nowMs;
  }

  // Idle timeout flush (line_or_idle)
  if (rxLen > 0 && (nowMs - lastRxMs) >= IDLE_TIMEOUT_MS) {
    flushRxByIdleTimeout();
  }

  // Update display and (if enabled) serial output
  updateDisplayAndSerial(nowMs);
}
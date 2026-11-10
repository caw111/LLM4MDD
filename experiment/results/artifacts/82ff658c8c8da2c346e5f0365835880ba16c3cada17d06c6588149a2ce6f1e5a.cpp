#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// Pin mapping (UNO): use numeric pins to avoid 'D2/D4/D5/D7' macro issues.
static const uint8_t PIN_BUTTON = 2;  // D2: 按钮切换串口输出开关状态
static const uint8_t PIN_DHT     = 7;  // D7: DHT11 数据脚
static const uint8_t PIN_TM_CLK  = 4; // D4: TM1637 CLK
static const uint8_t PIN_TM_DIO  = 5; // D5: TM1637 DIO

static const uint32_t SERIAL_BAUD = 9600;
static const uint16_t IDLE_TIMEOUT_MS = 20;

static const uint8_t TM_BRIGHTNESS = 7; // modest brightness

static const uint8_t DHT_TYPE = 11; // DHT11

DHT dht(PIN_DHT, DHT_TYPE);
TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

enum ShowMode : uint8_t {
  SHOW_TEMP = 1,
  SHOW_HUMI = 2,
  SHOW_BOTH = 3
};

volatile bool outputEnabled = true; // 串口输出开关状态（按钮D2切换）
static ShowMode showMode = SHOW_BOTH;

// Button debounce
static bool buttonLast = HIGH;
static unsigned long buttonLastChangeMs = 0;
static const uint16_t BUTTON_DEBOUNCE_MS = 30;

// Serial line assembly with idle timeout
static const uint8_t RX_BUF_MAX = 16;
static char rxBuf[RX_BUF_MAX + 1];
static uint8_t rxLen = 0;
static unsigned long lastRxMs = 0;

#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void clearRxBuf();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleCommandToken(const char *token);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void flushRxByIdleTimeout();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showTempHumiRolling(uint32_t nowMs, float temperatureC, float humidityPct);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateDisplayAndSerial(uint32_t nowMs);
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 178 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline void clearRxBuf() {
  rxBuf[0] = '\0';
  rxLen = 0;
}

static void handleCommandToken(const char *token) {
  if (!token || token[0] == '\0') return;

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
  uint8_t dots = 0;
  if (withDecimalPoint) dots = 0x01; // DP on rightmost digit
  tm1637.showNumberDecEx(value, dots, true);
}

static void showTempHumiRolling(uint32_t nowMs, float temperatureC, float humidityPct) {
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
  if (nowMs - lastReadMs < 1000) return; // DHT11 typical >= 1s
  lastReadMs = nowMs;

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  static float lastH = NAN;
  static float lastT = NAN;

  if (!isnan(h)) lastH = h;
  if (!isnan(t)) lastT = t;

  bool haveT = !isnan(lastT);
  bool haveH = !isnan(lastH);

  // Local display behavior
  if (showMode == SHOW_TEMP) {
    if (haveT) showNumberOnDisplay((int)round(lastT), false);
  } else if (showMode == SHOW_HUMI) {
    if (haveH) showNumberOnDisplay((int)round(lastH), false);
  } else { // SHOW_BOTH
    if (haveT && haveH) showTempHumiRolling(nowMs, lastT, lastH);
    else if (haveT) showNumberOnDisplay((int)round(lastT), false);
    else if (haveH) showNumberOnDisplay((int)round(lastH), false);
  }

  // Serial output behavior controlled by outputEnabled and showMode
  if (outputEnabled) {
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
    } else { // SHOW_BOTH
      if (haveT && haveH) {
        Serial.print("TEMP=");
        Serial.print((int)round(lastT));
        Serial.print(",HUMI=");
        Serial.println((int)round(lastH));
      } else {
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

  // Deterministic startup display
  tm1637.showNumberDec(0, true, 4, 0);
}

void loop() {
  uint32_t nowMs = millis();

  // Button D2: toggle serial output switch status
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
        Serial.print("OUTPUT=");
        Serial.println(outputEnabled ? "ON" : "OFF");
      }
    }
  }

  // Serial menu input: line_or_idle with idle timeout 20ms
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // CR/LF terminate tokens too
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

  // Idle timeout flush
  if (rxLen > 0 && (nowMs - lastRxMs) >= IDLE_TIMEOUT_MS) {
    flushRxByIdleTimeout();
  }

  // Update display and serial output
  updateDisplayAndSerial(nowMs);
}

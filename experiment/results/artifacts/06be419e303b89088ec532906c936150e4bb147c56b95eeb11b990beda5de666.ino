#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// ===== Pin mapping (compileable for Arduino UNO): authoritative remap =====
// D7 -> digital 7, D5 -> digital 5, D4 -> digital 4, D2 -> digital 2
static const uint8_t PIN_DHT = 7;      // DHT11 data
static const uint8_t PIN_FAN = 5;      // Fan PWM output (D5 PWM)
static const uint8_t PIN_TM1637_CLK = 4; // TM1637 CLK
static const uint8_t PIN_TM1637_DIO = 2; // TM1637 DIO

// ===== Device objects =====
DHT dht(PIN_DHT, DHT11);
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ===== Behavior requirements (explicit thresholds/timers left configurable) =====
const float TEMP_THRESHOLD_C = 28.0f;   // 启动阈值：温度高于此值风扇启动
const uint8_t FAN_PWM_ON = 200;          // 风扇启动时PWM占空比（0~255）
const uint8_t FAN_PWM_OFF = 0;          // 风扇关闭PWM占空比（0~255）

// DHT11 typical read rate: >= 1s (avoid excessive polling)
const uint32_t DHT_READ_INTERVAL_MS = 1000;

// Display update interval (can be same as DHT read interval)
const uint32_t DISPLAY_UPDATE_INTERVAL_MS = 1000;

// ===== State =====
float lastTempC = NAN;
uint32_t lastDhtReadMs = 0;
uint32_t lastDisplayUpdateMs = 0;

// Format helper for TM1637 (set to show one decimal place like "28.5")
// TM1637Display::showNumberDecEx supports decimal via dots; we use 1 decimal.
static void showTemperatureOnTM1637(float tempC) {
  if (isnan(tempC)) {
    display.clear();
    return;
  }

  // Clamp to reasonable range for 4-digit display:
  // We'll show as XX.X (up to 99.9). If out of range, show closest.
  bool neg = (tempC < 0.0f);
  float t = tempC;
  if (t > 99.9f) t = 99.9f;
  if (t < -9.9f) t = -9.9f;

  // Convert to tenths
  int16_t tenths = (int16_t)roundf(fabs(t) * 10.0f); // e.g., 28.3 -> 283

  // For XX.X: decimal point after digit 2 (i.e., 28.3).
  // With this library, dot bitmask 0x02 typically maps to position 1 (third digit).
  uint8_t dots = 0;
  dots |= 0x02;

  if (neg) {
    display.showNumberDecEx(-tenths, dots, true);
  } else {
    display.showNumberDecEx(tenths, dots, true);
  }
}

void setup() {
  pinMode(PIN_FAN, OUTPUT);
  analogWrite(PIN_FAN, FAN_PWM_OFF);

  // Serial is not requested (serial: null). No Serial.begin()

  dht.begin();

  // Initialize TM1637 display
  display.setBrightness(0x0f); // full brightness
  display.clear();
}

void loop() {
  uint32_t now = millis();

  // ---- Periodic DHT11 read ----
  if (now - lastDhtReadMs >= DHT_READ_INTERVAL_MS) {
    lastDhtReadMs = now;

    float t = dht.readTemperature(); // Celsius
    if (!isnan(t)) {
      lastTempC = t;
    }
    // If read failed, keep previous lastTempC (preserve behavior without crashing)
  }

  // ---- Periodic display update ----
  if (now - lastDisplayUpdateMs >= DISPLAY_UPDATE_INTERVAL_MS) {
    lastDisplayUpdateMs = now;
    showTemperatureOnTM1637(lastTempC);
  }

  // ---- Fan control based on threshold ----
  // Temperature higher than threshold => fan starts
  if (!isnan(lastTempC) && lastTempC > TEMP_THRESHOLD_C) {
    analogWrite(PIN_FAN, FAN_PWM_ON);
  } else {
    analogWrite(PIN_FAN, FAN_PWM_OFF);
  }
}
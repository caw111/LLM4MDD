#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// ---- Pin mapping (per provided authoritative configuration) ----
static const uint8_t DHT_PIN   = 7;  // D7
static const uint8_t FAN_PWM   = 5;  // D5
static const uint8_t BTN_PIN   = 2;  // D2 (button)
static const uint8_t TM_CLK_PIN = 4; // D4
static const uint8_t TM_DIO_PIN = 3; // D3

// ---- Device objects ----
static DHT dht(DHT_PIN, DHT11);
static TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// ---- Behavior parameters ----
static const float TEMP_THRESHOLD_C = 30.0f;  // temp > 30 opens fan
static const uint8_t FAN_PWM_MIN = 80;           // base speed when above threshold
static const uint8_t FAN_PWM_MAX = 255;          // max speed
static const float  TEMP_FULL_SCALE_C = 40.0f; // at 40C -> max speed

// Display & sampling
static const uint32_t SENSOR_PERIOD_MS = 2000;  // DHT11 reads are slow; use 2s interval
static const uint32_t BTN_DEBOUNCE_MS = 50;

// ---- State ----
static bool showTemperature = true; // 默认显示温度

static float lastTempC = NAN;
static float lastHumRH = NAN;

static uint32_t lastSensorMs = 0;

// Button debounce
static bool lastBtnReading = HIGH;
static bool btnStableState = HIGH;
static uint32_t btnLastChangeMs = 0;

#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t computeFanPWM(float tempC);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void renderValue(float value, bool asTemperature);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t computeFanPWM(float tempC) {
  if (isnan(tempC)) return 0;

  if (tempC <= TEMP_THRESHOLD_C) return 0;

  // Linear ramp: TEMP_THRESHOLD_C -> FAN_PWM_MIN, TEMP_FULL_SCALE_C -> FAN_PWM_MAX
  float span = (TEMP_FULL_SCALE_C - TEMP_THRESHOLD_C);
  if (span <= 0.01f) return FAN_PWM_MAX;

  float t = (tempC - TEMP_THRESHOLD_C) / span; // 0..1+
  if (t < 0.0f) t = 0.0f;
  if (t > 1.0f) t = 1.0f;

  uint8_t pwm = (uint8_t)round(FAN_PWM_MIN + t * (FAN_PWM_MAX - FAN_PWM_MIN));
  if (pwm > FAN_PWM_MAX) pwm = FAN_PWM_MAX;
  return pwm;
}

static void renderValue(float value, bool asTemperature) {
  // TM1637Display shows an integer on 4 digits; represent with rounding.
  // For temperature: show xxC (no decimals). For humidity: show xx% (no decimals).
  // We use simple formatting by placing value and a suffix using display.setSegments as needed
  // but TM1637Display supports showNumberDecEx for decimals / dots.
  // We'll display number and set colon/dots to approximate suffix if needed.
  // Keep behavior deterministic: show rounded value; if temp, light dot to indicate 'C'.
  // (No explicit instruction about suffix/units; requirements only say display temperature or humidity.)
  int16_t iv = (int16_t)round(value);
  if (iv < -999) iv = -999;
  if (iv > 999) iv = 999;

  // Use DP on the last digit as a visual hint for temperature; humidity uses no DP.
  // showNumberDecEx signature: showNumberDecEx(num, dots, leading_zero)
  // dots bitmask: 0x01 for digit0 (rightmost), 0x02 digit1, 0x04 digit2, 0x08 digit3.
  uint8_t dots = 0;
  if (asTemperature) dots = 0x01; // decimal point on rightmost digit

  // leading_zero = false
  display.showNumberDecEx(iv, dots, true);
}

void setup() {
  // DHT setup
  dht.begin();

  // Fan control
  pinMode(FAN_PWM, OUTPUT);
  analogWrite(FAN_PWM, 0);

  // Button input with pullup
  pinMode(BTN_PIN, INPUT_PULLUP);

  // TM1637 display brightness (0..7). Choose a reasonable default.
  display.setBrightness(4);

  // Initial button state
  lastBtnReading = digitalRead(BTN_PIN);
  btnStableState = lastBtnReading;
  btnLastChangeMs = millis();
}

void loop() {
  // --- Handle button with debounce ---
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    lastBtnChangeMs = millis();
    lastBtnReading = reading;
  }

  if ((millis() - btnLastChangeMs) >= BTN_DEBOUNCE_MS) {
    if (reading != btnStableState) {
      btnStableState = reading;

      // Button press detection: HIGH->LOW (INPUT_PULLUP)
      if (btnStableState == LOW) {
        showTemperature = !showTemperature;
      }
    }
  }

  // --- Periodic sensor read ---
  uint32_t now = millis();
  if (now - lastSensorMs >= SENSOR_PERIOD_MS) {
    lastSensorMs = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature(); // Celsius by default

    if (!isnan(t)) lastTempC = t;
    if (!isnan(h)) lastHumRH = h;

    // --- Fan control based on temperature ---
    uint8_t pwm = computeFanPWM(lastTempC);
    analogWrite(FAN_PWM, pwm);

    // --- Update display (default shows temperature; toggles via button) ---
    if (showTemperature) {
      if (!isnan(lastTempC)) {
        renderValue(lastTempC, true);
      }
    } else {
      if (!isnan(lastHumRH)) {
        renderValue(lastHumRH, false);
      }
    }
  }

  // Keep fan updated even if sensor hasn't refreshed recently.
  // (Requirement: when temp > 30 automatically open and increase speed with temp.
  // Actual temp is updated on SENSOR_PERIOD_MS; this maintains correct behavior.)
  // No additional reads to avoid DHT11 overload.
}

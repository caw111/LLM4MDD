#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// ---- Pin mapping (per provided authoritative configuration) ----
static const uint8_t DHT_PIN    = 7; // D7
static const uint8_t FAN_PWM    = 5; // D5 (PWM)
static const uint8_t BTN_PIN    = 2; // D2 (button)
static const uint8_t TM_CLK_PIN = 4; // D4 (CLK)
static const uint8_t TM_DIO_PIN = 3; // D3 (DIO)

// ---- Device objects ----
static DHT dht(DHT_PIN, DHT11);
static TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// ---- Behavior parameters ----
static const float TEMP_THRESHOLD_C = 30.0f; // temp > 30 opens fan
static const uint8_t FAN_PWM_MIN = 80;        // base speed when above threshold
static const uint8_t FAN_PWM_MAX = 255;       // max speed
static const float TEMP_FULL_SCALE_C = 40.0f; // at 40C -> max speed

// Display & sampling
static const uint32_t SENSOR_PERIOD_MS = 2000;  // DHT11 reads are slow; use 2s interval
static const uint32_t BTN_DEBOUNCE_MS  = 50;

// ---- State ----
static bool showTemperature = true; // 默认显示温度

static float lastTempC = NAN;
static float lastHumRH = NAN;

static uint32_t lastSensorMs = 0;

// Button debounce
static bool lastBtnReading = HIGH;
static bool btnStableState = HIGH;
static uint32_t btnLastChangeMs = 0;

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
  int16_t iv = (int16_t)round(value);
  if (iv < -999) iv = -999;
  if (iv > 999) iv = 999;

  // Use DP on the last digit as a visual hint for temperature; humidity uses no DP.
  uint8_t dots = 0;
  if (asTemperature) dots = 0x01; // DP on rightmost digit

  display.showNumberDecEx(iv, dots, true);
}

void setup() {
  dht.begin();

  pinMode(FAN_PWM, OUTPUT);
  analogWrite(FAN_PWM, 0);

  pinMode(BTN_PIN, INPUT_PULLUP);

  display.setBrightness(4);

  lastBtnReading = digitalRead(BTN_PIN);
  btnStableState = lastBtnReading;
  btnLastChangeMs = millis();
}

void loop() {
  // --- Handle button with debounce ---
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    btnLastChangeMs = millis();
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

    // --- Update display ---
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

  // No additional reads to avoid DHT11 overload.
}
#include <Arduino.h>

static constexpr uint8_t PIN_POT1 = A3; // io_0
static constexpr uint8_t PIN_POT2 = A4; // io_1
static constexpr uint8_t PIN_LED_PWM = 2; // io_2 (PWM)

static inline float mapAdcCountToFloat(float adcCount01k) {
  // Model: brightness = io_0 * 255.0 / 1023.0, with guard checks for bounds
  return adcCount01k * 255.0f / 1023.0f;
}

void setup() {
  pinMode(PIN_LED_PWM, OUTPUT);
  analogWrite(PIN_LED_PWM, 0); // Initialization: LED off
}

void loop() {
  // Sample inputs every control period (5ms nominal)
  int pot1Raw = analogRead(PIN_POT1); // 0..1023
  int pot2Raw = analogRead(PIN_POT2); // 0..1023

  // Model inputs as Real (f64 equivalent). Use float; expression matches exactly.
  float io_0 = static_cast<float>(pot1Raw);
  float io_1 = static_cast<float>(pot2Raw);

  // Accepted executable model output expression:
  // io_2 = (io_0 > io_1) ?
  //          (io_0 > 1023.0) ? 255.0 :
  //          (io_0 < 0.0) ? 0.0 :
  //          io_0 * 255.0 / 1023.0
  //        : 0.0
  float io_2;
  if (io_0 > io_1) {
    if (io_0 > 1023.0f) {
      io_2 = 255.0f;
    } else if (io_0 < 0.0f) {
      io_2 = 0.0f;
    } else {
      io_2 = mapAdcCountToFloat(io_0);
    }
  } else {
    io_2 = 0.0f;
  }

  // hardware binding: pwm_output pin D2, scale=1.0
  // Arduino analogWrite expects 0..255 integer.
  int pwm = static_cast<int>(io_2);
  if (pwm < 0) pwm = 0;
  if (pwm > 255) pwm = 255;

  analogWrite(PIN_LED_PWM, pwm);

  delay(5); // control_period_ms = 5
}
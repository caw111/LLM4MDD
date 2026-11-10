#include <Arduino.h>

// Frozen interface contract bindings:
// io_0 -> pot1 (A3)
// io_1 -> pot2 (A4)
// io_2 -> pot3 (A5)
// io_3 -> rgb1 R (D2)  PWM
// io_4 -> rgb1 G (D3)  PWM
// io_5 -> rgb1 B (D4)  PWM

static const uint8_t PIN_POT1 = A3;
static const uint8_t PIN_POT2 = A4;
static const uint8_t PIN_POT3 = A5;

static const uint8_t PIN_RGB_R = 2; // D2
static const uint8_t PIN_RGB_G = 3; // D3
static const uint8_t PIN_RGB_B = 4; // D4

// Control period: 5 ms (control_period_ms = 5)
static const uint32_t CONTROL_PERIOD_US = 5000;

static inline uint16_t readAdc10bit(uint8_t pin) {
  // Arduino analogRead returns 0..1023
  return (uint16_t)analogRead(pin);
}

static inline uint8_t map0to255_exact(double adc_count) {
  // out = io_x * 255.0 / 1023.0  (from model)
  // PWM expects 0..255. Convert with rounding to nearest integer to avoid truncation bias.
  double y = adc_count * 255.0 / 1023.0;
  if (y <= 0.0) return 0;
  if (y >= 255.0) return 255;
  // Round to nearest integer
  long yi = (long)(y + 0.5);
  if (yi < 0) yi = 0;
  if (yi > 255) yi = 255;
  return (uint8_t)yi;
}

void setup() {
  // Initialize outputs to 0 to ensure RGB LED starts off.
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  analogWrite(PIN_RGB_R, 0);
  analogWrite(PIN_RGB_G, 0);
  analogWrite(PIN_RGB_B, 0);

  // Configure analog inputs (optional; analog pins don't require pinMode for Arduino AVR generally).
  pinMode(PIN_POT1, INPUT);
  pinMode(PIN_POT2, INPUT);
  pinMode(PIN_POT3, INPUT);
}

void loop() {
  static uint32_t nextTickUs = micros();

  // Maintain control step timing at 5 ms.
  // Continuous update is satisfied by repeating this fast periodic loop.
  uint32_t nowUs = micros();
  if ((int32_t)(nowUs - nextTickUs) < 0) return;
  nextTickUs += CONTROL_PERIOD_US;

  // Sample inputs (io_0, io_1, io_2) as adc_count in 0..1023.
  double io_0 = (double)readAdc10bit(PIN_POT1);
  double io_1 = (double)readAdc10bit(PIN_POT2);
  double io_2 = (double)readAdc10bit(PIN_POT3);

  // Model outputs:
  // io_3 = io_0 * 255.0 / 1023.0
  // io_4 = io_1 * 255.0 / 1023.0
  // io_5 = io_2 * 255.0 / 1023.0
  // Note: if all inputs are 0, all outputs compute to 0 -> LED remains off.
  uint8_t pwm_r = map0to255_exact(io_0);
  uint8_t pwm_g = map0to255_exact(io_1);
  uint8_t pwm_b = map0to255_exact(io_2);

  // Apply PWM outputs to RGB LED channels.
  analogWrite(PIN_RGB_R, pwm_r);
  analogWrite(PIN_RGB_G, pwm_g);
  analogWrite(PIN_RGB_B, pwm_b);
}
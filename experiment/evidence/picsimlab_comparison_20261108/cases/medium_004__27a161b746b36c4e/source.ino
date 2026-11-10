#include <Arduino.h>

const uint8_t PIN_BTN = 2;   // D2, INPUT_PULLUP
const uint8_t PIN_PWM = 9;   // D9 PWM output
const uint8_t PIN_AIN = A0;  // A0 potentiometer input

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP); // low means pressed
  pinMode(PIN_PWM, OUTPUT);
  analogWrite(PIN_PWM, 0);       // LED off initially
}

void loop() {
  // <20ms responsiveness: simple polling with immediate update
  if (digitalRead(PIN_BTN) == LOW) {
    int raw = analogRead(PIN_AIN); // 0..1023
    // Map linearly to 0..255 with rounding (max error <= 1)
    uint8_t pwm = (uint8_t)((raw * 255 + 511) / 1023);
    analogWrite(PIN_PWM, pwm);
  } else {
    analogWrite(PIN_PWM, 0);
  }
}
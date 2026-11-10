#include <Arduino.h>

void setup() {
  pinMode(2, INPUT_PULLUP); // D2: low means pressed
  pinMode(9, OUTPUT);      // D9: PWM output
  analogWrite(9, 0);       // LED off initially
}

void loop() {
  // Read button (INPUT_PULLUP): LOW when pressed
  if (digitalRead(2) == LOW) {
    // Read potentiometer (0..1023) and map to PWM (0..255)
    int a = analogRead(A0); // 0..1023

    // Linear mapping with rounding to reduce off-by-1 issues:
    // pwm = round(a * 255 / 1023)
    // Use integer math: pwm = (a*255 + 511) / 1023
    uint8_t pwm = (uint8_t)((a * 255L + 511L) / 1023L);

    analogWrite(9, pwm);
  } else {
    analogWrite(9, 0); // Released: output 0
  }
}
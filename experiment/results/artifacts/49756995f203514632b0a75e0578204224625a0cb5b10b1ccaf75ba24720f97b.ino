#include <Arduino.h>

void setup() {
  // D9 used for PWM output (Arduino UNO: pin 9 supports PWM)
  pinMode(9, OUTPUT);
}

void loop() {
  // Brightness from 0 -> 255 then repeat
  for (int value = 0; value <= 255; value++) {
    analogWrite(9, value);
    delay(10); // Smooth time-based change
  }
  // Loop repeats automatically
}
#include <Arduino.h>

void setup() {
  pinMode(9, OUTPUT);
}

void loop() {
  for (int val = 0; val <= 255; val++) {
    analogWrite(9, val);
    delay(10);
  }
}
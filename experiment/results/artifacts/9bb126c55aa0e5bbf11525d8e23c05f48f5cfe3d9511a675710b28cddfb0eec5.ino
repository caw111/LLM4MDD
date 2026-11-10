#include <Arduino.h>

void setup() {
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  digitalWrite(13, analogRead(A0) > 500 ? HIGH : LOW);
}
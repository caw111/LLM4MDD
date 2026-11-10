#include <Arduino.h>

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  int a0 = analogRead(A0);
  if (a0 > 500) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
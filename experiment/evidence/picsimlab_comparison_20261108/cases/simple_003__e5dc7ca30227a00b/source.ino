#include <Arduino.h>

void setup() {
  pinMode(2, INPUT_PULLUP); // D2 button with internal pull-up
  pinMode(13, OUTPUT);     // D13 LED
}

void loop() {
  // Low level means pressed (INPUT_PULLUP)
  if (digitalRead(2) == LOW) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
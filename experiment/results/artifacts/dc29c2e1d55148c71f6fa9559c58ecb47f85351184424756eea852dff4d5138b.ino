#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  static unsigned long lastPrintMs = 0;

  unsigned long now = millis();
  if (now - lastPrintMs >= 2000UL) {
    lastPrintMs = now;
    Serial.println("Hello Arduino");
  }
}
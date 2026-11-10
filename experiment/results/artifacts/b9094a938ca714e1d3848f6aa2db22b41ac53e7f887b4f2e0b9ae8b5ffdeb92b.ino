#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  static unsigned long lastMs = 0;

  unsigned long now = millis();
  if (now - lastMs >= 2000UL) {
    lastMs = now;
    Serial.println("Hello Arduino");
  }
}
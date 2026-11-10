#include <Arduino.h>

const uint8_t LED_PIN = 12;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // initial off
}

void loop() {
  static unsigned long lastToggleMs = 0;
  static bool ledState = false; // currently OFF

  unsigned long now = millis();
  if (now - lastToggleMs >= 500UL) {  // toggle every 500ms
    lastToggleMs = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
}
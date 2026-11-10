#include <Arduino.h>

const uint8_t LED_PIN = 12;
const unsigned long TOGGLE_INTERVAL_MS = 500UL;

unsigned long lastToggleTime;
bool ledState = LOW;

void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  lastToggleTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastToggleTime >= TOGGLE_INTERVAL_MS) {
    lastToggleTime += TOGGLE_INTERVAL_MS;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
}
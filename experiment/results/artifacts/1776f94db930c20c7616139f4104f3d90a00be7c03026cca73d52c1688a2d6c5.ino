#include <Arduino.h>

const uint8_t LED_PIN = 9;
const unsigned long STEP_INTERVAL_MS = 10;

uint8_t brightness = 0;
unsigned long lastUpdate = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, brightness);
  lastUpdate = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastUpdate >= STEP_INTERVAL_MS) {
    lastUpdate = now;
    brightness = (brightness == 255) ? 0 : brightness + 1;
    analogWrite(LED_PIN, brightness);
  }
}
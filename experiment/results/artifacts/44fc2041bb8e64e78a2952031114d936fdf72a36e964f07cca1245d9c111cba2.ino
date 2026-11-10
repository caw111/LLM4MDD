#include <Arduino.h>

const uint8_t LED_PIN = 5;
const uint32_t HALF_PERIOD_MS = 1000;
const uint32_t PERIOD_MS = 2000;

uint32_t previousMillis = 0;
uint32_t phaseMillis = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  previousMillis = millis();
}

void loop() {
  const uint32_t now = millis();
  const uint32_t elapsed = now - previousMillis;

  if (elapsed == 0) {
    return;
  }

  previousMillis = now;
  phaseMillis = (phaseMillis + (elapsed % PERIOD_MS)) % PERIOD_MS;

  const uint32_t rampMillis =
      (phaseMillis <= HALF_PERIOD_MS)
          ? phaseMillis
          : PERIOD_MS - phaseMillis;

  const uint8_t brightness =
      (rampMillis * 255UL + HALF_PERIOD_MS / 2) / HALF_PERIOD_MS;

  analogWrite(LED_PIN, brightness);
}
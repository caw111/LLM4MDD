#include <Arduino.h>

const uint8_t PHOTORESISTOR_PIN = A0;
const uint8_t LED_COUNT = 10;
const uint8_t LED_PINS[LED_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  pinMode(PHOTORESISTOR_PIN, INPUT);
}

void loop() {
  const int sensorValue = analogRead(PHOTORESISTOR_PIN);

  // Lower readings mean brighter light: 1023 -> 0 LEDs, 0 -> 10 LEDs.
  const int litCount = map(sensorValue, 1023, 0, 0, LED_COUNT);

  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], i < litCount ? HIGH : LOW);
  }
}
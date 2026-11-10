#include <Arduino.h>

const uint8_t LED_COUNT = 5;
const uint8_t LED_PINS[LED_COUNT] = {2, 3, 4, 5, 6};
const uint8_t SENSOR_PINS[LED_COUNT] = {A0, A1, A2, A3, A4};
const int THRESHOLD = 500;

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
    pinMode(SENSOR_PINS[i], INPUT);
  }
}

void loop() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    const int sensorValue = analogRead(SENSOR_PINS[i]);

    // The sensor's analog reading increases as brightness decreases.
    digitalWrite(LED_PINS[i], sensorValue > THRESHOLD ? HIGH : LOW);
  }
}
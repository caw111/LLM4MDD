#include <Arduino.h>

const uint8_t LED_COUNT = 10;
const uint8_t LED_PINS[LED_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t POTENTIOMETER_PIN = A0;

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  pinMode(POTENTIOMETER_PIN, INPUT);
}

void loop() {
  const int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  const int illuminatedLEDs = map(potentiometerValue, 0, 1023, 0, LED_COUNT);

  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], i < illuminatedLEDs ? HIGH : LOW);
  }
}
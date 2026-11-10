#include <Arduino.h>

const uint8_t ledPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t threshold = 6;

void setup() {
  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], i < threshold ? HIGH : LOW);
  }
}

void loop() {
  // Outputs retain their states; no further action is required.
}
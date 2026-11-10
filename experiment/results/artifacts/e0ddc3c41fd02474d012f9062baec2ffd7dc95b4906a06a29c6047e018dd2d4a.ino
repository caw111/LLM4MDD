#include <Arduino.h>

static const uint8_t LED_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static const uint8_t PHOTO_PIN = A0;

void setup() {
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT); // set pin2..pin11 as OUTPUT
  }
  pinMode(PHOTO_PIN, INPUT); // set A0 as INPUT
}

void loop() {
  int photoValue = analogRead(PHOTO_PIN); // 1023..0 (smaller = brighter)
  int mappedValue = map(photoValue, 1023, 0, 0, 10); // map to 0..10

  if (mappedValue < 0) mappedValue = 0;
  if (mappedValue > 10) mappedValue = 10;

  // Turn on only LEDs up to the mapped value; others off
  for (uint8_t i = 0; i < 10; i++) {
    uint8_t ledIndex = i + 1; // LED anode pins are numbered 1..10
    digitalWrite(LED_PINS[i], (ledIndex < (uint8_t)mappedValue) ? HIGH : LOW);
  }
}
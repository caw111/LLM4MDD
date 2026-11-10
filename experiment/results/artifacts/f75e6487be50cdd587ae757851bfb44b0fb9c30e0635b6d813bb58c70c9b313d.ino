#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr unsigned long DEBOUNCE_MS = 30;

unsigned long count = 0;
unsigned long lastChangeMs = 0;

uint8_t lastRawState = HIGH;
uint8_t stableState = HIGH;

void setup() {
  // Connect the button between D2 and GND.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  lastRawState = digitalRead(BUTTON_PIN);
  lastChangeMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawState = digitalRead(BUTTON_PIN);

  if (rawState != lastRawState) {
    lastRawState = rawState;
    lastChangeMs = now;
  }

  if (rawState != stableState &&
      (unsigned long)(now - lastChangeMs) >= DEBOUNCE_MS) {
    stableState = rawState;

    // Count only the debounced press, not the release or a held button.
    if (stableState == LOW) {
      ++count;
      Serial.println(count);
    }
  }
}
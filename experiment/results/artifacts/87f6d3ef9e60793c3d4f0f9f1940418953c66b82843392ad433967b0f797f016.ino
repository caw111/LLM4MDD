#include <Arduino.h>

const uint8_t BUTTON_PIN = 2; // D2

static uint32_t counter = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up; button assumed to short to GND when pressed
  Serial.begin(9600);
}

void loop() {
  static uint8_t lastStableState = HIGH;
  static uint8_t lastReading = HIGH;
  static uint32_t lastDebounceMs = 0;

  const uint8_t reading = digitalRead(BUTTON_PIN);
  const uint32_t now = millis();

  if (reading != lastReading) {
    lastDebounceMs = now;
    lastReading = reading;
  }

  const uint16_t debounceDelayMs = 25;
  if ((now - lastDebounceMs) > debounceDelayMs) {
    if (reading != lastStableState) {
      lastStableState = reading;

      // Count on press (transition to LOW because of INPUT_PULLUP)
      if (lastStableState == LOW) {
        counter++;
        Serial.println(counter);
      }
    }
  }
}
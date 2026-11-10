#include <Arduino.h>

// LED order is bottom to top: bargraph A10 through A1.
// The supplied bar graph has ten segments and no A0 terminal.
const uint8_t LED_PINS[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const uint8_t BUTTON_PIN = 12;
const unsigned long DEBOUNCE_MS = 30;

uint8_t litCount = 0;
uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

void updateBarGraph() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], i < litCount ? HIGH : LOW);
  }
}

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  // Button connects pin 12 to ground when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lastButtonChangeMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeMs = now;
  }

  if (now - lastButtonChangeMs >= DEBOUNCE_MS &&
      rawButtonState != stableButtonState) {
    stableButtonState = rawButtonState;

    // Advance only on a debounced press, never while held or on release.
    if (stableButtonState == LOW) {
      // Presses cycle through 2, 4, 6, 8, 10, 0 LEDs.
      litCount = (litCount == LED_COUNT) ? 0 : litCount + 2;
      updateBarGraph();
    }
  }
}
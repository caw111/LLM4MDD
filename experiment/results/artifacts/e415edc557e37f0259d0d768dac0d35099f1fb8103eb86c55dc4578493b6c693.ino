#include <Arduino.h>

const uint8_t RED_PIN = 2;
const uint8_t GREEN_PIN = 3;
const uint8_t BLUE_PIN = 4;
const uint8_t BUTTON_PIN = 5;

const unsigned long DEBOUNCE_MS = 30;

uint8_t colorState = 0;  // 0: off, 1: red, 2: green, 3: blue
int lastButtonReading = HIGH;
int stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;

void updateLED() {
  // Common-cathode RGB LED: HIGH turns a channel on.
  digitalWrite(RED_PIN, colorState == 1 ? HIGH : LOW);
  digitalWrite(GREEN_PIN, colorState == 2 ? HIGH : LOW);
  digitalWrite(BLUE_PIN, colorState == 3 ? HIGH : LOW);
}

void setup() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Button connects pin 5 to GND when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  colorState = 0;
  updateLED();
}

void loop() {
  const unsigned long now = millis();
  const int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = now;
    lastButtonReading = reading;
  }

  if (now - lastDebounceTime >= DEBOUNCE_MS &&
      reading != stableButtonState) {
    stableButtonState = reading;

    // Advance only on a debounced press, not on release or while held.
    if (stableButtonState == LOW) {
      colorState = (colorState + 1) % 4;
      updateLED();
    }
  }
}
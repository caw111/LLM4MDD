#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;
const uint8_t RED_PIN = 3;
const uint8_t GREEN_PIN = 4;
const uint8_t BLUE_PIN = 5;
const uint8_t POT_PIN = A3;

const unsigned long DEBOUNCE_MS = 30;

int8_t selectedChannel = -1;  // -1: off, 0: red, 1: green, 2: blue
uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

void setup() {
  // Common-cathode RGB LED: LOW turns each channel off.
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);

  lastButtonReading = digitalRead(BUTTON_PIN);
  lastButtonChangeMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonReading) {
    lastButtonReading = buttonReading;
    lastButtonChangeMs = now;
  }

  if ((now - lastButtonChangeMs >= DEBOUNCE_MS) &&
      (buttonReading != stableButtonState)) {
    stableButtonState = buttonReading;

    // Advance only on a debounced press, not on release.
    if (stableButtonState == LOW) {
      selectedChannel = (selectedChannel + 1) % 3;

      // Turn off the previous channel before enabling the next.
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
    }
  }

  const uint8_t brightness =
      static_cast<uint8_t>(map(analogRead(POT_PIN), 0, 1023, 0, 255));

  analogWrite(RED_PIN, selectedChannel == 0 ? brightness : 0);
  analogWrite(GREEN_PIN, selectedChannel == 1 ? brightness : 0);
  analogWrite(BLUE_PIN, selectedChannel == 2 ? brightness : 0);
}
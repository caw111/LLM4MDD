#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;
const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;
const uint8_t LED_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

const unsigned long DEBOUNCE_MS = 150;

enum InputSource : uint8_t {
  NONE,
  POT1,
  POT2
};

InputSource activeInput = NONE;

uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeTime = 0;

void setup() {
  // Bargraph anodes A1 through A10 connect to digital pins 3 through 12.
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  activeInput = NONE;
  stableButtonState = HIGH;
  lastRawButtonState = digitalRead(BUTTON_PIN);
  lastButtonChangeTime = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeTime = now;
  }

  // Accept a transition only after the new state persists for 150 ms.
  if (rawButtonState != stableButtonState &&
      (unsigned long)(now - lastButtonChangeTime) >= DEBOUNCE_MS) {
    stableButtonState = rawButtonState;

    // Toggle only on a debounced press, not on release.
    if (stableButtonState == LOW) {
      activeInput = (activeInput == POT1) ? POT2 : POT1;
    }
  }

  uint8_t litLEDs = 0;

  // Keep the display off until the first valid button press.
  if (activeInput != NONE) {
    const uint8_t selectedPin = (activeInput == POT1) ? POT1_PIN : POT2_PIN;
    const int analogValue = analogRead(selectedPin);
    litLEDs = static_cast<uint8_t>(map(analogValue, 0, 1023, 0, 10));
  }

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(LED_PINS[i], (i < litLEDs) ? HIGH : LOW);
  }
}
#include <Arduino.h>

constexpr uint8_t LED_PIN = 3;
constexpr uint8_t BUTTON_PIN = 2;
constexpr unsigned long DEBOUNCE_MS = 30;

bool ledOn = true;
int lastRawButtonState = HIGH;
int stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

void setup() {
  digitalWrite(LED_PIN, HIGH);
  pinMode(LED_PIN, OUTPUT);

  // Button connects pin 2 to ground when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  const unsigned long now = millis();
  const int rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeMs = now;
  }

  if (rawButtonState != stableButtonState &&
      now - lastButtonChangeMs >= DEBOUNCE_MS) {
    stableButtonState = rawButtonState;

    // Toggle only on a debounced press, not on release or while held.
    if (stableButtonState == LOW) {
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }
  }
}
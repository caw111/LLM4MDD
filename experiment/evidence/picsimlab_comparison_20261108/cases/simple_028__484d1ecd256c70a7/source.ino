#include <Arduino.h>

constexpr uint8_t ON_BUTTON_PIN = 2;
constexpr uint8_t OFF_BUTTON_PIN = 3;
constexpr uint8_t LED_PIN = 13;
constexpr unsigned long DEBOUNCE_MS = 25;

struct Button {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChange;
};

Button onButton = {ON_BUTTON_PIN, HIGH, HIGH, 0};
Button offButton = {OFF_BUTTON_PIN, HIGH, HIGH, 0};

bool ledOn = false;

bool wasPressed(Button &button, unsigned long now) {
  const uint8_t reading = digitalRead(button.pin);

  if (reading != button.lastReading) {
    button.lastReading = reading;
    button.lastChange = now;
  }

  if (reading != button.stableState &&
      now - button.lastChange >= DEBOUNCE_MS) {
    button.stableState = reading;
    return button.stableState == LOW;
  }

  return false;
}

void setup() {
  // Connect each button between its input pin and GND.
  pinMode(ON_BUTTON_PIN, INPUT_PULLUP);
  pinMode(OFF_BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  const unsigned long now = millis();
  const bool turnOn = wasPressed(onButton, now);
  const bool turnOff = wasPressed(offButton, now);

  if (turnOn) {
    ledOn = true;
  }
  if (turnOff) {
    ledOn = false;  // OFF takes priority if both presses occur together.
  }

  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
}
#include <Arduino.h>

const uint8_t BUTTON_PIN = 3;   // D3 button input
const uint8_t LED_PIN = 10;     // D10 LED output

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // D3 button (active LOW)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Optional: ensure known stable state
  delay(10);
}

void loop() {
  static bool ledState = false;
  static bool lastButtonPressed = false;

  // Active LOW: pressed when reading LOW
  bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);

  // Detect a rising edge of "pressed" (transition from not pressed to pressed)
  if (buttonPressed && !lastButtonPressed) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }

  lastButtonPressed = buttonPressed;

  // Simple debounce
  delay(15);
}
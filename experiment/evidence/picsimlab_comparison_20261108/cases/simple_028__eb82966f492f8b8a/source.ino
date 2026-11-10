#include <Arduino.h>

const uint8_t LED_PIN = 13; // D13 LED

const uint8_t BTN_ON_PIN  = 2; // D2 button: turn LED ON
const uint8_t BTN_OFF_PIN = 3; // D3 button: turn LED OFF

void setup() {
  // Inputs: use internal pull-ups (buttons expected to connect to GND when pressed)
  pinMode(BTN_ON_PIN, INPUT_PULLUP);
  pinMode(BTN_OFF_PIN, INPUT_PULLUP);

  // Output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // Active-low buttons due to INPUT_PULLUP
  bool btnOnPressed  = (digitalRead(BTN_ON_PIN)  == LOW);
  bool btnOffPressed = (digitalRead(BTN_OFF_PIN) == LOW);

  if (btnOnPressed) {
    digitalWrite(LED_PIN, HIGH); // D2: turn LED ON
  }

  if (btnOffPressed) {
    digitalWrite(LED_PIN, LOW);  // D3: turn LED OFF
  }
}
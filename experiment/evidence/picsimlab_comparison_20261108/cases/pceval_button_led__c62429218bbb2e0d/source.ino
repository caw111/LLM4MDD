#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t LED_PIN = 13;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  // Enable the internal pull-up: the button connects pin 2 to GND.
  digitalWrite(BUTTON_PIN, HIGH);

  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, digitalRead(BUTTON_PIN) == LOW ? HIGH : LOW);
}
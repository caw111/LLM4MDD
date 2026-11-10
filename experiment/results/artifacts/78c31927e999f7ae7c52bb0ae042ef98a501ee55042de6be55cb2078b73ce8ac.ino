#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;
const uint8_t POT_PIN = A0;
const uint8_t LED_PIN = 9;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    const int potValue = analogRead(POT_PIN);
    const int brightness = (potValue * 255L) / 1023;
    analogWrite(LED_PIN, brightness);
  } else {
    analogWrite(LED_PIN, 0);
  }
}
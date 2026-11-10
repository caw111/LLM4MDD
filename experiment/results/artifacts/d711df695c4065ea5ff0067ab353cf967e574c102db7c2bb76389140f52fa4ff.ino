#include <Arduino.h>

const uint8_t BUTTON_PIN = 4;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);  // External 10k ohm pull-up to 5V.
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button not pressed");
  }
}
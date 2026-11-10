#include <Arduino.h>

const uint8_t BUTTON_PIN = 3;
const uint8_t BUZZER_PIN = 6;
const unsigned int BUZZER_FREQUENCY_HZ = 1000;

void setup() {
  // Connect the button between D3 and GND.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}
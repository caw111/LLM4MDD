#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 3;
constexpr uint8_t BUZZER_PIN = 9;
constexpr unsigned int TONE_FREQUENCY_HZ = 262;

void setup() {
  // The existing external resistor pulls the button input HIGH.
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    tone(BUZZER_PIN, TONE_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}
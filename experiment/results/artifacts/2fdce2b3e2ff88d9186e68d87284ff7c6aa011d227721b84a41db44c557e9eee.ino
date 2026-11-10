#include <Arduino.h>

const uint8_t BUZZER_PIN = 6; // D6

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // Buzz ON for 200ms
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);

  // Buzz OFF for the remaining 2s interval (2s - 200ms = 1800ms)
  digitalWrite(BUZZER_PIN, LOW);
  delay(1800);
}
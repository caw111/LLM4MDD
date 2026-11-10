#include <Arduino.h>

constexpr uint8_t BUZZER_PIN = 2;
constexpr unsigned int TONE_FREQUENCY_HZ = 262;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  tone(BUZZER_PIN, TONE_FREQUENCY_HZ);
}
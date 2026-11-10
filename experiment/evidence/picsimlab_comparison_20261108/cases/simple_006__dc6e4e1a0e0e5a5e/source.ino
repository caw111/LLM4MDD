#include <Arduino.h>

constexpr uint8_t BUZZER_PIN = 6;
constexpr unsigned long INTERVAL_MS = 2000;
constexpr unsigned long BEEP_DURATION_MS = 200;
constexpr unsigned int BEEP_FREQUENCY_HZ = 1000;

unsigned long lastBeepTime = 0;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  lastBeepTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastBeepTime >= INTERVAL_MS) {
    lastBeepTime = now;
    tone(BUZZER_PIN, BEEP_FREQUENCY_HZ, BEEP_DURATION_MS);
  }
}
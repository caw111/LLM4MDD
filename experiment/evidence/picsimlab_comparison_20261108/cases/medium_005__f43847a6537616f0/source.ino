#include <Arduino.h>

const uint8_t BUTTON_PIN = 3;  // Button between D3 and GND
const uint8_t BUZZER_PIN = 6;
const unsigned int BUZZER_FREQUENCY_HZ = 1000;
const unsigned long STOP_DELAY_MS = 1000UL;

bool buzzerOn = false;
bool stopPending = false;
unsigned long releaseTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  const unsigned long now = millis();
  const bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if (pressed) {
    stopPending = false;

    if (!buzzerOn) {
      tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
      buzzerOn = true;
    }
  } else if (buzzerOn) {
    if (!stopPending) {
      releaseTime = now;
      stopPending = true;
    }

    if (now - releaseTime >= STOP_DELAY_MS) {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
      buzzerOn = false;
      stopPending = false;
    }
  }
}
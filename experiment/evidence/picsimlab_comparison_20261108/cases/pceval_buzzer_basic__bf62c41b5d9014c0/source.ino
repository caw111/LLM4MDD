#include <Arduino.h>

const uint8_t BUZZER_POS_PIN = 2;   // Connected to Arduino pin2
const uint8_t BUZZER_GND_PIN = 14;  // Arduino Uno GND.1 corresponds to GND; not driven by code

const unsigned int TONE_FREQ_HZ = 262; // 262Hz

void setup() {
  pinMode(BUZZER_POS_PIN, OUTPUT);

  // Continuously play 262Hz tone
  // Use timer-based tone generation for accurate frequency.
  tone(BUZZER_POS_PIN, TONE_FREQ_HZ);
}

void loop() {
  // Keep playing continuously; tone() runs in background.
  // No additional behavior required.
}
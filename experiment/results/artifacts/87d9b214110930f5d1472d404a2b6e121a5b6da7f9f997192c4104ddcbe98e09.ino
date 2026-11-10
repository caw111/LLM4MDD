#include <Arduino.h>

const uint8_t POTENTIOMETER_PIN = A0;
const uint8_t BUZZER_PIN = 6;

const unsigned int MIN_FREQUENCY_HZ = 100;
const unsigned int MAX_FREQUENCY_HZ = 4000;

void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  const int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  const unsigned int frequencyHz = map(
    potentiometerValue, 0, 1023,
    MIN_FREQUENCY_HZ, MAX_FREQUENCY_HZ
  );

  tone(BUZZER_PIN, frequencyHz);
  delay(10);
}
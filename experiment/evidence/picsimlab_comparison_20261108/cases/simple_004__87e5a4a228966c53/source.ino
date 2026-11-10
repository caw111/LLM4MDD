#include <Arduino.h>

const uint8_t POTENTIOMETER_PIN = A0;
const uint8_t LED_PIN = 9;

void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
}

void loop() {
  const int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  const int brightness = map(potentiometerValue, 0, 1023, 0, 255);
  analogWrite(LED_PIN, brightness);
}
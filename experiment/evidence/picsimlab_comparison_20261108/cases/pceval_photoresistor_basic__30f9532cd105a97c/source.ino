#include <Arduino.h>

const uint8_t PHOTORESISTOR_PIN = A2;

void setup() {
  Serial.begin(9600);
  pinMode(PHOTORESISTOR_PIN, INPUT);
}

void loop() {
  const int value = analogRead(PHOTORESISTOR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(value);
  delay(500);
}
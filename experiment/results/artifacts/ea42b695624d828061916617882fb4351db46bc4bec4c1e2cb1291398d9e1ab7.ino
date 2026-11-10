#include <Arduino.h>

const uint8_t LIGHT_SENSOR_PIN = A1;
const uint8_t LED_PIN = 8;
const int LIGHT_THRESHOLD = 500;  // 光强阈值，可根据实际环境调整（0～1023）。

void setup() {
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  const int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  digitalWrite(LED_PIN, lightLevel < LIGHT_THRESHOLD ? HIGH : LOW);
  delay(20);
}
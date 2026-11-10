#include <Arduino.h>

constexpr uint8_t LED_PIN = 2;  // PWM-capable on Arduino Mega 2560
constexpr uint8_t POT1_PIN = A3;
constexpr uint8_t POT2_PIN = A4;

void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
}

void loop() {
  const int brightnessReading = analogRead(POT1_PIN);
  const int thresholdReading = analogRead(POT2_PIN);
  const int brightness = map(brightnessReading, 0, 1023, 0, 255);

  // Compare potentiometers in their shared 0–1023 range.
  analogWrite(LED_PIN,
              brightnessReading > thresholdReading ? brightness : 0);
}
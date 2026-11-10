#include <Arduino.h>

const uint8_t POTENTIOMETER_PIN = A0;
const uint8_t BUTTON_PIN = 2;
const unsigned long REPORT_INTERVAL_MS = 1000UL;

unsigned long lastReportMs = 0;

void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(9600);
  Serial.setTimeout(20);

  lastReportMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastReportMs >= REPORT_INTERVAL_MS) {
    lastReportMs = now;

    const int potentiometerValue = analogRead(POTENTIOMETER_PIN);
    const int buttonState = digitalRead(BUTTON_PIN);

    Serial.print(F("A0="));
    Serial.print(potentiometerValue);
    Serial.print(F(", D2="));
    Serial.println(buttonState);
  }
}
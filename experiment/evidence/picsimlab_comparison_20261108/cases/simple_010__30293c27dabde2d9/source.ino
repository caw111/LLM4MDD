#include <Arduino.h>

const unsigned long OUTPUT_INTERVAL_MS = 1000;
unsigned long lastOutputTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(20);
  pinMode(A0, INPUT);
  lastOutputTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastOutputTime >= OUTPUT_INTERVAL_MS) {
    lastOutputTime = now;
    Serial.println(analogRead(A0));
  }
}
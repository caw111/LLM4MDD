#include <Arduino.h>

const unsigned long PRINT_INTERVAL_MS = 2000UL;
unsigned long lastPrintTime = 0;

void setup() {
  Serial.begin(9600);
  lastPrintTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastPrintTime >= PRINT_INTERVAL_MS) {
    lastPrintTime += PRINT_INTERVAL_MS;
    Serial.println("Hello Arduino");
  }
}
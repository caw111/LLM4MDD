#include <Arduino.h>

const uint8_t LED_PIN = 13;   // D13

// Map A0 reading to blink interval: 0.2s ~ 2.0s
// A0 expected: 0..1023
static unsigned long lastToggleMs = 0;
static bool ledState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int raw = analogRead(A0);  // 0..1023

  // Compute interval in milliseconds from 0.2s..2.0s
  // interval_ms = 200 + raw*(2000-200)/1023
  unsigned long intervalMs = 200UL + (unsigned long)raw * 1800UL / 1023UL;

  unsigned long now = millis();
  if (now - lastToggleMs >= intervalMs) {
    lastToggleMs = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
}
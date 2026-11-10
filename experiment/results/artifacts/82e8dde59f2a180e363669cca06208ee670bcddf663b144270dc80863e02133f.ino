#include <Arduino.h>

const uint8_t LED_PIN = 13; // D13

// A0 controls blink frequency: 0.2~2 seconds
// Requirement: preserve behavior and map A0 value to delay range.
static unsigned long readBlinkDelayMs() {
  int a0 = analogRead(A0); // 0..1023
  // Map 0..1023 to 200..2000 ms
  unsigned long delayMs = (unsigned long)map(a0, 0, 1023, 200, 2000);
  return delayMs;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  unsigned long delayMs = readBlinkDelayMs();

  digitalWrite(LED_PIN, HIGH);
  delay(delayMs);

  digitalWrite(LED_PIN, LOW);
  delay(delayMs);
}
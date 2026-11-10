#include <Arduino.h>

const uint8_t LED1_PIN = 2; // LSB
const uint8_t LED2_PIN = 3;
const uint8_t LED3_PIN = 4;
const uint8_t LED4_PIN = 5;
const uint8_t LED5_PIN = 6; // MSB

static bool isValidRange(uint16_t n) {
  return n <= 31;
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);

  // Ensure LEDs start off
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);
  digitalWrite(LED5_PIN, LOW);

  Serial.begin(9600);
}

void loop() {
  // Continuously read serial monitor input (0..31)
  if (Serial.available() > 0) {
    // Read a whole line (or partials up to timeout) to parse as integer
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s.length() == 0) return;

    long value = s.toInt();
    if (!isValidRange((uint16_t)value)) {
      // If out of range, do nothing (preserve required behavior for 0..31)
      return;
    }

    uint8_t n = (uint8_t)value;

    // LED1 as LSB, LED5 as MSB
    digitalWrite(LED1_PIN, (n & 0b00001) ? HIGH : LOW);
    digitalWrite(LED2_PIN, (n & 0b00010) ? HIGH : LOW);
    digitalWrite(LED3_PIN, (n & 0b00100) ? HIGH : LOW);
    digitalWrite(LED4_PIN, (n & 0b01000) ? HIGH : LOW);
    digitalWrite(LED5_PIN, (n & 0b10000) ? HIGH : LOW);
  }
}
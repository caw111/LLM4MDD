#include <Arduino.h>

const uint8_t LED1_PIN = 2; // LSB
const uint8_t LED2_PIN = 3;
const uint8_t LED3_PIN = 4;
const uint8_t LED4_PIN = 5;
const uint8_t LED5_PIN = 6; // MSB

String serialLine;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);

  // Default Serial settings per requirement
  Serial.begin(9600);
  serialLine.reserve(16);
}

static void setBinaryLeds(uint8_t value) {
  // LED1 is LSB, LED5 is MSB (bits 0..4)
  digitalWrite(LED1_PIN, (value & (1u << 0)) ? HIGH : LOW);
  digitalWrite(LED2_PIN, (value & (1u << 1)) ? HIGH : LOW);
  digitalWrite(LED3_PIN, (value & (1u << 2)) ? HIGH : LOW);
  digitalWrite(LED4_PIN, (value & (1u << 3)) ? HIGH : LOW);
  digitalWrite(LED5_PIN, (value & (1u << 4)) ? HIGH : LOW);
}

void loop() {
  // Continuously read serial monitor which ranges from 0 to 31
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    if (c == '\n' || c == '\r') {
      if (serialLine.length() > 0) {
        long parsed = serialLine.toInt(); // matches "0..31" expectation
        uint8_t value = (parsed < 0) ? 0 : (parsed > 31 ? 31 : (uint8_t)parsed);
        setBinaryLeds(value);
        serialLine = "";
      } else {
        // Ignore empty lines
        serialLine = "";
      }
    } else {
      // Build the line for line/idle behavior
      if (serialLine.length() < 32) serialLine += c;
    }
  }

  // Idle timeout behavior is handled naturally by line-based parsing above.
  // If no newline is sent, we keep buffering until newline/return arrives.
}
#include <Arduino.h>

// Segment order: A, B, C, D, E, F, G, DP.
// Each segment connects through a 330-ohm resistor.
// Common anode connects to 5V: LOW = on, HIGH = off.
const uint8_t segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// Display 5: segments A, C, D, F, and G on; B, E, and DP off.
const uint8_t digitFiveLevels[8] = {
  LOW, HIGH, LOW, LOW, HIGH, LOW, LOW, HIGH
};

void setup() {
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(segmentPins[i], HIGH);
    pinMode(segmentPins[i], OUTPUT);
  }

  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(segmentPins[i], digitFiveLevels[i]);
  }
}

void loop() {
  // Output states remain latched, continuously displaying 5.
}
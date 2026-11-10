#include <Arduino.h>

// Pin mapping from provided wiring (Arduino Mega 2560)
const uint8_t led1Pin = 2;
const uint8_t led2Pin = 3;

const uint8_t barPins[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // A1..A10

// Timing requirements
const unsigned long stepIntervalMs = 2000;

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
  }

  // Initialization: all off
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(barPins[i], LOW);
  }
}

void loop() {
  static uint8_t segmentIndex = 0; // 0..10, where 10 means "fully lit" before reset
  static unsigned long lastChangeMs = 0;
  static bool ledToggle = false;    // alternate LEDs while filling

  unsigned long now = millis();

  // Total animation cycle:
  // - When segmentIndex is 0..9: fill one segment every 2000ms (A1..A10),
  //   and LEDs alternate every 2000ms during filling.
  // - After segmentIndex reaches 10 (all segments lit), reset to off and restart.
  if (segmentIndex < 10) {
    // Ensure bar state matches current segmentIndex:
    // segmentIndex segments ON (A1..A(segmentIndex)), with segmentIndex initially 0 => none on.
    // We'll update outputs only on timed steps per requirement.
    if (lastChangeMs == 0) lastChangeMs = now;

    if (now - lastChangeMs >= stepIntervalMs) {
      lastChangeMs = now;

      // Turn on next bar segment
      digitalWrite(barPins[segmentIndex], HIGH);
      segmentIndex++;

      // Alternate LEDs
      ledToggle = !ledToggle;
      digitalWrite(led1Pin, ledToggle ? HIGH : LOW);
      digitalWrite(led2Pin, ledToggle ? LOW : HIGH);
    }
  } else {
    // After bar graph fully lit: reset bargraph to off, restart animation
    for (uint8_t i = 0; i < 10; i++) {
      digitalWrite(barPins[i], LOW);
    }
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);

    // Restart
    segmentIndex = 0;
    ledToggle = false;
    lastChangeMs = 0;
  }
}
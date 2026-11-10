#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const unsigned long PHASE_TIME_MS = 2000;
static const unsigned long RESET_TIME_MS = 2000;

// Wiring preserved from the provided circuit:
// Servos (PWM): servo1 -> D2, servo2 -> D3, servo3 -> D4
// Bargraph LED A1..A10 -> D5..D13, bargraph common C1..C10 -> GND

// A1..A10 pins: A1..A10 in the circuit correspond to Arduino pins 5..13.
const uint8_t barPins[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

void setBargraphAll(bool on) {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(barPins[i], on ? HIGH : LOW);
  }
}

void setBargraphRange(uint8_t startIdxInclusive, uint8_t endIdxInclusive) {
  // Indices: 0..9 corresponding to A1..A10
  for (uint8_t i = 0; i < 10; i++) {
    bool on = (i >= startIdxInclusive && i <= endIdxInclusive);
    digitalWrite(barPins[i], on ? HIGH : LOW);
  }
}

void setup() {
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);

  // Initialize servos to 0°
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Initialize bargraph pins as outputs and all off
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
  }
  setBargraphAll(false);

  // Small stabilization delay
  delay(50);
}

void loop() {
  // Phase 1
  setBargraphAll(false);
  servo1.write(180);
  setBargraphRange(0, 2); // first 3 segments: A1..A3
  delay(PHASE_TIME_MS);

  // Phase 2
  setBargraphRange(0, 5); // next 3 segments in addition to first: total 6 (A1..A6)
  servo2.write(180);
  delay(PHASE_TIME_MS);

  // Phase 3
  setBargraphRange(0, 9); // all segments on: A1..A10
  servo3.write(180);
  delay(PHASE_TIME_MS);

  // Reset state: all servos return to 0° simultaneously, all LEDs off, wait 2 seconds
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  setBargraphAll(false);
  delay(RESET_TIME_MS);
}
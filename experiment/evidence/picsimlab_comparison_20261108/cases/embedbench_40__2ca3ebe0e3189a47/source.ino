#include <Arduino.h>

// Shift register (sr1: 74HC595) pins (per wiring)
static const uint8_t SR_DS  = A0; // mega:A0 -> sr1:DS
static const uint8_t SR_SHCP = A2; // mega:A2 -> sr1:SHCP
static const uint8_t SR_STCP = A1; // mega:A1 -> sr1:STCP

// Bar graph (bargraph1) pins (per wiring)
// A1..A10 are driven by mega pins 2..11 respectively.
// Each A* has a corresponding common C* tied to GND via mega:GND.1, so C is not GPIO-controlled.
static const uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// 7-seg digit encoding for segments A..G (sevseg1)
// Connections (per wiring): sr1 Q0..Q6 -> sevseg1 A,B,C,D,E,F,G
//
// We assume a typical common-cathode style 7-seg mapping for the simulation:
// HIGH on SR outputs illuminates segments.
// If your hardware is common-anode, invert the patterns.
static const uint8_t DIGIT_TO_SEG_A_TO_G[10] = {
  // 0: A B C D E F
  (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5),
  // 1: B C
  (1u<<1)|(1u<<2),
  // 2: A B D E G
  (1u<<0)|(1u<<1)|(1u<<3)|(1u<<4)|(1u<<6),
  // 3: A B C D G
  (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<6),
  // 4: B C F G
  (1u<<1)|(1u<<2)|(1u<<5)|(1u<<6),
  // 5: A C D F G
  (1u<<0)|(1u<<2)|(1u<<3)|(1u<<5)|(1u<<6),
  // 6: A C D E F G
  (1u<<0)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5)|(1u<<6),
  // 7: A B C
  (1u<<0)|(1u<<1)|(1u<<2),
  // 8: A B C D E F G
  (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5)|(1u<<6),
  // 9: A B C D F G
  (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<5)|(1u<<6)
};

// Shift register outputs are Q0..Q7. We use Q0..Q6 for A..G.
// Q7 is unused; keep it LOW.
static uint8_t digitToShiftByte(uint8_t digit) {
  uint8_t segMask = DIGIT_TO_SEG_A_TO_G[digit]; // bits 0..6 => A..G
  uint8_t shiftByte = 0;
  // Q0 -> A (bit0), Q1 -> B (bit1), ... Q6 -> G (bit6)
  shiftByte |= (segMask & 0x7Fu); // Q0..Q6
  // Q7 remains 0
  return shiftByte;
}

static void srWriteByte(uint8_t value) {
  // SR expects: shift in MSB-first or LSB-first depending on wiring/implementation.
  // We'll implement LSB-first such that the first shifted bit ends up at Q0.
  // This matches the mapping: Q0->A, Q1->B, ... Q6->G.
  digitalWrite(SR_STCP, LOW);
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_DS, (value >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }
  digitalWrite(SR_STCP, HIGH);
}

static void setBarGraph(uint8_t value) {
  // Light exactly 'value' LEDs, order A1..A10
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < value) ? HIGH : LOW);
  }
}

void setup() {
  // Shift register control pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);

  // Bar graph GPIO pins
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
  }

  // Initial state: display "0", bargraph all OFF.
  srWriteByte(digitToShiftByte(0));
  setBarGraph(0);
}

void loop() {
  static uint8_t current = 0;
  static unsigned long lastUpdate = 0;

  unsigned long now = millis();
  if (now - lastUpdate >= 2000UL) {
    lastUpdate = now;

    // Cycle 0..9
    current = (current + 1) % 10;

    // Synchronize both displays
    srWriteByte(digitToShiftByte(current));
    setBarGraph(current);
  }
}
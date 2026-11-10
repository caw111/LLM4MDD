#include <Arduino.h>

// Potentiometer (pot1) -> Mega A3
static const uint8_t PIN_POT = A3;

// Shift register 1 (sr1): controls sevseg1 segments via Q0..Q6 (A..G)
// SR1 control pins (shared with sr2 as per circuit)
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue (sr1 DS)

// Bar graph (bargraph1) is driven directly by Mega pins 2..11 (A1..A10)
static const uint8_t BAR_PINS_A[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // A1..A10

// Segment mapping:
// sr outputs: Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Assume segment ON = 1 for these outputs.
static const uint8_t DIGIT_TO_SEVSEG_BITS[10] = {
  // bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),                 // 0
  (1 << 1) | (1 << 2),                                                                 // 1
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),                              // 2
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),                              // 3
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),                                           // 4
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),                              // 5
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),                  // 6
  (1 << 0) | (1 << 1) | (1 << 2),                                                        // 7
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),   // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)                   // 9
};

static inline void shiftPulse(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(2);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
}

// Write distinct patterns into sr1 and sr2 using the daisy-chain:
// Circuit wiring indicates: sr1 DS is driven by Mega (SR1_DS), sr1 Q7S drives sr2 DS.
// With shared SHCP/STCP, shift 16 bits MSB-first into the cascade:
// - first 8 bits end up in the farthest register (sr2)
// - last 8 bits end up in sr1
static void writeTwoShiftRegisters(uint8_t byte_sr1, uint8_t byte_sr2) {
  uint16_t combined = ((uint16_t)byte_sr2 << 8) | (uint16_t)byte_sr1;

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  for (int i = 15; i >= 0; --i) {
    uint8_t bitVal = (combined >> i) & 0x01;
    digitalWrite(SR1_DS, bitVal ? HIGH : LOW);
    shiftPulse(SR1_SHCP);
  }

  // Latch
  shiftPulse(SR1_STCP);
}

void setup() {
  // Shift register control pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  // Bar graph pins
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS_A[i], OUTPUT);
    digitalWrite(BAR_PINS_A[i], LOW);
  }

  pinMode(PIN_POT, INPUT);

  // Startup requirement:
  // - both 7-segment displays show "00"
  // - sevseg1 shows tens digit and sevseg2 shows units digit
  // - all LED bar graph segments off
  uint8_t tens = 0;
  uint8_t units = 0;
  uint8_t pattern_sr1 = DIGIT_TO_SEVSEG_BITS[tens];  // sevseg1
  uint8_t pattern_sr2 = DIGIT_TO_SEVSEG_BITS[units]; // sevseg2
  writeTwoShiftRegisters(pattern_sr1, pattern_sr2);
}

void loop() {
  // Read potentiometer and map to 0..99
  int raw = analogRead(PIN_POT); // 0..1023
  long value = (long)raw * 99L / 1023L; // 0..99 inclusive
  if (value < 0) value = 0;
  if (value > 99) value = 99;

  // Update two 7-segment displays
  uint8_t tens = (uint8_t)(value / 10);
  uint8_t units = (uint8_t)(value % 10);

  uint8_t pattern_sr1 = DIGIT_TO_SEVSEG_BITS[tens];  // sevseg1: tens
  uint8_t pattern_sr2 = DIGIT_TO_SEVSEG_BITS[units]; // sevseg2: units
  writeTwoShiftRegisters(pattern_sr1, pattern_sr2);

  // Update LED bar graph:
  // 0-9 => 0 LEDs lit, 10-19 => 1, ..., 90-99 => 9.
  // bargraph indices 0..8 correspond to A1..A9 on the provided Mega pins.
  int segmentCount = (int)(value / 10); // 0..9 (works for 0..99 as desired)
  if (segmentCount < 0) segmentCount = 0;
  if (segmentCount > 9) segmentCount = 9;

  for (int i = 0; i < 10; i++) {
    // Only light A1..A9 (indices 0..8). A10 (index 9) must remain off.
    if (i < 9 && i < segmentCount) digitalWrite(BAR_PINS_A[i], HIGH);
    else digitalWrite(BAR_PINS_A[i], LOW);
  }
}
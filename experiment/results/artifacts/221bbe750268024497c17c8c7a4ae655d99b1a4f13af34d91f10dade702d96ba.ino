#include <Arduino.h>

static const uint8_t PIN_POT = A3;

// Shift register 1 (sr1) controls sevseg1 via its outputs Q0..Q7
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue

// Shift register 2 (sr2) controls sevseg2 via its outputs Q0..Q7
static const uint8_t SR2_SHCP = A2; // shared SHCP from sr1 to sr2 (gray)
static const uint8_t SR2_STCP = A1; // shared STCP from sr1 to sr2 (purple)
static const uint8_t SR2_DS   = A0; // sr2 DS from sr1 Q7S (blue). In this circuit DS is wired to sr1:Q7S, so we drive via sr1 only.

// Bar graph (bargraph1) is driven directly by Mega pins 2..11 (A1..A10 alternating with C1..C10 at GND.1)
static const uint8_t BAR_PINS_A[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // A1..A10
// All other C pins are tied to GND.1, so only A pins need to be set HIGH/LOW.

static const uint8_t DIGIT_TO_SEVSEG_BITS[10] = {
  // Map digit 0..9 to bits Q0..Q6 (A,B,C,D,E,F,G).
  // Connections:
  //   Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  //
  // Using common "Arduino-style" segment truth for a 7-seg:
  // 0: a b c d e f        (no g)
  // 1:   b c              (no a,d,e,f,g)
  // 2: a b   d e   g
  // 3: a b c d     g
  // 4:   b c     f g
  // 5: a   c d   f g
  // 6: a   c d e f g
  // 7: a b c             (no d,e,f,g)
  // 8: a b c d e f g
  // 9: a b c d   f g
  //
  // Because sr outputs likely drive segments directly, we represent "segment ON=1" bits.
  // Bit layout: bit0=A(Q0), bit1=B(Q1), bit2=C(Q2), bit3=D(Q3), bit4=E(Q4), bit5=F(Q5), bit6=G(Q6)
  0, // placeholder for index 0
};

static inline void shiftPulse(uint8_t pin) {
  digitalWrite(pin, HIGH);
  // small delay to satisfy shift register timing
  delayMicroseconds(2);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
}

// Shift out one byte to the chain:
// The circuit uses two 74HC595 in series:
//   - sr1 STCP and SHCP control sr1 latch/clock
//   - sr2 shares SHCP/STCP with sr1
//   - sr1 DS is driven by Mega DS line (blue). sr2 DS is driven from sr1 Q7S (internal serial chain).
//
// If we want to load distinct patterns into sr1 and sr2,
// we clock in 16 bits total: first sr1's byte bits appear first, then sr2's.
// With standard 74HC595 cascade: the first 8 bits clocked go to sr2, last 8 bits to sr1 when using a single DS line.
// However, the wiring shows ds->sr1:DS and sr1:Q7S->sr2:DS, meaning it's a standard cascade.
// For a standard cascade where both share the same clock/latch, the typical behavior is:
//   - Bits shifted out first end up in the farthest (sr2), and later bits end up in sr1.
// We'll implement that explicitly by clocking 16 bits where:
static void writeTwoShiftRegisters(uint8_t byte_sr1, uint8_t byte_sr2) {
  // We'll shift 16 bits, MSB first for each byte, in cascade order:
  // First byte to sr2, then byte to sr1:
  //   shift byte_sr2 (8 bits), then shift byte_sr1 (8 bits)
  // DS is SR1_DS (A0).
  uint16_t combined = ((uint16_t)byte_sr2 << 8) | (uint16_t)byte_sr1;

  // Ensure clocks start low
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  // Shift 16 bits MSB-first
  for (int i = 15; i >= 0; --i) {
    uint8_t bitVal = (combined >> i) & 0x01;
    digitalWrite(SR1_DS, bitVal ? HIGH : LOW);
    shiftPulse(SR1_SHCP);
  }

  // Latch both registers
  shiftPulse(SR1_STCP);
}

static uint8_t digitBits(uint8_t d) {
  // Precomputed below in setup using segment mapping constants
  return DIGIT_TO_SEVSEG_BITS[d];
}

void setup() {
  // Segment pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  // Bar graph pins (A1..A10)
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS_A[i], OUTPUT);
  }

  pinMode(PIN_POT, INPUT);

  // Initialize lookup table
  // Segment ON bits: a b c d e f g -> Q0..Q6 bits 0..6
  // a=bit0, b=bit1, c=bit2, d=bit3, e=bit4, f=bit5, g=bit6
  // digit -> bits
  DIGIT_TO_SEVSEG_BITS[0] = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);           // 0
  DIGIT_TO_SEVSEG_BITS[1] = (1<<1)|(1<<2);                                         // 1
  DIGIT_TO_SEVSEG_BITS[2] = (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6);                   // 2
  DIGIT_TO_SEVSEG_BITS[3] = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6);                   // 3
  DIGIT_TO_SEVSEG_BITS[4] = (1<<1)|(1<<2)|(1<<5)|(1<<6);                           // 4
  DIGIT_TO_SEVSEG_BITS[5] = (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6);                   // 5
  DIGIT_TO_SEVSEG_BITS[6] = (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);           // 6
  DIGIT_TO_SEVSEG_BITS[7] = (1<<0)|(1<<1)|(1<<2);                                 // 7
  DIGIT_TO_SEVSEG_BITS[8] = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);   // 8
  DIGIT_TO_SEVSEG_BITS[9] = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6);           // 9

  // Startup requirement:
  // - both 7-segment displays show "00"
  // - sevseg1 shows tens digit, sevseg2 shows units digit
  // - all LED bar graph segments off
  uint8_t tens = 0;
  uint8_t units = 0;

  // sr1 -> sevseg1, sr2 -> sevseg2
  uint8_t pattern_sr1 = digitBits(tens);  // sevseg1
  uint8_t pattern_sr2 = digitBits(units); // sevseg2

  writeTwoShiftRegisters(pattern_sr1, pattern_sr2);

  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS_A[i], LOW);
  }
}

void loop() {
  // Read potentiometer and map to 0..99
  int raw = analogRead(PIN_POT); // 0..1023
  long value = (long)raw * 99L / 1023L; // 0..99 inclusive
  if (value < 0) value = 0;
  if (value > 99) value = 99;

  // Display value on two 7-seg displays
  uint8_t tens = (uint8_t)(value / 10);
  uint8_t units = (uint8_t)(value % 10);

  uint8_t pattern_sr1 = digitBits(tens);
  uint8_t pattern_sr2 = digitBits(units);
  writeTwoShiftRegisters(pattern_sr1, pattern_sr2);

  // Bargraph requirement:
  // 0-9 -> 0 LEDs lit
  // 10-19 -> 1
  // ...
  // 90-99 -> 9 LEDs lit
  // bargraph indices 0-8 must correspond to A1..A9 (we have 10 pins 2..11 for A1..A10).
  // We'll light A1..A9 based on segmentCount (0..9), leaving A10 always off.
  int segmentCount = (int)((value >= 10) ? (value / 10) : 0); // 10..99 => 1..9, 0..9 => 0
  if (segmentCount < 0) segmentCount = 0;
  if (segmentCount > 9) segmentCount = 9;

  for (int i = 0; i < 10; i++) {
    // Light only first 9 LEDs (indices 0..8 mapped to A1..A9 pins 2..10)
    if (i < 9 && i < segmentCount) digitalWrite(BAR_PINS_A[i], HIGH);
    else digitalWrite(BAR_PINS_A[i], LOW);
  }

  // No noticeable delay; keep loop responsive
}
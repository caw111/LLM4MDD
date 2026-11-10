#include <Arduino.h>

// --- Pin mapping based on provided circuit wiring ---
// sr1 (tens digit) 74HC595
static const uint8_t SR1_SHCP = A1; // gray
static const uint8_t SR1_STCP = A0; // purple
static const uint8_t SR1_DS   = A2; // blue

// sr2 (units digit) 74HC595
// SR1_SHCP and SR1_STCP are shared to SR2 via wiring (gray/purple)
static const uint8_t SR2_DS   = A7; // sr1:Q7S -> sr2:DS (no explicit mapping in provided pins)

// Note: In the circuit, sr1's DS/SHCP/STCP are on A2/A1/A0.
// sr1's Q7S is wired to sr2's DS, so we must use the internal chaining by driving SR1.
// However, with only Arduino pins available, we still need to drive SR2's DS input.
// In this circuit, sr2:DS is connected to sr1:Q7S, meaning SR2 receives the shifted bit stream
// automatically from SR1; thus we do NOT need an Arduino pin for SR2_DS.
// Therefore, we'll only implement a single combined shift chain using both chips.
static const uint8_t POT_PIN = A3; // pot1:SIG -> mega:A3

// We'll use a combined 16-bit shift where:
// - Bits shifted into the chain go through sr1 first, then into sr2.
// - With two 74HC595s in series (sr1 output Q7S to sr2 DS), the last bits end up in sr2.
// We'll provide a helper to shift 16 bits and latch.

static const uint8_t NUM_DIGITS = 2;

// 7-seg segments (A..G) are connected to Q0..Q6 of each shift register.
// We'll assume active-HIGH segment outputs (as typical for Wokwi 7segment).
// We'll generate common segment patterns for digits 0-9.
static const uint8_t SEGMENT_MAP[10] = {
  // gfedcba (we will map to Q6..Q0 but easier: create bitmask for Q0..Q6)
  // We'll define bits as: bit0->Q0->segment A, bit1->Q1->segment B, ... bit6->Q6->segment G
  // Digit segments for common 7-seg (A,B,C,D,E,F,G):
  // 0: A B C D E F (no G)
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  0b0111111, // 0: A B C D E F
  0b0000110, // 1: B C
  0b1011011, // 2: A B D E G
  0b1001111, // 3: A B C D G
  0b1100110, // 4: B C F G
  0b1101101, // 5: A C D F G
  0b1111101, // 6: A C D E F G
  0b0000111, // 7: A B C
  0b1111111, // 8: A B C D E F G
  0b1101111  // 9: A B C D F G
};

// Shift-in will send 7 bits per digit (segments A..G) to each 74HC595.
// Q7S is unused (no dot). We'll still shift 8 bits for convenience.
// We'll shift MSB-first or LSB-first consistently with Q0..Q7 mapping.
// Typically, Wokwi's 74HC595 maps shifted bit0 to Q0 when using shiftOut.
// We'll avoid shiftOut and do our own clocking so we can control order.
//
// We'll use: shift MSB-first (bit7->bit0). We'll pack our 8-bit patterns into the chain.
// For each 74HC595:
// - Q0..Q6 correspond to segments A..G (active HIGH).
// - We'll set bit7 (Q7) to 0 (unused).
//
// With 2 chips chained:
// - Bits shifted first go into sr1, then into sr2.
// - After shifting 16 bits and latching, sr1 holds the first 8 bits, sr2 holds the last 8 bits.

static void pulse(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  delayMicroseconds(5);
}

static void set_sr1_sr2_outputs(uint8_t tensDigit, uint8_t unitsDigit, bool validRange) {
  uint8_t segT = 0;
  uint8_t segU = 0;

  if (validRange) {
    segT = SEGMENT_MAP[tensDigit] & 0x7F;
    segU = SEGMENT_MAP[unitsDigit] & 0x7F;
  } else {
    // requirement: outside 0-99 => show "00"
    segT = SEGMENT_MAP[0] & 0x7F;
    segU = SEGMENT_MAP[0] & 0x7F;
  }

  uint8_t byteSr1 = segT; // Q0..Q6, bit7=0
  uint8_t byteSr2 = segU;

  // Drive SHCP (clock) and DS (data) and STCP (latch) for chain.
  // Shared SHCP and STCP between sr1 and sr2, DS is sr1:DS.
  // We'll shift 16 bits MSB-first:
  // First 8 bits go into sr1; last 8 bits go into sr2.
  digitalWrite(SR1_STCP, LOW); // latch low

  // Shift first sr1 byte (MSB..LSB), then sr2 byte (MSB..LSB)
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR1_DS, (byteSr1 >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(5);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(5);
  }
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR1_DS, (byteSr2 >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(5);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(5);
  }

  // Latch outputs
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(SR1_STCP, LOW);
}

void setup() {
  // Configure pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  pinMode(POT_PIN, INPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Initialization: display "00"
  // (validRange = true but digits are 0,0)
  set_sr1_sr2_outputs(0, 0, true);
}

void loop() {
  int raw = analogRead(POT_PIN); // 0..1023

  // Map to 0..99 as requested
  // handle potential out-of-range (should not occur with Arduino's analogRead,
  // but preserve rule)
  long mapped = (long)raw * 99L / 1023L; // yields 0..99

  bool validRange = (mapped >= 0 && mapped <= 99);

  uint8_t tens = validRange ? (uint8_t)(mapped / 10) : 0;
  uint8_t units = validRange ? (uint8_t)(mapped % 10) : 0;

  set_sr1_sr2_outputs(tens, units, validRange);

  // Update dynamically; modest delay to avoid excessive SPI-like toggling.
  delay(10);
}
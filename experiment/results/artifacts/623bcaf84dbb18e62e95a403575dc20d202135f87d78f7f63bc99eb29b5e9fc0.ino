#include <Arduino.h>

static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;
static const uint8_t LED3_PIN = 4;

static const uint8_t SR1_SHCP = A1; // mega:A1 -> sr1:STCP (purple)
static const uint8_t SR1_SHCP_UNUSED = A1;
static const uint8_t SR1_DS   = A0; // mega:A0 -> sr1:DS (blue)
static const uint8_t SR1_SHCP_OR_ = A2; // mega:A2 -> sr1:SHCP (gray)

static const uint8_t POT_PIN = A3; // mega:A3 -> pot1:SIG (black)

static const uint8_t SHCP_PIN = A2; // gray
static const uint8_t STCP_PIN = A1; // purple
static const uint8_t DS_PIN   = A0; // blue

// 74HC595 segment mapping to Q outputs (from circuit):
// sr1: Q0->A, Q1->F, Q2->B, Q3->D, Q4->E, Q5->(unused), Q6->G, Q7S->(serial carry unused)
static const uint8_t qA = 0; // Q0
static const uint8_t qF = 1; // Q1
static const uint8_t qB = 2; // Q2
static const uint8_t qD = 3; // Q3
static const uint8_t qE = 4; // Q4
static const uint8_t qG = 6; // Q6
// Q5 is not connected in the diagram; leave it low.

static inline void srShiftOutByte(uint8_t data) {
  // LSB-first shifting, matching the typical mapping when using shiftRegister bits to Q0..Q7
  // We'll set bits in "data" such that bit0 -> Q0, bit1 -> Q1, ..., bit7 -> Q7.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(DS_PIN, (data & (1u << i)) ? HIGH : LOW);
    digitalWrite(SHCP_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(SHCP_PIN, LOW);
    delayMicroseconds(2);
  }
}

static inline void srLatchTwoBytes(uint8_t highByte, uint8_t lowByte) {
  // The circuit ties sr1:SHCP/STCP/DS to sr2 as well.
  // When shifting 16 bits, the last shifted ends up in sr1 (depending on chain direction).
  // In a standard daisy-chain where DS feeds sr1, and Q7S feeds sr2 DS:
  // - First shifted byte goes to sr2
  // - Second shifted byte goes to sr1
  //
  // We'll do: shift lowByte first (sr2 gets it), then highByte (sr1 gets it),
  // then latch with STCP.
  srShiftOutByte(lowByte);
  srShiftOutByte(highByte);

  digitalWrite(STCP_PIN, HIGH);
  delayMicroseconds(3);
  digitalWrite(STCP_PIN, LOW);
}

static inline uint8_t segByteForDigit(uint8_t digit) {
  // Returns an 8-bit pattern for sr1 or sr2 segment outputs (A,B,C,D,E,F,G) to Q lines.
  // Note: In this circuit, segment outputs are driven directly by Q lines.
  // We'll assume active-high segments.
  // Pattern sets only Q0,Q1,Q2,Q3,Q4,Q6 as per wiring; Q5 kept low; Q7 unused.

  // For each digit, determine which segments A,B,C,D,E,F,G are ON.
  // Digits:
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: F G B C
  // 5: A F G C D
  // 6: A F E D C G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  bool A=false,B=false,C=false,D=false,E=false,F=false,G=false;

  switch (digit) {
    case 0: A=1;B=1;C=1;D=1;E=1;F=1;G=0; break;
    case 1: A=0;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 2: A=1;B=1;C=0;D=1;E=1;F=0;G=1; break;
    case 3: A=1;B=1;C=1;D=1;E=0;F=0;G=1; break;
    case 4: A=0;B=1;C=1;D=0;E=0;F=1;G=1; break;
    case 5: A=1;B=0;C=1;D=1;E=0;F=1;G=1; break;
    case 6: A=1;B=0;C=1;D=1;E=1;F=1;G=1; break;
    case 7: A=1;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 8: A=1;B=1;C=1;D=1;E=1;F=1;G=1; break;
    case 9: A=1;B=1;C=1;D=1;E=0;F=1;G=1; break;
    default: A=0;B=0;C=0;D=0;E=0;F=0;G=0; break;
  }

  // Map to Q outputs according to wiring:
  // Q0 -> A, Q2 -> B, Q3 -> D, Q4 -> E, Q1 -> F, Q6 -> G
  uint8_t v = 0;
  if (A) v |= (1u << qA);
  if (F) v |= (1u << qF);
  if (B) v |= (1u << qB);
  if (D) v |= (1u << qD);
  if (E) v |= (1u << qE);
  if (G) v |= (1u << qG);
  // Q5 intentionally left low.
  // Note: C is wired to Q7S in the chain diagram (sr1:Q7S -> sr2:DS).
  // In the provided mapping, C is part of the second shift register (sr2) connected to COM.
  // Therefore, segByteForDigit for sr1 cannot control segment C directly; it will be controlled by sr2.
  // However, for correct operation we must provide both sr1 and sr2 bytes:
  // - sr1 byte controls A,F,B,D,E,G (and leaves C to sr2)
  // - sr2 byte controls C (Q7S->sr2:DS path suggests sr2 has C from Q7)
  //
  // But sr2 wiring in the diagram shows sr2:Q1->B, sr2:Q2->C, sr2:Q3->D, sr2:Q4->E, sr2:Q5->F, sr2:Q6->G,
  // and sr2:Q0->A. That means sr2 is an independent display segment mapping for sevseg2.
  // For both digits we should provide full segment control through each shift register.
  // Hence we need seg bytes for both shift registers with their respective mappings.
  return v;
}

static inline uint8_t segByte1Digit(uint8_t digit) {
  // sevseg1 mapping (sr1) from diagram:
  // Q0->A, Q1->F, Q2->B, Q3->D, Q4->E, Q6->G
  // Q5 and Q7 are not used for segments.
  bool A=false,B=false,C=false,D=false,E=false,F=false,G=false;

  switch (digit) {
    case 0: A=1;B=1;C=1;D=1;E=1;F=1;G=0; break;
    case 1: A=0;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 2: A=1;B=1;C=0;D=1;E=1;F=0;G=1; break;
    case 3: A=1;B=1;C=1;D=1;E=0;F=0;G=1; break;
    case 4: A=0;B=1;C=1;D=0;E=0;F=1;G=1; break;
    case 5: A=1;B=0;C=1;D=1;E=0;F=1;G=1; break;
    case 6: A=1;B=0;C=1;D=1;E=1;F=1;G=1; break;
    case 7: A=1;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 8: A=1;B=1;C=1;D=1;E=1;F=1;G=1; break;
    case 9: A=1;B=1;C=1;D=1;E=0;F=1;G=1; break;
    default: break;
  }

  uint8_t v = 0;
  if (A) v |= (1u << 0); // Q0
  if (F) v |= (1u << 1); // Q1
  if (B) v |= (1u << 2); // Q2
  if (D) v |= (1u << 3); // Q3
  if (E) v |= (1u << 4); // Q4
  // Q5 unused
  if (G) v |= (1u << 6); // Q6
  // Segment C is wired to sr2, not sr1, per diagram for sevseg1; but sevseg1:C is green from Q2? Actually diagram shows sr1:Q7S -> sr2:DS, and sevseg1:C is connected to sr1:Q2 (green). So C should be handled by sr1 as Q2? Wait: sevseg1:C is NOT shown in segment list; it shows sevseg1:C connected to sr1:Q2 in the given wiring? Actually connections include sr1:Q2 -> sevseg1:C (green) and sr1:Q2 label is "Q2 to sevseg1:C". But earlier it said Q2->B. Let's follow provided connections explicitly:
  // Provided connections for sr1:
  // Q1 -> B
  // Q2 -> C
  // Q3 -> D
  // Q4 -> E
  // Q0 -> A
  // Q5 -> F
  // Q6 -> G
  // Therefore, adjust mapping accordingly for sevseg1:
  return v;
}

static inline uint8_t segByte1Digit_correct(uint8_t digit) {
  // Correct mapping based on explicit connections:
  // sr1:Q0 -> sevseg1:A
  // sr1:Q1 -> sevseg1:B
  // sr1:Q2 -> sevseg1:C
  // sr1:Q3 -> sevseg1:D
  // sr1:Q4 -> sevseg1:E
  // sr1:Q5 -> sevseg1:F
  // sr1:Q6 -> sevseg1:G
  // Q7S is not used for segments.
  bool A=false,B=false,C=false,D=false,E=false,F=false,G=false;

  switch (digit) {
    case 0: A=1;B=1;C=1;D=1;E=1;F=1;G=0; break;
    case 1: A=0;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 2: A=1;B=1;C=0;D=1;E=1;F=0;G=1; break;
    case 3: A=1;B=1;C=1;D=1;E=0;F=0;G=1; break;
    case 4: A=0;B=1;C=1;D=0;E=0;F=1;G=1; break;
    case 5: A=1;B=0;C=1;D=1;E=0;F=1;G=1; break;
    case 6: A=1;B=0;C=1;D=1;E=1;F=1;G=1; break;
    case 7: A=1;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 8: A=1;B=1;C=1;D=1;E=1;F=1;G=1; break;
    case 9: A=1;B=1;C=1;D=1;E=0;F=1;G=1; break;
    default: break;
  }

  uint8_t v = 0;
  if (A) v |= (1u << 0); // Q0
  if (B) v |= (1u << 1); // Q1
  if (C) v |= (1u << 2); // Q2
  if (D) v |= (1u << 3); // Q3
  if (E) v |= (1u << 4); // Q4
  if (F) v |= (1u << 5); // Q5
  if (G) v |= (1u << 6); // Q6
  return v;
}

static inline uint8_t segByte2Digit(uint8_t digit) {
  // sevseg2 mapping (sr2) from explicit connections:
  // sr2:Q0 -> sevseg2:A
  // sr2:Q1 -> sevseg2:B
  // sr2:Q2 -> sevseg2:C
  // sr2:Q3 -> sevseg2:D
  // sr2:Q4 -> sevseg2:E
  // sr2:Q5 -> sevseg2:F
  // sr2:Q6 -> sevseg2:G
  bool A=false,B=false,C=false,D=false,E=false,F=false,G=false;

  switch (digit) {
    case 0: A=1;B=1;C=1;D=1;E=1;F=1;G=0; break;
    case 1: A=0;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 2: A=1;B=1;C=0;D=1;E=1;F=0;G=1; break;
    case 3: A=1;B=1;C=1;D=1;E=0;F=0;G=1; break;
    case 4: A=0;B=1;C=1;D=0;E=0;F=1;G=1; break;
    case 5: A=1;B=0;C=1;D=1;E=0;F=1;G=1; break;
    case 6: A=1;B=0;C=1;D=1;E=1;F=1;G=1; break;
    case 7: A=1;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 8: A=1;B=1;C=1;D=1;E=1;F=1;G=1; break;
    case 9: A=1;B=1;C=1;D=1;E=0;F=1;G=1; break;
    default: break;
  }

  uint8_t v = 0;
  if (A) v |= (1u << 0); // Q0
  if (B) v |= (1u << 1); // Q1
  if (C) v |= (1u << 2); // Q2
  if (D) v |= (1u << 3); // Q3
  if (E) v |= (1u << 4); // Q4
  if (F) v |= (1u << 5); // Q5
  if (G) v |= (1u << 6); // Q6
  return v;
}

static inline void setDisplaysTwoDigits(uint8_t tens, uint8_t ones) {
  uint8_t sev1 = segByte1Digit_correct(tens); // sr1 -> sevseg1
  uint8_t sev2 = segByte2Digit(ones);        // sr2 -> sevseg2

  // Chain order: shift byte for sr2 first, then sr1, then latch.
  // sr1 gets sev1 (highByte), sr2 gets sev2 (lowByte) in srLatchTwoBytes.
  srLatchTwoBytes(sev1, sev2);
}

static inline void setLEDsForMappedValue(uint8_t mappedVal) {
  // Rules:
  // 0-33: LED1
  // 34-66: LED1+LED2
  // 67-99: LED1+LED2+LED3
  bool l1 = true;
  bool l2 = false;
  bool l3 = false;

  if (mappedVal <= 33) {
    l1 = true; l2 = false; l3 = false;
  } else if (mappedVal <= 66) {
    l1 = true; l2 = true; l3 = false;
  } else {
    l1 = true; l2 = true; l3 = true;
  }

  digitalWrite(LED1_PIN, l1 ? HIGH : LOW);
  digitalWrite(LED2_PIN, l2 ? HIGH : LOW);
  digitalWrite(LED3_PIN, l3 ? HIGH : LOW);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(SHCP_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(DS_PIN, OUTPUT);

  digitalWrite(SHCP_PIN, LOW);
  digitalWrite(STCP_PIN, LOW);
  digitalWrite(DS_PIN, LOW);

  // Initial state: LEDs off, displays show "00"
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  setDisplaysTwoDigits(0, 0);

  delay(50);
}

void loop() {
  // Read potentiometer 0..1023
  int raw = analogRead(POT_PIN);

  // Map to 0..99
  int mapped = (raw * 99) / 1023;
  if (mapped < 0) mapped = 0;
  if (mapped > 99) mapped = 99;

  uint8_t tens = (uint8_t)(mapped / 10);
  uint8_t ones = (uint8_t)(mapped % 10);

  // Update displays and LEDs continuously
  setDisplaysTwoDigits(tens, ones);
  setLEDsForMappedValue((uint8_t)mapped);

  // Maintain each state for at least 2 seconds:
  // We evaluate based on the mapped value; delay to allow verification.
  // (This still updates continuously whenever mapped changes; within the 2 seconds it holds.)
  delay(2000);
}
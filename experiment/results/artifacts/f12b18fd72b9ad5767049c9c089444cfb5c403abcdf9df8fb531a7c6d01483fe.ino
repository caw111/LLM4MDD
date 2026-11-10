#include <Arduino.h>

static const uint8_t PIN_SR_DS   = A0; // blue
static const uint8_t PIN_SR_STCP = A1; // purple
static const uint8_t PIN_SR_SHCP = A2; // gray

// Shift register segment mapping (SR outputs Q0..Q7 -> sevseg1 pins A..G and DP)
// From wiring:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G. Q7 unused/DP not wired.
static const uint8_t SEG_BIT_A = 0; // Q0
static const uint8_t SEG_BIT_B = 1; // Q1
static const uint8_t SEG_BIT_C = 2; // Q2
static const uint8_t SEG_BIT_D = 3; // Q3
static const uint8_t SEG_BIT_E = 4; // Q4
static const uint8_t SEG_BIT_F = 5; // Q5
static const uint8_t SEG_BIT_G = 6; // Q6

static const uint8_t PIN_POT1_SIG = A3; // pot1
static const uint8_t PIN_POT2_SIG = A4; // pot2

// 7-seg digit patterns for common behavior with bit=1 meaning "segment ON".
// The wiring in Wokwi typically expects active-high for segments via 74HC595.
// We'll generate patterns for segments A..G only (bit positions 0..6). DP (bit7) left off.
static const uint8_t digitToMask[10] = {
  // 0: A B C D E F
  (1 << SEG_BIT_A) | (1 << SEG_BIT_B) | (1 << SEG_BIT_C) | (1 << SEG_BIT_D) | (1 << SEG_BIT_E) | (1 << SEG_BIT_F),
  // 1: B C
  (1 << SEG_BIT_B) | (1 << SEG_BIT_C),
  // 2: A B D E G
  (1 << SEG_BIT_A) | (1 << SEG_BIT_B) | (1 << SEG_BIT_D) | (1 << SEG_BIT_E) | (1 << SEG_BIT_G),
  // 3: A B C D G
  (1 << SEG_BIT_A) | (1 << SEG_BIT_B) | (1 << SEG_BIT_C) | (1 << SEG_BIT_D) | (1 << SEG_BIT_G),
  // 4: B C F G
  (1 << SEG_BIT_B) | (1 << SEG_BIT_C) | (1 << SEG_BIT_F) | (1 << SEG_BIT_G),
  // 5: A C D F G
  (1 << SEG_BIT_A) | (1 << SEG_BIT_C) | (1 << SEG_BIT_D) | (1 << SEG_BIT_F) | (1 << SEG_BIT_G),
  // 6: A C D E F G
  (1 << SEG_BIT_A) | (1 << SEG_BIT_C) | (1 << SEG_BIT_D) | (1 << SEG_BIT_E) | (1 << SEG_BIT_F) | (1 << SEG_BIT_G),
  // 7: A B C
  (1 << SEG_BIT_A) | (1 << SEG_BIT_B) | (1 << SEG_BIT_C),
  // 8: A B C D E F G
  (1 << SEG_BIT_A) | (1 << SEG_BIT_B) | (1 << SEG_BIT_C) | (1 << SEG_BIT_D) | (1 << SEG_BIT_E) | (1 << SEG_BIT_F) | (1 << SEG_BIT_G),
  // 9: A B C D F G
  (1 << SEG_BIT_A) | (1 << SEG_BIT_B) | (1 << SEG_BIT_C) | (1 << SEG_BIT_D) | (1 << SEG_BIT_F) | (1 << SEG_BIT_G)
};

static void srWriteByte(uint8_t value)
{
  // Shift out MSB->LSB to the 74HC595.
  // Wired: DS=A0, SHCP=A2, STCP=A1.
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(PIN_SR_SHCP, LOW);
    digitalWrite(PIN_SR_DS, (value & (1u << i)) ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
  }
  // Latch outputs
  digitalWrite(PIN_SR_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(PIN_SR_STCP, LOW);
}

static void displayDigit(uint8_t d)
{
  d %= 10;
  uint8_t mask = digitToMask[d]; // bits 0..6 for A..G
  // Ensure Q7 (DP) is off (bit7 = 0)
  srWriteByte(mask & 0x7F);
}

void setup()
{
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);

  // Ensure stable initial state before first latch
  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_STCP, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);

  pinMode(PIN_POT1_SIG, INPUT);
  pinMode(PIN_POT2_SIG, INPUT);

  // Initialization: show "0" on power-up/reset
  displayDigit(0);
}

void loop()
{
  // Read potentiometers (0..1023)
  int pot1 = analogRead(PIN_POT1_SIG);
  int pot2 = analogRead(PIN_POT2_SIG);

  // Map each to 0..9
  int d1 = map(pot1, 0, 1023, 0, 9);
  int d2 = map(pot2, 0, 1023, 0, 9);

  // Sum and show last digit
  int sum = d1 + d2;
  int outDigit = sum % 10;

  displayDigit((uint8_t)outDigit);

  // Continuous update; small delay to reduce flicker/bus saturation
  delay(10);
}
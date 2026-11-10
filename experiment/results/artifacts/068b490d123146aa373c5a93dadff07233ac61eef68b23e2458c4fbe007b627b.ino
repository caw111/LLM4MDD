#include <Arduino.h>

static const int PIN_SR_DS   = A0; // blue
static const int PIN_SR_STCP = A1; // purple
static const int PIN_SR_SHCP = A2; // gray

static const uint8_t DIGIT_TO_7SEG[10] = {
  // A, B, C, D, E, F, G  (bit 0..6)
  0b1111110, // 0
  0b0110000, // 1
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011  // 9
};

// Given circuit wiring to shift register outputs:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// We drive the 74HC595 with the same bit order: Q0..Q6.

static inline void shiftOut7(uint8_t value7bits) {
  // value7bits bit0..6 => Q0..Q6
  // 74HC595 shifts MSB->LSB depending on implementation; we choose an order
  // that preserves mapping: shift out bit6..bit0 so that last shifted ends up on Q0.
  digitalWrite(PIN_SR_STCP, LOW);
  for (int i = 6; i >= 0; --i) {
    digitalWrite(PIN_SR_SHCP, LOW);
    digitalWrite(PIN_SR_DS, (value7bits >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
  }
  digitalWrite(PIN_SR_STCP, HIGH);
  // Pulse to latch already done by STCP high edge; drop it back low.
  digitalWrite(PIN_SR_STCP, LOW);
}

static int mapPotToDigit(int raw) {
  // Map 0..1023 -> 0..9
  long mapped = (raw * 10L) / 1024L;
  if (mapped < 0) mapped = 0;
  if (mapped > 9) mapped = 9;
  return (int)mapped;
}

void setup() {
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);

  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_STCP, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);

  // Initialize to "0" on power-up/reset per requirements.
  shiftOut7(DIGIT_TO_7SEG[0]);
}

void loop() {
  int raw1 = analogRead(A3); // pot1 SIG
  int raw2 = analogRead(A4); // pot2 SIG

  int d1 = mapPotToDigit(raw1);
  int d2 = mapPotToDigit(raw2);

  int sum = d1 + d2;

  // If sum exceeds 9, display last digit of sum (e.g., 12 -> 2)
  int digit = sum % 10;

  shiftOut7(DIGIT_TO_7SEG[digit]);

  // Small delay to reduce jitter while still continuously updating
  delay(25);
}
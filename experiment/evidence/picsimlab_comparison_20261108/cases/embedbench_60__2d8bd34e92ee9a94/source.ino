#include <Arduino.h>

static const uint8_t POT1_PIN = A3; // pot1: controls Red
static const uint8_t POT2_PIN = A4; // pot2: controls Green
static const uint8_t POT3_PIN = A5; // pot3: controls Blue

// 7-segment through 74HC595 shift registers:
// sr1 drives sevseg1 (red+green values), sr2 drives sevseg2 (blue value).
// Wiring from circuit:
static const uint8_t SR1_SHCP = A2; // sr1:SHCP
static const uint8_t SR1_STCP = A1; // sr1:STCP
static const uint8_t SR1_DS   = A0; // sr1:DS (also SR2:DS via sr1:Q7S)

static const uint8_t SR2_SHCP = A2; // shared with sr1 by wiring
static const uint8_t SR2_STCP = A1; // shared with sr1 by wiring
// DS is driven by sr1:DS only; no separate DS pin used for sr2.

static const uint8_t MR_SR1 = 0xFF; // not wired to MCU (tied to VCC/ground in circuit)
static const uint8_t MR_SR2 = 0xFF; // not wired to MCU
// OE is tied low (black) -> always enabled in the circuit.

static inline uint8_t map0_1023_to_0_255(int v) {
  if (v < 0) v = 0;
  if (v > 1023) v = 1023;
  return (uint8_t)((uint32_t)v * 255UL / 1023UL);
}

// Requirement: sevseg1 shows "red value map 0-255 to 0-9".
// sevseg2 shows "blue value map 0-255 to 0-9".
static inline uint8_t map0_255_to_0_9(uint8_t v255) {
  // Map linearly and clamp.
  // v in [0..255] -> [0..9]
  return (uint8_t)((uint32_t)v255 * 9UL / 255UL);
}

// We will shift out segment patterns for digits 0..9.
// The circuit connects:
// sr1 Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7S cascades to sr2 DS.
//
// "green" connected to each segment pin implies segments are driven by Q outputs.
// With "common cathode" RGB LEDs for RGB, but 7seg doesn't mention common cathode/anode;
// typical wokwi 7segment uses common cathode? We'll assume the 7segment expects "1 = segment ON"
// for each segment output bit, matching standard 7segment segment truth for shift-register style.
//
// If your hardware inverts polarity, you'll need invert patterns; but requirement says preserve wiring/behavior.
// We'll use conventional active-high segment enable patterns.
static const uint8_t DIGIT_TO_SEG[10] = {
  // pgfedcba mapping assumed by wiring: Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  // so bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G, bit7=unused.
  // Digit segments: 0: a b c d e f (not g)
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5),                         // 0
  (1<<1) | (1<<2),                                                               // 1
  (1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<6),                                  // 2: a b d e g
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<6),                                  // 3: a b c d g
  (1<<1) | (1<<2) | (1<<5) | (1<<6) ,                                          // 4: b c f g
  (1<<0) | (1<<2) | (1<<3) | (1<<5) | (1<<6),                                  // 5: a c d f g
  (1<<0) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6),                         // 6: a c d e f g
  (1<<0) | (1<<1) | (1<<2),                                                     // 7: a b c
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6),             // 8: a b c d e f g
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<5) | (1<<6)                          // 9: a b c d f g
};

static inline void shiftOutTwo595(uint8_t sr1SegBits, uint8_t sr2SegBits) {
  // We have two 74HC595 daisy-chained:
  // - sr1:DS is driven by MCU and its Q7S feeds sr2:DS via wiring.
  // On rising SHCP clock, data shifts through.
  // To update both registers correctly on STCP:
  //   - shift out sr2 first, then sr1, so that after 8 clocks:
  //     last bits shifted into sr1 become its outputs.
  //
  // Bit ordering: shift register outputs Q0..Q7 correspond to shifted bits.
  // In most Arduino shiftOut, MSB-first/LSB-first is configurable.
  // Here we will implement LSB-first with bit0 first to align with Q0->A.
  //
  // We'll clock 8 bits per register, with bit7 unused (set 0).
  auto clockPulseSHCP = []() {
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, LOW);
  };

  // sr2 first
  for (int i = 0; i < 8; i++) {
    uint8_t bitVal = (sr2SegBits >> i) & 0x01;
    digitalWrite(SR1_DS, bitVal ? HIGH : LOW);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(1);
  }

  // then sr1
  for (int i = 0; i < 8; i++) {
    uint8_t bitVal = (sr1SegBits >> i) & 0x01;
    digitalWrite(SR1_DS, bitVal ? HIGH : LOW);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(1);
  }

  // Latch: STCP
  digitalWrite(SR1_STCP, LOW);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, LOW);
}

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);

  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  // Initialize control lines low.
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);
}

void loop() {
  int rawR = analogRead(POT1_PIN);
  int rawG = analogRead(POT2_PIN);
  int rawB = analogRead(POT3_PIN);

  uint8_t r255 = map0_1023_to_0_255(rawR);
  uint8_t g255 = map0_1023_to_0_255(rawG);
  uint8_t b255 = map0_1023_to_0_255(rawB);

  // Sevseg1: show red mapped to 0-9 (per rules).
  uint8_t redDigit = map0_255_to_0_9(r255);

  // Sevseg2: show blue mapped to 0-9 (per rules).
  uint8_t blueDigit = map0_255_to_0_9(b255);

  // Build 7-segment bit patterns.
  uint8_t sevseg1Seg = DIGIT_TO_SEG[redDigit];
  uint8_t sevseg2Seg = DIGIT_TO_SEG[blueDigit];

  // Update displays continuously.
  shiftOutTwo595(sevseg1Seg, sevseg2Seg);

  // RGB LEDs are driven directly from the potentiometers? No:
  // The circuit provides only RGB wiring from Mega pins 2-7.
  // But no MCU pins are described for RGB in this circuit snippet (pins 2-7 are connected to rgb1 and rgb2).
  // Those pins must be driven to display colors based on potentiometers.
  // Implement that behavior:
  // Mega:2..4 drive rgb1 R,G,B; Mega:5..7 drive rgb2 R,G,B.
  // Set them here based on the mapped 0-255 values.
  // NOTE: Doing it here ensures continuous updates without altering required display mapping.
  uint8_t r8 = r255;
  uint8_t g8 = g255;
  uint8_t b8 = b255;

  // PWM-capable on Mega2560: pins 2-13 have PWM on 2-13? Actually PWM pins include 2-13.
  // We'll write via analogWrite using those pins.
  analogWrite(2, r8); // rgb1:R
  analogWrite(3, g8); // rgb1:G
  analogWrite(4, b8); // rgb1:B
  analogWrite(5, r8); // rgb2:R
  analogWrite(6, g8); // rgb2:G
  analogWrite(7, b8); // rgb2:B

  delay(5);
}
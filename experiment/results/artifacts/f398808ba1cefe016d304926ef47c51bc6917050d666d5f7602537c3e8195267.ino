#include <Arduino.h>

static const uint8_t SEGMENT_ORDER[7] = {
  0, // Q0 -> A
  5, // Q5 -> F
  6, // Q6 -> G
  1, // Q1 -> B
  2, // Q2 -> C
  3, // Q3 -> D
  4  // Q4 -> E
};

// 7-seg digits for common-cathode, segments: A B C D E F G
// We'll map to the shift register outputs Q0..Q6:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
static const uint8_t DIGIT_MASK[10] = {
  // A B C D E F G
  // 0: A B C D E F (off G)
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),
  // 1: B C
  (1<<1)|(1<<2),
  // 2: A B D E G
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),
  // 3: A B C D G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),
  // 4: B C F G
  (1<<1)|(1<<2)|(1<<5)|(1<<6),
  // 5: A C D F G
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),
  // 6: A C D E F G
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 7: A B C
  (1<<0)|(1<<1)|(1<<2),
  // 8: A B C D E F G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 9: A B C D F G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)
};

class Shift595 {
public:
  Shift595(uint8_t dataPin, uint8_t latchPin, uint8_t oePin, uint8_t clkPin, uint8_t mrPin)
    : dataPin_(dataPin), latchPin_(latchPin), oePin_(oePin), clkPin_(clkPin), mrPin_(mrPin) {}

  void begin() {
    pinMode(dataPin_, OUTPUT);
    pinMode(latchPin_, OUTPUT);
    pinMode(oePin_, OUTPUT);
    pinMode(clkPin_, OUTPUT);
    pinMode(mrPin_, OUTPUT);

    // Release MR (active low per typical 74HC595 MR), and enable outputs.
    digitalWrite(mrPin_, HIGH);
    digitalWrite(oePin_, LOW);  // OE is tied to GND in wiring, but set anyway.

    // Ensure latch low before first use.
    digitalWrite(latchPin_, LOW);
    digitalWrite(clkPin_, LOW);

    clear();
  }

  void clear() {
    // Output 0s for Q0..Q7, with 8 bits shifted MSB->LSB or LSB->MSB?
    // We'll consistently shift 8 bits in MSB->LSB order while generating correct bit positions.
    // The wiring indicates Q0..Q7 correspond to specific outputs; we set bit values accordingly.
    setByte(0x00);
  }

  void setByte(uint8_t value) {
    // Shift out 8 bits MSB first to match typical shiftRegister behavior.
    // We'll map Q0..Q7 such that bit0 -> Q0, bit7 -> Q7.
    digitalWrite(latchPin_, LOW);
    for (int8_t i = 7; i >= 0; --i) {
      digitalWrite(clkPin_, LOW);
      digitalWrite(dataPin_, (value & (1u << i)) ? HIGH : LOW);
      digitalWrite(clkPin_, HIGH);
    }
    digitalWrite(latchPin_, LOW);
    digitalWrite(latchPin_, HIGH);
    digitalWrite(latchPin_, LOW);
  }

private:
  uint8_t dataPin_, latchPin_, oePin_, clkPin_, mrPin_;
};

// Wiring from circuit (Mega2560):
// sr1: DS=A0, SHCP=A2, STCP=A1, OE=GND, MR tied via sr1:VCC
// sr2: DS=Q7S from sr1 (Q7S), SHCP shared A2, STCP shared A1, OE shared GND
// But in the provided connections, sr1 DS=A0, sr1 SHCP=A2, sr1 STCP=A1, sr1 OE=GND.
// sr1 Q7S -> sr2 DS. Therefore we can drive via sr1 pins only (sr2 will latch via chaining).
static const uint8_t SR1_DS   = A0;
static const uint8_t SR1_SHCP = A2; // shift clock
static const uint8_t SR1_STCP = A1; // latch clock
static const uint8_t SR_OE    = 255; // OE tied to GND in circuit, not controllable by MCU
static const uint8_t SR_MR    = 255; // MR tied to VCC; not controllable by MCU

// We'll still provide dummy pins; but we won't use OE/MR since tied to GND/VCC in wiring.
static const uint8_t DUMMY_OE = 4;
static const uint8_t DUMMY_MR = 5;

// For the two chained 74HC595:
Shift595 sr1(SR1_DS, SR1_STCP, DUMMY_OE, SR1_SHCP, DUMMY_MR);

static const uint8_t SR1_QA = 0; // Q0->A
static const uint8_t SR1_QB = 1; // Q1->B
static const uint8_t SR1_QC = 2; // Q2->C
static const uint8_t SR1_QD = 3; // Q3->D
static const uint8_t SR1_QE = 4; // Q4->E
static const uint8_t SR1_QF = 5; // Q5->F
static const uint8_t SR1_QG = 6; // Q6->G
// Q7 unused on each SR in the connections (sr1 Q7S used to feed sr2 DS)

uint8_t digitToSrByte(uint8_t digit) {
  uint8_t mask = DIGIT_MASK[digit];

  // mask bits correspond to segments: bit0=A, bit1=B, ..., bit6=G.
  // Convert to shift-register byte bits Q0..Q6.
  uint8_t b = 0;
  // Q0->A
  if (mask & (1 << 0)) b |= (1u << SR1_QA);
  // Q1->B
  if (mask & (1 << 1)) b |= (1u << SR1_QB);
  // Q2->C
  if (mask & (1 << 2)) b |= (1u << SR1_QC);
  // Q3->D
  if (mask & (1 << 3)) b |= (1u << SR1_QD);
  // Q4->E
  if (mask & (1 << 4)) b |= (1u << SR1_QE);
  // Q5->F
  if (mask & (1 << 5)) b |= (1u << SR1_QF);
  // Q6->G
  if (mask & (1 << 6)) b |= (1u << SR1_QG);
  // Q7 left as 0
  return b;
}

// RGB pins from wiring:
// rgb1: R=2, G=3, B=4; COMs on GND already.
// rgb2: R=5, G=6, B=7
// rgb3: R=8, G=9, B=10
static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

static const uint8_t RGB3_R = 8;
static const uint8_t RGB3_G = 9;
static const uint8_t RGB3_B = 10;

void setRgb(bool r, bool g, bool b, uint8_t pinR, uint8_t pinG, uint8_t pinB) {
  // Common cathode LEDs: HIGH turns ON (anode driven).
  digitalWrite(pinR, r ? HIGH : LOW);
  digitalWrite(pinG, g ? HIGH : LOW);
  digitalWrite(pinB, b ? HIGH : LOW);
}

void applyColorIndex(uint8_t idx) {
  // Sequence (each 2 seconds):
  // Red:    rgb1 red
  //         rgb2 off, rgb3 off
  // Green:  rgb2 green
  // Blue:   rgb3 blue
  // Purple: rgb1 red + blue
  // Yellow: rgb2 red + green
  // Cyan:   rgb3 green + blue
  bool r1=false,g1=false,b1=false;
  bool r2=false,g2=false,b2=false;
  bool r3=false,g3=false,b3=false;

  switch (idx % 6) {
    case 0: // Red
      r1=true;
      break;
    case 1: // Green
      g2=true;
      break;
    case 2: // Blue
      b3=true;
      break;
    case 3: // Purple
      r1=true; b1=true;
      break;
    case 4: // Yellow
      r2=true; g2=true;
      break;
    case 5: // Cyan
      g3=true; b3=true;
      break;
  }

  setRgb(r1,g1,b1, RGB1_R, RGB1_G, RGB1_B);
  setRgb(r2,g2,b2, RGB2_R, RGB2_G, RGB2_B);
  setRgb(r3,g3,b3, RGB3_R, RGB3_G, RGB3_B);
}

uint8_t colorIndex = 0;
uint8_t countdown = 99;

unsigned long lastTickMs = 0;
static const unsigned long TICK_MS = 2000;

void updateDisplays(uint8_t value) {
  // Show "value" on sevseg1 (tens) and sevseg2 (ones)
  uint8_t tens = value / 10;
  uint8_t ones = value % 10;

  // Because displays are driven through two chained 74HC595s:
  // sr1 controls sevseg1 segments through Q0..Q6 (Q7S goes to sr2 DS).
  // sr2 controls sevseg2 segments through its Q0..Q6.
  //
  // When shifting through the chain, we must clock 16 bits total:
  // - bits for sr2 first (farther), then bits for sr1.
  // The easiest: use shiftOut-style manually for 16 bits while maintaining bit0->Q0.
  //
  // We'll implement manual 16-bit shift:
  // order: shift MSB first across both chips.
  // Let:
  //  - sr2 byte = digitToSrByte(ones)
  //  - sr1 byte = digitToSrByte(tens)
  // We'll shift 8 bits for sr2 (bits 15..8) then 8 bits for sr1 (bits 7..0).
  uint8_t sr2Byte = digitToSrByte(ones);
  uint8_t sr1Byte = digitToSrByte(tens);

  uint16_t chain = (uint16_t(sr2Byte) << 8) | uint16_t(sr1Byte);

  digitalWrite(SR1_STCP, LOW);
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_SHCP, LOW);
    digitalWrite(SR1_DS, (chain & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR1_SHCP, HIGH);
  }
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_STCP, HIGH);
  digitalWrite(SR1_STCP, LOW);
}

void setup() {
  // Dummy pins for class (OE/MR tied in circuit). Set to safe states.
  pinMode(DUMMY_OE, OUTPUT);
  digitalWrite(DUMMY_OE, LOW);
  pinMode(DUMMY_MR, OUTPUT);
  digitalWrite(DUMMY_MR, HIGH);

  // RGB pins
  pinMode(RGB1_R, OUTPUT); pinMode(RGB1_G, OUTPUT); pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT); pinMode(RGB2_G, OUTPUT); pinMode(RGB2_B, OUTPUT);
  pinMode(RGB3_R, OUTPUT); pinMode(RGB3_G, OUTPUT); pinMode(RGB3_B, OUTPUT);

  // Shift registers
  sr1.begin();

  // Initial state: first color = Red, displays show 99
  colorIndex = 0;
  countdown = 99;
  applyColorIndex(colorIndex);
  updateDisplays(countdown);

  lastTickMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastTickMs >= TICK_MS) {
    // Ensure synchronization: update both color and countdown simultaneously each tick.
    lastTickMs += TICK_MS;

    // Countdown
    if (countdown == 0) {
      countdown = 99;
      colorIndex = 0;
    } else {
      countdown--;
      // Advance color every tick
      colorIndex = (colorIndex + 1) % 6;
    }

    // Apply new state
    applyColorIndex(colorIndex);
    updateDisplays(countdown);
  }
}
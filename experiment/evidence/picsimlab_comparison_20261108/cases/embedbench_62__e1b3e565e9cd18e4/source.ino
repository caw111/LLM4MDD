#include <Arduino.h>

static const uint8_t SR1_SHCP = A2; // Shift Register 1 SHCP
static const uint8_t SR1_STCP = A1; // Shift Register 1 STCP
static const uint8_t SR1_DS   = A0; // Shift Register 1 DS

static const uint8_t SR2_SHCP = A2; // shared by wiring
static const uint8_t SR2_STCP = A1; // shared by wiring
static const uint8_t SR2_DS   = A0; // shared by wiring

// sr1: VCC, MR are tied to VCC in the circuit (and to sr1:MR input).
// sr1:OE is tied to GND (sr1:GND to sr2:GND and OE to black), so outputs always enabled.

// Bargraph pins A1..A10 are wired to Mega pins 8..13 only (partial wiring shown).
// Per given circuit, bargraph1 segments correspond to:
static const uint8_t BAR_SEG_PINS[10] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
// Based on circuit:
static const uint8_t BAR_A1_PIN = 8;   // bargraph1:A1
static const uint8_t BAR_A2_PIN = 9;   // bargraph1:A2
static const uint8_t BAR_A3_PIN = 10;  // bargraph1:A3
static const uint8_t BAR_A4_PIN = 11;  // bargraph1:A4
static const uint8_t BAR_A5_PIN = 12;  // bargraph1:A5
static const uint8_t BAR_A6_PIN = 13;  // bargraph1:A6
// bargraph1:A7..A8 are also wired:
static const uint8_t BAR_A7_PIN = 44;  // bargraph1:A7
static const uint8_t BAR_A8_PIN = 45;  // bargraph1:A8
// Remaining bargraph segments (A9, A10) are not wired in the provided circuit.
// We must preserve behavior as specified; with missing pins, those segments can't be driven.
// We'll treat only wired pins as controllable.

static const uint8_t BAR_WIRED_SEGMENTS[8] = {0,1,2,3,4,5,6,7}; // correspond to A1..A8
static const uint8_t BAR_WIRED_PINS[8]      = {BAR_A1_PIN, BAR_A2_PIN, BAR_A3_PIN, BAR_A4_PIN,
                                              BAR_A5_PIN, BAR_A6_PIN, BAR_A7_PIN, BAR_A8_PIN};

// RGB LED wiring:
// rgb1: COM to GND, R->2, G->3, B->4
static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;
// rgb2: COM to GND, R->5, G->6, B->7
static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

// 74HC595 setup (data/clock/latch as per circuit).
// Shift order and bit mapping to 7-segment pins depend on wiring:
// sr1 outputs Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G.
// For common-cathode 7-seg, segment 'on' = HIGH.
static const uint8_t DIGIT_TO_SEG[10] = {
  // A B C D E F G
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

// Bit positions in shift register for segments:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G.
// We'll pack bits in that order.
static uint8_t packSegBits(uint8_t segMaskABCDEFg) {
  // segMask bits correspond to A..G in DIGIT_TO_SEG (A=bit6 ... G=bit0?).
  // We used 7 bits in order A,B,C,D,E,F,G encoded as:
  // value bit i: bit6=A ... bit0=G (because examples are common 7-seg masks).
  // We'll map each segment to SR bit positions:
  // SR Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
  // That means:
  // SRbit0 (A) = mask(A) => mask bit6
  // SRbit1 (B) = mask(B) => mask bit5
  // ...
  // SRbit6 (G) = mask(G) => mask bit0
  uint8_t sr = 0;
  // A
  if (segMaskABCDEFg & (1 << 6)) sr |= (1 << 0);
  // B
  if (segMaskABCDEFg & (1 << 5)) sr |= (1 << 1);
  // C
  if (segMaskABCDEFg & (1 << 4)) sr |= (1 << 2);
  // D
  if (segMaskABCDEFg & (1 << 3)) sr |= (1 << 3);
  // E
  if (segMaskABCDEFg & (1 << 2)) sr |= (1 << 4);
  // F
  if (segMaskABCDEFg & (1 << 1)) sr |= (1 << 5);
  // G
  if (segMaskABCDEFg & (1 << 0)) sr |= (1 << 6);

  return sr;
}

static void srShiftByte(uint8_t value, uint8_t shcpPin, uint8_t stcpPin, uint8_t dsPin) {
  // Shift out 8 bits, MSB first is typical, but 74HC595 output depends on order.
  // We assume DS shifts into SER line and Q0 becomes first shifted bit if using LSBF.
  // We'll shift MSB-first to match standard Arduino shiftOut behavior only if order needed.
  // However wiring uses Q0..Q6 to segments; we need consistent mapping.
  // We'll shift out MSB first so that bit7 ends in Q7. But Q7 (Q7S) not used.
  // Since only Q0..Q6 matter, we'll place our packed bits into low 7 bits and shift out LSb first
  // to land packed bit0 at Q0. We'll do LSBF.
  for (int i = 0; i < 8; i++) {
    uint8_t bitVal = (value >> i) & 0x01; // LSB first
    digitalWrite(dsPin, bitVal ? HIGH : LOW);
    digitalWrite(shcpPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(shcpPin, LOW);
    delayMicroseconds(1);
  }

  // Latch
  digitalWrite(stcpPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(stcpPin, LOW);
}

static void update7Seg(uint8_t tens, uint8_t ones) {
  // sr1 corresponds to sevseg1 via circuit:
  // sevseg1 segments driven by sr1 Q0..Q6.
  // sr2 corresponds to sevseg2 via circuit:
  // sevseg2 segments driven by sr2 Q0..Q6.

  uint8_t sr1Byte = packSegBits(DIGIT_TO_SEG[tens]);
  uint8_t sr2Byte = packSegBits(DIGIT_TO_SEG[ones]);

  // SRs are chained:
  // sr1 Q7S -> sr2 DS. To update both simultaneously, we shift 16 bits? But DS/SHCP/STCP are shared.
  // Wiring:
  // - sr1 SHCP and STCP are shared to sr2.
  // - sr1 DS is from A0, and sr1 Q7S goes to sr2 DS.
  // So when we clock 8 times:
  //   sr1 receives 8 bits, sr2 receives the previous bits shifted through.
  // To set both sr1 and sr2 correctly, we must shift 16 bits:
  //   First 8 bits go to sr2, next 8 bits go to sr1 (typical chain, MSB/LSB order matters).
  //
  // We'll shift out 16 bits LSB-first into the chain such that:
  // - the first 8 shifted bits end up in sr2
  // - the next 8 shifted bits end up in sr1
  //
  // Since our srShiftByte sends LSB-first and latches at end,
  // implement chained shift with same LSB-first bit order across 16 bits.

  uint8_t tensByte = sr1Byte;
  uint8_t onesByte = sr2Byte;

  // Shift 16 bits total, LSB first for each register:
  // bit positions i=0..7 correspond to what goes into sr2? With chain:
  // After first 8 clocks, sr1 has received those bits (but those bits are in sr1 now),
  // and sr2 receives bits shifted from sr1 Q7S during those clocks.
  // Thus, if we send bits [7..0] for sr2 first, they will be held in sr2 after 8 clocks.
  //
  // To avoid ambiguity, we can do explicit using bit-by-bit clock where DS input is either from
  // the lower bits of the combined 16-bit value, with sr1 receiving least-significant 8 bits
  // and sr2 receiving most-significant 8 bits. We'll set:
  // combined[15:8] -> sr2, combined[7:0] -> sr1.
  uint16_t combined = ((uint16_t)onesByte << 8) | (uint16_t)tensByte;

  for (int i = 0; i < 16; i++) {
    uint8_t bitVal = (combined >> i) & 0x01; // LSB-first into chained registers
    digitalWrite(SR1_DS, bitVal ? HIGH : LOW);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(1);
  }

  // Latch both at once
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, LOW);
}

static void updateBargraph(uint8_t value) {
  // Requirement: bar lights segments equal to tens digit, from pin A1 to pin A10.
  // Additionally: if tens digit exceeds 8 (80-99), max 8 lit segments.
  uint8_t tens = value / 10;
  if (tens > 8) tens = 8;

  // Set all wired segments off first
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t pin = BAR_WIRED_PINS[i];
    digitalWrite(pin, LOW);
  }

  // Turn on first N wired segments (A1..A8)
  for (uint8_t i = 0; i < tens && i < 8; i++) {
    digitalWrite(BAR_WIRED_PINS[i], HIGH);
  }
}

static void setRGBFromValue(uint8_t value) {
  // Common cathode, so:
  // - Turning a color on requires driving its anode HIGH.
  // - Turning off drives LOW.
  bool rOn1, gOn1, bOn1;
  bool rOn2, gOn2, bOn2;

  if (value <= 5) { // Red 0-5
    rOn1 = true; gOn1 = false; bOn1 = false;
    rOn2 = true; gOn2 = false; bOn2 = false;
  } else if (value <= 9) { // Green 6-9
    rOn1 = false; gOn1 = true; bOn1 = false;
    rOn2 = false; gOn2 = true; bOn2 = false;
  } else { // Blue 10-99
    rOn1 = false; gOn1 = false; bOn1 = true;
    rOn2 = false; gOn2 = false; bOn2 = true;
  }

  digitalWrite(RGB1_R, rOn1 ? HIGH : LOW);
  digitalWrite(RGB1_G, gOn1 ? HIGH : LOW);
  digitalWrite(RGB1_B, bOn1 ? HIGH : LOW);

  digitalWrite(RGB2_R, rOn2 ? HIGH : LOW);
  digitalWrite(RGB2_G, gOn2 ? HIGH : LOW);
  digitalWrite(RGB2_B, bOn2 ? HIGH : LOW);
}

void setup() {
  // RGB pins
  pinMode(RGB1_R, OUTPUT); pinMode(RGB1_G, OUTPUT); pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT); pinMode(RGB2_G, OUTPUT); pinMode(RGB2_B, OUTPUT);

  // Shift register pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Bargraph segment pins (wired in circuit): A1..A8
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_WIRED_PINS[i], OUTPUT);
    digitalWrite(BAR_WIRED_PINS[i], LOW);
  }

  // Initial conditions:
  // Both 7-seg displays show "00"
  update7Seg(0, 0);
  // All bar graph LEDs off (already off)
  // RGB LEDs off
  digitalWrite(RGB1_R, LOW); digitalWrite(RGB1_G, LOW); digitalWrite(RGB1_B, LOW);
  digitalWrite(RGB2_R, LOW); digitalWrite(RGB2_G, LOW); digitalWrite(RGB2_B, LOW);
}

void loop() {
  static unsigned long lastMs = 0;
  static uint8_t value = 0;

  unsigned long now = millis();
  if (now - lastMs >= 2000UL) {
    lastMs = now;

    // Update all components simultaneously with current value
    uint8_t tens = value / 10;
    uint8_t ones = value % 10;

    update7Seg(tens, ones);
    updateBargraph(value);
    setRGBFromValue(value);

    // Increment for next cycle (0->1->...->99->00)
    value = (uint8_t)((value + 1) % 100);
  }
}
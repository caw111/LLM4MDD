#include <Arduino.h>

// --- RGB LED pins (Mega 2560) ---
static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

// --- Shift register control pins (74HC595 style, as wired) ---
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue

static const uint8_t SR1_MR   = A0;  // MR is tied via sr1:MR to sr1:VCC in circuit; keep pin configured
static const uint8_t SR1_OE   = 0;   // Circuit ties sr1:OE to GND; use pin 0 as GND (safer compile)

// These are tied to SR1 pins in circuit, but keep for clarity.
static const uint8_t SR2_SHCP = A2;
static const uint8_t SR2_STCP = A1;
static const uint8_t SR2_DS   = 0;  // DS fed from sr1 Q7S via wiring; not driven directly

// --- Bar graph pins (A1..A8 exist in provided wiring) ---
static const uint8_t BAR_A_PINS[8] = {A1, A2, A3, A4, A5, A6, A7, A8};

// --- 7-seg segment encoding ---
// Outputs from shift registers to segments:
// sevseg1 segments: Q0->A, Q1->F, Q2->E, Q3->D, Q4->C, Q5->B, Q6->G
// sevseg2 segments: Q0->A, Q1->F, Q2->E, Q3->D, Q4->C, Q5->B, Q6->G
// Table is for {A,B,C,D,E,F,G} active-high.
static const bool segTable[10][7] = {
//   A, B, C, D, E, F, G
  { 1, 1, 1, 1, 1, 1, 0 }, // 0
  { 0, 1, 1, 0, 0, 0, 0 }, // 1
  { 1, 1, 0, 1, 1, 0, 1 }, // 2
  { 1, 1, 1, 1, 0, 0, 1 }, // 3
  { 0, 1, 1, 0, 0, 1, 1 }, // 4
  { 1, 0, 1, 1, 0, 1, 1 }, // 5
  { 1, 0, 1, 1, 1, 1, 1 }, // 6
  { 1, 1, 1, 0, 0, 0, 0 }, // 7
  { 1, 1, 1, 1, 1, 1, 1 }, // 8
  { 1, 1, 1, 1, 0, 1, 1 }  // 9
};

static void pulseSHCP() {
  digitalWrite(SR1_SHCP, LOW);
  delayMicroseconds(1);
  digitalWrite(SR1_SHCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR1_SHCP, LOW);
  delayMicroseconds(1);
}

static void latchBoth() {
  // SR1_STCP and SR2_STCP are tied together in circuit; toggle once.
  digitalWrite(SR1_STCP, LOW);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, LOW);
}

// Daisy chain: shift sr1 then sr2 through sr1's cascade (sr1 Q7S -> sr2 DS).
// We shift 16 bits total: first sr1 byte (sevseg1), then sr2 byte (sevseg2).
static void set7SegDigits(uint8_t d1, uint8_t d2) {
  auto buildByteForSeg = [&](uint8_t digit) -> uint8_t {
    // Map {A,B,C,D,E,F,G} to shift-register outputs:
    // Q0=A, Q1=F, Q2=E, Q3=D, Q4=C, Q5=B, Q6=G
    uint8_t b = 0;
    if (segTable[digit][0]) b |= (1 << 0); // A -> Q0
    if (segTable[digit][5]) b |= (1 << 1); // F -> Q1
    if (segTable[digit][4]) b |= (1 << 2); // E -> Q2
    if (segTable[digit][3]) b |= (1 << 3); // D -> Q3
    if (segTable[digit][2]) b |= (1 << 4); // C -> Q4
    if (segTable[digit][1]) b |= (1 << 5); // B -> Q5
    if (segTable[digit][6]) b |= (1 << 6); // G -> Q6
    // Q7 not used; keep 0.
    return b;
  };

  uint8_t byte1 = buildByteForSeg(d1); // sevseg1
  uint8_t byte2 = buildByteForSeg(d2); // sevseg2

  // Ensure latch disabled during shifting.
  digitalWrite(SR1_STCP, LOW);

  // Shift LSB-first: bit i of the byte goes on DS before each clock.
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t bit = (byte1 >> i) & 0x01;
    digitalWrite(SR1_DS, bit ? HIGH : LOW);
    pulseSHCP();
  }
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t bit = (byte2 >> i) & 0x01;
    digitalWrite(SR1_DS, bit ? HIGH : LOW);
    pulseSHCP();
  }

  latchBoth();
}

static void setBarGraph(uint8_t value) {
  // Requirement 3+6: bargraph lights segments equal to tens digit, capped at 8.
  uint8_t tens = value / 10;
  uint8_t lit = tens;
  if (lit > 8) lit = 8;

  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(BAR_A_PINS[i], LOW);
  }
  for (uint8_t i = 0; i < lit; i++) {
    digitalWrite(BAR_A_PINS[i], HIGH);
  }
}

static void setRGB(uint8_t value) {
  // Requirement 4:
  // Red: 0-5
  // Green: 6-9
  // Blue: 10-99
  bool r = false, g = false, b = false;

  if (value <= 5) {
    r = true;
  } else if (value <= 9) {
    g = true;
  } else {
    b = true;
  }

  digitalWrite(RGB1_R, r ? HIGH : LOW);
  digitalWrite(RGB1_G, g ? HIGH : LOW);
  digitalWrite(RGB1_B, b ? HIGH : LOW);

  digitalWrite(RGB2_R, r ? HIGH : LOW);
  digitalWrite(RGB2_G, g ? HIGH : LOW);
  digitalWrite(RGB2_B, b ? HIGH : LOW);
}

void setup() {
  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Shift register pins
  pinMode(SR1_DS, OUTPUT);
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);

  // Even if OE is physically tied to GND in the circuit, configure the related behavior:
  // Use output-low for SR1_OE placeholder so it doesn't float in simulation/hardware.
  pinMode(SR1_OE, OUTPUT);
  digitalWrite(SR1_OE, LOW);

  // Bar graph pins
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW); // Requirement: all off initially
  }

  // Initialize RGB off (Requirement)
  digitalWrite(RGB1_R, LOW);
  digitalWrite(RGB1_G, LOW);
  digitalWrite(RGB1_B, LOW);

  digitalWrite(RGB2_R, LOW);
  digitalWrite(RGB2_G, LOW);
  digitalWrite(RGB2_B, LOW);

  // Initialize 7-seg displays to "00" (Requirement)
  set7SegDigits(0, 0);
}

void loop() {
  static uint8_t value = 0; // 0..99
  static unsigned long last = 0;

  unsigned long now = millis();
  if (now - last >= 2000UL) {
    last = now;

    uint8_t tens = value / 10;
    uint8_t ones = value % 10;

    // Requirement 5: update all components with each increment.
    // Compute then apply in quick succession.
    set7SegDigits(tens, ones); // both digits always show current number as two digits
    setBarGraph(value);        // tens digit segments (capped at 8)
    setRGB(value);             // color based on range

    value++;
    if (value >= 100) value = 0;
  }
}
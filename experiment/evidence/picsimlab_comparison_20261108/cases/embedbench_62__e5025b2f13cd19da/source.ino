#include <Arduino.h>

static const uint8_t BAR_PINS[10] = {A1, A2, A3, A4, A5, A6, A7, A8, A9, A10}; // not all exist in circuit

// 7-seg segment wiring through shift registers
// sr1 outputs -> sevseg1 segments
// Q0->A, Q1->F, Q2->E, Q3->D, Q4->C, Q5->B, Q6->G, Q7S->DS of sr2
// sr2 outputs -> sevseg2 segments
// Q0->A, Q1->F, Q2->E, Q3->D, Q4->C, Q5->B, Q6->G

// Shift register pins (74HC595 style)
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue

static const uint8_t SR2_SHCP = A2;
static const uint8_t SR2_STCP = A1;
static const uint8_t SR2_DS   = 0;  // driven by sr1 Q7S through wiring; not used directly

static const uint8_t SR1_OE   = GND; // tied to GND in circuit via sr1:GND->sr1:OE
static const uint8_t SR1_MR   = A0;  // not used; see setup below (MR tied via sr1:MR to sr1:VCC in circuit)
static const uint8_t SR1_VCC  = 0;   // not used

// RGB LEDs wiring
static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

// Bar graph wiring (only A1..A8 are present in provided connections)
static const uint8_t BAR_A_PINS[8] = {A1, A2, A3, A4, A5, A6, A7, A8};

// Because the provided circuit only connects A1..A8, we drive those only.
// Requirement 3: "from pin:A1 to pin:A10" - but wiring given doesn't include A9/A10.
// We preserve the provided wiring by only controlling A1..A8.
static const uint8_t BAR_C_COMMON = GND; // all C1..C8 tied to GND. in circuit: mega:GND.1 to bargraph1:C*

// 7-seg segment bits for digits 0..9 with segments {A,B,C,D,E,F,G}.
// We'll output these to shift-register outputs Q0..Q6, where:
// Q0=A, Q5=B, Q4=C, Q3=D, Q2=E, Q1=F, Q6=G
// So we need mapping digit segments -> shift bits positions 0..6.
static const bool segTable[10][7] = {
// A, B, C, D, E, F, G
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

// Outputs are active-high (as typical shift-register driving LED cathodes/anodes in Wokwi style).
// We'll treat the Qn lines as active-high to illuminate segments.
static void pulseSHCP() {
  digitalWrite(SR1_SHCP, LOW);
  delayMicroseconds(1);
  digitalWrite(SR1_SHCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR1_SHCP, LOW);
  delayMicroseconds(1);
}

static void latchBoth() {
  // SR1_STCP and SR2_STCP tied together in circuit, so one latch works.
  digitalWrite(SR1_STCP, LOW);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, LOW);
}

// Shift out 16 bits total to daisy chain sr1->sr2.
// But the chain is sr1 Q7S connected to sr2 DS (given).
// So we must send 8 bits for sr2 last, and sr1 first? For 595 daisy chain:
// Bits shifted in first end up in farther register last.
// We'll send: SR1 bits first (to DS), then SR2 bits, with final latch.
// If wrong, it will swap displays. We'll implement standard: first 8 bits go to sr1.
static void set7SegDigits(uint8_t d1, uint8_t d2) {
  // Build shift bytes for sr1 (sevseg1) and sr2 (sevseg2).
  auto buildByte = [&](uint8_t digit) -> uint8_t {
    // Q0=A, Q5=B, Q4=C, Q3=D, Q2=E, Q1=F, Q6=G, Q7=don't care (used for cascade only)
    uint8_t b = 0;
    b |= (segTable[digit][0] ? (1 << 0) : 0); // A -> Q0
    b |= (segTable[digit][5] ? (1 << 1) : 0); // F -> Q1
    b |= (segTable[digit][4] ? (1 << 2) : 0); // E -> Q2
    b |= (segTable[digit][3] ? (1 << 3) : 0); // D -> Q3
    b |= (segTable[digit][2] ? (1 << 4) : 0); // C -> Q4
    b |= (segTable[digit][1] ? (1 << 5) : 0); // B -> Q5
    b |= (segTable[digit][6] ? (1 << 6) : 0); // G -> Q6
    // Q7 left as 0; it will feed sr2 DS, but Wokwi cascade uses Q7S.
    // We'll set Q7=0 so that sr2 only takes correct shifted data through its DS via Q7S.
    b |= (0 << 7);
    return b;
  };

  uint8_t byte1 = buildByte(d1); // for sevseg1
  uint8_t byte2 = buildByte(d2); // for sevseg2

  // Shift out 16 bits, MSB first? For 595, shifting usually occurs LSB first with DS sampled each clock.
  // We'll shift LSB first for straightforward mapping Qn corresponds to shifted bit positions.
  // Send 8 bits for sr1, then 8 bits for sr2.
  digitalWrite(SR1_STCP, LOW);

  for (int i = 0; i < 8; i++) {
    uint8_t bit = (byte1 >> i) & 0x01;
    digitalWrite(SR1_DS, bit ? HIGH : LOW);
    pulseSHCP();
  }
  for (int i = 0; i < 8; i++) {
    uint8_t bit = (byte2 >> i) & 0x01;
    // Now DS is still SR1_DS in circuit; sr2 receives via sr1 Q7S cascade.
    // Standard daisy chain expects these bits to be shifted after the first 8.
    digitalWrite(SR1_DS, bit ? HIGH : LOW);
    pulseSHCP();
  }

  latchBoth();
}

static void setBarGraph(uint8_t value) {
  // Requirement: tens digit segments lit; cap at 8.
  uint8_t tens = value / 10;
  uint8_t lit = tens;
  if (lit > 8) lit = 8;

  // Turn off all connected segments A1..A8
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(BAR_A_PINS[i], LOW);
  }
  // Turn on first 'lit' segments
  for (uint8_t i = 0; i < lit; i++) {
    digitalWrite(BAR_A_PINS[i], HIGH);
  }

  // Note: A9 and A10 are not wired in provided circuit; leave them untouched.
}

static void setRGB(uint8_t value) {
  // Requirement: Red 0-5, Green 6-9, Blue 10-99
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

  // Shift register control pins
  pinMode(SR1_DS, OUTPUT);
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);

  // Bar graph segment pins (wired A1..A8)
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }

  // Initialize RGB off
  digitalWrite(RGB1_R, LOW);
  digitalWrite(RGB1_G, LOW);
  digitalWrite(RGB1_B, LOW);

  digitalWrite(RGB2_R, LOW);
  digitalWrite(RGB2_G, LOW);
  digitalWrite(RGB2_B, LOW);

  // Initialize 7-seg displays to "00"
  set7SegDigits(0, 0);

  // Ensure bar graph off (already done)
}

void loop() {
  static uint8_t value = 0;
  static unsigned long last = 0;

  unsigned long now = millis();
  if (now - last >= 2000UL) {
    last = now;

    uint8_t tens = value / 10;
    uint8_t ones = value % 10;

    // Update all components "simultaneously" with this increment
    // (compute first, then apply in quick succession)
    set7SegDigits(ones == 0 && tens == 0 ? 0 : tens, ones); // sevseg1=tens, sevseg2=ones? requirement says both show current number as two digits.
    // The above line sets sevseg1 to tens, sevseg2 to ones, as typical left/right ordering.
    // But we must ensure initial "00": tens=0, ones=0 -> OK.

    // Correct ordering explicitly:
    set7SegDigits(tens, ones);

    setBarGraph(value);
    setRGB(value);

    value++;
    if (value >= 100) value = 0;
  }
}
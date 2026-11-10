#include <Arduino.h>

// ---------------- Pin mapping (from provided circuit) ----------------
// RGB LEDs (common cathode)
static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

// Shift register 1 (sr1) and 2 (sr2) control shared digit lines
// Mega to sr1:
static const uint8_t SR_DATA1 = A0; // sr1:DS
static const uint8_t SR_SHCP1 = A2; // sr1:SHCP
static const uint8_t SR_STCP1 = A1; // sr1:STCP
static const uint8_t SR_OE   = 0;   // sr1:OE tied to GND in circuit (black)

// Shift register 2:
// sr2 shares SHCP and STCP; DS is connected from sr1:Q7S via sr2:DS
// Control is handled by shifting through sr1; the wiring links output to sr2 data.

static const uint8_t SR_MR = 0; // both MRs tied to VCC via sr1:VCC; no control needed.

// ---------------- 7-seg segments ----------------
// Common pin of both displays (sevseg1:COM.1 and sevseg2:COM.1) connected to sr1:MR (VCC)
// Segment pins: A,B,C,D,E,F,G are driven by sr Q0..Q6 for each register.
// Wiring (sr1 -> sevseg1): Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
// Wiring (sr2 -> sevseg2): Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
//
// Assumption: Typical 74HC595 outputs HIGH drives LED segments ON given wiring.
// If your display is inverted, swap digit encoding polarities accordingly.

static uint8_t digitToSegmentsCommonCathode(uint8_t digit) {
  // Segment order: A B C D E F G -> bits 0..6
  // Common-cathode, active HIGH segment.
  // Standard 7-seg for digits 0..9:
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  switch (digit) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);                         // A B C D E F
    case 1: return (1<<1)|(1<<2);                                                         // B C
    case 2: return (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6);                                   // A B D E G
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6);                                     // A B C D G
    case 4: return (1<<1)|(1<<2)|(1<<5)|(1<<6);                                         // B C F G
    case 5: return (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6);                                   // A C D F G
    case 6: return (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);                             // A C D E F G
    case 7: return (1<<0)|(1<<1)|(1<<2);                                                         // A B C
    case 8: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);                         // all
    case 9: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6);                                 // A B C D F G
    default: return 0;
  }
}

static void srPulse(uint8_t pin, uint8_t levelLowThenHigh = HIGH) {
  // (Not used) placeholder
  (void)pin;
  (void)levelLowThenHigh;
}

static void shiftRegisterWriteDigits(uint8_t tensDigit, uint8_t unitsDigit) {
  // Shift into sr1 and sr2 to set sevseg1 = tensDigit, sevseg2 = unitsDigit.
  // Wiring: sr1 DS is A0. SR_SHCP on A2 clocks, SR_STCP on A1 latches.
  // sr1 outputs Q0..Q6 go to sevseg1 segments.
  // sr2 is fed by sr1's Q7S output (as per wiring sr1:Q7S -> sr2:DS).
  //
  // We need two 8-bit transfers:
  // - sr1's bits 0..7: Q0..Q6 used for A..G, Q7S unused for segs, but still part of shift chain.
  // - sr2's bits 0..7 come from sr1's Q7S behavior -> effectively both registers receive bits
  //   corresponding to a longer shift chain. With the provided cross-connection, the simplest
  //   approach is to send 16 bits: high byte for sr2 then low byte for sr1.
  //
  // However: sr1:Q7S is connected to sr2:DS, meaning sr2 is in series after sr1. With classic
  // daisy-chain behavior, you shift LSB/bit order through; after 8 clocks, the next bits enter sr2.
  //
  // We'll implement standard MSB-first and send:
  //   byte1 -> sr1, byte2 -> sr2, using 16 clocks with sr1 first.
  //
  // To ensure correct mapping, we build bytes where bit0->A, bit6->G. bit7 is don't care (0).
  const uint8_t segsTens  = digitToSegmentsCommonCathode(tensDigit);
  const uint8_t segsUnits = digitToSegmentsCommonCathode(unitsDigit);

  // Byte bit mapping to shift register Q outputs:
  // sr: Q0=A ... Q6=G. We'll place those in bits 0..6.
  const uint8_t byteSr1 = segsTens;   // goes to sevseg1
  const uint8_t byteSr2 = segsUnits;  // goes to sevseg2

  // Daisy-chain order: first shifted bits end up in sr1; subsequent bits in sr2.
  // With 16 clocks, do: [byteSr2][byteSr1] using MSB-first so that sr1 receives least-significant byte.
  // Common daisy chain method for 2x595 with MSB-first:
  //   shift highByte first then lowByte.
  uint16_t chain = ((uint16_t)byteSr2 << 8) | (uint16_t)byteSr1;

  noInterrupts();
  digitalWrite(SR_SHCP1, LOW);
  digitalWrite(SR_STCP1, LOW);

  // Shift 16 bits MSB first
  for (int i = 15; i >= 0; --i) {
    digitalWrite(A0, (chain & (1u << i)) ? HIGH : LOW); // DS
    digitalWrite(SR_SHCP1, HIGH);
    digitalWrite(SR_SHCP1, LOW);
  }

  // Latch outputs
  digitalWrite(SR_STCP1, HIGH);
  digitalWrite(SR_STCP1, LOW);
  interrupts();
}

// ---------------- Timing ----------------
static const uint32_t PERIOD_MS = 2000;

// ---------------- RGB color sequence ----------------
struct RGBColor { uint8_t r, g, b; };

static const RGBColor sequence[] = {
  // Common cathode: setting HIGH lights (based on typical wiring)
  {HIGH, LOW,  LOW}, // Red
  {LOW,  HIGH, LOW}, // Green
  {LOW,  LOW,  HIGH},// Blue
  {HIGH, HIGH, LOW}, // Yellow
  {LOW,  HIGH, HIGH},// Cyan
  {HIGH, LOW,  HIGH},// Magenta
  {HIGH, HIGH, HIGH} // White
};

static const uint8_t SEQ_LEN = sizeof(sequence) / sizeof(sequence[0]);

static void setBothRGB(uint8_t idx) {
  const RGBColor &c = sequence[idx % SEQ_LEN];

  // rgb1
  digitalWrite(RGB1_R, c.r);
  digitalWrite(RGB1_G, c.g);
  digitalWrite(RGB1_B, c.b);

  // rgb2
  digitalWrite(RGB2_R, c.r);
  digitalWrite(RGB2_G, c.g);
  digitalWrite(RGB2_B, c.b);
}

void setup() {
  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Shift registers pins
  pinMode(A0, OUTPUT); // sr1:DS
  pinMode(SR_SHCP1, OUTPUT); // sr1:SHCP (A2)
  pinMode(SR_STCP1, OUTPUT); // sr1:STCP (A1)

  // Ensure stable initial levels
  digitalWrite(SR_SHCP1, LOW);
  digitalWrite(SR_STCP1, LOW);
  digitalWrite(A0, LOW);

  // Initial State:
  // RGB LEDs should display Red
  setBothRGB(0);

  // 7-seg displays show "00"
  shiftRegisterWriteDigits(0, 0);
}

void loop() {
  static uint8_t colorIndex = 0;
  static uint8_t number = 0;

  static uint32_t lastRgbMs = 0;
  static uint32_t lastSegMs = 0;

  uint32_t now = millis();

  // Synchronization: both update every 2 seconds using same period,
  // but implemented independently while keeping the update cadence aligned.
  if (now - lastRgbMs >= PERIOD_MS) {
    lastRgbMs += PERIOD_MS; // preserve cadence
    colorIndex = (colorIndex + 1) % SEQ_LEN;
    setBothRGB(colorIndex);
  }

  if (now - lastSegMs >= PERIOD_MS) {
    lastSegMs += PERIOD_MS; // preserve cadence
    number = (number + 1) % 100;

    uint8_t tens = number / 10;
    uint8_t units = number % 10;
    shiftRegisterWriteDigits(tens, units);
  }
}
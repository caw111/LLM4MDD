#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

enum RgbColor {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  CYAN,
  MAGENTA,
  WHITE
};

// ---- RGB pins (Mega 2560) ----
static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

// ---- Shift register pins (Mega 2560) ----
// sr1/sr2 share clock/latch; data differs.
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue

// ---- Timing ----
static const unsigned long INTERVAL_MS = 2000;

// ---- 74HC595 segment mapping ----
// Using common-cathode 7-seg. We assume the Q outputs drive segments
// A..G as wired in the circuit:
//
// sr1 -> sevseg1:
// Q0 -> A, Q1 -> B, Q2 -> C, Q3 -> D, Q4 -> E, Q5 -> F, Q6 -> G
// Q7 -> (S) is connected to DS of sr2; it must remain unused.
//
// For a typical common-cathode 7-seg, segments are ON with HIGH.
// We'll generate patterns for hex digits 0..9 accordingly.
static const uint8_t DIGIT_TO_SEG_A_ON = 0; // placeholder; not used

// Segment bits in Q0..Q6: bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G, bit7=don't-care
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitTo74hc595Pattern(uint8_t digit);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void pulseSTCP();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOutByteToShiftRegister(uint8_t byte, bool dsForSr1);
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplays(uint8_t tensDigit, uint8_t onesDigit);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbAll(RgbColor c);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 168 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_56\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitTo74hc595Pattern(uint8_t digit) {
  // For digits 0..9, set segments ON (1) for common-cathode:
  // a b c d e f g
  // 0: 1 1 1 1 1 1 0  => 0b01111110 with bit6=g -> 0, but we will map into bit positions below.
  // We'll construct directly into bits: bit0=A ... bit6=G.
  switch (digit) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5); // A B C D E F
    case 1: return (1<<1)|(1<<2);                             // B C
    case 2: return (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6);       // A B D E G
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6);       // A B C D G
    case 4: return (1<<1)|(1<<2)|(1<<5)|(1<<6);             // B C F G
    case 5: return (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6);       // A C D F G
    case 6: return (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); // A C D E F G
    case 7: return (1<<0)|(1<<1)|(1<<2);                     // A B C
    case 8: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); // all
    case 9: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6); // A B C D F G
    default: return 0;
  }
}

// ---- Shift register control ----
static inline void pulseSTCP() {
  digitalWrite(SR1_STCP, LOW);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR1_STCP, LOW);
}

static void shiftOutByteToShiftRegister(uint8_t byte, bool dsForSr1) {
  // dsForSr1 exists only conceptually; actual wiring uses SR1_DS (A0) as ds line.
  // For our circuit: sr2's DS input is connected to sr1's Q7S (sr1:Q7S -> sr2:DS),
  // meaning when we clock out a second byte, it will arrive at sr2.
  (void)dsForSr1;

  // We will use the standard "MSB first" or "LSB first"? With 74HC595,
  // the bit order matters. We must match the Q mapping provided in the circuit:
  // sr1:Q0 is connected to segment A, sr1:Q1 to segment B, ..., sr1:Q6 to G.
  // Therefore, the first bit shifted into Q0 should correspond to bit0 of our pattern.
  //
  // 74HC595 shifts such that after 8 clock pulses, the last shifted bit ends up in Q0
  // if using "LSB first" style. To ensure pattern bit0 -> Q0, we will clock out LSB first.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR1_SHCP, LOW);
    digitalWrite(SR1_DS, (byte & (1u << i)) ? HIGH : LOW);
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(1);
  }
}

static void updateDisplays(uint8_t tensDigit, uint8_t onesDigit) {
  // We need to update both sr1 (sevseg1 tens) and sr2 (sevseg2 units).
  // Because sr1:Q7S feeds sr2:DS, the chain works like:
  // - Clock out first byte -> sr2 receives it after shifting through sr1? Actually in a chain,
  //   data shifted first typically ends up in the farthest register after all clocks.
  //   Standard daisy-chain order for two 74HC595 with SR1 connected to SR2:
  //   shift in byte for sr2 first, then byte for sr1, then latch both together.
  //
  // We'll implement that standard order for correct mapping:
  //   byte2 (for sr2 / sevseg2 / units) first
  //   byte1 (for sr1 / sevseg1 / tens) second
  //
  // This ensures sr2 gets the first byte shifted into the chain.
  uint8_t patternSr2 = digitTo74hc595Pattern(onesDigit);
  uint8_t patternSr1 = digitTo74hc595Pattern(tensDigit);

  // Shift out: sr2 byte first, then sr1 byte.
  shiftOutByteToShiftRegister(patternSr2, true);
  shiftOutByteToShiftRegister(patternSr1, true);

  // Latch outputs
  pulseSTCP();
}

static void setRgbAll(RgbColor c) {
  // Both RGB LEDs display the same color.
  uint8_t r = 0, g = 0, b = 0;
  switch (c) {
    case RED:     r = 255; g = 0;   b = 0;   break;
    case GREEN:   r = 0;   g = 255; b = 0;   break;
    case BLUE:    r = 0;   g = 0;   b = 255; break;
    case YELLOW:  r = 255; g = 255; b = 0;   break;
    case CYAN:    r = 0;   g = 255; b = 255; break;
    case MAGENTA: r = 255; g = 0;   b = 255; break;
    case WHITE:   r = 255; g = 255; b = 255; break;
  }

  analogWrite(RGB1_R, r);
  analogWrite(RGB1_G, g);
  analogWrite(RGB1_B, b);

  analogWrite(RGB2_R, r);
  analogWrite(RGB2_G, g);
  analogWrite(RGB2_B, b);
}

void setup() {
  // ---- RGB outputs ----
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // ---- Shift register control ----
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  // Initialize shift register lines to a safe state
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Initial State:
  // - RGB LEDs show Red
  // - 7-seg displays show "00"
  setRgbAll(RED);
  updateDisplays(0, 0);
}

void loop() {
  static unsigned long lastTick = 0;
  static uint8_t colorIndex = 0; // 0..6
  static uint8_t number = 0;     // 0..99

  const unsigned long now = millis();
  if (now - lastTick >= INTERVAL_MS) {
    lastTick = now;

    // Advance color
    colorIndex = (colorIndex + 1) % 7;
    RgbColor c;
    switch (colorIndex) {
      case 0: c = RED; break;
      case 1: c = GREEN; break;
      case 2: c = BLUE; break;
      case 3: c = YELLOW; break;
      case 4: c = CYAN; break;
      case 5: c = MAGENTA; break;
      default: c = WHITE; break; // 6
    }
    setRgbAll(c);

    // Advance two-digit number every 2s; wraps at 99 -> 00
    number = (number + 1) % 100;
    uint8_t tens = number / 10;
    uint8_t ones = number % 10;
    updateDisplays(tens, ones);
  }
}

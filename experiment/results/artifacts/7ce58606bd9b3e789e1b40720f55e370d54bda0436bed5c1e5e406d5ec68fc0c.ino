#include <Arduino.h>

static inline void setBit(volatile uint16_t &val, uint8_t idx, bool on) {
  if (on) val |= (1u << idx);
  else val &= ~(1u << idx);
}

// Mapping for 7-seg segments A..G (and optional DP not used)
// We assume common cathode segments are driven ON by HIGH from the shift registers.
// Wiring from sr1 Q0..Q7S to sevseg1: A,B,C,D,E,F,G and (Q7S -> DS unused).
// sr1 Q0 -> A, Q1 -> B, Q2 -> C, Q3 -> D, Q4 -> E, Q5 -> F, Q6 -> G, Q7S -> (DS/DP)
static const uint8_t segA = 0;
static const uint8_t segB = 1;
static const uint8_t segC = 2;
static const uint8_t segD = 3;
static const uint8_t segE = 4;
static const uint8_t segF = 5;
static const uint8_t segG = 6;

// Segment patterns for digits 0..9 for segments A..G (common cathode, active HIGH)
static const bool digitSeg[10][7] = {
  // A, B, C, D, E, F, G
  { true,  true,  true,  true,  true,  true,  false }, // 0
  { false, true,  true,  false, false, false, false }, // 1
  { true,  true,  false, true,  true,  false, true  }, // 2
  { true,  true,  true,  true,  false, false, true  }, // 3
  { false, true,  true,  false, false, true,  true  }, // 4
  { true,  false, true,  true,  false, true,  true  }, // 5
  { true,  false, true,  true,  true,  true,  true  }, // 6
  { true,  true,  true,  false, false, false, false }, // 7
  { true,  true,  true,  true,  true,  true,  true  }, // 8
  { true,  true,  true,  true,  false, true,  true  }  // 9
};

// Shift register control pins (from wiring)
// sr1/2: SHCP=A2, STCP=A1, DS=A0
static const uint8_t SR_SHCP = A2;
static const uint8_t SR_STCP = A1;
static const uint8_t SR_DS   = A0;

// LEDs (direct to Mega pins)
static const uint8_t LED1 = 2;
static const uint8_t LED2 = 3;
static const uint8_t LED3 = 4;

// RGB1: R=5, G=6, B=7
static const uint8_t RGB1_R = 5;
static const uint8_t RGB1_G = 6;
static const uint8_t RGB1_B = 7;

// RGB2: R=8, G=9, B=10
static const uint8_t RGB2_R = 8;
static const uint8_t RGB2_G = 9;
static const uint8_t RGB2_B = 10;

// For 7-seg via sr1 (sevseg1) and sr2 (sevseg2).
// We will shift 16 bits total: first 8 bits for sr1, then 8 bits for sr2.
// DS is connected to sr1:DS and also sr2:DS via SHCP/STCP common.
// On each shift pulse, the DS bit is shifted into both, but the bit ends in sr1 Qx/ sr2 QxS
// based on cascade. With standard 74HC595 cascading, sr1 is earlier in chain and sr2 is later.
// We'll output MSB->LSB by shifting sr2 then sr1? To avoid ambiguity, we will use a known approach:
// For two cascaded 74HC595: shift register bits stream where the first 8 shifted end up in the last device.
// That is, if DS feeds sr1 first and sr1 Q7S feeds sr2 DS, then:
/// - Bits shifted out first go to sr2? Actually: DS -> sr1 serial input, sr1 Q7S -> sr2 serial input.
/// So the bit that enters sr1 first will exit sr2 after 16 shifts; therefore:
/// - After 16 shifts, the most recently shifted 8 bits reside in sr1, and the earliest 8 bits reside in sr2.
/// Therefore, to place desired patterns:
/// - Shift out: [pattern for sr2][pattern for sr1], 8 bits each, with sr2 bits sent first.
/// - Then latch.
static void writeDigits(uint8_t leftDigit, uint8_t rightDigit, bool showDP=false) {
  uint8_t leftSegBits = 0;  // for sevseg1 (sr1)
  uint8_t rightSegBits = 0; // for sevseg2 (sr2)

  auto makeBits = [&](uint8_t d) -> uint8_t {
    uint8_t bits = 0;
    // sr1 Q0..Q6 -> segments A..G; Q7S -> DP (DS), not wired on given 7segment attrs (but keep off)
    for (uint8_t s = 0; s < 7; s++) {
      if (digitSeg[d][s]) bits |= (1u << s);
    }
    // DP at bit 7 (Q7S / DS pin to segment DP)
    if (showDP) bits |= (1u << 7);
    return bits;
  };

  leftSegBits = makeBits(leftDigit);   // sevseg1
  rightSegBits = makeBits(rightDigit); // sevseg2

  // Shift out 16 bits: first sr2 (right display), then sr1 (left display).
  // We'll shift MSB->LSB within each 8-bit chunk to align Q0..Q7 mapping.
  // With cascade and our chosen shifting order, we just need consistent bit ordering.
  // We'll output bits from bit7 down to bit0; within 74HC595, bit order corresponds to serial shifting.
  // Since Q0..Q6 map to A..G, we set bit positions accordingly and keep order consistent.
  noInterrupts();
  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);

  // sr2 pattern first (right display)
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR_DS, (rightSegBits >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }
  // sr1 pattern next (left display)
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR_DS, (leftSegBits >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }

  // Latch
  digitalWrite(SR_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(SR_STCP, LOW);
  interrupts();
}

static void allRGBOff() {
  digitalWrite(RGB1_R, LOW);
  digitalWrite(RGB1_G, LOW);
  digitalWrite(RGB1_B, LOW);
  digitalWrite(RGB2_R, LOW);
  digitalWrite(RGB2_G, LOW);
  digitalWrite(RGB2_B, LOW);
}

static void rgbBoth(uint8_t r, uint8_t g, uint8_t b) {
  digitalWrite(RGB1_R, r ? HIGH : LOW);
  digitalWrite(RGB1_G, g ? HIGH : LOW);
  digitalWrite(RGB1_B, b ? HIGH : LOW);
  digitalWrite(RGB2_R, r ? HIGH : LOW);
  digitalWrite(RGB2_G, g ? HIGH : LOW);
  digitalWrite(RGB2_B, b ? HIGH : LOW);
}

static void allSingleLEDOff() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void setup() {
  // Shift register control pins
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_DS, OUTPUT);

  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_DS, LOW);

  // Single LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // RGB LEDs
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Initialization requirements:
  // - Displays show "99"
  // - All LEDs and RGB LEDs are off
  allSingleLEDOff();
  allRGBOff();
  writeDigits(9, 9, false);
}

void loop() {
  while (true) {
    // Countdown from 9 to 0; each step lasts 2 seconds.
    for (int value = 9; value >= 0; --value) {
      // Display current value as "value value"
      // (e.g., 8 -> "88", 0 -> "00")
      writeDigits((uint8_t)value, (uint8_t)value, false);

      // Single-color LEDs light up in sequence: led1, then led2, then led3,
      // each staying on for 2 seconds during the countdown step.
      allSingleLEDOff();
      if (value == 9) digitalWrite(LED1, HIGH);
      else if (value == 8) digitalWrite(LED2, HIGH);
      else if (value == 7) digitalWrite(LED3, HIGH);
      else if (value == 6) digitalWrite(LED1, HIGH);
      else if (value == 5) digitalWrite(LED2, HIGH);
      else if (value == 4) digitalWrite(LED3, HIGH);
      else if (value == 3) digitalWrite(LED1, HIGH);
      else if (value == 2) digitalWrite(LED2, HIGH);
      else if (value == 1) digitalWrite(LED3, HIGH);
      else if (value == 0) {
        // When countdown reaches 0:
        // all single-color LEDs should turn off
        allSingleLEDOff();
      }

      // RGB cycling through red, green, blue, changing every 2 seconds during countdown.
      // When countdown reaches 0: steady white light.
      if (value == 0) {
        rgbBoth(1, 1, 1); // steady white
      } else {
        // Determine color by step index (starting with value 9 at red)
        // Step order: 9->red, 8->green, 7->blue, 6->red, ...
        int stepIndex = 9 - value; // 0..9
        int mod = stepIndex % 3;
        if (mod == 0) rgbBoth(1, 0, 0);       // red
        else if (mod == 1) rgbBoth(0, 1, 0);  // green
        else rgbBoth(0, 0, 1);               // blue
      }

      delay(2000);
    }

    // At value 0, requirements say show "00" and keep LEDs/RGB as specified.
    // Immediately reset and restart countdown.
    allSingleLEDOff();
    allRGBOff();
    writeDigits(9, 9, false);
    delay(10); // small settle before restarting
  }
}
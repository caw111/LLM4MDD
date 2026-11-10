#include <Arduino.h>

// ===== Frozen interface bindings =====
// RGB PWM pins (Arduino Mega):
// rgb1: R=D2(io_0), G=D3(io_1), B=D4(io_2)
// rgb2: R=D5(io_3), G=D6(io_4), B=D7(io_5)
// Sevseg shift595 pins:
// SHCP= A2 (sr1->SHCP, also sr2), STCP= A1, DS= A0
// MR tied HIGH (sr1:MR to sr1 VCC, sr2:MR to sr1 VCC)
// OE tied LOW (sr1:GND.3->sr1:GND->OE low, also sr2:OE via sr1:OE)
// Shift register chain: sr1 controls sevseg1, sr2 controls sevseg2.
// DS is shared; latch updates both simultaneously.

static const uint8_t RGB1_R_PIN = 2; // io_0
static const uint8_t RGB1_G_PIN = 3; // io_1
static const uint8_t RGB1_B_PIN = 4; // io_2
static const uint8_t RGB2_R_PIN = 5; // io_3
static const uint8_t RGB2_G_PIN = 6; // io_4
static const uint8_t RGB2_B_PIN = 7; // io_5

static const uint8_t SR_DS_PIN  = A0; // data
static const uint8_t SR_STCP_PIN = A1; // latch
static const uint8_t SR_SHCP_PIN = A2; // clock

// Control period contract: 10ms
static const uint32_t CONTROL_PERIOD_MS = 10;

// Display update cadence: every 2 seconds increment; wrap 99->00
static const uint32_t SEGMENT_PERIOD_MS = 2000;

// Time-base at 10ms scheduling
static uint32_t nextTickMs = 0;

// Model states (must preserve exact controller state semantics)
static int32_t colorIndex = 0; // initial 0 => Red
static int32_t segValue = 0;    // initial 0 => "00"

// ===== Utility: set PWM =====
static inline void setPwmCount(uint8_t pin, uint8_t value255) {
  analogWrite(pin, value255); // Mega uses 8-bit PWM, 0..255
}

// ===== Color mapping =====
// Model outputs require: when colorIndex==0 => io_0=255, io_1/io_2=0; and rgb2 same.
// For other colors, implement the required sequence with both LEDs synchronized.
// Use simple saturated RGB mapping in 0..255 scale.
static void applyColorIndexToRgb(int32_t idx) {
  uint8_t r = 0, g = 0, b = 0;

  switch (idx) {
    case 0: // Red
      r = 255; g = 0;   b = 0;   break;
    case 1: // Green
      r = 0;   g = 255; b = 0;   break;
    case 2: // Blue
      r = 0;   g = 0;   b = 255; break;
    case 3: // Yellow
      r = 255; g = 255; b = 0;   break;
    case 4: // Cyan
      r = 0;   g = 255; b = 255; break;
    case 5: // Magenta
      r = 255; g = 0;   b = 255; break;
    case 6: // White
      r = 255; g = 255; b = 255; break;
    default:
      // If out of range, clamp/wrap to keep behavior defined
      idx = ((idx % 7) + 7) % 7;
      applyColorIndexToRgb(idx);
      return;
  }

  // rgb1 and rgb2 must match same color at same time
  setPwmCount(RGB1_R_PIN, r); setPwmCount(RGB1_G_PIN, g); setPwmCount(RGB1_B_PIN, b);
  setPwmCount(RGB2_R_PIN, r); setPwmCount(RGB2_G_PIN, g); setPwmCount(RGB2_B_PIN, b);
}

// ===== 7-segment shift595 number =====
// Frozen contract indicates shift595_number with:
// - leading_zero=true, invalid_policy=blank
// - two digits:
//   tens digit on sevseg1 (sr1 register), units digit on sevseg2 (sr2 register).
// - active_low segment bits: digit segment bits [0..6], active_low=true.
// Connections (sr1->sevseg1):
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// plus a Q7S that drives decimal point on sr2->DS (unused here).
//
// We'll implement conventional 7-seg digit segment mapping, active-low.
// Bit order in shift registers: [Q0..Q6,Q7] => [A,B,C,D,E,F,G,DP].
// Only segments A..G are used; DP left off (i.e., inactive).
static uint8_t digitToActiveLowSegmentsAtoG(int d) {
  // segments: A,B,C,D,E,F,G
  // active LOW => 0 means ON, 1 means OFF.
  // Patterns for common cathode displays driving segments directly would use ON=1.
  // We need active-low -> invert those patterns.
  // We'll use known common mapping:
  // Digit segments ON sets for A B C D E F G (without DP):
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
  bool onA=false,onB=false,onC=false,onD=false,onE=false,onF=false,onG=false;

  switch (d) {
    case 0: onA=onB=onC=onD=onE=onF=true; onG=false; break;
    case 1: onB=onC=true; break;
    case 2: onA=onB=true; onD=onE=true; onG=true; break;
    case 3: onA=onB=onC=onD=true; onG=true; break;
    case 4: onB=onC=true; onF=onG=true; break;
    case 5: onA=onC=onD=true; onF=true; onG=true; break;
    case 6: onA=onC=onD=true; onE=onF=onG=true; break;
    case 7: onA=onB=onC=true; break;
    case 8: onA=onB=onC=onD=onE=onF=onG=true; break;
    case 9: onA=onB=onC=onD=true; onF=true; onG=true; break;
    default: // blank -> all off (active-low => all segments OFF => bits=1)
      onA=onB=onC=onD=onE=onF=onG=false; break;
  }

  // active-low encoding for A..G bits in positions 0..6:
  // bit=0 means segment ON.
  uint8_t bits = 0;
  auto setBit = [&](int bitPos, bool segmentOn) {
    // active-low: ON->0, OFF->1
    if (!segmentOn) bits |= (uint8_t)(1u << bitPos);
  };
  setBit(0, onA); // Q0->A
  setBit(1, onB); // Q1->B
  setBit(2, onC); // Q2->C
  setBit(3, onD); // Q3->D
  setBit(4, onE); // Q4->E
  setBit(5, onF); // Q5->F
  setBit(6, onG); // Q6->G

  // DP (Q7) inactive/off: since active-low, DP OFF => bit=1
  bits |= (1u << 7);

  return bits;
}

static void shift595WriteTwoDigits(int32_t value) {
  int tens = (value / 10) % 10;
  int ones = value % 10;

  // We must shift data to both registers in one chain and latch both.
  // Order assumption for typical chained 74HC595:
  // - First shifted bits end up in the farthest device (sr2) depending on wiring.
  // In this circuit:
  //   sr1:DS=A0 feeds sr1 serial input.
  //   sr1:Q7S connects to sr2:DS.
  // Therefore, bits shifted out of sr1 towards Q7S then enter sr2 serial.
  // That means the first 8 bits shifted will be for sr2 or sr1 depending on convention.
  // Let's derive: For N chained 74HC595, shifting MSB-first typically maps:
  // - The bit stream enters the first (nearest) register; after 8 clocks, those bits reside in it.
  // - After another 8 clocks, subsequent bits reside in the second register.
  // With sr1 as first in chain (A0 -> sr1 DS), the first 8 shifted bits should end in sr1,
  // then next 8 bits in sr2.
  //
  // But latch updates all, and our display mapping needs:
  // - sevseg1 (sr1) shows tens
  // - sevseg2 (sr2) shows ones
  //
  // We'll shift 8 bits for sr1 first (tens), then 8 bits for sr2 (ones).
  uint8_t sr1Byte = digitToActiveLowSegmentsAtoG(tens);
  uint8_t sr2Byte = digitToActiveLowSegmentsAtoG(ones);

  // Shift out MSB->LSB
  noInterrupts();
  digitalWrite(SR_STCP_PIN, LOW);

  auto shiftOutByte = [&](uint8_t b) {
    for (int i = 7; i >= 0; --i) {
      digitalWrite(SR_DS_PIN, (b >> i) & 0x01 ? HIGH : LOW);
      digitalWrite(SR_SHCP_PIN, HIGH);
      // small hold
      digitalWrite(SR_SHCP_PIN, LOW);
    }
  };

  shiftOutByte(sr1Byte);
  shiftOutByte(sr2Byte);

  digitalWrite(SR_STCP_PIN, HIGH);
  interrupts();
}

// ===== setup/loop =====
void setup() {
  // RGB PWM pins
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);
  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  // Shift register pins
  pinMode(SR_DS_PIN, OUTPUT);
  pinMode(SR_STCP_PIN, OUTPUT);
  pinMode(SR_SHCP_PIN, OUTPUT);

  digitalWrite(SR_DS_PIN, LOW);
  digitalWrite(SR_STCP_PIN, LOW);
  digitalWrite(SR_SHCP_PIN, LOW);

  // Initial state per contract:
  // - RGB display Red (colorIndex=0 => R high, G/B low)
  // - sevseg display "00" (segValue=0)
  colorIndex = 0;
  segValue = 0;

  applyColorIndexToRgb(colorIndex);
  shift595WriteTwoDigits(segValue);

  nextTickMs = millis();
}

void loop() {
  // 10ms control period scheduling
  while ((int32_t)(millis() - nextTickMs) >= 0) {
    nextTickMs += CONTROL_PERIOD_MS;

    // Update cadence: "independent but synchronously" means both change on 2s boundaries.
    // We'll implement a single shared 2s counter tick for both state machines.
    static uint32_t segElapsedMs = 0;
    segElapsedMs += CONTROL_PERIOD_MS;

    if (segElapsedMs >= SEGMENT_PERIOD_MS) {
      // Advance exactly on 2s boundaries as long as scheduler aligns closely.
      // If drift caused multiple periods, handle multiples (rare).
      do {
        segElapsedMs -= SEGMENT_PERIOD_MS;

        // RGB color advance (2 seconds each), sequence length 7
        colorIndex = (colorIndex + 1) % 7;

        // Segment value advance, wrap 99->00
        segValue = segValue + 1;
        if (segValue > 99) segValue = 0;
      } while (segElapsedMs >= SEGMENT_PERIOD_MS);
    }

    // Apply outputs each control tick (keeping exact contract structure).
    // RGB outputs are purely functions of colorIndex.
    applyColorIndexToRgb(colorIndex);

    // sevseg output is a function of segValue
    shift595WriteTwoDigits(segValue);
  }
}
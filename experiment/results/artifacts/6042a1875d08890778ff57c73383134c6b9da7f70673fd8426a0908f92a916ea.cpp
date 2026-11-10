#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// ===== Pins (from provided circuit wiring / Frozen contract) =====
// RGB1: D2 (R), D3 (G), D4 (B)  (common cathode, COM to GND.1)
static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

// RGB2: D5 (R), D6 (G), D7 (B)
static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

// Shift-register chain for 2 digits:
// SR1: clock=A2 (SHCP), latch=A1 (STCP), data=A0 (DS), MR tied to SR1 VCC, OE tied to GND.
// SR2 shares clock/latch/OE, DS comes from SR1 Q7S.
static const uint8_t SR_CLOCK = A2; // SHCP
static const uint8_t SR_LATCH = A1; // STCP
static const uint8_t SR_DATA  = A0; // DS

// Bar graph segments (8 physical outputs as per circuit bindings)
static const uint8_t BAR0 = 8;   // A1
static const uint8_t BAR1 = 9;   // A2
static const uint8_t BAR2 = 10;  // A3
static const uint8_t BAR3 = 11;  // A4
static const uint8_t BAR4 = 12;  // A5
static const uint8_t BAR5 = 13;  // A6
static const uint8_t BAR6 = 44;  // A7
static const uint8_t BAR7 = 45;  // A8

// ===== 7-seg segment bits mapping (copied from frozen interface facts) =====
// The frozen contract states shift595_number uses:
static const uint8_t DIGIT0_SEG_BITS[7] = {0,1,2,3,4,5,6}; // sevseg1 digit 0 -> sr1 Q0..Q6
static const uint8_t DIGIT1_SEG_BITS[7] = {0,1,2,3,4,5,6}; // sevseg2 digit 1 -> sr2 Q0..Q6

// Active-low segments in the provided contract ("active_low": true).
// Segment pins on sevseg: A,B,C,D,E,F,G (decimal points not used).
// From circuit:
// sevseg1: A->sr1:Q0, B->Q1, C->Q2, D->Q3, E->Q4, F->Q5, G->Q6
// sevseg2: A->sr2:Q0, B->Q1, C->Q2, D->Q3, E->Q4, F->Q5, G->Q6
//
// Therefore we implement standard 7-seg encoding (common cathode LED logic)
// but because outputs are active-low, we invert before shifting.
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t encodeDigitSegmentsActiveLow(uint8_t digit);
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595_number_twoDigits(uint8_t leftDigit, uint8_t rightDigit);
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setRGBOff();
#line 135 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setRGBForValue(uint8_t value);
#line 157 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBarGraphTens(uint8_t tensDigit);
#line 173 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick2sAndUpdateOutputs();
#line 203 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 250 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_62\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t encodeDigitSegmentsActiveLow(uint8_t digit) {
  // Bit positions: [A,B,C,D,E,F,G] -> bit0..bit6
  // We'll create active-high "segment on" for common-cathode convention,
  // then invert for active-low shift-register outputs.
  uint8_t segOn = 0;

  // Common 7-seg (a,b,c,d,e,f,g):
  // 0: a b c d e f
  // 1: b c
  // 2: a b d e g
  // 3: a b c d g
  // 4: b c f g
  // 5: a c d f g
  // 6: a c d e f g
  // 7: a b c
  // 8: a b c d e f g
  // 9: a b c d f g
  switch (digit) {
    case 0: segOn = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5); break; // A B C D E F
    case 1: segOn = (1<<1)|(1<<2); break; // B C
    case 2: segOn = (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6); break; // A B D E G
    case 3: segOn = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6); break; // A B C D G
    case 4: segOn = (1<<1)|(1<<2)|(1<<5)|(1<<6); break; // B C F G
    case 5: segOn = (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6); break; // A C D F G
    case 6: segOn = (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); break; // A C D E F G
    case 7: segOn = (1<<0)|(1<<1)|(1<<2); break; // A B C
    case 8: segOn = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); break; // all
    case 9: segOn = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6); break; // A B C D F G
    default: segOn = 0; break;
  }

  // Active-low: segment output bit should be 0 when we want LED on.
  // So produce active-low mask: bit=1 means OFF.
  uint8_t segOffMask = (~segOn) & 0x7F;
  return segOffMask;
}

// Shift registers:
// The frozen contract describes a "shift595_number" that loads two registers:
// digit 0 (sevseg1) -> sr1; digit 1 (sevseg2) -> sr2.
// Segment bits and active_low are copied exactly; DS comes from SR1 through SR1 Q7S to SR2 DS.
// That implies we must shift out 16 bits total in the order consistent with 74HC595:
static void shift595_number_twoDigits(uint8_t leftDigit, uint8_t rightDigit) {
  // Compute active-low segment patterns for each digit.
  uint8_t leftSegOff  = encodeDigitSegmentsActiveLow(leftDigit);  // A..G off-mask, bit0..bit6
  uint8_t rightSegOff = encodeDigitSegmentsActiveLow(rightDigit);

  // 74HC595 has 8 outputs Q0..Q7. Only Q0..Q6 are used for A..G.
  // Q7 isn't connected (no requirement). We'll set it to OFF (i.e., segment inactive) for active-low.
  // Since active-low, "off" means output HIGH. We maintain bit7=1 to be OFF.
  uint8_t leftByte  = (leftSegOff & 0x7F) | (1u << 7);
  uint8_t rightByte = (rightSegOff & 0x7F) | (1u << 7);

  // Determine shifting order:
  // For a daisy-chain of two 74HC595, data shifted first ends up in the farthest (SR2),
  // then later bits in SR1, depending on wiring. Typical Arduino daisy chain expects:
  // - Shift out SR2 byte first (far), then SR1 byte last (near).
  // We'll follow this conventional ordering.
  const uint8_t farByte  = rightByte; // SR2
  const uint8_t nearByte = leftByte;  // SR1

  // Ensure latch low during shifting.
  digitalWrite(SR_LATCH, LOW);
  digitalWrite(SR_CLOCK, LOW);

  // Shift out 16 bits, MSB first for each byte.
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_DATA, (farByte >> i) & 1 ? HIGH : LOW);
    digitalWrite(SR_CLOCK, HIGH);
    digitalWrite(SR_CLOCK, LOW);
  }
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_DATA, (nearByte >> i) & 1 ? HIGH : LOW);
    digitalWrite(SR_CLOCK, HIGH);
    digitalWrite(SR_CLOCK, LOW);
  }

  // Latch updated outputs simultaneously.
  digitalWrite(SR_LATCH, HIGH);
  digitalWrite(SR_LATCH, LOW);
}

static void setRGBOff() {
  analogWrite(RGB1_R, 0);
  analogWrite(RGB1_G, 0);
  analogWrite(RGB1_B, 0);
  analogWrite(RGB2_R, 0);
  analogWrite(RGB2_G, 0);
  analogWrite(RGB2_B, 0);
}

static void setRGBForValue(uint8_t value) {
  // Contract: ranges:
  // Red: 0-5, Green: 6-9, Blue: 10-99
  // Also: model controls which channel is nonzero; initial is 0 (off).
  // Use full intensity for the active color (0 otherwise).
  setRGBOff();

  if (value <= 5) {
    // Red
    analogWrite(RGB1_R, 255);
    analogWrite(RGB2_R, 255);
  } else if (value <= 9) {
    // Green
    analogWrite(RGB1_G, 255);
    analogWrite(RGB2_G, 255);
  } else {
    // Blue (10..99)
    analogWrite(RGB1_B, 255);
    analogWrite(RGB2_B, 255);
  }
}

static void setBarGraphTens(uint8_t tensDigit) {
  // Contract: bar graph lights segments equal to tens digit,
  // but capped at 8 for 80..99.
  if (tensDigit > 8) tensDigit = 8;

  // Map across 8 outputs (io_7_segment_0..7).
  const uint8_t count = tensDigit; // 0..8
  uint8_t pins[8] = {BAR0, BAR1, BAR2, BAR3, BAR4, BAR5, BAR6, BAR7};
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(pins[i], (i < count) ? HIGH : LOW); // active_high per contract
  }
}

int countModel = 0;     // controller state: count
int elapsedMsModel = 0; // controller state: elapsedMs

static void controllerTick2sAndUpdateOutputs() {
  // Frozen model behavior: elapsedMs is accumulated with 10ms loop period,
  // and count increments by 1 every 2 seconds (2000ms), wrapping 0..99.
  // Additionally, outputs update simultaneously at each increment.
  // We'll implement with 10ms base to respect control_period_ms=10.
  elapsedMsModel += 10;

  if (elapsedMsModel >= 2000) {
    // Match "displayed number increments by 1 every 2 seconds"
    // If drift occurs, we still increment once and subtract 2000 to preserve cadence.
    elapsedMsModel -= 2000;

    countModel = (countModel + 1) % 100;

    // Synchronous outputs for this increment.
    uint8_t v = (uint8_t)countModel;
    uint8_t tens = v / 10;
    uint8_t ones = v % 10;

    // 7-seg displays always show current number as two digits.
    shift595_number_twoDigits(0 + tens, 0 + ones);

    // Bargraph shows tens digit segments (capped at 8).
    setBarGraphTens(tens);

    // RGB color based on value ranges.
    setRGBForValue(v);
  }
}

void setup() {
  // Initialize model per frozen controller: count=0, elapsedMs=0.
  countModel = 0;
  elapsedMsModel = 0;

  // Pin modes for RGB PWM pins.
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Bar graph pins.
  pinMode(BAR0, OUTPUT);
  pinMode(BAR1, OUTPUT);
  pinMode(BAR2, OUTPUT);
  pinMode(BAR3, OUTPUT);
  pinMode(BAR4, OUTPUT);
  pinMode(BAR5, OUTPUT);
  pinMode(BAR6, OUTPUT);
  pinMode(BAR7, OUTPUT);

  // Shift register pins.
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_DATA, OUTPUT);

  digitalWrite(SR_CLOCK, LOW);
  digitalWrite(SR_LATCH, LOW);
  digitalWrite(SR_DATA, LOW);

  // Initialization requirements:
  // - Both 7-segment displays show "00"
  // - All bar graph LEDs are off
  // - RGB LEDs are off

  shift595_number_twoDigits(0, 0);

  // bar graph all off
  uint8_t pins[8] = {BAR0, BAR1, BAR2, BAR3, BAR4, BAR5, BAR6, BAR7};
  for (uint8_t i = 0; i < 8; ++i) digitalWrite(pins[i], LOW);

  // rgb off
  setRGBOff();
}

void loop() {
  // Control period: 10ms.
  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last >= 10) {
    // Keep exact 10ms increments for the model's elapsedMsModel.
    last += 10;

    controllerTick2sAndUpdateOutputs();
  }
}

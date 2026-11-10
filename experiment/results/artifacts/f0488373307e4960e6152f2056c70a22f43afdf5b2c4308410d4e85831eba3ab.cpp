#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_4_digit_7_segment_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// 4-digit 7-segment display (Common Anode)
// Wiring (preserve as provided):
// Segments A..G,DP -> Arduino pins 2..9
// Digit selects DIG1..DIG4 -> Arduino pins 10..13
//
// Common anode: COM connected to +5V.
// Segment ON  : drive segment pin LOW
// Segment OFF : drive segment pin HIGH
// Digit ON    : drive DIG pin LOW
// Digit OFF   : drive DIG pin HIGH

#include <Arduino.h>

static const uint8_t SEG_PINS[8] = {2, 3, 4, 5, 6, 7, 8, 9};   // A,B,C,D,E,F,G,DP
static const uint8_t DIG_PINS[4] = {10, 11, 12, 13};             // DIG1..DIG4

// Segment bit order: A,B,C,D,E,F,G,DP (bit 0..7)
// Value indicates which segments are ON.
static const uint8_t DIGIT_TO_SEG_ON[10] = {
  // 0
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),                 // A B C D E F
  // 1
  (1 << 1) | (1 << 2),                                                                     // B C
  // 2
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),                                 // A B D E G
  // 3
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),                                 // A B C D G
  // 4
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),                                             // B C F G
  // 5
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),                                 // A C D F G
  // 6
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),                     // A C D E F G
  // 7
  (1 << 0) | (1 << 1) | (1 << 2),                                                         // A B C
  // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),         // A B C D E F G
  // 9
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)                        // A B C D F G
};

#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_4_digit_7_segment_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllSegmentsOff();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_4_digit_7_segment_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setSegments(uint8_t segOnMaskWithDP);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_4_digit_7_segment_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllDigitsOff();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_4_digit_7_segment_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showDigitOnDigitIndex(uint8_t digitValue, uint8_t digitIndex0to3);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_4_digit_7_segment_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_4_digit_7_segment_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_4_digit_7_segment_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllSegmentsOff() {
  // For common anode: OFF = HIGH
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SEG_PINS[i], HIGH);
  }
}

static void setSegments(uint8_t segOnMaskWithDP) {
  // For common anode: ON = LOW, OFF = HIGH
  for (uint8_t i = 0; i < 8; i++) {
    const bool on = (segOnMaskWithDP & (1 << i)) != 0;
    digitalWrite(SEG_PINS[i], on ? LOW : HIGH);
  }
}

static void setAllDigitsOff() {
  // DIG ON = LOW, OFF = HIGH
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(DIG_PINS[i], HIGH);
  }
}

static void showDigitOnDigitIndex(uint8_t digitValue, uint8_t digitIndex0to3) {
  // Ensure digit select only for the requested position
  setAllDigitsOff();
  setSegments((digitValue < 10) ? DIGIT_TO_SEG_ON[digitValue] : 0);

  // Enable requested digit
  digitalWrite(DIG_PINS[digitIndex0to3], LOW);
}

void setup() {
  // 1. Set 2~13 pin as output.
  for (uint8_t i = 0; i < 8; i++) pinMode(SEG_PINS[i], OUTPUT);
  for (uint8_t i = 0; i < 4; i++) pinMode(DIG_PINS[i], OUTPUT);

  // Start from all off
  setAllSegmentsOff();
  setAllDigitsOff();
}

void loop() {
  // 2. Display number 1 on digit1, 2 on digit2, 3 on digit3, 4 on digit4,
  //    with an 1000ms delay between each step.
  showDigitOnDigitIndex(1, 0); // digit1 (DIG1)
  delay(1000);

  showDigitOnDigitIndex(2, 1); // digit2 (DIG2)
  delay(1000);

  showDigitOnDigitIndex(3, 2); // digit3 (DIG3)
  delay(1000);

  showDigitOnDigitIndex(4, 3); // digit4 (DIG4)
  delay(1000);
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t SEG_PINS[8] = {2, 3, 4, 5, 6, 7, 8, 9}; // A,B,C,D,E,F,G,DP

// Common anode: HIGH turns a segment OFF, LOW turns a segment ON.
static const uint8_t DIGIT_TO_SEGMENTS_COMMON_ANODE[10] = {
  //  A, B, C, D, E, F, G, DP  (DP always OFF)
  //  bit = 1 => OFF (HIGH), bit = 0 => ON (LOW) when written as LOW=on for that segment
  // We'll store as direct outputs (HIGH/LOW) per segment pin below.
  0,1,2,3,4,5,6,7 // placeholder to satisfy compiler (overwritten below)
};

// We'll store patterns directly as HIGH/LOW per segment order A,B,C,D,E,F,G,DP.
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t pattern(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t dp);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDigit(uint8_t d);
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_7_segment_display_counter\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t pattern(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t dp) {
  // each parameter is either HIGH (off) or LOW (on) for common-anode.
  // returns not used; just for readability.
  uint8_t arr[8] = {a, b, c, d, e, f, g, dp};
  (void)arr;
  return 0;
}

static const uint8_t SEG_PATTERN[10][8] = {
  // Digits for common anode, DP off (HIGH)
  // Order: A B C D E F G DP
  {LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  HIGH, HIGH}, // 0
  {HIGH, LOW,  LOW,  HIGH, HIGH, HIGH, HIGH, HIGH}, // 1
  {LOW,  LOW,  HIGH, LOW,  LOW,  HIGH, LOW,  HIGH}, // 2
  {LOW,  LOW,  LOW,  LOW,  HIGH, HIGH, LOW,  HIGH}, // 3
  {HIGH, LOW,  LOW,  HIGH, HIGH, LOW,  LOW,  HIGH}, // 4
  {LOW,  HIGH, LOW,  LOW,  HIGH, LOW,  LOW,  HIGH}, // 5
  {LOW,  HIGH, LOW,  LOW,  LOW,  LOW,  LOW,  HIGH}, // 6
  {LOW,  LOW,  LOW,  HIGH, HIGH, HIGH, HIGH, HIGH}, // 7
  {LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  HIGH}, // 8
  {LOW,  LOW,  LOW,  LOW,  HIGH, LOW,  LOW,  HIGH}  // 9
};

void displayDigit(uint8_t d) {
  if (d > 9) return;
  // Write segments A..G,DP to pins 2..9 respectively.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SEG_PINS[i], SEG_PATTERN[d][i]);
  }
}

void setup() {
  // 1. Set 2~9 pin as an output.
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(SEG_PINS[i], OUTPUT);
  }

  // On common-anode display, we must set proper levels to show elapsed time.
  // Initialize with 0 (or blank) is not specified; choose 0.
  displayDigit(0);
}

void loop() {
  // 2. Show elapsed time in seconds on the 7-segment display.
  // Success criteria only requires 1..3 appear between 1–2s, 2–3s, 3–4s.
  unsigned long ms = millis();
  unsigned long sec = ms / 1000UL;

  // Determine digit for elapsed seconds (1->1, 2->2, 3->3, others not required).
  // We'll display mod-10 for general behavior, but ensure 1-3 intervals are correct.
  uint8_t digit = (uint8_t)(sec % 10UL);
  displayDigit(digit);
}

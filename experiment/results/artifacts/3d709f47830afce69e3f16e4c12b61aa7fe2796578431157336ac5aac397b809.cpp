#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_7_segment_display_counter\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Segment order: A, B, C, D, E, F, G, DP.
// Each pin connects through a 330-ohm resistor; common anode connects to 5V.
const uint8_t segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// Bits 0..6 represent segments A..G. A set bit means illuminated.
const uint8_t digitPatterns[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_7_segment_display_counter\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDigit(uint8_t digit);
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_7_segment_display_counter\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_7_segment_display_counter\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_7_segment_display_counter\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDigit(uint8_t digit) {
  const uint8_t pattern = digitPatterns[digit];

  for (uint8_t segment = 0; segment < 7; ++segment) {
    // Common anode: LOW illuminates a segment; HIGH turns it off.
    digitalWrite(segmentPins[segment],
                 (pattern & (1U << segment)) ? LOW : HIGH);
  }

  digitalWrite(segmentPins[7], HIGH); // Decimal point remains off.
}

void setup() {
  for (uint8_t i = 0; i < 8; ++i) {
    // Preload HIGH before enabling output to avoid startup flashes.
    digitalWrite(segmentPins[i], HIGH);
    pinMode(segmentPins[i], OUTPUT);
  }

  displayDigit(0);
}

void loop() {
  // Show the units digit of elapsed seconds:
  // 0 during 0–1 s, 1 during 1–2 s, 2 during 2–3 s, etc.
  const uint8_t digit = (millis() / 1000UL) % 10UL;
  static uint8_t previousDigit = 0;

  if (digit != previousDigit) {
    displayDigit(digit);
    previousDigit = digit;
  }
}

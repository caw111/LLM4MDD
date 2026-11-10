#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_4_digit_7_segment_display\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Segment order: A, B, C, D, E, F, G, DP.
const uint8_t segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t digitPins[4] = {10, 11, 12, 13};

// A set bit lights the corresponding segment (A = bit 0).
const uint8_t numberPatterns[4] = {
  0b00000110,  // 1: B, C
  0b01011011,  // 2: A, B, D, E, G
  0b01001111,  // 3: A, B, C, D, G
  0b01100110   // 4: B, C, F, G
};

#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_4_digit_7_segment_display\\direct\\attempt_0\\sketch\\sketch.ino"
void showDigit(uint8_t index);
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_4_digit_7_segment_display\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_4_digit_7_segment_display\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_4_digit_7_segment_display\\direct\\attempt_0\\sketch\\sketch.ino"
void showDigit(uint8_t index) {
  // Disable all common-anode digits before changing segment states.
  for (uint8_t digit = 0; digit < 4; ++digit) {
    digitalWrite(digitPins[digit], LOW);
  }

  // Common-anode segments are active LOW; DP remains off.
  for (uint8_t segment = 0; segment < 8; ++segment) {
    bool illuminated = (numberPatterns[index] & (1U << segment)) != 0;
    digitalWrite(segmentPins[segment], illuminated ? LOW : HIGH);
  }

  digitalWrite(digitPins[index], HIGH);
}

void setup() {
  // Initialize all pins 2 through 13 as outputs with the display blank.
  for (uint8_t pin = 2; pin <= 13; ++pin) {
    digitalWrite(pin, pin <= 9 ? HIGH : LOW);
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  // Display 1 on DIG1, then 2 on DIG2, 3 on DIG3, and 4 on DIG4.
  for (uint8_t index = 0; index < 4; ++index) {
    showDigit(index);
    delay(1000);
  }
}

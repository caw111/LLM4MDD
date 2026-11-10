#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Arduino Uno pin constants for CLK=D4, DIO=D5.
// On AVR/Arduino, D4/D5 are not predefined macros in this context, so use numeric pins.
static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

TM1637Display display(TM1637_CLK, TM1637_DIO);

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint16_t mapAdcTo0_9999(uint16_t adc);
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline uint16_t mapAdcTo0_9999(uint16_t adc) {
  // Linear mapping: 0..1023 -> 0..9999, floor
  // value = floor(adc * 9999 / 1023)
  return (uint32_t)adc * 9999UL / 1023UL;
}

void setup() {
  // Display always on and ready
  display.setBrightness(0x0F); // max brightness
  display.clear();
}

void loop() {
  uint16_t adc = (uint16_t)analogRead(A0);        // 0..1023
  uint16_t val = mapAdcTo0_9999(adc);             // 0..9999, floor

  // Display full decimal number on 4 digits.
  // Keep leading zeros/allowed blank behavior per library defaults:
  // use showNumberDec(val, false, 4, 0) as in the provided sketch.
  display.showNumberDec((int)val, false, 4, 0);

  // No delay: response must be <= 50ms including bus transfer.
}

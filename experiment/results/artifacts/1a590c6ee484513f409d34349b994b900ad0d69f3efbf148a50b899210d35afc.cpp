#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Arduino Uno: D4/D5 are digital pins; use numeric pin IDs to avoid macro scope issues.
static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

TM1637Display display(TM1637_CLK, TM1637_DIO);

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup() {
  // Display always on
  display.setBrightness(0x0f);

  // Ensure initialized state; show 0 with leading zeros as full decimal on 4 digits
  // showNumberDec(number, leadingZeros, length, dots)
  display.showNumberDec(0, true, 4, 0);
}

void loop() {
  // A0 analog input: 0..1023 -> map to 0..9999 with floor
  const uint16_t adc = analogRead(A0); // 10-bit ADC
  const uint32_t mapped = (static_cast<uint32_t>(adc) * 9999UL) / 1023UL; // floor

  // Display full decimal number with leading zeros allowed on 4 digits
  display.showNumberDec(static_cast<uint16_t>(mapped), true, 4, 0);
}

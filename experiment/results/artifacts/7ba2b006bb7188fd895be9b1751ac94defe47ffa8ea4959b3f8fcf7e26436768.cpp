#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t TM1637_CLK = D4;
static const uint8_t TM1637_DIO = D5;

// TM1637Display treats "blank" digits as no-segment; "clearDisplay" blanks all.
// To keep "display always开启" and show full decimal value (allow leading zeros),
// we update the 4-digit display on every loop iteration.
TM1637Display display(TM1637_CLK, TM1637_DIO);

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Ensure display is on and ready
  display.setBrightness(0x0f);
  display.setSegments(nullptr); // no-op safety for some versions
  display.showNumberDec(0, true, 4, 0); // leading zeros, 4 digits, dummy

  // Optional: keep deterministic timing behavior
  // (No extra behavior required by spec)
}

void loop() {
  // Read A0 (0..1023) and map to 0..9999 using downward flooring
  // 9999 * adc / 1023 with integer division floors.
  const uint16_t adc = analogRead(A0);
  const uint32_t mapped = (static_cast<uint32_t>(adc) * 9999UL) / 1023UL;

  // Show complete decimal number. For 0..9999, showNumberDec with leadingZeros=true
  // will display full 4-digit decimal with leading zeros allowed.
  // Include delay-free behavior to meet <=50ms response requirement.
  display.showNumberDec(static_cast<uint16_t>(mapped), true, 4, 0);
}

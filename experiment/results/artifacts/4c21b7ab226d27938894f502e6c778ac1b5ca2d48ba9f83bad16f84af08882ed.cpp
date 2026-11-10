#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// TM1637 4-digit display
// CLK -> D2, DIO -> D3
static const uint8_t TM1637_CLK_PIN = 2;
static const uint8_t TM1637_DIO_PIN = 3;

TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Set brightness (0..7). Requirement doesn't specify; choose a sensible default.
  display.setBrightness(5);

  // Display 1234
  display.showNumberDec(1234, true); // leading zero suppression enabled (doesn't affect 1234)
}

void loop() {
  // Keep displaying 1234 continuously as required
  display.showNumberDec(1234, true);
}

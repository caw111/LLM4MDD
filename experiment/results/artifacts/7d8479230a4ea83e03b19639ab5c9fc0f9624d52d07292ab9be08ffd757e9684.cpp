#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <TM1637Display.h>

const int BUTTON_PIN = 2;
const int TM1637_CLK = 4;  // CLK = D4
const int TM1637_DIO = 5;  // DIO = D5

TM1637Display display(TM1637_CLK, TM1637_DIO);

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.setBrightness(0x0f); // max brightness
}

void loop() {
  // D2 button pressed => display 1111, otherwise 0000
  bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if (pressed) {
    display.showNumberDec(1111, false, 4, 0);
  } else {
    display.showNumberDec(0, false, 4, 0);
  }

  delay(20); // simple debounce
}

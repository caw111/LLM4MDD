#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(12, OUTPUT); // D12
  pinMode(13, OUTPUT); // D13
}

void loop() {
  static bool state = false;

  // 每 1 秒切换一次两个 LED 的状态
  // LED1: D12, LED2: D13
  state = !state;
  digitalWrite(12, state ? HIGH : LOW);
  digitalWrite(13, state ? LOW : HIGH);

  delay(1000);
}

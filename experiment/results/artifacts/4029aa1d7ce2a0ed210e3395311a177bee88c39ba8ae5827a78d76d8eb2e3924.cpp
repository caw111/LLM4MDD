#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(A0);       // 0..1023
  Serial.println(value);            // line_or_idle: newline-delimited output
  delay(1000);                      // 每秒输出一次
}

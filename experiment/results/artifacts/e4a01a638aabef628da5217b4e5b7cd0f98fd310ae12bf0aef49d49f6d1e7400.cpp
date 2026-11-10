#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
}

void loop() {
  static unsigned long lastPrintMs = 0;

  unsigned long now = millis();
  if (now - lastPrintMs >= 2000UL) {
    lastPrintMs = now;
    Serial.println("Hello Arduino");
  }
}

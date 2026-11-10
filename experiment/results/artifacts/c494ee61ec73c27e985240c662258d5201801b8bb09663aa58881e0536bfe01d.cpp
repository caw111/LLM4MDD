#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_RGB_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_RGB_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_RGB_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_RGB_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);

  // RGB LED common (COM) is connected to 5V.
  // Turn on only blue segment at full brightness (active LOW).
  digitalWrite(4, HIGH); // R off
  digitalWrite(3, HIGH); // G off
  digitalWrite(2, LOW);  // B on
}

void loop() {
  // No actions required
}

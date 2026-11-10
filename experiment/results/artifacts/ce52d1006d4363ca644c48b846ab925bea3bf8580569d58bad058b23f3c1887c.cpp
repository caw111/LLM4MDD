#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_20\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;

#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_20\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_20\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_20\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  servo1.attach(2);      // mega:2 -> servo1:PWM (orange)
  servo1.write(0);       // start at 0° on power/reset
}

void loop() {
  // Move from 0° to 180° in increments of 10°, hold 2s at each position
  for (int pos = 0; pos <= 180; pos += 10) {
    servo1.write(pos);
    delay(2000);
  }

  // Move back from 180° to 0° in decrements of 10°, hold 2s at each position
  for (int pos = 180; pos >= 0; pos -= 10) {
    servo1.write(pos);
    delay(2000);
  }
}

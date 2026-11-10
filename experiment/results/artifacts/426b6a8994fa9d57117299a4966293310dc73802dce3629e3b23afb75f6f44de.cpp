#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // D9 used for PWM output (Arduino UNO: pin 9 supports PWM)
  pinMode(9, OUTPUT);
}

void loop() {
  // Brightness from 0 -> 255 then repeat
  for (int value = 0; value <= 255; value++) {
    analogWrite(9, value);
    delay(10); // Smooth time-based change
  }
  // Loop repeats automatically
}

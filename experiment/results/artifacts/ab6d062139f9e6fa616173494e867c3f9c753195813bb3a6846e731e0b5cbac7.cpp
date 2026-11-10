#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Read potentiometer connected to A0 and control PWM LED brightness on D9.

const uint8_t PWM_PIN = 9;   // D9 (PWM output)
const uint8_t POT_PIN = A0;  // A0 (analog input)

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(PWM_PIN, OUTPUT);
}

void loop() {
  int potValue = analogRead(POT_PIN); // 0..1023
  // Map to 8-bit PWM duty: 0..255
  uint8_t duty = (uint8_t)((potValue * 255L) / 1023L);
  analogWrite(PWM_PIN, duty);
}

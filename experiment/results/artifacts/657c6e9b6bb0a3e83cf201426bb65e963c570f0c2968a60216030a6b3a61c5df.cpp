#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_led_RGB_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_led_RGB_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_led_RGB_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_led_RGB_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(4, OUTPUT); // R pin
  pinMode(3, OUTPUT); // G pin
  pinMode(2, OUTPUT); // B pin

  // Common pin is connected to 5V, so active segment is driven low.
  digitalWrite(4, HIGH); // R off
  digitalWrite(3, HIGH); // G off
  digitalWrite(2, LOW);  // B on (full brightness)
}

void loop() {
  // No actions required
}

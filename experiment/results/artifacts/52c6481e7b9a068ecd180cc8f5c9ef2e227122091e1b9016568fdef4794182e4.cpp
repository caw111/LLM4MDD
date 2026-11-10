#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_92\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
const int LED_PIN = 2;     // led1:A -> mega:2
const int POT1_PIN = A3;   // pot1:SIG -> mega:A3
const int POT2_PIN = A4;   // pot2:SIG -> mega:A4

#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_92\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_92\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_92\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0); // LED off on power-up/reset
}

void loop() {
  int pot1Value = analogRead(POT1_PIN); // 0..1023
  int pot2Value = analogRead(POT2_PIN); // 0..1023

  int brightness = map(pot1Value, 0, 1023, 0, 255); // 0..255

  if (pot1Value > pot2Value) {
    analogWrite(LED_PIN, brightness);
  } else {
    analogWrite(LED_PIN, 0);
  }
}

#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_photoresistor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_photoresistor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_photoresistor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_photoresistor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(A2);
  Serial.print("LDR Value: ");
  Serial.println(value);
  delay(500);
}

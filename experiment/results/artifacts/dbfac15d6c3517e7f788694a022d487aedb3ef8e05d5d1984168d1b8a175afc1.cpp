#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_024\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_024\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_024\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_024\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  digitalWrite(13, analogRead(A0) > 500 ? HIGH : LOW);
}

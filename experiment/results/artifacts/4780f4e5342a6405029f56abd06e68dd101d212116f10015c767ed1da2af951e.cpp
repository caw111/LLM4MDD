#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_010\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const unsigned long OUTPUT_INTERVAL_MS = 1000;
unsigned long lastOutputTime = 0;

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_010\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_010\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_010\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(20);
  pinMode(A0, INPUT);
  lastOutputTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastOutputTime >= OUTPUT_INTERVAL_MS) {
    lastOutputTime = now;
    Serial.println(analogRead(A0));
  }
}

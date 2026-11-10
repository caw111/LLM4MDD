#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_004\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t POTENTIOMETER_PIN = A0;
const uint8_t LED_PIN = 9;

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_004\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_004\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_004\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
}

void loop() {
  const int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  const int brightness = map(potentiometerValue, 0, 1023, 0, 255);
  analogWrite(LED_PIN, brightness);
}

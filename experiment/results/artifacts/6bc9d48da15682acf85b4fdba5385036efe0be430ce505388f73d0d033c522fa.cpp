#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_007\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LIGHT_SENSOR_PIN = A1;
const uint8_t LED_PIN = 8;
const int LIGHT_THRESHOLD = 500;  // 光强阈值，可根据实际环境调整（0～1023）。

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_007\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_007\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_007\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  const int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  digitalWrite(LED_PIN, lightLevel < LIGHT_THRESHOLD ? HIGH : LOW);
  delay(20);
}

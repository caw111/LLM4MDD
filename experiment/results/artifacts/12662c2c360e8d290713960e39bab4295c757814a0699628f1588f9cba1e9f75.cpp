#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_029\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t LIGHT_SENSOR_PIN = A0;
constexpr uint8_t LED_PWM_PIN = 9;
constexpr uint8_t AUX_OUTPUT_PIN = 13;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_029\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_029\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_029\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(AUX_OUTPUT_PIN, OUTPUT);

  analogWrite(LED_PWM_PIN, 0);
  digitalWrite(AUX_OUTPUT_PIN, LOW);
}

void loop() {
  // 假定光照越强，A0 的读数越大；LED 亮度随之降低。
  const int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  const int brightness = map(lightLevel, 0, 1023, 255, 0);

  analogWrite(LED_PWM_PIN, brightness);
  delay(10);
}

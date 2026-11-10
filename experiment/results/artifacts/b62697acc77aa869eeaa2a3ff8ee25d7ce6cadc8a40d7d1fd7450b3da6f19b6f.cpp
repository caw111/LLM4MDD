#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_003\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t LED_PIN = 13;

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_003\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_003\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_003\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, digitalRead(BUTTON_PIN) == LOW ? HIGH : LOW);
}

void loop() {
  digitalWrite(LED_PIN, digitalRead(BUTTON_PIN) == LOW ? HIGH : LOW);
}

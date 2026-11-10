#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_bar_led_basic\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t ledPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t threshold = 6;

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_bar_led_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_bar_led_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_bar_led_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], i < threshold ? HIGH : LOW);
  }
}

void loop() {
  // Outputs retain their states; no further action is required.
}

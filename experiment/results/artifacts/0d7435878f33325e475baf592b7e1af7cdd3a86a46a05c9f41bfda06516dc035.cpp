#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_photoresistor_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t PHOTORESISTOR_PIN = A0;
const uint8_t LED_COUNT = 10;
const uint8_t LED_PINS[LED_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_photoresistor_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_photoresistor_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_photoresistor_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  pinMode(PHOTORESISTOR_PIN, INPUT);
}

void loop() {
  const int sensorValue = analogRead(PHOTORESISTOR_PIN);

  // Lower readings mean brighter light: 1023 -> 0 LEDs, 0 -> 10 LEDs.
  const int litCount = map(sensorValue, 1023, 0, 0, LED_COUNT);

  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], i < litCount ? HIGH : LOW);
  }
}

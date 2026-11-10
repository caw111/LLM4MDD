#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_023\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t POTENTIOMETER_PIN = A0;
const uint8_t BUZZER_PIN = 6;

const unsigned int MIN_FREQUENCY_HZ = 100;
const unsigned int MAX_FREQUENCY_HZ = 4000;

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_023\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_023\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_023\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  const int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  const unsigned int frequencyHz = map(
    potentiometerValue, 0, 1023,
    MIN_FREQUENCY_HZ, MAX_FREQUENCY_HZ
  );

  tone(BUZZER_PIN, frequencyHz);
  delay(10);
}

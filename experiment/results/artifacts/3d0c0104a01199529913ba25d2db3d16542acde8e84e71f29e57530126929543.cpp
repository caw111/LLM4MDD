#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_022\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 3;
const uint8_t BUZZER_PIN = 6;
const unsigned int BUZZER_FREQUENCY_HZ = 1000;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_022\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_022\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_022\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Connect the button between D3 and GND.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

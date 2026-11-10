#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_buzzer\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 3;
constexpr uint8_t BUZZER_PIN = 9;
constexpr unsigned int TONE_FREQUENCY_HZ = 262;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_buzzer\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_buzzer\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_buzzer\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // The existing external resistor pulls the button input HIGH.
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    tone(BUZZER_PIN, TONE_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

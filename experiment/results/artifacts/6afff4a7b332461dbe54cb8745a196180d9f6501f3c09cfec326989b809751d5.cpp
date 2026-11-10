#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_021\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t BUZZER_PIN = 6;
constexpr unsigned int TONE_FREQUENCY_HZ = 1000;
constexpr unsigned long BEEP_INTERVAL_MS = 1000;
constexpr unsigned long BEEP_DURATION_MS = 100;

unsigned long lastBeepStartMs = 0;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_021\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_021\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_021\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  const unsigned long now = millis();

  if (now - lastBeepStartMs >= BEEP_INTERVAL_MS) {
    lastBeepStartMs += BEEP_INTERVAL_MS;
    tone(BUZZER_PIN, TONE_FREQUENCY_HZ, BEEP_DURATION_MS);
  }
}

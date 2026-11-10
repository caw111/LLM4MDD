#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_006\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t BUZZER_PIN = 6;
constexpr unsigned long INTERVAL_MS = 2000;
constexpr unsigned long BEEP_DURATION_MS = 200;
constexpr unsigned int BEEP_FREQUENCY_HZ = 1000;

unsigned long lastBeepTime = 0;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_006\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_006\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_006\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  lastBeepTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastBeepTime >= INTERVAL_MS) {
    lastBeepTime = now;
    tone(BUZZER_PIN, BEEP_FREQUENCY_HZ, BEEP_DURATION_MS);
  }
}

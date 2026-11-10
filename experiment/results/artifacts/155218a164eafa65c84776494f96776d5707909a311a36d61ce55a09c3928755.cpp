#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_buzzer_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUZZER_POS_PIN = 2;   // Connected to Arduino pin2
const uint8_t BUZZER_GND_PIN = 14;  // Arduino Uno GND.1 corresponds to GND; not driven by code

const unsigned int TONE_FREQ_HZ = 262; // 262Hz

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_buzzer_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_buzzer_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_buzzer_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZER_POS_PIN, OUTPUT);

  // Continuously play 262Hz tone
  // Use timer-based tone generation for accurate frequency.
  tone(BUZZER_POS_PIN, TONE_FREQ_HZ);
}

void loop() {
  // Keep playing continuously; tone() runs in background.
  // No additional behavior required.
}

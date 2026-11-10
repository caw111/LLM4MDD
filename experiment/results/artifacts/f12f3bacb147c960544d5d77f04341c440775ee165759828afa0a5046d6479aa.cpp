#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Requirements:
// - Use A1 photoresistor to detect light intensity.
// - If below a threshold, turn ON D8 LED.

const uint8_t PHOTO_PIN = A1;
const uint8_t LED_PIN = 8;

// Threshold for "dark" condition (raw ADC value).
// Adjust as needed to match your specific photoresistor/LDR and wiring.
const int THRESHOLD = 500;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int light = analogRead(PHOTO_PIN); // 0..1023
  if (light < THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

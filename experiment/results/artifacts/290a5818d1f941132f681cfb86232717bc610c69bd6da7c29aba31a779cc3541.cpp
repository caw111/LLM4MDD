#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_19\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// LED control sequence for Arduino Mega 2560
// Wiring preserved:
//   mega:GND -> led1:C
//   led1:A   -> mega pin 2 (red)

const uint8_t LED_PIN = 2;

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_19\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_19\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_19\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED off on power/reset
}

void loop() {
  digitalWrite(LED_PIN, HIGH); delay(1000); // ON for 1s
  digitalWrite(LED_PIN, LOW);  delay(2000); // OFF for 2s
  digitalWrite(LED_PIN, HIGH); delay(3000); // ON for 3s
  digitalWrite(LED_PIN, LOW);  delay(3000); // OFF for 3s
}

#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_bar_led_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_bar_led_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_bar_led_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_bar_led_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Set bar graph LED anode pins (A1..A10) to OUTPUT
  const int pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  for (unsigned i = 0; i < (sizeof(pins) / sizeof(pins[0])); i++) {
    pinMode(pins[i], OUTPUT);
  }

  // Threshold: turn on LEDs only up to 6
  // Success criteria: pins 1..6 HIGH, pins 7..10 LOW
  digitalWrite(2, HIGH);  // A1
  digitalWrite(3, HIGH);  // A2
  digitalWrite(4, HIGH);  // A3
  digitalWrite(5, HIGH);  // A4
  digitalWrite(6, HIGH);  // A5
  digitalWrite(7, HIGH);  // A6

  digitalWrite(8, LOW);   // A7
  digitalWrite(9, LOW);   // A8
  digitalWrite(10, LOW);  // A9
  digitalWrite(11, LOW);  // A10
}

void loop() {
  // No actions required in loop
}

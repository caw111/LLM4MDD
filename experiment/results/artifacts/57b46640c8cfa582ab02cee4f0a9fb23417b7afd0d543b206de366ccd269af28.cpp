#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LED_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t POT_PIN = A0;

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // 1. Set pin2..pin11 as OUTPUT
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  // 2. Set potentiometer signal pin a0 as INPUT
  pinMode(POT_PIN, INPUT);
}

void loop() {
  // 3. Read potentiometer (0..1023) and map to 0..10
  int potValue = analogRead(POT_PIN);              // 0..1023
  int mappedValue = map(potValue, 0, 1023, 0, 10); // 0..10

  // 4. Turn on only the LEDs up to the mapped value, others off.
  // Success criteria: LEDs with number smaller than mapped value => HIGH
  for (uint8_t i = 0; i < 10; i++) {
    // i is 0-based; LED "pin number" (1..10) corresponds to i+1
    // Requirement: anode pins whose number is smaller than mapped value => HIGH
    // i+1 < mappedValue  <=>  i < mappedValue-1
    if ((int)(i + 1) < mappedValue) {
      digitalWrite(LED_PINS[i], HIGH);
    } else {
      digitalWrite(LED_PINS[i], LOW);
    }
  }
}

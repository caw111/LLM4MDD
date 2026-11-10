#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_8\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Mega pins 2–11 drive bargraph anodes A1–A10, respectively.
// Logical LED positions are zero-based: 0–9 (A0, A1, ... in the task).
// All cathodes are grounded, so HIGH lights an LED.
const uint8_t ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t ledCount = sizeof(ledPins) / sizeof(ledPins[0]);
const unsigned long intervalMs = 2000UL;

bool evenPositionsOn = true;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_8\\direct\\attempt_0\\sketch\\sketch.ino"
void displayPattern();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_8\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_8\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_8\\direct\\attempt_0\\sketch\\sketch.ino"
void displayPattern() {
  // Turn the previous group off before turning the next group on.
  for (uint8_t position = 0; position < ledCount; ++position) {
    if (((position % 2) == 0) != evenPositionsOn) {
      digitalWrite(ledPins[position], LOW);
    }
  }

  for (uint8_t position = 0; position < ledCount; ++position) {
    if (((position % 2) == 0) == evenPositionsOn) {
      digitalWrite(ledPins[position], HIGH);
    }
  }
}

void setup() {
  for (uint8_t position = 0; position < ledCount; ++position) {
    digitalWrite(ledPins[position], LOW);
    pinMode(ledPins[position], OUTPUT);
  }

  displayPattern();
}

void loop() {
  delay(intervalMs);
  evenPositionsOn = !evenPositionsOn;
  displayPattern();
}

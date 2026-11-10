#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t led1Pin = 2;
const uint8_t led2Pin = 3;

const uint8_t barPins[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

const unsigned long stepDurationMs = 2000UL;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
  }

  // Initialization: all off
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(barPins[i], LOW);
  }
}

void loop() {
  while (true) {
    // Loading Animation (1 segment per 2000ms; LEDs blink alternately every 2000ms)
    for (uint8_t seg = 0; seg < 10; seg++) {
      // Turn on segments up to current one
      for (uint8_t i = 0; i <= seg; i++) {
        digitalWrite(barPins[i], HIGH);
      }
      for (uint8_t i = seg + 1; i < 10; i++) {
        digitalWrite(barPins[i], LOW);
      }

      // Blink LEDs alternately every 2000ms while filling up:
      // For each segment step (2000ms), toggle which LED is ON.
      static bool ledToggle = false;
      ledToggle = !ledToggle;

      digitalWrite(led1Pin, ledToggle ? HIGH : LOW);
      digitalWrite(led2Pin, ledToggle ? LOW : HIGH);

      delay(stepDurationMs);
    }

    // After bargraph is fully lit, reset to off and restart
    for (uint8_t i = 0; i < 10; i++) {
      digitalWrite(barPins[i], LOW);
    }
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);

    // Restart immediately
  }
}

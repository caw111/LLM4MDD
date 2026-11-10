#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Mega 2560 sketch for synchronized "knight rider" on two 10-LED bar graphs.
// bargraph1 pins: A1..A10 connected to Mega pins 2..11 respectively
// bargraph2 pins: A1..A10 connected to Mega pins 3..12 respectively

const uint8_t BAR1_PINS[10] = {A1, A2, A3, A4, A5, A6, A7, A8, A9, A10};
const uint8_t BAR2_PINS[10] = {A1, A2, A3, A4, A5, A6, A7, A8, A9, A10};

const unsigned long STEP_DELAY_MS = 2000; // 2 seconds

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff() {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR1_PINS[i], LOW);
    digitalWrite(BAR2_PINS[i], LOW);
  }
}

void setup() {
  // Initialize all LEDs OFF on both bar graphs
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR1_PINS[i], OUTPUT);
    pinMode(BAR2_PINS[i], OUTPUT);
    digitalWrite(BAR1_PINS[i], LOW);
    digitalWrite(BAR2_PINS[i], LOW);
  }

  allOff();
}

void loop() {
  // Left to right: bargraph1 LED i (1..10), bargraph2 LED i-1 (0 -> all off)
  // If bargraph1 reaches LED 1, bargraph2 turns off.
  for (int i = 1; i <= 10; i++) {
    // Update bargraph1
    allOff(); // ensure only the intended LEDs are on at each step

    // bargraph1 lights up at LED i
    digitalWrite(BAR1_PINS[i - 1], HIGH);

    // bargraph2 lights up at LED (i-1) if i-1 in 1..10; else all off
    int j = i - 1;
    if (j >= 1 && j <= 10) {
      digitalWrite(BAR2_PINS[j - 1], HIGH);
    }

    delay(STEP_DELAY_MS);
  }

  // Right to left: bargraph1 LED i (10..1), bargraph2 LED i-1 (0 -> all off)
  // If bargraph1 reaches LED 1, bargraph2 turns off.
  for (int i = 10; i >= 1; i--) {
    allOff();

    digitalWrite(BAR1_PINS[i - 1], HIGH);

    int j = i - 1;
    if (j >= 1 && j <= 10) {
      digitalWrite(BAR2_PINS[j - 1], HIGH);
    }

    delay(STEP_DELAY_MS);
  }
}

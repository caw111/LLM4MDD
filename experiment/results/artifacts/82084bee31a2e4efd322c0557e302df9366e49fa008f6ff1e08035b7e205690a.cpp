#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Wiring (preserved):
// bargraph1 LEDs: A1..A10  -> Arduino Mega pins 2..11
// bargraph2 LEDs: A1..A10  -> Arduino Mega pins 3..12

// Mega (ATmega2560) has digital pins mapped as:
const uint8_t BAR1_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};   // A1..A10
const uint8_t BAR2_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};  // A1..A10

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_26\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff() {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR1_PINS[i], LOW);
    digitalWrite(BAR2_PINS[i], LOW);
  }
}

// index i in [0..9] corresponds to LED (i+1) on bargraph
// When bargraph1 is at LED (i+1), bargraph2 is at LED (i) => bargraph2 index = i-1
// Special rules:
// - If bargraph1 reaches LED 1 (i=0), bargraph2 turns off (no LED lit).
// - Synchronize both bars with the same timing.
void setup() {
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR1_PINS[i], OUTPUT);
    pinMode(BAR2_PINS[i], OUTPUT);
    digitalWrite(BAR1_PINS[i], LOW);
    digitalWrite(BAR2_PINS[i], LOW);
  }
  allOff();
}

void loop() {
  const unsigned long delayMs = 2000;

  // 1) Left-to-right: bargraph1 LED1..LED10, bargraph2 LED0..LED9 (LED0 => off)
  for (int i = 0; i <= 9; i++) { // i maps to bargraph1 LED (i+1)
    // Turn everything off first (ensures only one LED lit per bargraph at a time)
    digitalWrite(BAR1_PINS[i], HIGH);
    if (i - 1 >= 0) digitalWrite(BAR2_PINS[i - 1], HIGH);
    // Ensure others are off
    for (int k = 0; k < 10; k++) {
      if (k != i) digitalWrite(BAR1_PINS[k], LOW);
      if (k != i - 1) digitalWrite(BAR2_PINS[k], LOW);
    }
    delay(delayMs);
  }

  // 2) Right-to-left: bargraph1 LED10..LED1, bargraph2 LED9..LED0 (LED0 => off)
  for (int i = 9; i >= 0; i--) { // i maps to bargraph1 LED (i+1)
    // When i corresponds to bargraph1 at LED (i+1),
    // bargraph2 should be at LED (i) => bargraph2 index = i-1.
    digitalWrite(BAR1_PINS[i], HIGH);
    if (i - 1 >= 0) digitalWrite(BAR2_PINS[i - 1], HIGH);

    // Ensure others are off
    for (int k = 0; k < 10; k++) {
      if (k != i) digitalWrite(BAR1_PINS[k], LOW);
      if (k != i - 1) digitalWrite(BAR2_PINS[k], LOW);
    }
    delay(delayMs);
  }
}

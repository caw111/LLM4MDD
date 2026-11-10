#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_96\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
const uint8_t LED_PINS[] = {2, 3, 4};
const uint8_t BAR_PINS[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const uint8_t BAR_COUNT = sizeof(BAR_PINS) / sizeof(BAR_PINS[0]);
const unsigned long PHASE_DURATION_MS = 2000UL;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_96\\direct\\attempt_0\\sketch\\sketch.ino"
void setPhase(int8_t activeLed, uint8_t litSegments);
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_96\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_96\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_96\\direct\\attempt_0\\sketch\\sketch.ino"
void setPhase(int8_t activeLed, uint8_t litSegments) {
  // Turn off the previous LED before activating another.
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
  }

  // BAR_PINS map to bargraph anodes A1 through A10, not analog pins.
  for (uint8_t i = 0; i < BAR_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], i < litSegments ? HIGH : LOW);
  }

  if (activeLed >= 0 && activeLed < LED_COUNT) {
    digitalWrite(LED_PINS[activeLed], HIGH);
  }
}

void setup() {
  // Set output latches LOW before enabling outputs to prevent startup flashes.
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < BAR_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }
}

void loop() {
  // Exactly one LED is active per phase, so exactly one segment is lit.
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    setPhase(i, 1);
    delay(PHASE_DURATION_MS);
  }

  // Final phase: individual LEDs off, all ten bargraph segments on.
  setPhase(-1, BAR_COUNT);
  delay(PHASE_DURATION_MS);
}

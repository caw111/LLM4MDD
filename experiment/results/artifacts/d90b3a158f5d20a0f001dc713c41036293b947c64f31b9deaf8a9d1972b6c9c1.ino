#include <Arduino.h>

static const uint8_t NUM_LEDS = 10;

// Potentiometer analog inputs (as per wiring)
static const uint8_t POT1_PIN = A3; // lower bound, 0-9
static const uint8_t POT2_PIN = A4; // upper bound, 0-9
static const uint8_t POT3_PIN = A5; // fill percentage, 0-100

// LED bargraph pins (as per wiring): A1..A10
static const uint8_t LED_PINS[NUM_LEDS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// Map/quantization constants for analog-to-range
// Using 10-bit ADC: 0..1023
static const int ADC_MAX = 1023;

static int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

// Read analog and quantize to [0..9]
static int readQuantized9(uint8_t pin) {
  int adc = analogRead(pin);
  // Quantize to 10 discrete steps: 0..9 inclusive
  // Use range 0..9 by dividing into 10 bins.
  long q = (long)adc * 10L / (ADC_MAX + 1L); // yields 0..9
  return clampInt((int)q, 0, 9);
}

// Read analog and quantize to [0..100]
static int readQuantized100(uint8_t pin) {
  int adc = analogRead(pin);
  long q = (long)adc * 101L / (ADC_MAX + 1L); // yields 0..100
  return clampInt((int)q, 0, 100);
}

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW); // Requirement: all off at startup
  }
}

void loop() {
  int lower = readQuantized9(POT1_PIN); // 0..9
  int upper = readQuantized9(POT2_PIN); // 0..9
  int fillPercent = readQuantized100(POT3_PIN); // 0..100

  // Inclusive endpoints determine the active range length
  int rangeLen = abs(upper - lower) + 1; // 1..10

  // Determine how many LEDs within active range are lit based on fill %
  // "0%" => 0 LEDs lit, "100%" => all range LEDs lit.
  int litCount = (int)((long)rangeLen * (long)fillPercent / 100L);
  litCount = clampInt(litCount, 0, rangeLen);

  // Clear all LEDs first
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }

  // Direction handling
  int start, end, step;
  if (lower <= upper) {
    start = lower;
    end = upper;
    step = +1;
  } else {
    start = upper;
    end = lower;
    step = +1; // will step from start to end, start/end already ordered
  }

  // Ensure ordering in step scenario above:
  if (lower > upper) {
    // start = upper, end = lower, stepping from start to end
    // step +1 works since end > start here.
    // (left as explicit clarity)
  }

  // Light sequentially from ordered endpoints inclusive
  int idx = start;
  for (int k = 0; k < litCount; k++) {
    // idx ranges within 0..9
    digitalWrite(LED_PINS[idx], HIGH);
    idx += step;
    // idx should not exceed end due to litCount <= rangeLen
    if (idx > end && step > 0) break;
  }

  // Continuous update
  // (No delay required; keep responsive to pot changes)
}
#include <Arduino.h>

const uint8_t POT1_PIN = A3; // lower bound (0-9)
const uint8_t POT2_PIN = A4; // upper bound (0-9)
const uint8_t POT3_PIN = A5; // fill percentage (0-100)

// Bargraph connections:
// A1..A10 are on Mega pins 2..11.
// (C1..C10 share Mega GND.1, already handled by circuit)
const uint8_t LED_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

static int mapTo0to9(int raw) {
  raw = constrain(raw, 0, 1023);
  // Map 0..1023 -> 0..9 with inclusive endpoints
  return (raw * 9) / 1023;
}

static int mapTo0to100(int raw) {
  raw = constrain(raw, 0, 1023);
  // Map 0..1023 -> 0..100 with inclusive endpoints
  return (raw * 100) / 1023;
}

void setup() {
  // Ensure all LEDs are off at start
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
}

void loop() {
  // Read potentiometers continuously
  int pot1Raw = analogRead(POT1_PIN);
  int pot2Raw = analogRead(POT2_PIN);
  int pot3Raw = analogRead(POT3_PIN);

  int lower = mapTo0to9(pot1Raw);  // 0-9
  int upper = mapTo0to9(pot2Raw);  // 0-9
  int fillPct = mapTo0to100(pot3Raw); // 0-100

  // Determine direction and active range size (inclusive)
  int start = lower;
  int end = upper;

  int dir = (start <= end) ? 1 : -1;

  int minB = min(lower, upper);
  int maxB = max(lower, upper);

  int rangeSize = (maxB - minB) + 1; // inclusive endpoints => 1..10

  // Convert fill percentage to number of LEDs to light within active range.
  // fillPct=0 => 0 LEDs, fillPct=100 => rangeSize LEDs.
  int ledsToLight = (int)((rangeSize * (long)fillPct) / 100);

  // Clear all LEDs first
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }

  // Light LEDs sequentially from the direction start to end (inclusive)
  // for ledsToLight LEDs.
  // Index mapping: LED number k (0-9) corresponds to LED_PINS[k] (A(k+1)).
  if (ledsToLight > 0) {
    for (int n = 0; n < ledsToLight; n++) {
      int ledNum = (dir == 1) ? (minB + n) : (maxB - n);
      if (ledNum >= 0 && ledNum <= 9) {
        digitalWrite(LED_PINS[ledNum], HIGH);
      }
    }
  }

  // Short delay to reduce flicker while keeping continuous updates
  delay(20);
}
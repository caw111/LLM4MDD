#include <Arduino.h>

static const uint8_t LED_RED_PIN    = 2; // mega:2
static const uint8_t LED_YELLOW_PIN = 3; // mega:3
static const uint8_t LED_GREEN_PIN  = 4; // mega:4

// 74HC595 pins (as wired)
static const uint8_t SR_DS   = A0; // mega:A0 -> sr1:DS
static const uint8_t SR_STCP = A1; // mega:A1 -> sr1:STCP
static const uint8_t SR_SHCP = A2; // mega:A2 -> sr1:SHCP
static const uint8_t SR_MR   = SR_DS; // not used directly; wired to sevseg1:COM.1 through sr1:MR

// Shift register output mapping (Q0..Q6 to segments A..G)
static const uint8_t Q0_SEG_A = 0;
static const uint8_t Q1_SEG_B = 1;
static const uint8_t Q2_SEG_C = 2;
static const uint8_t Q3_SEG_D = 3;
static const uint8_t Q4_SEG_E = 4;
static const uint8_t Q5_SEG_F = 5;
static const uint8_t Q6_SEG_G = 6;

// Segment bit positions in our shift-register byte:
// bit0 -> Q0 -> segment A
// bit1 -> Q1 -> segment B
// ...
// bit6 -> Q6 -> segment G
static uint8_t segByte = 0;

static void srWriteByteLSBFirst(uint8_t data)
{
  // Using DS, SHCP, STCP pins
  // OE is tied low in the circuit, MR is tied via SR1:MR to sevseg1:COM.1
  // We'll just shift and latch.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (data & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_SHCP, HIGH);
  }
  digitalWrite(SR_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR_STCP, LOW);
}

static void setSegmentsForDigit(uint8_t digit)
{
  // 7-seg (common behavior) with segments A,B,C,D,E,F,G
  // We assume segments are active-HIGH at the Q outputs (typical in this wiring).
  // Patterns are ordered by bits: A=bit0, B=bit1, C=bit2, D=bit3, E=bit4, F=bit5, G=bit6.
  // For digits 0-9:
  // 0: A B C D E F (no G)
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  static const uint8_t patterns[10] = {
    // 0
    (1<<Q0_SEG_A) | (1<<Q1_SEG_B) | (1<<Q2_SEG_C) | (1<<Q3_SEG_D) | (1<<Q4_SEG_E) | (1<<Q5_SEG_F),
    // 1
    (1<<Q1_SEG_B) | (1<<Q2_SEG_C),
    // 2
    (1<<Q0_SEG_A) | (1<<Q1_SEG_B) | (1<<Q3_SEG_D) | (1<<Q4_SEG_E) | (1<<Q6_SEG_G),
    // 3
    (1<<Q0_SEG_A) | (1<<Q1_SEG_B) | (1<<Q2_SEG_C) | (1<<Q3_SEG_D) | (1<<Q6_SEG_G),
    // 4
    (1<<Q1_SEG_B) | (1<<Q2_SEG_C) | (1<<Q5_SEG_F) | (1<<Q6_SEG_G),
    // 5
    (1<<Q0_SEG_A) | (1<<Q2_SEG_C) | (1<<Q3_SEG_D) | (1<<Q5_SEG_F) | (1<<Q6_SEG_G),
    // 6
    (1<<Q0_SEG_A) | (1<<Q2_SEG_C) | (1<<Q3_SEG_D) | (1<<Q4_SEG_E) | (1<<Q5_SEG_F) | (1<<Q6_SEG_G),
    // 7
    (1<<Q0_SEG_A) | (1<<Q1_SEG_B) | (1<<Q2_SEG_C),
    // 8
    (1<<Q0_SEG_A) | (1<<Q1_SEG_B) | (1<<Q2_SEG_C) | (1<<Q3_SEG_D) | (1<<Q4_SEG_E) | (1<<Q5_SEG_F) | (1<<Q6_SEG_G),
    // 9
    (1<<Q0_SEG_A) | (1<<Q1_SEG_B) | (1<<Q2_SEG_C) | (1<<Q3_SEG_D) | (1<<Q5_SEG_F) | (1<<Q6_SEG_G)
  };

  uint8_t d = digit % 10;
  segByte = patterns[d];
  srWriteByteLSBFirst(segByte);
}

void setup()
{
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);

  // Initialize LEDs off
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);

  // Shift register control pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_SHCP, LOW);

  // Seed randomness
  randomSeed(analogRead(A15) ^ micros());

  // Display '0' on the 7-segment at power-up/reset
  setSegmentsForDigit(0);
}

void loop()
{
  // Every 2 seconds: randomly select 0..3 LEDs to light
  static unsigned long last = 0;
  unsigned long now = millis();
  if (now - last >= 2000UL) {
    last = now;

    uint8_t count = random(0, 4); // 0..3 inclusive upper bound exclusive -> 0..3

    // Turn all LEDs off first
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);

    // Randomly pick which LEDs to light (without replacement)
    uint8_t indices[3] = {0, 1, 2};

    // Fisher-Yates shuffle of indices
    for (int i = 2; i > 0; --i) {
      int j = random(0, i + 1);
      uint8_t tmp = indices[i];
      indices[i] = indices[j];
      indices[j] = tmp;
    }

    // Light 'count' LEDs using shuffled order
    for (uint8_t i = 0; i < count; i++) {
      switch (indices[i]) {
        case 0: digitalWrite(LED_RED_PIN, HIGH); break;
        case 1: digitalWrite(LED_YELLOW_PIN, HIGH); break;
        case 2: digitalWrite(LED_GREEN_PIN, HIGH); break;
      }
    }

    // Update 7-seg immediately to show number of lit LEDs
    setSegmentsForDigit(count);
  }

  // Keep looping
}
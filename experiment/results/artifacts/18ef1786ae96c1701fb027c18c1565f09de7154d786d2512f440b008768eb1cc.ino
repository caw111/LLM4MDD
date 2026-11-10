#include <Arduino.h>

// ---- Shift register (sr1: 74HC595) pin mapping (from circuit) ----
// A0..DS, A1..STCP, A2..SHCP (per wiring)
static const uint8_t SR_DS_PIN   = A0; // DS
static const uint8_t SR_STCP_PIN = A1; // STCP (latch)
static const uint8_t SR_SHCP_PIN = A2; // SHCP (shift clock)

// ---- 7-segment display (sevseg1) segment mapping via SR outputs ----
// SR outputs Q0..Q6 are wired to segments A..G respectively:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
//
// OE and MR are tied to GND in the circuit, so outputs are always enabled.

// ---- Bar graph (bargraph1) GPIO mapping (direct control) ----
// A1..A10 are on Arduino pins 2..11 (per wiring).
// C1..C10 are all tied to GND.1, so this is effectively a low-side matrix:
// drive A1..A10 HIGH to light the corresponding LED.
// (Mega's GND rail pin is irrelevant in software.)
static const uint8_t BAR_LED_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// ---- Segment encoding for values 0..9 (active HIGH) ----
// We assume segment lines are driven HIGH by the 74HC595 to turn segments ON.
static const uint8_t SEG_A = 1 << 0; // Q0
static const uint8_t SEG_B = 1 << 1; // Q1
static const uint8_t SEG_C = 1 << 2; // Q2
static const uint8_t SEG_D = 1 << 3; // Q3
static const uint8_t SEG_E = 1 << 4; // Q4
static const uint8_t SEG_F = 1 << 5; // Q5
static const uint8_t SEG_G = 1 << 6; // Q6

// Bit positions correspond to Q0..Q6.
static const uint8_t digitToSegments[10] = {
  // 0: A B C D E F
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
  // 1: B C
  SEG_B | SEG_C,
  // 2: A B D E G
  SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,
  // 3: A B C D G
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,
  // 4: B C F G
  SEG_B | SEG_C | SEG_F | SEG_G,
  // 5: A C D F G
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,
  // 6: A C D E F G
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
  // 7: A B C
  SEG_A | SEG_B | SEG_C,
  // 8: A B C D E F G
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
  // 9: A B C D F G
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G
};

static void srWrite7Segments(uint8_t segBits_Q0toQ6) {
  // 74HC595 shifts bits MSB->LSB depending on order; we will shift Q6..Q0
  // such that the first shifted ends up at Q7 typically. To avoid ambiguity,
  // we explicitly map by sending 8 bits where only Q0..Q6 are set and Q7=0.
  //
  // We'll shift out in order Q7..Q0 (common for these parts), so the last
  // shifted bit ends up at Q0. Since Q7 isn't connected, it's 0 anyway.
  uint8_t q7 = 0;

  uint8_t b7 = (q7 ? 1 : 0);
  uint8_t b6 = (segBits_Q0toQ6 & SEG_G) ? 1 : 0; // Q6
  uint8_t b5 = (segBits_Q0toQ6 & SEG_F) ? 1 : 0; // Q5
  uint8_t b4 = (segBits_Q0toQ6 & SEG_E) ? 1 : 0; // Q4
  uint8_t b3 = (segBits_Q0toQ6 & SEG_D) ? 1 : 0; // Q3
  uint8_t b2 = (segBits_Q0toQ6 & SEG_C) ? 1 : 0; // Q2
  uint8_t b1 = (segBits_Q0toQ6 & SEG_B) ? 1 : 0; // Q1
  uint8_t b0 = (segBits_Q0toQ6 & SEG_A) ? 1 : 0; // Q0

  digitalWrite(SR_STCP_PIN, LOW);
  // Shift bits: b7, b6, b5, b4, b3, b2, b1, b0
  // Pulse SHCP for each bit on the rising edge.
  digitalWrite(SR_DS_PIN, b7);
  digitalWrite(SR_SHCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_SHCP_PIN, LOW);

  digitalWrite(SR_DS_PIN, b6);
  digitalWrite(SR_SHCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_SHCP_PIN, LOW);

  digitalWrite(SR_DS_PIN, b5);
  digitalWrite(SR_SHCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_SHCP_PIN, LOW);

  digitalWrite(SR_DS_PIN, b4);
  digitalWrite(SR_SHCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_SHCP_PIN, LOW);

  digitalWrite(SR_DS_PIN, b3);
  digitalWrite(SR_SHCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_SHCP_PIN, LOW);

  digitalWrite(SR_DS_PIN, b2);
  digitalWrite(SR_SHCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_SHCP_PIN, LOW);

  digitalWrite(SR_DS_PIN, b1);
  digitalWrite(SR_SHCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_SHCP_PIN, LOW);

  digitalWrite(SR_DS_PIN, b0);
  digitalWrite(SR_SHCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_SHCP_PIN, LOW);

  // Latch
  digitalWrite(SR_STCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_STCP_PIN, LOW);
}

static void setBarGraph(uint8_t value0to9) {
  // Illuminate number of LEDs equal to current value.
  // Order from pin A1 to A10 corresponds to BAR_LED_PINS[0..9].
  for (uint8_t i = 0; i < 10; i++) {
    if (i < value0to9) {
      digitalWrite(BAR_LED_PINS[i], HIGH);
    } else {
      digitalWrite(BAR_LED_PINS[i], LOW);
    }
  }
}

void setup() {
  pinMode(SR_DS_PIN, OUTPUT);
  pinMode(SR_SHCP_PIN, OUTPUT);
  pinMode(SR_STCP_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_LED_PINS[i], OUTPUT);
  }

  // Initial state: display "0" and bar graph with no LEDs lit.
  uint8_t current = 0;
  srWrite7Segments(digitToSegments[current]);
  setBarGraph(0); // no LEDs lit
}

void loop() {
  static uint8_t value = 0;
  static unsigned long lastMs = 0;

  unsigned long now = millis();
  if (now - lastMs >= 2000) {
    lastMs = now;
    value = (value >= 9) ? 0 : (value + 1);

    // Synchronize: 7-seg shows numeric value, bargraph lights that many LEDs.
    srWrite7Segments(digitToSegments[value]);
    setBarGraph(value);
  }
}
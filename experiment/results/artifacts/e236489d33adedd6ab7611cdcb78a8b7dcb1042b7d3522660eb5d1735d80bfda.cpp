#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_44\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_POT = A3;

// SR1 (tens digit) - connections given in circuit
static const uint8_t SR1_SHCP = A2;  // shift register clock
static const uint8_t SR1_STCP = A1;  // latch clock
static const uint8_t SR1_DS   = A0;  // data

// SR2 (units digit) shares clocks with SR1 per wiring,
// but we keep explicit mapping for clarity.
static const uint8_t SR2_DS   = A0;  // via sr1:Q7S -> sr2:DS, data is driven by SR1 internal Q7S

// Bargraph shift register(s) share SR clocks and use SR2's outputs Q0..Q7S and DS path
// Wiring: mega:2..11 -> bargraph1:A1..A10, bargraph1:C1..C10 -> mega:GND.1.
// These are individual segment anode/control lines (bargraph is a 10-seg device directly driven).
static const uint8_t BAR_0_PIN = 2;   // bargraph A1
static const uint8_t BAR_1_PIN = 3;   // bargraph A2
static const uint8_t BAR_2_PIN = 4;   // bargraph A3
static const uint8_t BAR_3_PIN = 5;   // bargraph A4
static const uint8_t BAR_4_PIN = 6;   // bargraph A5
static const uint8_t BAR_5_PIN = 7;   // bargraph A6
static const uint8_t BAR_6_PIN = 8;   // bargraph A7
static const uint8_t BAR_7_PIN = 9;   // bargraph A8
static const uint8_t BAR_8_PIN = 10;  // bargraph A9
static const uint8_t BAR_9_PIN = 11;  // bargraph A10

// Lookup for 7-segment digits for common wiring used by the provided shift registers:
// These assignments correspond to:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// The circuit implies "on" corresponds to logic HIGH at Q outputs.
// We use standard segment truth for digits 0-9 with segments A..G.
static const uint8_t digitTo7SegOnMask[10] = {
  // 0: A B C D E F
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: B C
  (1 << 1) | (1 << 2),
  // 2: A B D E G
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3: A B C D G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4: B C F G
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5: A C D F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6: A C D E F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: A B C
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8: A B C D E F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: A B C D F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

// Two cascaded 74HC595 share SHCP and STCP from SR1,
// and the DS chain is handled by shifting out to SR1; SR1's Q7S feeds SR2 DS.
// Therefore, we need to clock out 16 bits total:
// - First 8 bits go into SR1 (tens digit segments A..G mapped to Q0..Q6).
// - Next 8 bits go into SR2 (units digit segments A..G mapped to Q0..Q6).
//
// We will shift MSB-first or LSB-first depending on DS->Q0 mapping.
// Standard 74HC595: serial input goes to shift register, and on each rising edge
// the first shifted bit appears at Q0 after 8 clocks (LSB-first behavior).
// Most common approach: shift out bits LSB-first where bit0 corresponds to Q0.
// We'll implement LSB-first: clock 8 times, output bit0 first -> ends at Q0.
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_44\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut16_tensUnits(uint8_t tensDigit, uint8_t unitsDigit);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_44\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphFromValue(uint8_t value0to99);
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_44\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_44\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_44\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut16_tensUnits(uint8_t tensDigit, uint8_t unitsDigit) {
  // SR1 expects bits for segments A..G across Q0..Q6.
  uint8_t tensMask  = digitTo7SegOnMask[tensDigit];
  uint8_t unitsMask = digitTo7SegOnMask[unitsDigit];

  // Bits 0..6 correspond to A..G; bit7 unused -> keep 0.
  uint16_t word = 0;
  word |= (uint16_t)(tensMask & 0x7F) << 0;   // placed in lower 8 bits for SR1
  word |= (uint16_t)(unitsMask & 0x7F) << 8;  // placed in upper 8 bits for SR2

  // LSB-first: bit0 shifts to SR1 Q0, ... bit7 to SR1 Q7,
  // bit8 to SR2 Q0, ... bit15 to SR2 Q7.
  for (uint8_t i = 0; i < 16; i++) {
    uint8_t bitVal = (word >> i) & 0x01;
    digitalWrite(SR1_DS, bitVal ? HIGH : LOW); // DS line is shared to sr1:DS

    // Rising edge on SHCP shifts data
    digitalWrite(SR1_SHCP, HIGH);
    // short hold
    delayMicroseconds(2);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(2);
  }

  // Latch outputs
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR1_STCP, LOW);
}

static void setBargraphFromValue(uint8_t value0to99) {
  // Requirement:
  // 0-9 -> 0 LEDs
  // 10-19 -> 1 LED
  // ...
  // 90-99 -> 9 LEDs (indices 0-8)
  uint8_t lit = value0to99 / 10;     // 0..9
  // Map 0 LEDs lit -> all off. Only indices 0..8 exist per requirement.
  // Set A1..A9 (BAR_0_PIN..BAR_8_PIN) according to lit, and turn A10 (BAR_9_PIN) off always.
  for (uint8_t i = 0; i < 9; i++) {
    uint8_t pin = (i == 0) ? BAR_0_PIN :
                  (i == 1) ? BAR_1_PIN :
                  (i == 2) ? BAR_2_PIN :
                  (i == 3) ? BAR_3_PIN :
                  (i == 4) ? BAR_4_PIN :
                  (i == 5) ? BAR_5_PIN :
                  (i == 6) ? BAR_6_PIN :
                  (i == 7) ? BAR_7_PIN :
                              BAR_8_PIN;
    digitalWrite(pin, (i < lit) ? HIGH : LOW);
  }
  digitalWrite(BAR_9_PIN, LOW); // bargraph segment 10 always off (not mentioned in requirements)
}

void setup() {
  // Pot input
  pinMode(PIN_POT, INPUT);

  // Shift register control/data pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Bargraph pins (A1..A10 wired to mega:2..11)
  pinMode(BAR_0_PIN, OUTPUT);
  pinMode(BAR_1_PIN, OUTPUT);
  pinMode(BAR_2_PIN, OUTPUT);
  pinMode(BAR_3_PIN, OUTPUT);
  pinMode(BAR_4_PIN, OUTPUT);
  pinMode(BAR_5_PIN, OUTPUT);
  pinMode(BAR_6_PIN, OUTPUT);
  pinMode(BAR_7_PIN, OUTPUT);
  pinMode(BAR_8_PIN, OUTPUT);
  pinMode(BAR_9_PIN, OUTPUT);

  // Initialization requirements:
  // - both 7-seg show "00" (tens digit = 0, units digit = 0)
  // - all bargraph segments off
  shiftOut16_tensUnits(0, 0);

  // 0 value => 0 LEDs lit (also ensures A10 stays off)
  setBargraphFromValue(0);
}

void loop() {
  // Read potentiometer and map to 0..99
  int raw = analogRead(PIN_POT); // 0..1023
  uint8_t value = (uint8_t)((long)raw * 99L / 1023L); // 0..99

  uint8_t tens = value / 10;
  uint8_t units = value % 10;

  // Update displays and bargraph continuously/responsively
  shiftOut16_tensUnits(tens, units);
  setBargraphFromValue(value);
}

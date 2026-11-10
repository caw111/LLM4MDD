#include <Arduino.h>
#include <Servo.h>

static const uint8_t SR1_SHCP = A2;   // gray
static const uint8_t SR1_STCP = A1;   // purple
static const uint8_t SR1_DS   = A0;   // blue
static const uint8_t SR_OE     = A?;  // not wired to mega; OE tied between regs via GND in circuit
// OE is connected to GND through sr1:OE -> GND rail, so OE is always enabled.
// We still keep the latch pins as wired.

static const uint8_t SR1_Q0 = 0; // unused (wired to segments)
static const uint8_t SR1_Q1 = 0; // unused
static const uint8_t SR1_Q2 = 0; // unused
static const uint8_t SR1_Q3 = 0; // unused
static const uint8_t SR1_Q4 = 0; // unused
static const uint8_t SR1_Q5 = 0; // unused
static const uint8_t SR1_Q6 = 0; // unused

// Segment pins are handled by shift registers; we only shift bytes to the cascaded chips.

// Wiring for cascaded shift registers (sr1 -> sr2):
// - SHCP common: sr1:SHCP tied to sr2:SHCP
// - STCP common: sr1:STCP tied to sr2:STCP
// - DS wiring:
//   mega:A0 -> sr1:DS
//   sr1:Q7S -> sr2:DS
// So we provide 16 bits total, MSB first for sr2 then sr1, or vice-versa depending on chaining order.
// We'll use a standard approach: shift 16 bits MSB-first into the chain such that:
//   - bits shifted earlier end up in sr2 (further downstream).
//   - bits shifted later end up in sr1 (closest to DS).
//
// To make behavior match the wiring intent, we set:
//   tens display is sevseg1 driven by sr1
//   units display is sevseg2 driven by sr2
//
// Therefore, we want:
//   sr1 byte = pattern for tens digit
//   sr2 byte = pattern for units digit

// SevSeg segment mapping assumed typical for 74HC595 + sevseg with A..G on Q0..Q6:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7->DP (unused)
// We'll drive with common-anode/common-cathode assumption: "green segments" usually indicate
// direct segment outputs; in Wokwi's 7-seg model the inputs are active HIGH for segment LEDs.
// We'll treat 1 bit => segment ON.
static uint8_t digitToSegments(uint8_t d) {
  // Segments for digits 0-9 as A,B,C,D,E,F,G (DP off)
  // A B C D E F G
  // 0: 1 1 1 1 1 1 0
  // 1: 0 1 1 0 0 0 0
  // 2: 1 1 0 1 1 0 1
  // 3: 1 1 1 1 0 0 1
  // 4: 0 1 1 0 0 1 1
  // 5: 1 0 1 1 0 1 1
  // 6: 1 0 1 1 1 1 1
  // 7: 1 1 1 0 0 0 0
  // 8: 1 1 1 1 1 1 1
  // 9: 1 1 1 1 0 1 1
  switch (d) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(0<<6); // 0x3F
    case 1: return (0<<0)|(1<<1)|(1<<2)|(0<<3)|(0<<4)|(0<<5)|(0<<6); // 0x06
    case 2: return (1<<0)|(1<<1)|(0<<2)|(1<<3)|(1<<4)|(0<<5)|(1<<6); // 0x5B
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(0<<4)|(0<<5)|(1<<6); // 0x4F
    case 4: return (0<<0)|(1<<1)|(1<<2)|(0<<3)|(0<<4)|(1<<5)|(1<<6); // 0x66
    case 5: return (1<<0)|(0<<1)|(1<<2)|(1<<3)|(0<<4)|(1<<5)|(1<<6); // 0x6D
    case 6: return (1<<0)|(0<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); // 0x7D
    case 7: return (1<<0)|(1<<1)|(1<<2)|(0<<3)|(0<<4)|(0<<5)|(0<<6); // 0x07
    case 8: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); // 0x7F
    case 9: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(0<<4)|(1<<5)|(1<<6); // 0x6F
    default: return 0;
  }
}

static uint8_t srShift16_tens_units(uint8_t tens, uint8_t units) {
  // Not used directly
  return 0;
}

static void shiftOutWord(uint8_t sr1Byte, uint8_t sr2Byte) {
  // Assumes OE permanently enabled (tied to GND).
  // Common STCP and SHCP are wired to A1 and A2.
  // DS provided via A0 to sr1:DS and chain to sr2 via sr1:Q7S.
  //
  // We'll shift 16 bits MSB-first:
  // - First 8 bits go to sr2 (farthest)
  // - Next 8 bits go to sr1 (closest)
  //
  // That means:
  //   bits[15..8] = sr2Byte
  //   bits[7..0]  = sr1Byte
  const uint16_t word = ((uint16_t)sr2Byte << 8) | sr1Byte;

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_DS, (word >> i) & 0x01);
    // Pulse SHCP (clock)
    digitalWrite(SR1_SHCP, HIGH);
    digitalWrite(SR1_SHCP, LOW);
  }

  // Latch outputs
  digitalWrite(SR1_STCP, HIGH);
  digitalWrite(SR1_STCP, LOW);
}

// Shift register data pin is mega:A0
static const uint8_t SR1_DS_PIN = A0;

Servo servo1;
Servo servo2;
Servo servo3;

static const uint8_t SERVO1_PWM = 2; // mega:2
static const uint8_t SERVO2_PWM = 3; // mega:3
static const uint8_t SERVO3_PWM = 4; // mega:4

static uint8_t currentNumber = 0;
static unsigned long lastIncrementMs = 0;

static int angleForServo1(uint8_t n) { return (int)round(n * 1.8); } // n*1.8
static int angleForServo2(uint8_t n) { return (int)round(n * 1.2); } // n*1.2
static int angleForServo3(uint8_t n) { return (int)round(n * 0.6); } // n*0.6

static int clampAngle(int a) {
  if (a < 0) return 0;
  if (a > 180) return 180;
  return a;
}

static void updateDisplaysAndServos(uint8_t n) {
  uint8_t tens = (n / 10) % 10;
  uint8_t ones = n % 10;

  uint8_t tensPattern = digitToSegments(tens);
  uint8_t onesPattern = digitToSegments(ones);

  // Update displays immediately
  shiftOutWord(tensPattern, onesPattern);

  // Update servos immediately
  servo1.write(clampAngle(angleForServo1(n)));
  servo2.write(clampAngle(angleForServo2(n)));
  servo3.write(clampAngle(angleForServo3(n)));
}

void setup() {
  // Shift register control pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS_PIN, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS_PIN, LOW);

  // Servos
  servo1.attach(SERVO1_PWM);
  servo2.attach(SERVO2_PWM);
  servo3.attach(SERVO3_PWM);

  // Initialization: show "00" and servos at 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Update display to 00
  updateDisplaysAndServos(0);

  lastIncrementMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastIncrementMs >= 2000UL) {
    lastIncrementMs = now;

    // Increment number every 2 seconds, wrap at 99 -> 0
    currentNumber++;
    if (currentNumber > 99) currentNumber = 0;

    // Update display and servos immediately
    updateDisplaysAndServos(currentNumber);
  }
}
#include <Arduino.h>
#include <Servo.h>

// Shift register wiring (as provided)
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue

// Cascaded 74HC595 chain is sr1 -> sr2:
// - SHCP and STCP are tied across both registers.
// - DS: mega:A0 -> sr1:DS, and sr1:Q7S -> sr2:DS.
// Thus we must shift 16 bits total:
//   bits[15..8] go to sr2, bits[7..0] go to sr1.

static uint8_t digitToSegments(uint8_t d) {
  // Wokwi 7-seg model expects bits for segments A..G on Q0..Q6.
  // Mapping assumed:
  //   Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  // Bit = 1 means segment ON.
  switch (d) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(0<<6); // 0
    case 1: return (0<<0)|(1<<1)|(1<<2)|(0<<3)|(0<<4)|(0<<5)|(0<<6); // 1
    case 2: return (1<<0)|(1<<1)|(0<<2)|(1<<3)|(1<<4)|(0<<5)|(1<<6); // 2
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(0<<4)|(0<<5)|(1<<6); // 3
    case 4: return (0<<0)|(1<<1)|(1<<2)|(0<<3)|(0<<4)|(1<<5)|(1<<6); // 4
    case 5: return (1<<0)|(0<<1)|(1<<2)|(1<<3)|(0<<4)|(1<<5)|(1<<6); // 5
    case 6: return (1<<0)|(0<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); // 6
    case 7: return (1<<0)|(1<<1)|(1<<2)|(0<<3)|(0<<4)|(0<<5)|(0<<6); // 7
    case 8: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); // 8
    case 9: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(0<<4)|(1<<5)|(1<<6); // 9
    default: return 0;
  }
}

static void shiftOutWord(uint8_t sr1Byte, uint8_t sr2Byte) {
  // Assumes OE is permanently enabled (tied to GND via wiring), so latch determines output.
  // Shift MSB-first for a 16-bit chain:
  //   first 8 bits shifted go to sr2, next 8 go to sr1.
  uint16_t word = ((uint16_t)sr2Byte << 8) | sr1Byte;

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_DS, (word >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    digitalWrite(SR1_SHCP, LOW);
  }

  digitalWrite(SR1_STCP, HIGH);
  digitalWrite(SR1_STCP, LOW);
}

// Servo wiring (as provided)
static const uint8_t SERVO1_PWM = 2; // mega:2
static const uint8_t SERVO2_PWM = 3; // mega:3
static const uint8_t SERVO3_PWM = 4; // mega:4

Servo servo1;
Servo servo2;
Servo servo3;

static int angleForServo1(uint8_t n) { return (int)round(n * 1.8); }
static int angleForServo2(uint8_t n) { return (int)round(n * 1.2); }
static int angleForServo3(uint8_t n) { return (int)round(n * 0.6); }

static int clampAngle(int a) {
  if (a < 0) return 0;
  if (a > 180) return 180;
  return a;
}

static uint8_t currentNumber = 0;
static unsigned long lastIncrementMs = 0;

static void updateDisplaysAndServos(uint8_t n) {
  uint8_t tens = (n / 10) % 10;
  uint8_t ones = n % 10;

  // Requirement: one display is tens, other is units.
  // Wiring intent from circuit:
  // - sevseg1 is driven by sr1 (tens digit)
  // - sevseg2 is driven by sr2 (units digit)
  uint8_t tensPattern = digitToSegments(tens);
  uint8_t onesPattern = digitToSegments(ones);

  // Update displays immediately
  // shiftOutWord(sr1Byte, sr2Byte)
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
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Servos
  servo1.attach(SERVO1_PWM);
  servo2.attach(SERVO2_PWM);
  servo3.attach(SERVO3_PWM);

  // Initialization: show "00" and servos at 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

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
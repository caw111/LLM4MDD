#include <Arduino.h>
#include <Servo.h>

static const uint8_t SR1_SHCP = A1; // SHCP (latch)
static const uint8_t SR1_STCP = A2; // STCP (clock?) (note: per circuit, A1=SHCP, A2=STCP in naming)
static const uint8_t SR1_DS   = A0;  // DS

// Per provided wiring:
// mega:A2 -> sr1:SHCP (gray)
// mega:A1 -> sr1:STCP (purple)
// mega:A0 -> sr1:DS (blue)
static const uint8_t SR1_SHCP_PIN = A2;
static const uint8_t SR1_STCP_PIN = A1;
static const uint8_t SR1_DS_PIN   = A0;

// OE and MR are tied together in the circuit (sr1:OE and sr1:MR connected to GND)
static const uint8_t SR_OE_PIN = 0; // not used (wired to GND)
static const uint8_t SR_MR_PIN = 0; // not used (wired to VCC? actually MR is connected to GND in circuit)

static const uint8_t SERVO1_PWM_PIN = 2; // mega:2 -> servo1:PWM
static const uint8_t SERVO2_PWM_PIN = 3; // mega:3 -> servo2:PWM
static const uint8_t SERVO3_PWM_PIN = 4; // mega:4 -> servo3:PWM

// 7-seg segment mapping for common cathode (standard for logic segment driver with 74HC595 Qx->segments)
// Bit order based on typical assignment for a,b,c,d,e,f,g,dp:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7S->DP (per wiring comments)
// We'll set dp=0 always.
static const uint8_t segMap[10] = {
  // a b c d e f g (dp handled separately)
  // 0: a b c d e f
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5),
  // 1: b c
  (1<<1) | (1<<2),
  // 2: a b d e g
  (1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<6),
  // 3: a b c d g
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<6),
  // 4: b c f g
  (1<<1) | (1<<2) | (1<<5) | (1<<6),
  // 5: a c d f g
  (1<<0) | (1<<2) | (1<<3) | (1<<5) | (1<<6),
  // 6: a c d e f g
  (1<<0) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6),
  // 7: a b c
  (1<<0) | (1<<1) | (1<<2),
  // 8: a b c d e f g
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6),
  // 9: a b c d f g
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<5) | (1<<6)
};

Servo servo1;
Servo servo2;
Servo servo3;

static void srShift8(uint8_t value) {
  // Shift out MSB->LSB to match typical 74HC595 behavior.
  // But mapping in circuit uses Q0..Q7 directly to segments, so we want bit0->Q0, bit7->Q7.
  // When shifting MSB->LSB with standard shiftOut, bit0 ends up in Q0 if we shift 8 bits MSB-first.
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR1_SHCP_PIN, LOW); // keep clock low
    digitalWrite(SR1_DS_PIN, (value & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR1_SHCP_PIN, HIGH); // pulse shift register clock
  }
}

static void srLatch() {
  // STCP is the latch pin (per circuit wiring: mega:A1 -> sr1:STCP, shared with sr2)
  digitalWrite(SR1_STCP_PIN, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR1_STCP_PIN, LOW);
}

static void displayDigitToShiftRegisters(uint8_t tens, uint8_t ones) {
  // Two chained 74HC595s:
  // - sr1 drives sevseg1 segments A..G (Q0..Q6) and dp via Q7S (connected to sr2:DS per circuit)
  // - sr2 drives sevseg2 segments
  //
  // Wiring indicates sr1 and sr2 are cascaded:
  // mega:SR1:SHCP and STCP are shared (clock/latch), and sr1:Q7S -> sr2:DS.
  //
  // Therefore, we must shift 16 bits: first for sr2 (later in chain) then for sr1,
  // using cascade behavior.
  //
  // But with only SER clock and DS provided for the whole chain, the first 8 bits shifted
  // end up in the first register in the chain depending on MSB-first ordering.
  //
  // We'll avoid ambiguity by shifting 16 bits with explicit bit placement assuming:
  // - Bit order shifted MSB->LSB across the chain, where the first 8 shifted go to sr2,
  //   and the next 8 go to sr1 (common for Q7S cascading to DS).
  //
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7->dp
  //
  // We will set dp bit7 = 0.
  uint8_t segT = segMap[tens % 10] & 0x7F; // tens digit on sevseg1 via sr1
  uint8_t segU = segMap[ones % 10] & 0x7F; // ones digit on sevseg2 via sr2

  uint8_t sr1Out = segT; // bits 0..6; dp=0 at bit7
  uint8_t sr2Out = segU;

  // Shift 16 bits: sr2 first, then sr1.
  // Ensure dp bit7 = 0.
  uint16_t chain = ((uint16_t)sr2Out << 8) | (uint16_t)sr1Out;

  // Shift MSB->LSB for 16 bits
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_SHCP_PIN, LOW);
    digitalWrite(SR1_DS_PIN, (chain & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR1_SHCP_PIN, HIGH);
  }
  srLatch(); // latch updates both displays
}

void setup() {
  pinMode(SR1_SHCP_PIN, OUTPUT);
  pinMode(SR1_STCP_PIN, OUTPUT);
  pinMode(SR1_DS_PIN, OUTPUT);

  digitalWrite(SR1_SHCP_PIN, LOW);
  digitalWrite(SR1_STCP_PIN, LOW);
  digitalWrite(SR1_DS_PIN, LOW);

  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);
  servo3.attach(SERVO3_PWM_PIN);

  // Initialization: show "00" and servos at 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  displayDigitToShiftRegisters(0, 0);
}

void loop() {
  static int number = 0;
  static unsigned long lastUpdateMs = 0;

  unsigned long now = millis();
  if (now - lastUpdateMs >= 2000UL) {
    lastUpdateMs = now;

    // increment and wrap
    number++;
    if (number > 99) number = 0;

    uint8_t tens = (uint8_t)(number / 10);
    uint8_t ones = (uint8_t)(number % 10);

    // Display update immediately
    displayDigitToShiftRegisters(tens, ones);

    // Servo update immediately based on the displayed number (0..99)
    int angle1 = (int)round(number * 1.8); // degrees
    int angle2 = (int)round(number * 1.2);
    int angle3 = (int)round(number * 0.6);

    // Clamp to typical servo range to prevent out-of-bounds write
    angle1 = constrain(angle1, 0, 180);
    angle2 = constrain(angle2, 0, 180);
    angle3 = constrain(angle3, 0, 180);

    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);
  }
}
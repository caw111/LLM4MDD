#include <Arduino.h>
#include <Servo.h>

// Pins from the provided wiring (Arduino Mega 2560)
static const uint8_t SR_DATA_PIN  = A0; // sr1:DS
static const uint8_t SR_SHCP_PIN  = A1; // sr1:STCP (and connected to sr2)
static const uint8_t SR_STCP_PIN  = A2; // sr1:SHCP (and connected to sr2)

// Note: The wiring list shows:
// mega:A2 -> sr1:SHCP (gray)
// mega:A1 -> sr1:STCP (purple)
// mega:A0 -> sr1:DS (blue)
// So we map accordingly:
static const uint8_t SR_SHCP = A2;
static const uint8_t SR_STCP = A1;
static const uint8_t SR_DS   = A0;

// Servo pins
static const uint8_t SERVO1_PWM = 2; // mega:2 -> servo1:PWM
static const uint8_t SERVO2_PWM = 5; // mega:3 -> servo2:PWM in wiring; but list says mega:3 -> servo2:PWM
// Fix according to wiring exactly:
static const uint8_t SERVO2_PWM_WIRED = 3;

// Pot pins
static const uint8_t POT1_SIG = A3;
static const uint8_t POT2_SIG = A4;

// 7-seg codes: segment mapping inferred from shift-register outputs Q0..Q7S to segments
// Connections:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7S-> (cascaded DS to sr2; not used here for digit)
static const uint8_t SEGMENT_DIGIT[10] = {
  // A B C D E F G (bit0..bit6 respectively)
  // 0: A B C D E F
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),
  // 1: B C
  (1<<1)|(1<<2),
  // 2: A B D E G
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),
  // 3: A B C D G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),
  // 4: B C F G
  (1<<1)|(1<<2)|(1<<5)|(1<<6),
  // 5: A C D F G
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),
  // 6: A C D E F G
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 7: A B C
  (1<<0)|(1<<1)|(1<<2),
  // 8: A B C D E F G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 9: A B C D F G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)
};

Servo servo1;
Servo servo2;

// Smooth motion control
static float curAng1 = 0.0f, curAng2 = 0.0f;
static float tgtAng1 = 0.0f, tgtAng2 = 0.0f;
static unsigned long lastUpdateMs = 0;

// Smooth step rate (degrees per second)
static const float MAX_RATE_DEG_PER_SEC = 60.0f; // smooth enough; still "smoothly"

static inline int mapPotToDeg(int potRaw) {
  // 0..99 degrees
  long v = potRaw;
  // pot raw: 0..1023
  long deg = (v * 99L) / 1023L;
  if (deg < 0) deg = 0;
  if (deg > 99) deg = 99;
  return (int)deg;
}

static void shiftOutOneDigit(uint8_t digit, bool digitSelectFirst) {
  // digitSelectFirst: true -> show on sevseg1 (sr1), false -> show on sevseg2 (sr2)
  //
  // With cascading:
  // - Shift register sr1 receives SR_DS bits through SHCP (shift clock), and after 8 bits,
  //   sr2 receives subsequent bits via Q7S->sr2:DS.
  // - We must clock 16 bits total to set both registers:
  //   [sr1 segments][sr2 segments]
  //
  // We'll drive sr1 with the digit segments (or blank), sr2 with blank/digit accordingly.
  uint8_t b1 = 0;
  uint8_t b2 = 0;

  if (digitSelectFirst) {
    b1 = SEGMENT_DIGIT[digit];
    b2 = 0; // blank
  } else {
    b1 = 0; // blank
    b2 = SEGMENT_DIGIT[digit];
  }

  // SR: Q0..Q6 correspond to segments A..G.
  // We send 8 bits LSB-first: Q0 first.
  // For 16-bit cascade: first 8 bits go to sr1, next 8 bits to sr2.
  uint16_t word = ((uint16_t)b2 << 8) | b1;

  // shift 16 bits, LSB-first
  digitalWrite(SR_STCP, LOW);
  for (int i = 0; i < 16; i++) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (word >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SR_SHCP, HIGH);
  }
  // latch
  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

static void showTwoDigitNumber(int value) {
  // value: 0..99
  int tens = value / 10;
  int ones = value % 10;

  // We must output to both displays simultaneously:
  // - sr1 controls sevseg1 (tens)
  // - sr2 controls sevseg2 (ones)
  //
  // Build cascade word: lower byte = sr1, upper byte = sr2
  uint8_t b1 = SEGMENT_DIGIT[tens];
  uint8_t b2 = SEGMENT_DIGIT[ones];
  uint16_t word = ((uint16_t)b2 << 8) | b1;

  digitalWrite(SR_STCP, LOW);
  for (int i = 0; i < 16; i++) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (word >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SR_SHCP, HIGH);
  }
  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

static void updateServosSmooth() {
  unsigned long now = millis();
  float dt = (now - lastUpdateMs) / 1000.0f;
  if (dt < 0.001f) return;
  lastUpdateMs = now;

  // Servo1
  float delta1 = tgtAng1 - curAng1;
  float maxStep1 = MAX_RATE_DEG_PER_SEC * dt;
  if (delta1 > maxStep1) delta1 = maxStep1;
  if (delta1 < -maxStep1) delta1 = -maxStep1;
  curAng1 += delta1;
  servo1.write((int)lround(curAng1));

  // Servo2
  float delta2 = tgtAng2 - curAng2;
  float maxStep2 = MAX_RATE_DEG_PER_SEC * dt;
  if (delta2 > maxStep2) delta2 = maxStep2;
  if (delta2 < -maxStep2) delta2 = -maxStep2;
  curAng2 += delta2;
  servo2.write((int)lround(curAng2));
}

void setup() {
  // Shift register pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);

  // Servos
  servo1.attach(SERVO1_PWM);
  servo2.attach(SERVO2_PWM_WIRED);

  // Initialization: displays "00" and servos at 0 degrees
  curAng1 = 0.0f; curAng2 = 0.0f;
  tgtAng1 = 0.0f; tgtAng2 = 0.0f;

  servo1.write(0);
  servo2.write(0);

  showTwoDigitNumber(0);

  lastUpdateMs = millis();
}

void loop() {
  // Read potentiometers
  int pot1Raw = analogRead(POT1_SIG);
  int pot2Raw = analogRead(POT2_SIG);

  int deg1 = mapPotToDeg(pot1Raw);
  int deg2 = mapPotToDeg(pot2Raw);

  // Update targets
  tgtAng1 = (float)deg1;
  tgtAng2 = (float)deg2;

  // Displays should show current angles of servo1 (tens and units)
  // Use target angle as "current angles"? Requirement: "show the current angles of servo1"
  // We'll show the smoothed current angle of servo1.
  int displayVal = (int)lround(curAng1);
  if (displayVal < 0) displayVal = 0;
  if (displayVal > 99) displayVal = 99;
  showTwoDigitNumber(displayVal);

  // Smooth servo movement towards targets
  updateServosSmooth();
}
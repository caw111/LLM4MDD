#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t LED1_PIN = 2;   // led1:A -> mega:2
static const uint8_t LED2_PIN = 3;   // led2:A -> mega:3

// 74HC595 connections (sr1)
static const uint8_t SR_DATA_PIN  = A0; // mega:A0 -> sr1:DS
static const uint8_t SR_LATCH_PIN = A1; // mega:A1 -> sr1:STCP
static const uint8_t SR_CLOCK_PIN = A2; // mega:A2 -> sr1:SHCP

// Servos
static const uint8_t SERVO1_PWM_PIN = 4; // mega:4 -> servo1:PWM
static const uint8_t SERVO2_PWM_PIN = 5; // mega:5 -> servo2:PWM

Servo servo1;
Servo servo2;

// 7-seg decoder: mapping from digits 0-9 to Q outputs (Q0..Q7).
// Wiring from sr1:Qn -> sevseg1 pins:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G (Q7 unused).
// We'll assume a common-cathode style where segments are ON with HIGH.
// If your hardware is common-anode, you'd need inversion; not changed here.
static const uint8_t digitToSegments[10] = {
  // A B C D E F G (Q0..Q6)
  // 0: A,B,C,D,E,F on, G off
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: B,C on
  (0 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5) ,
  // 2: A,B,D,E,G on
  (1 << 0) | (1 << 1) | (0 << 2) | (1 << 3) | (1 << 4) | (0 << 5) | (1 << 6),
  // 3: A,B,C,D,G on
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (0 << 5) | (1 << 6),
  // 4: B,C,F,G on
  (0 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (1 << 5) | (1 << 6),
  // 5: A,C,D,F,G on
  (1 << 0) | (0 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5) | (1 << 6),
  // 6: A,C,D,E,F,G on
  (1 << 0) | (0 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: A,B,C on
  (1 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5),
  // 8: A,B,C,D,E,F,G on
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: A,B,C,D,F,G on
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5) | (1 << 6)
};

// digitToSegments[0..9] uses bits 0..6 for segments A..G.
// Q7 is left off (bit7 = 0).
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void set7SegDigit(uint8_t digit);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startServoMove(ServoController &s, int fromDeg, int toDeg, unsigned long nowMs);
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startServoHold(ServoController &s, unsigned long nowMs);
#line 110 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateServo(ServoController &s, Servo &servo, unsigned long nowMs);
#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 184 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void set7SegDigit(uint8_t digit) {
  if (digit > 9) digit = 0;
  uint8_t pattern = digitToSegments[digit] & 0x7F;

  // Ensure bit 7 is 0; shift out MSB->LSB or LSB->MSB doesn't matter if wiring expects Qn = bit n.
  // We'll clock out MSB first? We'll implement LSB-first so that the first shifted bit corresponds to DS->Q0.
  // Many implementations use LSB-first with "Q0 is first shifted"; we will follow that here.
  digitalWrite(SR_LATCH_PIN, LOW);
  // shift out 8 bits: bit0 -> Q0, bit1 -> Q1 ... bit7 -> Q7
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (pattern >> i) & 0x01);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }
  digitalWrite(SR_LATCH_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(SR_LATCH_PIN, LOW);
}

// ---------------- LED alternating ----------------
unsigned long lastLedChange = 0;
bool led1State = false; // "first led1 off, led2 on"
bool ledPhaseLed1On = false; // false => led1 off / led2 on

// ---------------- 7-seg counting ----------------
unsigned long lastSegChange = 0;
uint8_t currentDigit = 0;

// ---------------- Servos sweeping ----------------
// Each servo sweep takes 4s total per rule, moving between endpoints with 2s hold at endpoints.
// That implies: move time = 2s (0<->180 or 180<->0) and hold time = 2s.
// We'll do opposite directions.
enum ServoState { MOVE = 0, HOLD = 1 };

struct ServoController {
  ServoState state;
  float startAngle;
  float endAngle;
  unsigned long stateStartMs;
  unsigned long moveDurationMs;
  unsigned long holdDurationMs;
  bool initialized; // if not, allow immediate start without initial 2s stop
  int currentTargetAngle; // last endpoint target
};

ServoController s1, s2;

static void startServoMove(ServoController &s, int fromDeg, int toDeg, unsigned long nowMs) {
  s.state = MOVE;
  s.startAngle = fromDeg;
  s.endAngle = toDeg;
  s.stateStartMs = nowMs;
  s.currentTargetAngle = toDeg;
}

static void startServoHold(ServoController &s, unsigned long nowMs) {
  s.state = HOLD;
  s.stateStartMs = nowMs;
}

static void updateServo(ServoController &s, Servo &servo, unsigned long nowMs) {
  if (s.state == MOVE) {
    unsigned long elapsed = nowMs - s.stateStartMs;
    if (elapsed >= s.moveDurationMs) {
      servo.write((int)round(s.endAngle));
      // At endpoint, decide whether to hold 2s. For the very first state (before first sweep),
      // "the servo don't need to stop for 2 second".
      // We'll interpret "before the servo start the first sweep" as: if s.initialized==false,
      // then we start with MOVE immediately at t=0 without requiring initial hold.
      // Once the servo reaches an endpoint due to our sweep, we always hold 2s.
      startServoHold(s, nowMs);
    } else {
      float t = (float)elapsed / (float)s.moveDurationMs; // 0..1
      float angle = s.startAngle + (s.endAngle - s.startAngle) * t;
      servo.write((int)round(angle));
    }
  } else { // HOLD
    unsigned long elapsed = nowMs - s.stateStartMs;
    if (elapsed >= s.holdDurationMs) {
      // Next move: bounce between 0 and 180
      int nextFrom = (int)round(s.endAngle);
      int nextTo = (nextFrom == 0) ? 180 : 0;
      startServoMove(s, nextFrom, nextTo, nowMs);
    }
  }
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);

  // Initial LED requirement: first led1 off, led2 on
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  ledPhaseLed1On = false;
  lastLedChange = millis();

  // Initial 7-seg digit
  set7SegDigit(0);
  currentDigit = 0;
  lastSegChange = millis();

  // Setup servos and controllers
  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);

  // Per requirement: servo positions may already be at 0° or 180° before start; no 2s stop needed initially.
  // We'll still set them explicitly to their starting endpoints:
  servo1.write(0);
  servo2.write(180);

  // Servo timing: move 2s, hold 2s => each full sweep cycle 4s.
  s1.moveDurationMs = 2000;
  s1.holdDurationMs = 2000;
  s1.initialized = false; // allow immediate first move
  s2.moveDurationMs = 2000;
  s2.holdDurationMs = 2000;
  s2.initialized = false;

  unsigned long nowMs = millis();

  // Start first sweeps immediately at t=0:
  // servo1: 0 -> 180 -> 0 ...
  // servo2: 180 -> 0 -> 180 ...
  startServoMove(s1, 0, 180, nowMs);
  startServoMove(s2, 180, 0, nowMs);
  s1.initialized = true;
  s2.initialized = true;
}

void loop() {
  unsigned long nowMs = millis();

  // LEDs: alternate every 2s, led1 off/led2 on first, then led1 on/led2 off.
  if (nowMs - lastLedChange >= 2000UL) {
    lastLedChange = nowMs;

    ledPhaseLed1On = !ledPhaseLed1On;
    if (ledPhaseLed1On) {
      // led1 on, led2 off
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
    } else {
      // led1 off, led2 on
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
    }
  }

  // 7-seg: count 0..9, each for 2s, reset after 9
  if (nowMs - lastSegChange >= 2000UL) {
    lastSegChange = nowMs;

    currentDigit++;
    if (currentDigit > 9) currentDigit = 0;
    set7SegDigit(currentDigit);
  }

  // Servos: move continuously with 2s hold at endpoints, total sweep cycle 4s each.
  updateServo(s1, servo1, nowMs);
  updateServo(s2, servo2, nowMs);
}

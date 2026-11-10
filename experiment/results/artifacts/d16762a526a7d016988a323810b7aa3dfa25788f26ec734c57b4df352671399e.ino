#include <Arduino.h>
#include <Servo.h>

// ---------------- LEDs ----------------
static const uint8_t LED1_PIN = 2; // mega:2
static const uint8_t LED2_PIN = 3; // mega:3

// ---------------- 74HC595 (7-seg) ----------------
static const uint8_t SR_DATA_PIN  = A0; // mega:A0 -> sr1:DS
static const uint8_t SR_LATCH_PIN = A1; // mega:A1 -> sr1:STCP
static const uint8_t SR_CLOCK_PIN = A2; // mega:A2 -> sr1:SHCP

// digit -> segments bits [0..6] for A..G (Q0..Q6)
static const uint8_t digitToSegments[10] = {
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),                          // 0
  (0 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5),                          // 1
  (1 << 0) | (1 << 1) | (0 << 2) | (1 << 3) | (1 << 4) | (0 << 5) | (1 << 6),              // 2
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (0 << 5) | (1 << 6),              // 3
  (0 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (1 << 5) | (1 << 6),              // 4
  (1 << 0) | (0 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5) | (1 << 6),              // 5
  (1 << 0) | (0 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),              // 6
  (1 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5),                          // 7
  (1 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5) | (1 << 6),              // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5) | (1 << 6)               // 9
};

static void set7SegDigit(uint8_t digit) {
  if (digit > 9) digit = 0;
  uint8_t pattern = digitToSegments[digit] & 0x7F; // only A..G bits, Q0..Q6

  digitalWrite(SR_LATCH_PIN, LOW);
  for (uint8_t i = 0; i < 8; i++) { // bit7 (Q7) ignored
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (pattern >> i) & 0x01);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }
  digitalWrite(SR_LATCH_PIN, HIGH);
}

// ---------------- Servos ----------------
static const uint8_t SERVO1_PWM_PIN = 4; // mega:4 -> servo1:PWM
static const uint8_t SERVO2_PWM_PIN = 5; // mega:5 -> servo2:PWM

Servo servo1;
Servo servo2;

// Full cycle is 4s: move 0<->180 in 2s, then hold 2s at endpoints.
// servo1: 0->180->0, servo2: 180->0->180 (opposite directions at same time).
enum ServoState { MOVE = 0, HOLD = 1 };

struct ServoController {
  ServoState state;
  float startAngle;
  float endAngle;
  unsigned long stateStartMs;
  unsigned long moveDurationMs;
  unsigned long holdDurationMs;
};

static ServoController s1;
static ServoController s2;

static void startServoMove(ServoController &s, int fromDeg, int toDeg, unsigned long nowMs) {
  s.state = MOVE;
  s.startAngle = (float)fromDeg;
  s.endAngle = (float)toDeg;
  s.stateStartMs = nowMs;
}

static void startServoHold(ServoController &s, unsigned long nowMs) {
  s.state = HOLD;
  s.stateStartMs = nowMs;
}

static void updateServo(ServoController &s, Servo &servo, unsigned long nowMs) {
  if (s.state == MOVE) {
    unsigned long elapsed = nowMs - s.stateStartMs;
    if (elapsed >= s.moveDurationMs) {
      servo.write((int)lroundf(s.endAngle));
      startServoHold(s, nowMs);
    } else {
      float t = (float)elapsed / (float)s.moveDurationMs; // 0..1
      float angle = s.startAngle + (s.endAngle - s.startAngle) * t;
      servo.write((int)lroundf(angle));
    }
  } else { // HOLD
    unsigned long elapsed = nowMs - s.stateStartMs;
    if (elapsed >= s.holdDurationMs) {
      int nextFrom = (int)lroundf(s.endAngle);
      int nextTo = (nextFrom == 0) ? 180 : 0;
      startServoMove(s, nextFrom, nextTo, nowMs);
    }
  }
}

// ---------------- Global timing states ----------------
static unsigned long lastLedChange = 0;
static bool ledPhaseLed1On = false; // false => led1 off, led2 on

static unsigned long lastSegChange = 0;
static uint8_t currentDigit = 0;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // First: led1 off, led2 on
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  ledPhaseLed1On = false;
  lastLedChange = millis();

  // 7-seg via 74HC595
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);

  set7SegDigit(0);
  currentDigit = 0;
  lastSegChange = millis();

  // Servos
  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);

  servo1.write(0);
  servo2.write(180);

  // Timing: move 2s, hold 2s (each full cycle = 4s)
  s1.moveDurationMs = 2000UL;
  s1.holdDurationMs = 2000UL;
  s2.moveDurationMs = 2000UL;
  s2.holdDurationMs = 2000UL;

  unsigned long nowMs = millis();

  // Start first sweeps immediately at t=0 (no initial 2s hold required).
  startServoMove(s1, 0, 180, nowMs);
  startServoMove(s2, 180, 0, nowMs);
}

void loop() {
  unsigned long nowMs = millis();

  // LEDs: alternate every 2s, with led1 off/led2 on first
  if (nowMs - lastLedChange >= 2000UL) {
    lastLedChange = nowMs;
    ledPhaseLed1On = !ledPhaseLed1On;

    if (ledPhaseLed1On) {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
    } else {
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

  // Servos: move/hold behavior, opposite directions simultaneously
  updateServo(s1, servo1, nowMs);
  updateServo(s2, servo2, nowMs);
}
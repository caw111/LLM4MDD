#include <Arduino.h>
#include <Servo.h>

static const uint8_t BTN1_PIN = 2;   // io_0
static const uint8_t BTN2_PIN = 3;   // io_1
static const uint8_t BTN3_PIN = 4;   // io_2

static const uint8_t SEG0_PIN = 5;   // io_3_segment_0 (D5)
static const uint8_t SEG1_PIN = 6;   // io_3_segment_1 (D6)
static const uint8_t SEG2_PIN = 7;   // io_3_segment_2 (D7)
static const uint8_t SEG3_PIN = 8;   // io_3_segment_3 (D8)
static const uint8_t SEG4_PIN = 9;   // io_3_segment_4 (D9)
static const uint8_t SEG5_PIN = 10;  // io_3_segment_5 (D10)
static const uint8_t SEG6_PIN = 11;  // io_3_segment_6 (D11)
static const uint8_t SEG7_PIN = 12;  // io_3_segment_7 (D12)
static const uint8_t SEG8_PIN = 13;  // io_3_segment_8 (D13)
static const uint8_t SEG9_PIN = 44;  // io_3_segment_9 (D44)

static const uint8_t SERVO_PWM_PIN = 45; // io_4 (servo_angle)

static const uint32_t CONTROL_PERIOD_MS = 5;
static const uint32_t HOLD_MS = 2000;

Servo servo1;

// Controller state (per accepted model)
static double activeAngleDeg = 0.0;
static int32_t activeLedLevel = 0;   // 0..10
static uint32_t holdElapsedMs = 0;

static bool btn1PrevPressed = false;
static bool btn2PrevPressed = false;
static bool btn3PrevPressed = false;

static bool readPressed(uint8_t pin) {
  // active_high=false means button pressed corresponds to LOW at pin due to pull-up
  return digitalRead(pin) == LOW;
}

static void applyOutputs() {
  // LED bar outputs: active_high true -> HIGH lights segments
  digitalWrite(SEG0_PIN, (activeLedLevel >= 1) ? HIGH : LOW);
  digitalWrite(SEG1_PIN, (activeLedLevel >= 2) ? HIGH : LOW);
  digitalWrite(SEG2_PIN, (activeLedLevel >= 3) ? HIGH : LOW);
  digitalWrite(SEG3_PIN, (activeLedLevel >= 4) ? HIGH : LOW);
  digitalWrite(SEG4_PIN, (activeLedLevel >= 5) ? HIGH : LOW);
  digitalWrite(SEG5_PIN, (activeLedLevel >= 6) ? HIGH : LOW);
  digitalWrite(SEG6_PIN, (activeLedLevel >= 7) ? HIGH : LOW);
  digitalWrite(SEG7_PIN, (activeLedLevel >= 8) ? HIGH : LOW);
  digitalWrite(SEG8_PIN, (activeLedLevel >= 9) ? HIGH : LOW);
  digitalWrite(SEG9_PIN, (activeLedLevel >= 10) ? HIGH : LOW);

  // Servo output holds last valid; model simply outputs activeAngleDeg
  servo1.write((int)activeAngleDeg);
}

void setup() {
  // Button inputs with pull-up
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // LED bar segments outputs
  pinMode(SEG0_PIN, OUTPUT);
  pinMode(SEG1_PIN, OUTPUT);
  pinMode(SEG2_PIN, OUTPUT);
  pinMode(SEG3_PIN, OUTPUT);
  pinMode(SEG4_PIN, OUTPUT);
  pinMode(SEG5_PIN, OUTPUT);
  pinMode(SEG6_PIN, OUTPUT);
  pinMode(SEG7_PIN, OUTPUT);
  pinMode(SEG8_PIN, OUTPUT);
  pinMode(SEG9_PIN, OUTPUT);

  // Servo
  servo1.attach(SERVO_PWM_PIN);

  // Initial state per model: angle 0, LED level 0
  activeAngleDeg = 0.0;
  activeLedLevel = 0;
  holdElapsedMs = 0;
  btn1PrevPressed = false;
  btn2PrevPressed = false;
  btn3PrevPressed = false;

  applyOutputs();
}

void loop() {
  const uint32_t tickStart = millis();

  // Read model inputs (pressed states)
  const bool io_0 = readPressed(BTN1_PIN);
  const bool io_1 = readPressed(BTN2_PIN);
  const bool io_2 = readPressed(BTN3_PIN);

  // Updates per accepted SysML tick action
  if (io_0 && (!btn1PrevPressed)) {
    activeAngleDeg = 30.0;
    activeLedLevel = 3;
    holdElapsedMs = 0;
  }
  if (io_1 && (!btn2PrevPressed)) {
    activeAngleDeg = 60.0;
    activeLedLevel = 6;
    holdElapsedMs = 0;
  }
  if (io_2 && (!btn3PrevPressed)) {
    activeAngleDeg = 90.0;
    activeLedLevel = 10;
    holdElapsedMs = 0;
  }

  if (holdElapsedMs < HOLD_MS) {
    // holdElapsedMs := holdElapsedMs (per model), but to enforce time maintenance
    // we accumulate elapsed time in wall clock at control period granularity.
    // This preserves "maintain at least 2000ms" behavior for selected states.
    holdElapsedMs = (holdElapsedMs + CONTROL_PERIOD_MS <= HOLD_MS) ? (holdElapsedMs + CONTROL_PERIOD_MS) : HOLD_MS;
  }

  // Update prev pressed values
  btn1PrevPressed = io_0;
  btn2PrevPressed = io_1;
  btn3PrevPressed = io_2;

  // Outputs per model
  applyOutputs();

  // Enforce control period ~5ms
  const uint32_t elapsed = millis() - tickStart;
  if (elapsed < CONTROL_PERIOD_MS) {
    delay(CONTROL_PERIOD_MS - elapsed);
  }
}
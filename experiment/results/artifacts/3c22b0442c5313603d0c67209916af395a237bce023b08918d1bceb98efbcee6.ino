#include <Arduino.h>
#include <Servo.h>

static const uint8_t PIN_SERVO1 = 44;
static const uint8_t PIN_SERVO2 = 45;

static const uint8_t PIN_LED1 = 2;   // led1 -> D2
static const uint8_t PIN_LED2 = 3;   // led2 -> D3

// bargraph1 segments -> D4..D13 in order A1..A10
static const uint8_t PIN_BAR[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

static const uint32_t CONTROL_PERIOD_MS = 10;
static const uint32_t STEP_PAUSE_MS = 2000;

// Model internal state: t_ms, s1_step, s2_step
static int32_t t_ms = 0;
static int32_t s1_step = 0;
static int32_t s2_step = 18; // 18 * 10deg = 180deg

// Servo commands derived from steps (degrees)
static inline int16_t servo1Deg() { return (int16_t)(s1_step * 10); }
static inline int16_t servo2Deg() { return (int16_t)(s2_step * 10); }

// Output expressions derived from commanded servo angles
static inline bool led1_on(int16_t s1deg) {
  // ON when Servo1 between 0 and 90 inclusive; OFF when 91..180
  return (s1deg >= 0 && s1deg <= 90);
}
static inline bool led2_on(int16_t s2deg) {
  // ON when Servo2 between 90 and 180 inclusive; OFF when 0..89
  return (s2deg >= 90 && s2deg <= 180);
}
static inline void bargraphFromServo1(int16_t s1deg, bool outSeg[10]) {
  // Map 0..180 -> segments 0..10 lit, where:
  // 0deg -> 0 lit, 90deg -> 5 lit, 180deg -> 10 lit
  // Equivalent: litCount = floor(s1deg/18) for multiples of 10 gives:
  // 0->0, 10->0, 20->1, ..., 180->10. But requirement expects sync with increments of 10:
  // for 0..180 in 10deg steps, the intended bargraph is:
  // segmentsLit = s1_step (since 0deg=0 segments, 90deg=9? not matching).
  // Use the explicit required examples: 90deg => 5 segments.
  // Therefore segmentsLit = (s1deg / 18) using integer division does not yield 5 at 90 (90/18=5).
  // For 180 => 10 (180/18=10). For 0 => 0. Matches examples.
  int16_t litCount = s1deg / 18;
  if (litCount < 0) litCount = 0;
  if (litCount > 10) litCount = 10;
  for (int i = 0; i < 10; ++i) outSeg[i] = (i < litCount);
}

Servo servo1;
Servo servo2;

static void applyOutputs(int16_t s1deg, int16_t s2deg) {
  const bool led1 = led1_on(s1deg);
  const bool led2 = led2_on(s2deg);

  digitalWrite(PIN_LED1, led1 ? HIGH : LOW);
  digitalWrite(PIN_LED2, led2 ? HIGH : LOW);

  bool seg[10];
  bargraphFromServo1(s1deg, seg);

  for (int i = 0; i < 10; ++i) {
    digitalWrite(PIN_BAR[i], seg[i] ? HIGH : LOW);
  }
}

static void writeServos(int16_t s1deg, int16_t s2deg) {
  // Use model's commanded angles directly.
  servo1.write(s1deg);
  servo2.write(s2deg);
}

static void modelStep() {
  // Accept the provided "ACCEPTED executable model bundle" semantics:
  // - On tick start: t_ms := 0
  // - Then if false { ... } else { s1_step := 0; s2_step := 18 }
  // Therefore, s1_step and s2_step are held at initial values and never advance.
  // This is the frozen controller behavior to be preserved.
  t_ms = 0;
  s1_step = 0;
  s2_step = 18;
}

void setup() {
  // Initialize IO
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  for (int i = 0; i < 10; ++i) {
    pinMode(PIN_BAR[i], OUTPUT);
    digitalWrite(PIN_BAR[i], LOW);
  }

  // Initial state: both servos at 0°, all LEDs off.
  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);

  servo1.write(0);
  servo2.write(0);

  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  // Align internal model state to contract initial step values.
  t_ms = 0;
  s1_step = 0;
  s2_step = 18;
}

void loop() {
  static uint32_t nextTick = 0;
  uint32_t now = millis();
  if (now < nextTick) return;
  nextTick = now + CONTROL_PERIOD_MS;

  // Update controller state according to accepted model
  modelStep();

  // Compute outputs from commanded servo angles derived from step indexes.
  int16_t s1deg = servo1Deg(); // s1_step * 10
  int16_t s2deg = servo2Deg(); // s2_step * 10

  // Apply actuator commands and output LEDs/bargraph every control period.
  writeServos(s1deg, s2deg);
  applyOutputs(s1deg, s2deg);
}
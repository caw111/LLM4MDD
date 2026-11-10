#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t LED1_PIN = 2;  // io_0
static const uint8_t LED2_PIN = 3;  // io_1
static const uint8_t LED3_PIN = 4;  // io_2
static const uint8_t SERVO1_PIN = 5; // io_3
static const uint8_t SERVO2_PIN = 6; // io_4

// Control period granularity: 10 ms (contract)
static const uint32_t CONTROL_PERIOD_MS = 10;

// Step timing: increment/decrement by 30 degrees every 2 seconds,
// with immediate move then dwell for 2 seconds.
static const uint32_t STEP_DWELL_MS = 2000;

// LED3 pulse duration: 2 seconds
static const uint32_t LED3_PULSE_MS = 2000;

// Angles and step sizes per requirement
static const double SERVO1_START_DEG = 0.0;
static const double SERVO1_TARGET_DEG = 180.0;
static const double SERVO2_START_DEG = 180.0;
static const double SERVO2_TARGET_DEG = 0.0;
static const double STEP_DEG = 30.0;

Servo servo1;
Servo servo2;

// Model state encoded explicitly to match SysML "accepted executable model" behavior
// States: Servo1 moving in 2s steps until 180, then Servo2 moving in 2s steps until 0,
// then LED3 on for 2s while holding targets, then reset and repeat.
enum CycleState : uint8_t {
  MOVING_SERVO1 = 0,
  MOVING_SERVO2 = 1,
  LED3_PULSE    = 2
};

static CycleState state;
static uint8_t servo1StepIndex; // 0..6 for 0..180 (7 positions: 0,30,...,180)
static uint8_t servo2StepIndex; // 0..6 for 180..0 (7 positions: 180,150,...,0)

static uint32_t stateStartMs;

// Sample of contract initial outputs at first control step:
// leds off, servo1=0, servo2=180.
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(bool led1, bool led2, bool led3, double s1_deg, double s2_deg);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void enterState(CycleState newState, uint32_t nowMs);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void applyOutputs(bool led1, bool led2, bool led3, double s1_deg, double s2_deg) {
  digitalWrite(LED1_PIN, led1 ? HIGH : LOW);
  digitalWrite(LED2_PIN, led2 ? HIGH : LOW);
  digitalWrite(LED3_PIN, led3 ? HIGH : LOW);
  servo1.write((int)lround(s1_deg));
  servo2.write((int)lround(s2_deg));
}

static void enterState(CycleState newState, uint32_t nowMs) {
  state = newState;
  stateStartMs = nowMs;

  // Set exact commanded outputs for this state right away (immediately move).
  // Also preserve the required LED semantics.
  if (state == MOVING_SERVO1) {
    // servo1StepIndex already set by caller; servo2StepIndex not used here.
    double s1_deg = SERVO1_START_DEG + STEP_DEG * (double)servo1StepIndex;
    double s2_deg = SERVO2_TARGET_DEG; // During servo1 moving phase, servo2 is held at its start position? Requirement says "Then Servo2 ..."
    // But cycle semantics imply servo2 moves only after servo1 reaches 180.
    // Therefore, servo2 remains at its start (180) until servo2 phase begins.
    s2_deg = SERVO2_START_DEG;
    applyOutputs(true, false, false, s1_deg, s2_deg);
  } else if (state == MOVING_SERVO2) {
    // servo2StepIndex already set by caller.
    double s2_deg = SERVO2_START_DEG - STEP_DEG * (double)servo2StepIndex;
    double s1_deg = SERVO1_TARGET_DEG; // hold servo1 at 180 during servo2 phase
    applyOutputs(false, true, false, s1_deg, s2_deg);
  } else { // LED3_PULSE
    // Hold at final target positions; LED3 on for 2 seconds.
    applyOutputs(false, false, true, SERVO1_TARGET_DEG, SERVO2_TARGET_DEG);
  }
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Contract initialization:
  // all LEDs off, servo1 commanded to 0°, servo2 commanded to 180°.
  // Initialize model sequence.
  servo1StepIndex = 0; // 0 deg
  servo2StepIndex = 0; // 180 deg

  uint32_t now = millis();
  enterState(MOVING_SERVO1, now);
}

void loop() {
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();

  // Enforce 10ms control-period granularity for sequencing.
  if (now - lastTickMs < CONTROL_PERIOD_MS) {
    return;
  }
  // Advance in 10ms increments deterministically (handle possible delays)
  lastTickMs = now - ((now - lastTickMs) % CONTROL_PERIOD_MS);

  // State machine update driven by elapsed time since state entry.
  uint32_t elapsed = now - stateStartMs;

  if (state == MOVING_SERVO1) {
    // servo1 changes every 2 seconds, immediately on boundary.
    // Sequence: servo1StepIndex: 0->1->...->6, with 2s dwell each.
    if (elapsed >= STEP_DWELL_MS) {
      // Move to next step (immediate update on transition)
      if (servo1StepIndex < 6) {
        servo1StepIndex++;
        // Keep servo2 at 180 during servo1 movement.
        enterState(MOVING_SERVO1, now); // re-enter to apply outputs with new s1 index
      } else {
        // Reached 180 at servo1StepIndex==6; next is servo2 movement phase.
        servo2StepIndex = 0; // start servo2 at 180
        enterState(MOVING_SERVO2, now);
      }
    }
  } else if (state == MOVING_SERVO2) {
    if (elapsed >= STEP_DWELL_MS) {
      if (servo2StepIndex < 6) {
        servo2StepIndex++;
        enterState(MOVING_SERVO2, now); // re-enter to apply new s2 index
      } else {
        // Reached servo2 0; next LED3 pulse for 2 seconds.
        enterState(LED3_PULSE, now);
      }
    }
  } else { // LED3_PULSE
    if (elapsed >= LED3_PULSE_MS) {
      // Reset cycle: servo1 back to initial 0, servo2 back to initial 180.
      servo1StepIndex = 0;
      servo2StepIndex = 0;
      enterState(MOVING_SERVO1, now);
    }
  }

  // No other behavior required; outputs are set on state entry and re-entry.
  // Contract semantics: invalid_policy hold_last_valid is naturally satisfied since
  // we continuously command angles at state transitions only (and keep them otherwise).
}

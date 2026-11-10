#include <Arduino.h>
#include <Servo.h>

static const uint8_t SERVO1_PIN = 2;   // mega D2
static const uint8_t SERVO2_PIN = 3;   // mega D3
static const uint8_t SERVO3_PIN = 4;   // mega D4

// Bargraph segment outputs: A1..A10 on mega D5..D13, D44
static const uint8_t SEG_PINS[10] = {
  5,  // A1 -> D5
  6,  // A2 -> D6
  7,  // A3 -> D7
  8,  // A4 -> D8
  9,  // A5 -> D9
  10, // A6 -> D10
  11, // A7 -> D11
  12, // A8 -> D12
  13, // A9 -> D13
  44  // A10 -> D44
};

enum Phase {
  PH_INIT = 0,
  PH_PHASE1 = 1,
  PH_PHASE2 = 2,
  PH_PHASE3 = 3,
  PH_RESET  = 4
};

static Servo servo1;
static Servo servo2;
static Servo servo3;

static Phase phase = PH_INIT;
static uint32_t phaseStartMs = 0;

static void setAllServosTarget(float deg) {
  const int angle = (int)deg; // contracted angles are integer (0 or 180)
  servo1.write(angle);
  servo2.write(angle);
  servo3.write(angle);
}

static void setServoTargets(float deg1, float deg2, float deg3) {
  servo1.write((int)deg1);
  servo2.write((int)deg2);
  servo3.write((int)deg3);
}

static void setBargraphSegments(bool onFirstN) {
  // Contract: turn on exactly the first N segments based on the given step.
  for (int i = 0; i < 10; ++i) {
    digitalWrite(SEG_PINS[i], onFirstN ? HIGH : LOW);
  }
}

static void setBargraphUpTo(int countOn) {
  // countOn in [0..10]
  for (int i = 0; i < 10; ++i) {
    digitalWrite(SEG_PINS[i], (i < countOn) ? HIGH : LOW);
  }
}

static void applyOutputsForPhase(Phase p) {
  // Maintain exact required controller state semantics:
  // - io_0/1/2 are servo targets (deg)
  // - io_3_segment_k are bargraph segment bool outputs
  switch (p) {
    case PH_INIT:
      setServoTargets(0.0f, 0.0f, 0.0f);
      setBargraphUpTo(0);
      break;

    case PH_PHASE1:
      setServoTargets(180.0f, 0.0f, 0.0f);
      setBargraphUpTo(3);
      break;

    case PH_PHASE2:
      setServoTargets(180.0f, 180.0f, 0.0f);
      setBargraphUpTo(6);
      break;

    case PH_PHASE3:
      setServoTargets(180.0f, 180.0f, 180.0f);
      setBargraphUpTo(10);
      break;

    case PH_RESET:
      setServoTargets(0.0f, 0.0f, 0.0f);
      setBargraphUpTo(0);
      break;
  }
}

void setup() {
  // GPIO init for bargraph segments (active HIGH)
  for (int i = 0; i < 10; ++i) {
    pinMode(SEG_PINS[i], OUTPUT);
    digitalWrite(SEG_PINS[i], LOW);
  }

  // Servo init
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization state: all servos at 0°, all segments off
  phase = PH_INIT;
  phaseStartMs = millis();
  applyOutputsForPhase(phase);
}

void loop() {
  const uint32_t now = millis();

  // Each phase must persist for at least 2 seconds.
  // We use deterministic state transitions with 2000ms holds.
  const uint32_t HOLD_MS = 2000;

  switch (phase) {
    case PH_INIT:
      // Immediately after setup, go to phase 1 while keeping init outputs applied initially.
      // Hold init state for >=2 seconds to be safe with the "at least 2 seconds" requirement.
      if (now - phaseStartMs >= HOLD_MS) {
        phase = PH_PHASE1;
        phaseStartMs = now;
        applyOutputsForPhase(phase);
      }
      break;

    case PH_PHASE1:
      if (now - phaseStartMs >= HOLD_MS) {
        phase = PH_PHASE2;
        phaseStartMs = now;
        applyOutputsForPhase(phase);
      }
      break;

    case PH_PHASE2:
      if (now - phaseStartMs >= HOLD_MS) {
        phase = PH_PHASE3;
        phaseStartMs = now;
        applyOutputsForPhase(phase);
      }
      break;

    case PH_PHASE3:
      if (now - phaseStartMs >= HOLD_MS) {
        phase = PH_RESET;
        phaseStartMs = now;
        applyOutputsForPhase(phase);
      }
      break;

    case PH_RESET:
      if (now - phaseStartMs >= HOLD_MS) {
        phase = PH_PHASE1;
        phaseStartMs = now;
        applyOutputsForPhase(phase);
      }
      break;
  }

  // No serial IO per contract; keep loop lightweight.
}
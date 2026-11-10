#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Frozen interface contract bindings:
// io_0 -> servo1 target angle (deg), pin D2
// io_1 -> servo2 target angle (deg), pin D3
// io_2 -> servo3 target angle (deg), pin D4
// io_3 -> elapsed_ms delta since previous control step (ms), sample/control period 10ms

static const uint8_t SERVO1_PIN = 2; // D2
static const uint8_t SERVO2_PIN = 3; // D3
static const uint8_t SERVO3_PIN = 4; // D4

static const int32_t STEP_HOLD_MS = 2000;
static const uint32_t CONTROL_PERIOD_MS = 10;

Servo servo1;
Servo servo2;
Servo servo3;

// Controller state: exact names per model
static int32_t phaseIndex = 0;
static int32_t phaseElapsedMs = 0;

// Update gating (elapsed_ms delta_since_previous_step semantics)
static uint32_t lastTickMs = 0;

#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float angleForServo1(int32_t idx);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float angleForServo2(int32_t idx);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float angleForServo3(int32_t idx);
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void smoothUpdateToTargets(float t1, float t2, float t3);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick(uint32_t nowMs);
#line 117 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_21\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float angleForServo1(int32_t idx) {
  // io_0
  if (idx == 0) return 0.0f;
  if (idx == 1) return 45.0f;
  if (idx == 2) return 90.0f;
  if (idx == 3) return 135.0f;
  if (idx == 4) return 180.0f;
  return 0.0f;
}
static float angleForServo2(int32_t idx) {
  // io_1
  if (idx == 0) return 0.0f;
  if (idx == 1) return 45.0f;
  if (idx == 2) return 90.0f;
  if (idx == 3) return 135.0f;
  if (idx == 4) return 180.0f;
  return 0.0f;
}
static float angleForServo3(int32_t idx) {
  // io_2
  if (idx == 0) return 0.0f;
  if (idx == 1) return 90.0f;
  if (idx == 2) return 135.0f;
  if (idx == 3) return 180.0f;
  if (idx == 4) return 0.0f;
  return 45.0f;
}

// Smooth servo movement: issue continuous intermediate steps with fixed update rate,
// but ensure target angles are exactly those produced by the controller.
static float currentAngle1 = 0.0f, currentAngle2 = 0.0f, currentAngle3 = 0.0f;
static const float MAX_STEP_DEG_PER_CONTROL = 1.0f; // smoothness guard

static void smoothUpdateToTargets(float t1, float t2, float t3) {
  // Servo 1
  float d1 = t1 - currentAngle1;
  if (fabs(d1) > MAX_STEP_DEG_PER_CONTROL) d1 = (d1 > 0 ? MAX_STEP_DEG_PER_CONTROL : -MAX_STEP_DEG_PER_CONTROL);
  currentAngle1 += d1;
  servo1.write((int)roundf(currentAngle1));

  // Servo 2
  float d2 = t2 - currentAngle2;
  if (fabs(d2) > MAX_STEP_DEG_PER_CONTROL) d2 = (d2 > 0 ? MAX_STEP_DEG_PER_CONTROL : -MAX_STEP_DEG_PER_CONTROL);
  currentAngle2 += d2;
  servo2.write((int)roundf(currentAngle2));

  // Servo 3
  float d3 = t3 - currentAngle3;
  if (fabs(d3) > MAX_STEP_DEG_PER_CONTROL) d3 = (d3 > 0 ? MAX_STEP_DEG_PER_CONTROL : -MAX_STEP_DEG_PER_CONTROL);
  currentAngle3 += d3;
  servo3.write((int)roundf(currentAngle3));
}

static void controllerTick(uint32_t nowMs) {
  // io_3: delta since previous control step
  uint32_t deltaU = nowMs - lastTickMs;
  lastTickMs = nowMs;

  int32_t io_3 = (int32_t)deltaU;

  // Model: tick action
  // first start;
  // then assign phaseElapsedMs := phaseElapsedMs + io_3;
  phaseElapsedMs = phaseElapsedMs + io_3;

  // then if (phaseElapsedMs >= 2000) { ... }
  if (phaseElapsedMs >= STEP_HOLD_MS) {
    // first start;
    // then assign phaseElapsedMs := phaseElapsedMs - 2000;
    phaseElapsedMs = phaseElapsedMs - STEP_HOLD_MS;

    // then assign phaseIndex := phaseIndex + 1;
    phaseIndex = phaseIndex + 1;

    // then if (phaseIndex >= 6) { phaseIndex := 0; }
    if (phaseIndex >= 6) {
      phaseIndex = 0;
    }
  }

  // outputs:
  float out0 = angleForServo1(phaseIndex); // io_0
  float out1 = angleForServo2(phaseIndex); // io_1
  float out2 = angleForServo3(phaseIndex); // io_2

  // Apply to servos with smooth motion towards targets.
  smoothUpdateToTargets(out0, out1, out2);
}

void setup() {
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization: start all servos at 0 degrees.
  phaseIndex = 0;
  phaseElapsedMs = 0;

  currentAngle1 = 0.0f;
  currentAngle2 = 0.0f;
  currentAngle3 = 0.0f;

  servo1.write((int)roundf(currentAngle1));
  servo2.write((int)roundf(currentAngle2));
  servo3.write((int)roundf(currentAngle3));

  lastTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  // Maintain model sample period ~10ms using a software gate.
  static uint32_t nextMs = 0;
  if (nextMs == 0) nextMs = now;

  // Wait until CONTROL_PERIOD_MS has elapsed; do not block more than necessary.
  if ((int32_t)(now - nextMs) >= 0) {
    // Move next deadline forward by exact period to reduce drift.
    nextMs += CONTROL_PERIOD_MS;
    controllerTick(now);
  }
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t BTN1_PIN = 2;   // io_0 input, active_high=false (pullup), true_level=LOW
static const uint8_t SERVO1_PIN = 3;  // io_1 output deg
static const uint8_t SERVO2_PIN = 4;  // io_2 output deg
static const uint8_t SERVO3_PIN = 5;  // io_3 output deg

// Frozen model parameters
static const uint32_t CONTROL_PERIOD_MS = 5;
static const uint32_t STATE_MIN_MS = 2000;

// --- Hardware: servos ---
Servo servo1;
Servo servo2;
Servo servo3;

// --- Frozen controller state ---
static bool previousPressed = false;     // initial false
static int currentState = 1;            // initial 1
static uint32_t stateEnteredAtMs = 0;   // initial 0 (ms since last state entry per model)
static bool pendingAdvance = false;     // initial false

// --- Edge/advance timing ---
static uint32_t lastTickMs = 0;

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float servoAngle1Deg(int state);
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float servoAngle2Deg(int state);
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float servoAngle3Deg(int state);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readIo0DebouncedModelTrueLevel();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputsForCurrentState();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tickModelAndUpdateOutputs();
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_66\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float servoAngle1Deg(int state) {
  if (state == 1) return 0.0f;
  if (state == 2) return 90.0f;
  if (state == 3) return 180.0f;
  if (state == 4) return 45.0f;
  return 0.0f; // state 5 (and default)
}
static inline float servoAngle2Deg(int state) {
  if (state == 1) return 0.0f;
  if (state == 2) return 45.0f;
  if (state == 3) return 90.0f;
  if (state == 4) return 135.0f;
  return 0.0f; // state 5 (and default)
}
static inline float servoAngle3Deg(int state) {
  if (state == 1) return 0.0f;
  if (state == 2) return 135.0f;
  if (state == 3) return 0.0f;
  if (state == 4) return 90.0f;
  return 0.0f; // state 5 (and default)
}

static inline bool readIo0DebouncedModelTrueLevel() {
  // Model semantics:
  // Hardware: active_high=false, pullup=true => btn reads LOW when pressed.
  // So io_0 "true" corresponds to LOW level.
  return (digitalRead(BTN1_PIN) == LOW);
}

static void applyOutputsForCurrentState() {
  // Outputs io_1..io_3 are target angles in degrees; invalid_policy hold_last_valid,
  // but model never produces invalid states; still safe.
  servo1.write((int)servoAngle1Deg(currentState));
  servo2.write((int)servoAngle2Deg(currentState));
  servo3.write((int)servoAngle3Deg(currentState));
}

static void tickModelAndUpdateOutputs() {
  // perform action tick:
  // pendingAdvance := pendingAdvance OR (io_0 AND NOT previousPressed)
  bool io_0 = readIo0DebouncedModelTrueLevel();
  pendingAdvance = pendingAdvance || (io_0 && (!previousPressed));

  // previousPressed := io_0
  previousPressed = io_0;

  // if (pendingAdvance AND (stateEnteredAtMs >= 2000)) then:
  if (pendingAdvance && (stateEnteredAtMs >= STATE_MIN_MS)) {
    pendingAdvance = false;
    currentState = (currentState == 5) ? 1 : (currentState + 1);
    stateEnteredAtMs = 0;
  }

  // then if (stateEnteredAtMs > 2000) { stateEnteredAtMs := stateEnteredAtMs }
  // In C++ terms, this is a no-op (self assignment) and model also doesn't increment here.
  // We must still preserve "at least 2 seconds" behavior: stateEnteredAtMs in model
  // must be driven by elapsed time. We'll implement that outside the frozen update,
  // but keep the model semantics consistent by treating stateEnteredAtMs as elapsed ms.
  // Therefore, the second conditional is satisfied by no-op and can be ignored.

  applyOutputsForCurrentState();
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);

  // Attach servos to PWM pins with given wiring.
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization requirement: all servos to 0 degrees.
  currentState = 1;
  previousPressed = false;
  pendingAdvance = false;
  stateEnteredAtMs = 0;

  applyOutputsForCurrentState();

  lastTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastTickMs) < CONTROL_PERIOD_MS) {
    return;
  }
  // Maintain fixed period behavior as close as possible.
  lastTickMs += CONTROL_PERIOD_MS;

  // Model timing semantics: stateEnteredAtMs represents elapsed ms since stateEnteredAtMs reset.
  // Frozen model keeps stateEnteredAtMs constant unless the conditional increments it; we interpret
  // "stateEnteredAtMs := stateEnteredAtMs" as no change, so we implement elapsed tracking here
  // to realize the required 2-second minimum per state in physical time.
  if (stateEnteredAtMs == 0) {
    // start counting from 0 at tick right after entry
    stateEnteredAtMs = 0;
  } else {
    stateEnteredAtMs += CONTROL_PERIOD_MS;
  }

  tickModelAndUpdateOutputs();
}

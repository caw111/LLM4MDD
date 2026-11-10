#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_20\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Frozen hardware contract binding:
// io_1 (servo_angle, deg) -> pin D2 (Mega 2560)

// Controller parameters from accepted model:
static const uint8_t SERVO_PIN = 2;     // D2
static const int32_t STEP_HOLD_MS = 2000; // 2 seconds hold per 10° step
static const int32_t STEP_DEG = 10;     // 10° increments
static const int32_t MIN_DEG = 0;
static const int32_t MAX_DEG = 180;

Servo servo1;

// Controller state (exact names/roles per model)
static int32_t accumulatedMs = 0; // initial 0
static int32_t positionDeg   = 0; // initial 0
static bool direction        = true; // initial true: true = increment/up, false = decrement/down

// Timing input port io_0 (i32 ms delta since previous control step)
static uint32_t lastTickMicros = 0;
static int32_t lastIo0 = 0;

#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_20\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_20\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_20\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  // Ensure deterministic initial condition:
  // servo1 should start at 0° upon powering on or resetting.
  servo1.attach(SERVO_PIN);

  // Command initial output angle (io_1 = positionDeg = 0)
  positionDeg = 0;
  direction = true;
  accumulatedMs = 0;
  servo1.write(positionDeg);

  lastTickMicros = micros();
}

void loop() {
  // Control period: 10 ms (from frozen contract)
  // We'll block in a tight but deterministic manner.
  static uint32_t nextDueMicros = 0;
  uint32_t now = micros();
  if (nextDueMicros == 0) nextDueMicros = now;
  if ((int32_t)(now - nextDueMicros) < 0) return;
  nextDueMicros += 10000; // 10ms in microseconds

  // Compute io_0 as delta_since_previous_step (i32 ms)
  uint32_t cur = micros();
  uint32_t elapsedUs = cur - lastTickMicros;
  lastTickMicros = cur;

  // Convert to ms i32 (integer)
  // Note: model expects Integer io_0. We floor by integer conversion.
  int32_t io_0 = (int32_t)(elapsedUs / 1000U);
  lastIo0 = io_0;

  // ---- Controller tick ----
  // perform action tick:
  // accumulatedMs := accumulatedMs + io_0
  accumulatedMs = accumulatedMs + io_0;

  // then if accumulatedMs >= 2000 { ... }
  if (accumulatedMs >= STEP_HOLD_MS) {
    // First: accumulatedMs := accumulatedMs - 2000
    accumulatedMs = accumulatedMs - STEP_HOLD_MS;

    if (direction) {
      // if positionDeg + 10 <= 180 then positionDeg := positionDeg + 10
      if (positionDeg + STEP_DEG <= MAX_DEG) {
        positionDeg = positionDeg + STEP_DEG;
      } else {
        // else positionDeg := 180; direction := false
        positionDeg = MAX_DEG;
        direction = false;
      }
    } else {
      // else (direction == false)
      // if positionDeg - 10 >= 0 then positionDeg := positionDeg - 10
      if (positionDeg - STEP_DEG >= MIN_DEG) {
        positionDeg = positionDeg - STEP_DEG;
      } else {
        // else positionDeg := 0; direction := true
        positionDeg = MIN_DEG;
        direction = true;
      }
    }
  }

  // ---- Output io_1 := positionDeg (deg) with invalid_policy hold_last_valid ----
  // We always drive the servo to the current commanded value.
  servo1.write(positionDeg);
}

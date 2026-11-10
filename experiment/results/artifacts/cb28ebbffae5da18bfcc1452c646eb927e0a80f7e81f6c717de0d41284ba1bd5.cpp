#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t PIN_POT1 = A3;   // io_1
static const uint8_t PIN_POT2 = A4;   // io_3
static const uint8_t PIN_POT3 = A5;   // io_4 (speed control, required by task)
static const uint8_t PIN_LED  = 2;    // io_2 PWM
static const uint8_t PIN_SERVO = 3;   // io_0 servo PWM pin (D3)

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float adcToF64(int v);
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float potToServoAngleF64(float pot);
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float potToLedPwmCountF64(float pot);
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint16_t stepDelayMsFromPot3(uint16_t pot3);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float adcToF64(int v) { return (float)v; }

// Exact mappings from Frozen interface contract:
// io_0 = io_1 * 180.0 / 1023.0
// io_2 = io_3 * 255.0 / 1023.0
static inline float potToServoAngleF64(float pot) { return pot * 180.0f / 1023.0f; }
static inline float potToLedPwmCountF64(float pot) { return pot * 255.0f / 1023.0f; }

static Servo servo1;
static uint8_t ledPwm = 0;

// State for smooth servo motion (must be continuous and smooth).
static float servoCurrentAngleDeg = 0.0f;
static float servoTargetAngleDeg  = 0.0f;

static unsigned long lastStepMs = 0;
static bool initialized = false;

static uint16_t stepDelayMsFromPot3(uint16_t pot3) {
  // The prose requires a delay range (e.g., 10..100ms).
  // No explicit range endpoints are given in the frozen contract; use 10..100ms.
  // Maintain monotonic behavior so higher pot3 => faster movement (smaller delay).
  const uint16_t minDelay = 10;
  const uint16_t maxDelay = 100;

  // pot3: 0..1023 -> delay: max..min
  uint32_t range = (uint32_t)(maxDelay - minDelay); // 90
  uint32_t scaled = (uint32_t)pot3 * range / 1023u;  // 0..range
  uint16_t delayMs = (uint16_t)(maxDelay - scaled);  // max..min
  return delayMs;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);

  servo1.attach(PIN_SERVO);

  // Initialize outputs based on current analog readings (hold_last_valid is irrelevant here
  // because ADC reads are always valid; still, we need deterministic initial state).
  int pot1 = analogRead(PIN_POT1);
  int pot2 = analogRead(PIN_POT2);
  int pot3 = analogRead(PIN_POT3);

  float io_1 = adcToF64(pot1); // pot1 adc_count
  float io_3 = adcToF64(pot2); // pot2 adc_count (named io_3 in contract)

  servoTargetAngleDeg = potToServoAngleF64(io_1);
  servoCurrentAngleDeg = servoTargetAngleDeg;

  servo1.write((int)servoCurrentAngleDeg);

  float io_2 = potToLedPwmCountF64(io_3);
  ledPwm = (uint8_t)constrain((int)lroundf(io_2), 0, 255);
  analogWrite(PIN_LED, ledPwm);

  // Initialize step scheduler
  (void)pot3;
  lastStepMs = millis();
  initialized = true;
}

void loop() {
  // Control period is 5 ms in frozen contract, but servo stepping depends on pot3 delay.
  // We'll enforce a 5 ms tick for input sampling and output computations.
  static unsigned long lastTickMs = 0;
  unsigned long now = millis();
  if (!initialized) {
    lastTickMs = now;
    initialized = true;
  }
  if (now - lastTickMs < 5) return;
  lastTickMs += 5;

  // Read analog inputs
  int pot1_adc = analogRead(PIN_POT1); // io_1
  int pot2_adc = analogRead(PIN_POT2); // io_3
  int pot3_adc = analogRead(PIN_POT3); // io_4

  float io_1 = adcToF64(pot1_adc); // adc_count
  float io_3 = adcToF64(pot2_adc); // adc_count
  float io_4 = adcToF64(pot3_adc); // adc_count (used for speed delay)

  // Compute frozen-contract outputs (exact expressions)
  float io_0_deg = potToServoAngleF64(io_1); // servo target angle
  float io_2_pwm = potToLedPwmCountF64(io_3); // led brightness pwm_count

  servoTargetAngleDeg = io_0_deg;

  uint8_t newLedPwm = (uint8_t)constrain((int)lroundf(io_2_pwm), 0, 255);
  if (newLedPwm != ledPwm) {
    ledPwm = newLedPwm;
    analogWrite(PIN_LED, ledPwm);
  }

  // Smooth servo motion: move by 1 degree steps towards target,
  // with delay between each step derived from pot3.
  uint16_t stepDelayMs = stepDelayMsFromPot3((uint16_t)pot3_adc);

  // If we are already at target, nothing to do.
  float diff = servoTargetAngleDeg - servoCurrentAngleDeg;
  if (fabsf(diff) >= 0.5f) {
    if ((unsigned long)(now - lastStepMs) >= stepDelayMs) {
      lastStepMs = now;

      // Step by 1 degree per step (smooth motion).
      // Choose direction based on diff.
      if (diff > 0) servoCurrentAngleDeg += 1.0f;
      else servoCurrentAngleDeg -= 1.0f;

      // Clamp to avoid overshoot and keep within 0..180.
      if (servoCurrentAngleDeg > 180.0f) servoCurrentAngleDeg = 180.0f;
      if (servoCurrentAngleDeg < 0.0f) servoCurrentAngleDeg = 0.0f;

      // If we stepped past target, snap to target.
      if ((diff > 0 && servoCurrentAngleDeg > servoTargetAngleDeg) ||
          (diff < 0 && servoCurrentAngleDeg < servoTargetAngleDeg)) {
        servoCurrentAngleDeg = servoTargetAngleDeg;
      }

      servo1.write((int)lroundf(servoCurrentAngleDeg));
    }
  } else {
    // Close enough; snap to target to maintain exact state convergence.
    if (servoCurrentAngleDeg != servoTargetAngleDeg) {
      servoCurrentAngleDeg = servoTargetAngleDeg;
      servo1.write((int)lroundf(servoCurrentAngleDeg));
    }
    lastStepMs = now; // reset scheduler while holding
  }

  // Keep (unused) io_4 referenced so behavior stays aligned with contract/state maintenance.
  (void)io_4;
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PWM_PIN = 5;   // D5
static const uint16_t PERIOD_MS = 2000;
static const uint16_t UP_MS = 1000;
static const uint8_t MAX_PWM = 255;

// Controller state
static int32_t tMs = 0;            // i32, initial 0

// Timing for io_1 = delta_since_previous_step (ms)
static uint32_t lastStepMs = 0;
static bool initialized = false;

#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t clampToPwmRange(int32_t v);
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline uint8_t clampToPwmRange(int32_t v) {
  if (v < 0) return 0;
  if (v > 255) return 255;
  return (uint8_t)v;
}

void setup() {
  pinMode(PWM_PIN, OUTPUT);

  // Ensure initial brightness = 0
  analogWrite(PWM_PIN, 0);

  lastStepMs = millis();
  initialized = true;
}

void loop() {
  if (!initialized) {
    lastStepMs = millis();
    initialized = true;
  }

  uint32_t nowMs = millis();
  uint32_t deltaU32 = nowMs - lastStepMs; // wraps naturally on AVR
  lastStepMs = nowMs;

  // io_1 : i32, ms delta since previous step
  int32_t io_1 = (int32_t)deltaU32;

  // === Controller tick ===
  // tMs := (if (tMs + io_1 >= 2000) then (tMs + io_1) - 2000 else tMs + io_1)
  int32_t sum = tMs + io_1;
  if (sum >= (int32_t)PERIOD_MS) {
    tMs = sum - (int32_t)PERIOD_MS;
  } else {
    tMs = sum;
  }

  // io_0 := (if tMs < 1000 then (tMs/1000.0)*255 else ((2000 - tMs)/1000.0)*255)
  double pwmReal;
  if (tMs < (int32_t)UP_MS) {
    pwmReal = ((double)tMs / 1000.0) * 255.0;
  } else {
    pwmReal = ((double)((int32_t)PERIOD_MS - tMs) / 1000.0) * 255.0;
  }

  // Map to PWM count with rounding to preserve expected tolerance.
  int32_t pwmInt = (int32_t)lround(pwmReal);

  // Apply output
  analogWrite(PWM_PIN, clampToPwmRange(pwmInt));
}

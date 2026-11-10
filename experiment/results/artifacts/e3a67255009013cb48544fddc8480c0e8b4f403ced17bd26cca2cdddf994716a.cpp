#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_4\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings:
// io_0 -> D2 (PWM) : RGB red intensity
// io_1 -> D3 (PWM) : RGB green intensity
// io_2 -> D4 (PWM) : RGB blue intensity
static constexpr uint8_t PIN_RED   = 2;  // D2
static constexpr uint8_t PIN_GREEN = 3;  // D3
static constexpr uint8_t PIN_BLUE  = 4;  // D4

static constexpr uint16_t CONTROL_PERIOD_MS = 10; // contract: control_period_ms

// ACCEPTED executable model bundle:
// Controller states empty; outputs fixed at 0.0 for all observed steps.
// Therefore the LED must remain off indefinitely.
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_4\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(float io_0, float io_1, float io_2);
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_4\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_4\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_4\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void applyOutputs(float io_0, float io_1, float io_2) {
  // Contract values are in pwm_count domain with initial 0.0; preserve exact behavior:
  // keep channels disabled/off at startup and forever.
  (void)io_0; (void)io_1; (void)io_2;

  analogWrite(PIN_RED, 0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE, 0);
}

void setup() {
  // Ensure common-cathode LED is off initially (PWM initial 0).
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  // Initialize PWM outputs to 0 to match controller outputs.
  applyOutputs(0.0f, 0.0f, 0.0f);
}

void loop() {
  // Model has no time-driven updates (states/updates empty).
  // Keep outputs exactly at 0.0 while respecting control period.
  applyOutputs(0.0f, 0.0f, 0.0f);
  delay(CONTROL_PERIOD_MS);
}

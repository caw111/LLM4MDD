#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_RED   = 2;  // io_0
static const uint8_t PIN_GREEN = 3;  // io_1
static const uint8_t PIN_BLUE  = 4;  // io_2
static const uint8_t PIN_POT   = A3; // io_3

// Frozen controller logic thresholds (ADC counts)
static const float TH1 = 341.0f;
static const float TH2 = 682.0f;

// Map controller outputs (0.0/1.0 pwm_count) to Arduino PWM duty.
// Contract says unit is "pwm_count" with scale 1; implement as analogWrite values.
// Using full 8-bit PWM when output==1, else 0.
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t pwmFromController(float v);
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline uint8_t pwmFromController(float v) {
  return (v >= 0.5f) ? 255 : 0;
}

void setup() {
  // Enforce RGB LED off at power-up/reset:
  // Initialize PWM channels to 0 for D2/D3/D4.
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  analogWrite(PIN_RED, 0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE, 0);
}

void loop() {
  // sample_period_ms = 5 in the frozen model contract
  static uint32_t lastTickMs = 0;
  uint32_t nowMs = millis();
  if (nowMs - lastTickMs < 5) return;
  lastTickMs = nowMs;

  // io_3: analog_input A3 -> adc_count
  // Arduino Mega ADC is 10-bit: 0..1023.
  int raw = analogRead(PIN_POT);
  float io_3 = (float)raw;

  // Controller outputs (exact decision logic as given)
  float io_0 = (((io_3) < TH1) ? 1.0f : 0.0f); // red
  float io_1 = (((io_3) >= TH1 && (io_3) < TH2) ? 1.0f : 0.0f); // green
  float io_2 = (((io_3) >= TH2) ? 1.0f : 0.0f); // blue

  // Apply PWM outputs to mapped pins
  analogWrite(PIN_RED, pwmFromController(io_0));
  analogWrite(PIN_GREEN, pwmFromController(io_1));
  analogWrite(PIN_BLUE, pwmFromController(io_2));
}

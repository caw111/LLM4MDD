#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static constexpr uint8_t PIN_LDR_A0 = A0;
static constexpr uint8_t PIN_PWM_D9 = 9;
static constexpr uint8_t PIN_STATUS_D13 = 13;

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float floorf_like(float x);
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float floorf_like(float x) { return floorf(x); }

void setup() {
  // Match frozen pin bindings
  pinMode(PIN_LDR_A0, INPUT);
  pinMode(PIN_PWM_D9, OUTPUT);
  pinMode(PIN_STATUS_D13, OUTPUT);

  // Initialize outputs to initial values from contract
  analogWrite(PIN_PWM_D9, 0);     // io_1 initial = 0 (pwm_count)
  digitalWrite(PIN_STATUS_D13, LOW); // io_2 initial = false => LOW
}

void loop() {
  static uint32_t nextTick = 0;
  const uint32_t now = millis();
  if (now < nextTick) return;
  nextTick = now + CONTROL_PERIOD_MS;

  // io_0: input f64, unit adc_count, initial 0, sample_period 10
  // Hardware binding: analog_input offset=0 scale=1 pin=A0
  const int raw = analogRead(PIN_LDR_A0); // 0..1023
  const float io_0 = static_cast<float>(raw);

  // Controller outputs per accepted executable model
  // io_1:
  // if io_0 <= 0.0 then 1023.0
  // else floor(1023.0 * (1.0 - (io_0 / 1023.0)))
  float io_1_count;
  if (io_0 <= 0.0f) {
    io_1_count = 1023.0f;
  } else {
    float ratio = io_0 / 1023.0f;
    float inv = 1.0f - ratio;
    io_1_count = floorf_like(1023.0f * inv);
  }

  // Ensure duty count within 0..1023 and map to Arduino 8-bit PWM output.
  // Arduino UNO analogWrite uses 0..255 duty on Timer0/1/2.
  // Preserve requested "pwm_count" semantics by scaling 0..1023 -> 0..255.
  // No additional behavior from model is introduced.
  if (io_1_count < 0.0f) io_1_count = 0.0f;
  if (io_1_count > 1023.0f) io_1_count = 1023.0f;

  const uint8_t pwm255 = static_cast<uint8_t>(floorf_like((io_1_count * 255.0f) / 1023.0f));
  analogWrite(PIN_PWM_D9, pwm255);

  // io_2: Boolean = true => HIGH because active_high=true
  digitalWrite(PIN_STATUS_D13, HIGH);

  // No serial, no further outputs.
}

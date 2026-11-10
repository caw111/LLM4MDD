#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// Interface io_0: input bool from D3, active-high, no pullup (per contract)
static constexpr uint8_t PIN_D3 = 3;
// Interface io_1: output i32 Hz to D6 via tone_output: continuous tone while button pressed
static constexpr uint8_t PIN_D6 = 6;

// Model output Hz: if io_0 then 440 else 0
static constexpr int32_t TONE_HZ_WHEN_PRESSED = 440;
static constexpr int32_t TONE_HZ_WHEN_NOT_PRESSED = 0;

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool read_io_0();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void write_io_1(int32_t hz);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline bool read_io_0() {
  // active_high, pullup=false: raw HIGH means true/pressed
  return digitalRead(PIN_D3) == HIGH;
}

static inline void write_io_1(int32_t hz) {
  // "silence" invalid_policy: set tone to 0 (stop)
  if (hz <= 0) {
    noTone(PIN_D6);
  } else {
    // Use Arduino tone (nonzero frequency while pressed)
    tone(PIN_D6, (unsigned int)hz);
  }
}

void setup() {
  pinMode(PIN_D3, INPUT);     // pullup=false (no internal pull-up)
  pinMode(PIN_D6, OUTPUT);    // tone library will drive pin

  // Initialize outputs to model initial value: io_0 initial false => io_1 = 0
  noTone(PIN_D6);
}

void loop() {
  static uint32_t nextTick = millis();
  uint32_t now = millis();
  if ((int32_t)(now - nextTick) < 0) return;

  // Maintain 5ms control period (5ms planning/control_period_ms)
  nextTick = nextTick + CONTROL_PERIOD_MS;

  // === Accepted executable model evaluation ===
  bool io_0 = read_io_0();
  int32_t io_1 = (io_0 ? TONE_HZ_WHEN_PRESSED : TONE_HZ_WHEN_NOT_PRESSED);

  // === Output actuation ===
  write_io_1(io_1);
}

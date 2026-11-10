#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_1\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen hardware contract bindings
// io_0 -> led1 on D2 (active-high)
static constexpr uint8_t PIN_LED1 = 2;
// io_1 -> led2 on D3 (active-high)
static constexpr uint8_t PIN_LED2 = 3;

// Frozen interface contract timing
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Controller model state
static int32_t accumulatedMs = 0;

#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_1\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(bool io_0, bool io_1);
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_1\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_1\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_1\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(bool io_0, bool io_1) {
  digitalWrite(PIN_LED1, io_0 ? HIGH : LOW);
  digitalWrite(PIN_LED2, io_1 ? HIGH : LOW);
}

void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  // Initialization: both LEDs off
  accumulatedMs = 0;
  applyOutputs(false, false);
}

void loop() {
  static uint32_t lastMs = 0;
  uint32_t now = millis();

  // Enforce the 10ms control period deterministically
  if (now - lastMs < CONTROL_PERIOD_MS) return;
  lastMs = now;

  // io_2: delta since previous control step in ms (i32)
  int32_t io_2 = (int32_t)(now - lastMs + CONTROL_PERIOD_MS);
  if (io_2 < 0) io_2 = 0;

  // Tick update sequence (as in frozen model)
  // updates:
  // 1) accumulatedMs := accumulatedMs + io_2
  accumulatedMs = accumulatedMs + io_2;

  // 2) if accumulatedMs >= 20000 then accumulatedMs := accumulatedMs - 20000
  //    (single subtraction per tick, matching model exactly)
  if (accumulatedMs >= 20000) {
    accumulatedMs = accumulatedMs - 20000;
  }

  // outputs (as Boolean expressions over accumulatedMs)
  bool io_0 = (accumulatedMs >= 5000 && accumulatedMs < 10000) ? true
              : (accumulatedMs >= 10000 && accumulatedMs < 15000) ? false
              : false;

  bool io_1 = (accumulatedMs >= 10000 && accumulatedMs < 15000) ? true : false;

  applyOutputs(io_0, io_1);
}

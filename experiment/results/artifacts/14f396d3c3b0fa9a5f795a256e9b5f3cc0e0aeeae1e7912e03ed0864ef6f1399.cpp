#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_9\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// ---- Frozen interface contract mapping ----
// Outputs (active_high=true, digital outputs)
// io_0_segment_0 -> D2
// io_0_segment_1 -> D3
// io_0_segment_2 -> D4
// io_0_segment_3 -> D5
// io_0_segment_4 -> D6
// io_0_segment_5 -> D7
// io_0_segment_6 -> D8
// io_0_segment_7 -> D9
// io_0_segment_8 -> D10
// io_0_segment_9 -> D11
//
// Inputs (active_high=false, pullup=true)
// io_1 -> D12 (pressed = LOW -> model true_level is LOW? per contract model value true when button pressed/LOW)

static constexpr uint8_t PIN_SEG_0 = 2;   // io_0_segment_0
static constexpr uint8_t PIN_SEG_1 = 3;   // io_0_segment_1
static constexpr uint8_t PIN_SEG_2 = 4;   // io_0_segment_2
static constexpr uint8_t PIN_SEG_3 = 5;   // io_0_segment_3
static constexpr uint8_t PIN_SEG_4 = 6;   // io_0_segment_4
static constexpr uint8_t PIN_SEG_5 = 7;   // io_0_segment_5
static constexpr uint8_t PIN_SEG_6 = 8;   // io_0_segment_6
static constexpr uint8_t PIN_SEG_7 = 9;   // io_0_segment_7
static constexpr uint8_t PIN_SEG_8 = 10;  // io_0_segment_8
static constexpr uint8_t PIN_SEG_9 = 11;  // io_0_segment_9

static constexpr uint8_t PIN_BTN   = 12;  // io_1 (active_low with pullup)

// ---- Accepted executable model states ----
static int32_t stepIndex = 0;       // i32 initial 0
static bool previousPressed = false;

// ---- Port helpers ----
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_9\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeSeg(uint8_t pin, bool value);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_9\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateOutputs();
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_9\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 110 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_9\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_9\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void writeSeg(uint8_t pin, bool value) {
  digitalWrite(pin, value ? HIGH : LOW); // active_high=true
}

// ---- Controller tick: implements the ACCEPTED model exactly ----
static void tick(bool io_1 /* model value */) {
  // if io_1 and not previousPressed then stepIndex := (stepIndex >= 5 ? 0 : stepIndex + 1)
  if (io_1 && (!previousPressed)) {
    if (stepIndex >= 5) {
      stepIndex = 0;
    } else {
      stepIndex = stepIndex + 1;
    }
  }
  previousPressed = io_1;
}

// ---- Output update: implements the ACCEPTED model exactly ----
static void updateOutputs() {
  // out io_0_segment_0 = (stepIndex >= 0)
  writeSeg(PIN_SEG_0, (stepIndex >= 0));
  writeSeg(PIN_SEG_1, (stepIndex >= 0));

  // out io_0_segment_2 = (stepIndex >= 1)
  writeSeg(PIN_SEG_2, (stepIndex >= 1));
  writeSeg(PIN_SEG_3, (stepIndex >= 1));

  // out io_0_segment_4 = (stepIndex >= 2)
  writeSeg(PIN_SEG_4, (stepIndex >= 2));
  writeSeg(PIN_SEG_5, (stepIndex >= 2));

  // out io_0_segment_6 = (stepIndex >= 3)
  writeSeg(PIN_SEG_6, (stepIndex >= 3));
  writeSeg(PIN_SEG_7, (stepIndex >= 3));

  // out io_0_segment_8 = (stepIndex >= 4)
  writeSeg(PIN_SEG_8, (stepIndex >= 4));
  writeSeg(PIN_SEG_9, (stepIndex >= 4));
}

void setup() {
  // Inputs
  pinMode(PIN_BTN, INPUT_PULLUP);

  // Outputs
  pinMode(PIN_SEG_0, OUTPUT);
  pinMode(PIN_SEG_1, OUTPUT);
  pinMode(PIN_SEG_2, OUTPUT);
  pinMode(PIN_SEG_3, OUTPUT);
  pinMode(PIN_SEG_4, OUTPUT);
  pinMode(PIN_SEG_5, OUTPUT);
  pinMode(PIN_SEG_6, OUTPUT);
  pinMode(PIN_SEG_7, OUTPUT);
  pinMode(PIN_SEG_8, OUTPUT);
  pinMode(PIN_SEG_9, OUTPUT);

  // Initialization: all LEDs off
  writeSeg(PIN_SEG_0, false);
  writeSeg(PIN_SEG_1, false);
  writeSeg(PIN_SEG_2, false);
  writeSeg(PIN_SEG_3, false);
  writeSeg(PIN_SEG_4, false);
  writeSeg(PIN_SEG_5, false);
  writeSeg(PIN_SEG_6, false);
  writeSeg(PIN_SEG_7, false);
  writeSeg(PIN_SEG_8, false);
  writeSeg(PIN_SEG_9, false);

  // Model initial state already set by globals:
  // stepIndex = 0
  // previousPressed = false
}

void loop() {
  // Control period configured in contract as 5ms
  static uint32_t lastMs = 0;
  uint32_t now = millis();
  if (now - lastMs < 5) return;
  lastMs = now;

  // Read io_1 with active_high=false and pullup=true:
  // Hardware is LOW when pressed; contract model semantics:
  // true_level = LOW (i.e., model true when pin reads LOW)
  bool btnLevel = (digitalRead(PIN_BTN) == LOW); // model io_1

  // Tick controller
  tick(btnLevel);

  // Update outputs (ordered updates implied by model: outputs derived from stepIndex)
  updateOutputs();
}

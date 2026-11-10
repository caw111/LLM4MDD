#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

//
// Frozen hardware contract bindings
//
static constexpr uint8_t PIN_BUTTON = 2;   // io_0 input (active_high=true)
static constexpr uint8_t PIN_LED_RED = 8;   // io_1 output (active_high=true)
static constexpr uint8_t PIN_LED_YEL = 9;   // io_2 output (active_high=true)
static constexpr uint8_t PIN_LED_GRN = 10;  // io_3 output (active_high=true)

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

//
// ACCEPTED executable model (generated behavior)
// Controller states/variables:
// - previousPressed (bool, initial false)
// - mode (i32, initial 0): 0=GREEN, 1=YELLOW countdown, 2=RED countdown
// - yellowRemainingMs (i32, initial 0) decremented by 10 each tick in mode==1
// - redRemainingMs (i32, initial 0) decremented by 10 each tick in mode==2
//
// Outputs:
// io_1 (RED)   = mode == 2
// io_2 (YELL)  = mode == 1
// io_3 (GREEN) = mode == 0
//
struct Controller {
  bool previousPressed = false;
  int32_t mode = 0;
  int32_t yellowRemainingMs = 0;
  int32_t redRemainingMs = 0;

  // One model tick: preserves ordered updates exactly as in the bundle.
  void tick(bool io_0) {
    // Update 1:
    // if (io_0 and not previousPressed) { mode=1; yellowRemainingMs=1000; redRemainingMs=2000; }
    if (io_0 && (!previousPressed)) {
      mode = 1;
      yellowRemainingMs = 1000;
      redRemainingMs = 2000;
    }

    // Update 2:
    // if (mode == 1) { yellowRemainingMs := yellowRemainingMs - 10; if (yellowRemainingMs - 10 <= 0) mode := 2; }
    if (mode == 1) {
      yellowRemainingMs = yellowRemainingMs - 10;
      if (yellowRemainingMs - 10 <= 0) {
        mode = 2;
      }
    }

    // Update 3:
    // if (mode == 2) { redRemainingMs := redRemainingMs - 10; if (redRemainingMs - 10 <= 0) mode := 0; }
    if (mode == 2) {
      redRemainingMs = redRemainingMs - 10;
      if (redRemainingMs - 10 <= 0) {
        mode = 0;
      }
    }

    // Update 4:
    // previousPressed := io_0
    previousPressed = io_0;
  }

  // Output expressions
  bool io_1_red() const { return mode == 2; }
  bool io_2_yellow() const { return mode == 1; }
  bool io_3_green() const { return mode == 0; }
};

static Controller controller;

#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setOutputsFromModel();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void setOutputsFromModel() {
  // Active-high digital outputs
  digitalWrite(PIN_LED_RED, controller.io_1_red() ? HIGH : LOW);
  digitalWrite(PIN_LED_YEL, controller.io_2_yellow() ? HIGH : LOW);
  digitalWrite(PIN_LED_GRN, controller.io_3_green() ? HIGH : LOW);
}

void setup() {
  // Interface initialization
  pinMode(PIN_BUTTON, INPUT); // contract: INPUT, read HIGH when pressed
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YEL, OUTPUT);
  pinMode(PIN_LED_GRN, OUTPUT);

  // Model initial state already in controller constructor:
  // previousPressed=false, mode=0, yellowRemainingMs=0, redRemainingMs=0
  // Apply initial outputs immediately (mode==0 => green ON)
  setOutputsFromModel();
}

void loop() {
  static uint32_t nextTickMs = 0;
  const uint32_t now = millis();

  // Schedule to control period granularity (10 ms).
  // If we're late, catch up by advancing nextTickMs to now + period.
  if (nextTickMs == 0) {
    nextTickMs = now;
  }
  if ((int32_t)(now - nextTickMs) < 0) {
    return; // not time yet
  }

  // Execute exactly one model tick per period boundary.
  // (If multiple periods passed, we still only tick once, matching typical cooperative scheduling granularity.)
  nextTickMs = now + CONTROL_PERIOD_MS;

  // Read io_0: active_high=true, pin D2
  const bool io_0_pressed = (digitalRead(PIN_BUTTON) == HIGH);

  controller.tick(io_0_pressed);

  // Apply outputs after tick
  setOutputsFromModel();
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t PIN_LED1 = 2;   // io_0
static constexpr uint8_t PIN_LED2 = 3;   // io_1

static constexpr uint8_t PIN_BAR_A1  = 4;  // io_2_segment_0
static constexpr uint8_t PIN_BAR_A2  = 5;  // io_2_segment_1
static constexpr uint8_t PIN_BAR_A3  = 6;  // io_2_segment_2
static constexpr uint8_t PIN_BAR_A4  = 7;  // io_2_segment_3
static constexpr uint8_t PIN_BAR_A5  = 8;  // io_2_segment_4
static constexpr uint8_t PIN_BAR_A6  = 9;  // io_2_segment_5
static constexpr uint8_t PIN_BAR_A7  = 10; // io_2_segment_6
static constexpr uint8_t PIN_BAR_A8  = 11; // io_2_segment_7
static constexpr uint8_t PIN_BAR_A9  = 12; // io_2_segment_8
static constexpr uint8_t PIN_BAR_A10 = 13; // io_2_segment_9

// Frozen controller model state (exact initial values)
static int32_t segmentIndex = 0;  // initial 0
static int32_t accumulated  = 0;  // initial 0
static bool ledToggle       = false; // initial false

// Control period fixed at 10 ms (as given)
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// elapsed_ms delta since previous control step (io_3)
static uint32_t lastStepMs = 0;

#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBarSegment(uint8_t pin, bool value);
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick(int32_t io_3);
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_95\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBarSegment(uint8_t pin, bool value) {
  digitalWrite(pin, value ? HIGH : LOW); // active_high
}

static void applyOutputs() {
  // io_0 = ledToggle
  digitalWrite(PIN_LED1, ledToggle ? HIGH : LOW);

  // io_1 = (if ledToggle ? false else true)
  digitalWrite(PIN_LED2, (!ledToggle) ? HIGH : LOW);

  // io_2_segment_i = segmentIndex >= i
  setBarSegment(PIN_BAR_A1,  segmentIndex >= 0);
  setBarSegment(PIN_BAR_A2,  segmentIndex >= 1);
  setBarSegment(PIN_BAR_A3,  segmentIndex >= 2);
  setBarSegment(PIN_BAR_A4,  segmentIndex >= 3);
  setBarSegment(PIN_BAR_A5,  segmentIndex >= 4);
  setBarSegment(PIN_BAR_A6,  segmentIndex >= 5);
  setBarSegment(PIN_BAR_A7,  segmentIndex >= 6);
  setBarSegment(PIN_BAR_A8,  segmentIndex >= 7);
  setBarSegment(PIN_BAR_A9,  segmentIndex >= 8);
  setBarSegment(PIN_BAR_A10, segmentIndex >= 9);
}

static void controllerTick(int32_t io_3) {
  // perform action tick:
  // accumulated := accumulated + io_3
  accumulated = accumulated + io_3;

  if (accumulated >= 2000) {
    // accumulated := accumulated - 2000
    accumulated = accumulated - 2000;

    // ledToggle := not ledToggle
    ledToggle = !ledToggle;

    if (segmentIndex < 9) {
      segmentIndex = segmentIndex + 1;
    } else {
      segmentIndex = 0;
    }
  }
}

void setup() {
  // Initialize pins
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  pinMode(PIN_BAR_A1, OUTPUT);
  pinMode(PIN_BAR_A2, OUTPUT);
  pinMode(PIN_BAR_A3, OUTPUT);
  pinMode(PIN_BAR_A4, OUTPUT);
  pinMode(PIN_BAR_A5, OUTPUT);
  pinMode(PIN_BAR_A6, OUTPUT);
  pinMode(PIN_BAR_A7, OUTPUT);
  pinMode(PIN_BAR_A8, OUTPUT);
  pinMode(PIN_BAR_A9, OUTPUT);
  pinMode(PIN_BAR_A10, OUTPUT);

  // Initialization per requirement: all LEDs and bargraph off.
  // Model initial state (ledToggle=false, segmentIndex=0) would turn A1 on.
  // To satisfy "all off on reset", force outputs off while keeping controller state initial.
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  digitalWrite(PIN_BAR_A1, LOW);
  digitalWrite(PIN_BAR_A2, LOW);
  digitalWrite(PIN_BAR_A3, LOW);
  digitalWrite(PIN_BAR_A4, LOW);
  digitalWrite(PIN_BAR_A5, LOW);
  digitalWrite(PIN_BAR_A6, LOW);
  digitalWrite(PIN_BAR_A7, LOW);
  digitalWrite(PIN_BAR_A8, LOW);
  digitalWrite(PIN_BAR_A9, LOW);
  digitalWrite(PIN_BAR_A10, LOW);

  lastStepMs = millis();
}

void loop() {
  uint32_t now = millis();
  if (now - lastStepMs >= CONTROL_PERIOD_MS) {
    // Compute io_3 as actual delta since previous control step
    uint32_t delta = now - lastStepMs;
    lastStepMs = now;

    // Apply controller logic (frozen ACCEPTED model)
    controllerTick((int32_t)delta);

    // Apply outputs from model
    applyOutputs();
  }
}

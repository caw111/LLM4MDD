#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_5\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

namespace GeneratedModel {

struct Controller {
  // Inputs
  bool io_0 = false; // Button k1 (active on press; model true corresponds to pressed)

  // State
  bool ledState = true;         // initial true => LED ON at power-up
  bool previousPressed = false; // initial false

  // Outputs
  bool io_1 = true; // ledState

  static constexpr uint8_t PIN_LED = 3; // Mega D3
  static constexpr uint8_t PIN_K1  = 2; // Mega D2 (input with pull-up)

  void begin() {
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_K1, INPUT_PULLUP);

    // Enforce initial LED state (ON)
    ledState = true;
    previousPressed = false;
    io_1 = ledState;
    digitalWrite(PIN_LED, io_1 ? HIGH : LOW);
  }

  void tick() {
    // Equivalent to:
    // if (io_0 && !previousPressed) ledState = !ledState;
    if (io_0 && !previousPressed) {
      ledState = ledState ? false : true;
    }
    previousPressed = io_0;
    io_1 = ledState;

    digitalWrite(PIN_LED, io_1 ? HIGH : LOW);
  }
};

} // namespace GeneratedModel

// 5ms control period
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

GeneratedModel::Controller controller;
static uint32_t nextTickMs = 0;

#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_5\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_5\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_5\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  controller.begin();
  nextTickMs = millis() + CONTROL_PERIOD_MS;
}

void loop() {
  // Run on fixed period to match sample period semantics (5ms)
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) >= 0) {
    // Sample input: wiring uses pull-up, active_low pressed => model true when LOW
    // i.e., io_0 = (digitalRead(D2) == LOW)
    controller.io_0 = (digitalRead(2) == LOW);

    controller.tick();

    nextTickMs += CONTROL_PERIOD_MS;
  }
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_RGB_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract (ACCEPTED model):
// io_0 -> D4 : initial false
// io_1 -> D3 : initial false
// io_2 -> D2 : initial true
//
// Model semantics: true_level=HIGH, false_level=LOW (active_high=true)

static constexpr uint8_t IO_0_PIN = 4; // io_0
static constexpr uint8_t IO_1_PIN = 3; // io_1
static constexpr uint8_t IO_2_PIN = 2; // io_2

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_RGB_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_RGB_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_RGB_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  pinMode(IO_0_PIN, OUTPUT);
  pinMode(IO_1_PIN, OUTPUT);
  pinMode(IO_2_PIN, OUTPUT);

  // Apply frozen outputs immediately during setup and keep them constant.
  digitalWrite(IO_0_PIN, LOW);   // io_0 = false
  digitalWrite(IO_1_PIN, LOW);   // io_1 = false
  digitalWrite(IO_2_PIN, HIGH);  // io_2 = true
}

void loop() {
  // No actions required.
  // Keep outputs constant.
}

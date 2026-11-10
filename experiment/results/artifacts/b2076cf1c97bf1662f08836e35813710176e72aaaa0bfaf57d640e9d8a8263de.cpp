#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_IO_0 = 2;   // button input (active LOW when pressed)
static const uint8_t PIN_IO_1 = 13;  // LED output (active HIGH)

static bool io_0_model = false; // button pressed state (true when pressed)
static bool io_1_model = false; // LED on state (true => LED ON)

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  // Interface contract: configure button pin as INPUT with pull-up
  pinMode(PIN_IO_0, INPUT_PULLUP);

  // Interface contract: configure LED pin as OUTPUT, active-high
  pinMode(PIN_IO_1, OUTPUT);
  digitalWrite(PIN_IO_1, LOW); // initial model is false

  io_0_model = false;
  io_1_model = false;
}

void loop() {
  // Control period is 5 ms; implement a fixed-rate loop.
  static unsigned long nextTickMs = 0;
  unsigned long nowMs = millis();
  if (nextTickMs == 0) nextTickMs = nowMs;
  if ((long)(nowMs - nextTickMs) < 0) return;

  // Keep drift bounded
  nextTickMs += 5;

  // Read hardware for io_0 (bool model true when button pressed => reads LOW when pressed)
  int raw = digitalRead(PIN_IO_0); // HIGH when released, LOW when pressed
  bool io_0_hw = (raw == LOW);      // model semantics: true_level = LOW
  io_0_model = io_0_hw;

  // ACCEPTED model output expression: io_1 := io_0
  io_1_model = io_0_model;

  // Drive hardware for io_1 (active_high true => HIGH turns LED on)
  digitalWrite(PIN_IO_1, io_1_model ? HIGH : LOW);
}

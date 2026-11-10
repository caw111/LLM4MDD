#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_A1 = A1; // analog_input
static const uint8_t PIN_D8 = 8;  // digital_output

// Accepted executable model controller state
static double io_0_prev = 0.0; // initial 0.0

// Control period (ms) = 5
static const unsigned long CONTROL_PERIOD_MS = 5;

static unsigned long lastTickMs = 0;

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  // Hardware bindings
  pinMode(PIN_D8, OUTPUT);

  // Initial states per contract
  io_0_prev = 0.0;
  digitalWrite(PIN_D8, LOW); // io_1 initial false
}

void loop() {
  unsigned long now = millis();
  if (now - lastTickMs >= CONTROL_PERIOD_MS) {
    lastTickMs += CONTROL_PERIOD_MS;

    // Sample input io_0 from A1
    // Model contract uses "io_0 < 512.0" where io_0 is treated as adc_count (0..1023)
    double io_0 = (double)analogRead(PIN_A1);

    // Controller action tick:
    // - first start
    // - then assign io_0_prev := io_0
    io_0_prev = io_0;

    // Controller output io_1 = (if io_0 < 512.0 then true else false)
    bool io_1 = (io_0 < 512.0);

    // digital_boolean semantics: true_level HIGH, false_level LOW; active_high true
    digitalWrite(PIN_D8, io_1 ? HIGH : LOW);
  }
}

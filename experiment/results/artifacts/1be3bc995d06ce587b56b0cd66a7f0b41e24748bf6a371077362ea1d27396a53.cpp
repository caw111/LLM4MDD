#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t LED_PIN = 13;   // D13
static constexpr uint8_t ADC_PIN = A0;   // A0
static constexpr unsigned long CONTROL_PERIOD_MS = 10;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // initial=false (LED off)
}

void loop() {
  static unsigned long next_tick = 0;
  const unsigned long now = millis();
  if (now < next_tick) return;

  // Schedule next tick with fixed period to preserve timing.
  next_tick = now + CONTROL_PERIOD_MS;

  // io_0: analog input in adc_count, offset 0.0, scale 1.0
  // Arduino UNO: analogRead returns 0..1023, use float to match model Real.
  float io_0 = (float)analogRead(ADC_PIN);

  // io_1: boolean output io_0 > 500.0
  bool io_1 = (io_0 > 500.0f);

  // digital_output active_high on D13
  digitalWrite(LED_PIN, io_1 ? HIGH : LOW);
}

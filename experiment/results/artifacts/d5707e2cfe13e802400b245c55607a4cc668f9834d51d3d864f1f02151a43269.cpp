#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t LM35_PIN = A2;

// Frozen interface contract
static constexpr unsigned long CONTROL_PERIOD_MS = 10;
static constexpr unsigned long SERIAL_BAUD = 9600;
static constexpr float SERIAL_VALUE_SCALE = 0.1f; // serial_0_value_0 = adcCount * 0.1

// Model state
static double adcCount = 0.0; // f64

// Serial fixed prefix and report gate
static const char* SERIAL_PREFIX = "TEMP_C=";

static unsigned long nextTickMs = 0;

#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double readIo0_adcCount();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void emitSerialFixedTemp(double tempC);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double readIo0_adcCount() {
  // io_0 type/unit: adc_count; initial 0
  // Map analog input A2 directly to "adc counts" as floating value.
  int raw = analogRead(LM35_PIN); // 0..1023
  return (double)raw;
}

static void emitSerialFixedTemp(double tempC) {
  // Requirement: fixed-prefix text line request with text="TEMP_C=";
  // Output line composition: "TEMP_C=" + f64 temperature value
  Serial.print(SERIAL_PREFIX);
  // Format controlled by SysML model (f64). Use default float formatting.
  // This preserves behavior expected for exact values in typical Arduino environments.
  Serial.print(tempC, 6);
  Serial.print('\n');
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  // Ensure analog subsystem initialized implicitly by analogRead usage.

  nextTickMs = millis();
  // Initialize model state exactly as contract
  adcCount = 0.0;
}

void loop() {
  unsigned long now = millis();
  if (now - nextTickMs >= CONTROL_PERIOD_MS) {
    // Keep periodic behavior stable: advance by multiples of period
    nextTickMs += CONTROL_PERIOD_MS;

    // ACCEPTED executable model tick:
    // first start;
    // then assign adcCount := io_0;
    double io_0 = readIo0_adcCount();
    adcCount = io_0;

    // Outputs (io_1 gate and serial_0_trigger are always true per model)
    const bool io_1 = true;
    const bool serial_0_trigger = true;
    (void)io_1;
    (void)serial_0_trigger;

    // serial_0_value_0 := adcCount * 0.1
    double serialValue0 = adcCount * SERIAL_VALUE_SCALE;

    // io_1 / serial_0_trigger gate controls printing fixed prefix line.
    // Since both are true always, print every control step.
    emitSerialFixedTemp(serialValue0);
  }
}

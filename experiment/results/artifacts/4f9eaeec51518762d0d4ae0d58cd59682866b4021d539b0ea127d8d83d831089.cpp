#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint32_t CONTROL_PERIOD_MS = 10;
static const uint32_t SERIAL_BAUD = 9600;

// Frozen model constants / semantics
// exceeded := (io_0 >= 500.0 or io_1 >= 50.0)
// serial_0_trigger := elapsedTotal == 0  (before elapsedTotal update in this 10ms step)
// serial_1_trigger := exceeded and (elapsedTotal == 0)
// elapsedTotal update: elapsedTotal = elapsedTotal + io_3; if elapsedTotal >= 1000 => elapsedTotal -= 1000

// Hardware bindings
static const uint8_t PIN_LED_WARNING = 13; // D13
static const uint8_t PIN_A0 = A0;
static const uint8_t PIN_A1 = A1;

#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint32_t readAdcFloat(uint8_t pin);
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline uint32_t readAdcFloat(uint8_t pin) {
  // Model uses f64 but no explicit mapping/units provided; contract binds to scale=1.0 offset=0.0.
  // So report raw ADC count as floating value.
  return (uint32_t)analogRead(pin);
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(PIN_LED_WARNING, OUTPUT);
  digitalWrite(PIN_LED_WARNING, LOW);
}

void loop() {
  // Use elapsed time from the actual scheduler; io_3 is delta_since_previous_step in ms (i32)
  static uint32_t lastStepMs = 0;
  uint32_t nowMs = millis();
  if (lastStepMs == 0) {
    lastStepMs = nowMs;
  }

  // Maintain 10ms dispatch granularity (control_period_ms = 10)
  uint32_t elapsedMs = nowMs - lastStepMs;
  if (elapsedMs < CONTROL_PERIOD_MS) {
    // idle until next tick
    return;
  }

  // We may have drift; to preserve "delta_since_previous_step", use actual elapsedMs
  // but still dispatch only once per loop iteration (keeping behavior stable).
  int32_t io_3 = (int32_t)elapsedMs;

  // Read inputs (io_0, io_1 as f64 report values)
  // io_0 := A0 raw ADC count
  // io_1 := A1 raw ADC count
  float io_0 = (float)readAdcFloat(PIN_A0);
  float io_1 = (float)readAdcFloat(PIN_A1);

  // Controller state
  static int32_t elapsedTotal = 0; // i32 initial 0
  static bool exceeded = false;     // bool initial false

  // IMPORTANT: Model outputs are computed from current state and current inputs.
  // Based on SysML action semantics (single tick with ordered updates), triggers for this
  // control step use the pre-update elapsedTotal value because serial_0_trigger is:
  // "elapsedTotal == 0" and elapsedTotal is updated in the same tick after initial state.
  // We implement by evaluating outputs before applying state updates, matching the "oracle"
  // expectation at the 1-second boundary.
  bool serial_0_trigger = (elapsedTotal == 0);
  bool newExceeded = (io_0 >= 500.0f) || (io_1 >= 50.0f);
  bool serial_1_trigger = newExceeded && (elapsedTotal == 0);

  // Perform model updates in order:
  // elapsedTotal := elapsedTotal + io_3
  int32_t updatedElapsedTotal = elapsedTotal + io_3;

  // exceeded := io_0 >= 500.0 or io_1 >= 50.0
  exceeded = newExceeded;

  // if elapsedTotal >= 1000 then elapsedTotal := elapsedTotal - 1000
  if (updatedElapsedTotal >= 1000) {
    updatedElapsedTotal -= 1000;
  }
  elapsedTotal = updatedElapsedTotal;

  // Outputs (io_2, serial triggers/values)
  // io_2 := exceeded
  bool io_2 = exceeded;

  // serial_0_value_0 := io_0
  // serial_0_value_1 := io_1
  float serial_0_value_0 = io_0;
  float serial_0_value_1 = io_1;

  // serial_1_value_0 := exceeded
  bool serial_1_value_0 = exceeded;

  // Apply WARNING LED: active_high, model boolean true => HIGH
  digitalWrite(PIN_LED_WARNING, io_2 ? HIGH : LOW);

  // Serial output: each second via serial_0_trigger (elapsedTotal == 0)
  // and extra WARNING via serial_1_trigger (exceeded && elapsedTotal == 0).
  //
  // Text format not specified; we implement a deterministic structured format.
  // Must preserve exact values and triggered timing.
  if (serial_0_trigger) {
    // Always output light and temperature values once per second.
    // Example format:
    // LIGHT=<value> TEMP=<value>
    Serial.print("LIGHT=");
    Serial.print(serial_0_value_0, 3);
    Serial.print(" TEMP=");
    Serial.print(serial_0_value_1, 3);

    if (serial_1_trigger) {
      // Extra WARNING output when threshold exceeded at the 1-second boundary
      // and LED is on.
      Serial.print(" WARNING=1");
    } else {
      Serial.print(" WARNING=0");
    }
    Serial.println();
  }

  // Update step time reference
  // Move lastStepMs forward by exactly CONTROL_PERIOD_MS to reduce drift.
  // If elapsedMs was larger than period, resync by subtracting one period repeatedly would be complex;
  // simple approach: set lastStepMs = nowMs - (elapsedMs % CONTROL_PERIOD_MS).
  lastStepMs = nowMs - (elapsedMs % CONTROL_PERIOD_MS);
}

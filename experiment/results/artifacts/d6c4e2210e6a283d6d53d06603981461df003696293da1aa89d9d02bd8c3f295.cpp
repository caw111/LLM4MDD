#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

static const uint8_t DHT_PIN = 7; // D7
static const uint8_t DHT_TYPE = DHT11;
static const unsigned long SERIAL_BAUD = 9600;

DHT dht(DHT_PIN, DHT_TYPE);

// Frozen interface contract variables
static float io_0 = 0.0f;   // temperature (degC)
static float io_1 = 0.0f;   // humidity (%RH)
static bool  io_2 = false;   // validity
static int32_t io_3 = 0;     // elapsed_ms delta since previous control step

// Controller state
static int32_t reportAccumMs = 0;

// Timing for elapsed_ms delta since previous control step
static unsigned long lastStepMicros = 0;

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateInputsFromDHT();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateInputsFromDHT() {
  // Acquire DHT11 on D7; "invalid_policy": hold_last_valid.
  // DHT11 read itself can fail; we update io_2 only on success, while holding io_0/io_1 last valid.
  float t = dht.readTemperature(); // degC
  float h = dht.readHumidity();    // %RH

  bool ok = !(isnan(t) || isnan(h));
  if (ok) {
    io_0 = t;
    io_1 = h;
    io_2 = true;
  } else {
    io_2 = false;
    // hold_last_valid: keep io_0/io_1 unchanged
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  // DHT init
  dht.begin();

  // Initialize controller timing
  lastStepMicros = micros();
  reportAccumMs = 0;

  // Initial interface values per contract
  io_0 = 0.0f;
  io_1 = 0.0f;
  io_2 = false;
  io_3 = 0;
}

void loop() {
  // Control period: 10ms (Frozen contract: control_period_ms=10)
  // Implement exact tick cadence in software.
  static unsigned long nextTickMicros = 0;
  const unsigned long controlPeriodMicros = 10UL * 1000UL;

  unsigned long nowMicros = micros();
  if (nextTickMicros == 0) nextTickMicros = nowMicros;

  if ((long)(nowMicros - nextTickMicros) < 0) {
    return;
  }
  nextTickMicros += controlPeriodMicros;

  // Compute elapsed_ms delta since previous control step: io_3
  // Use actual elapsed micros for the delta, then quantize to ms integer.
  unsigned long prevMicros = lastStepMicros;
  lastStepMicros = nowMicros;

  unsigned long deltaMicros = (nowMicros >= prevMicros) ? (nowMicros - prevMicros) : 0;
  io_3 = (int32_t)(deltaMicros / 1000UL); // ms

  // Update DHT inputs each tick (inputs sample_period_ms is 2000ms per contract,
  // but requirement only mandates reading DHT11 values on D7; invalid_policy hold_last_valid.
  // Avoid violating DHT sampling constraints by throttling DHT reads to >=2000ms.
  // We'll implement a conservative throttle: only read DHT every 2000ms.
  static unsigned long lastDhtReadMillis = 0;
  unsigned long nowMillis = millis();
  if ((nowMillis - lastDhtReadMillis) >= 2000UL) {
    lastDhtReadMillis = nowMillis;
    updateInputsFromDHT();
  } else {
    // Hold previous inputs; io_2 remains what it was last set to during last read attempt.
    // Contract: invalid_policy=hold_last_valid for temperature/humidity; io_2 is "validity from D7".
    // When not reading, keep the last known io_2 value (consistent with holding behavior).
    // Do nothing.
  }

  // Controller update:
  // reportAccumMs := reportAccumMs + io_3
  reportAccumMs = reportAccumMs + io_3;

  // if reportAccumMs >= 1000 then reportAccumMs := reportAccumMs - 1000
  // (Note: single-step per tick, exactly as frozen model indicates)
  if (reportAccumMs >= 1000) {
    reportAccumMs = reportAccumMs - 1000;
  }

  // outputs:
  // serial_0_trigger = if reportAccumMs >= 0 then ((reportAccumMs >= 1000) and io_2) else false
  // Since reportAccumMs is maintained by subtracting 1000 when >=1000, this will generally be false
  // immediately after subtraction; preserve exact expression semantics.
  bool serial_0_trigger = false;
  if (reportAccumMs >= 0) {
    serial_0_trigger = (reportAccumMs >= 1000) && io_2;
  } else {
    serial_0_trigger = false;
  }

  float serial_0_value_0 = io_0; // if io_2 ? io_0 else io_0
  float serial_0_value_1 = io_1; // if io_2 ? io_1 else io_1
  bool  serial_0_value_2 = io_2;

  // Emit serial output once per second via gate
  // Contract requires "every 1 second" with this single report gate.
  if (serial_0_trigger) {
    // Framework default behavior for multiple numeric values within one serial channel group:
    // We'll print as: temp humidity valid in one line, comma-separated.
    // Line_or_idle with idle_timeout_ms=20 is compatible with newline-terminated lines.
    Serial.print(serial_0_value_0, 2);
    Serial.print(' ');
    Serial.print(serial_0_value_1, 2);
    Serial.print(' ');
    Serial.print(serial_0_value_2 ? 1 : 0);
    Serial.print('\n');
  }
}

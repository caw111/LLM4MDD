#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include "DHT.h"

namespace GeneratedModel {
namespace ScalarValues {
  using f64 = double;
  using bool_ = bool;
  using i32 = int32_t;
}
}

static constexpr uint8_t DHT_PIN = 2;     // SDA -> D2
static constexpr uint8_t DHT_TYPE = DHT22; // DHT22 sensor

DHT dht(DHT_PIN, DHT_TYPE);

// Frozen interface contract wiring/semantics
static GeneratedModel::ScalarValues::f64 io_0 = 0.0;                 // temperature to print (C)
static GeneratedModel::ScalarValues::bool_ io_1 = false;            // dht_valid
static GeneratedModel::ScalarValues::i32 io_2 = 0;                    // delta ms since previous control step

// Controller state
static GeneratedModel::ScalarValues::f64 lastTemperature = 0.0;

// Timing contract
static constexpr uint32_t control_period_ms = 10;
static uint32_t prev_step_ms = 0;

static uint32_t last_dht_read_ms = 0; // for DHT22 >= 2000ms constraint
static bool first_dht_read_done = false;

// Serial contract
static constexpr uint32_t serial_baud = 9600;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serial_print_current_temperature(double tempC);
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controller_tick();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serial_print_current_temperature(double tempC) {
  Serial.print(F("Current temperature: "));
  // The model uses the f64 value directly; print without forced rounding.
  Serial.println(tempC);
}

static void controller_tick() {
  // Exact ACCEPTED executable model updates and outputs:
  // if (io_1) lastTemperature := io_0
  if (io_1) {
    lastTemperature = io_0;
  }

  // serial_0_trigger = io_1
  // serial_0_value_0 = lastTemperature
  bool serial_0_trigger = io_1;
  if (serial_0_trigger) {
    serial_print_current_temperature(lastTemperature);
  }
}

void setup() {
  Serial.begin(serial_baud);

  dht.begin();

  prev_step_ms = millis();
  last_dht_read_ms = 0;
  first_dht_read_done = false;

  // initial controller state already matches contract: lastTemperature = 0.0
}

void loop() {
  // Control loop at ~10ms granularity
  uint32_t now = millis();
  uint32_t elapsed = now - prev_step_ms;
  if (elapsed < control_period_ms) {
    return;
  }

  // Advance step accounting in case loop runs late; keep delta in contract variable io_2.
  io_2 = static_cast<GeneratedModel::ScalarValues::i32>(elapsed);
  prev_step_ms = now;

  // Read DHT sensor (model assumes cadence handled by model/hardware contract:
  // sample_period_ms = 2000ms; hold_last_valid for invalid samples.)
  // We'll compute validity and maintain io_0 with hold-last-valid policy.
  bool new_valid = false;
  double new_temp = io_0; // default hold-last-valid

  if (!first_dht_read_done || (now - last_dht_read_ms) >= 2000UL) {
    first_dht_read_done = true;
    last_dht_read_ms = now;

    float t = dht.readTemperature(); // Celsius
    // DHT library returns NAN on invalid reading.
    if (!isnan(t)) {
      new_valid = true;
      new_temp = static_cast<double>(t);
    }
  }

  if (new_valid) {
    io_0 = new_temp;
    io_1 = true;
  } else {
    // invalid_policy: hold_last_valid
    // Keep io_0 unchanged; io_1 indicates validity of latest sample.
    // Contract: io_1 is "Whether the latest DHT temperature reading is valid"
    // and invalid_policy holds last valid temperature value, not io_1.
    io_1 = false;
  }

  // Tick controller: preserves exact state update order and outputs.
  controller_tick();
}

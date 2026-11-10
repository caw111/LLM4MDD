#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_parking_spot_monitor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t PIN_RGB_RED   = 4; // io_4
static constexpr uint8_t PIN_RGB_GREEN = 3; // io_3
static constexpr uint8_t PIN_RGB_BLUE  = 2; // io_2

static constexpr uint8_t PIN_TRIG = 8;
static constexpr uint8_t PIN_ECHO = 9;

static constexpr uint32_t CONTROL_PERIOD_MS = 10; // frozen contract

// Ultrasonic acquisition contract details
static constexpr uint32_t ULTRASONIC_TIMEOUT_US = 30000; // within 25000..60000
static constexpr uint32_t ULTRASONIC_TRIGGER_PERIOD_MS = 60; // sample_period_ms=60
static constexpr float CM_PER_TICK_DIVISOR = 58.0f; // distance_cm = raw / 58

// Contract signals (model inputs/outputs)
static float ultrasonic_distance_cm = 0.0f; // io_0
static bool ultrasonic_valid = false;        // io_1

// Contract outputs
static bool io_2 = false; // BLUE (initial false)
static bool io_3 = false; // GREEN
static bool io_4 = false; // RED

// Model: io_2 = io_1 and (io_0 < 50.0)
// Model: io_3 = io_1 and (io_0 >= 50.0)
// Model: io_4 = false
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_parking_spot_monitor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void model_update();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_parking_spot_monitor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void hardware_init();
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_parking_spot_monitor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool ultrasonic_read(float &distance_cm_out);
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_parking_spot_monitor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void hardware_update_outputs_from_model();
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_parking_spot_monitor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_parking_spot_monitor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_parking_spot_monitor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void model_update() {
  io_2 = ultrasonic_valid && (ultrasonic_distance_cm < 50.0f);
  io_3 = ultrasonic_valid && (ultrasonic_distance_cm >= 50.0f);
  io_4 = false;
}

// Hardware: set up pins as specified in prompt
static void hardware_init() {
  // Requirement: set pin8, pin2~pin4 as OUTPUT, and pin9 as INPUT
  pinMode(PIN_TRIG, OUTPUT);   // pin8
  pinMode(PIN_RGB_BLUE, OUTPUT);  // pin2
  pinMode(PIN_RGB_GREEN, OUTPUT); // pin3
  pinMode(PIN_RGB_RED, OUTPUT);   // pin4
  pinMode(PIN_ECHO, INPUT);       // pin9

  // Ensure known levels (model outputs are initialized false)
  digitalWrite(PIN_RGB_BLUE, LOW);
  digitalWrite(PIN_RGB_GREEN, LOW);
  digitalWrite(PIN_RGB_RED, LOW);
}

static bool ultrasonic_read(float &distance_cm_out) {
  // Implements HC-SR04 style: trigger a pulse, measure echo duration.
  // invalid_policy: hold_last_valid is handled by caller (only update cm when valid).
  // timeout_us: 30000
  // Return validity; distance computed by dividing by 58.
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  // 10us trigger pulse
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Use pulseIn with timeout
  unsigned long duration_us = pulseIn(PIN_ECHO, HIGH, ULTRASONIC_TIMEOUT_US);
  if (duration_us == 0) {
    return false;
  }

  // HC-SR04 echo time corresponds to distance; required rule: divide by 58.
  // Note: contract says divide the sensor value by 58; we treat duration_us as that raw sensor value.
  distance_cm_out = (float)duration_us / CM_PER_TICK_DIVISOR;
  return true;
}

static void hardware_update_outputs_from_model() {
  // Contract semantics: active_high, true_level HIGH, false_level LOW.
  // io_2 is "RGB LED blue channel" connected to D2 (pin2)
  digitalWrite(PIN_RGB_BLUE, io_2 ? HIGH : LOW);
  // io_3 is "RGB LED green channel" connected to D3 (pin3)
  digitalWrite(PIN_RGB_GREEN, io_3 ? HIGH : LOW);
  // io_4 is "RGB LED red channel" connected to D4 (pin4)
  digitalWrite(PIN_RGB_RED, io_4 ? HIGH : LOW);
}

void setup() {
  hardware_init();
  // Contract initial values: inputs initial 0/false, outputs initial false
  ultrasonic_distance_cm = 0.0f;
  ultrasonic_valid = false;
  io_2 = false; io_3 = false; io_4 = false;

  model_update();
  hardware_update_outputs_from_model();
}

void loop() {
  static uint32_t t_next_control = millis();
  static uint32_t t_next_ultrasonic = millis();

  while ((int32_t)(millis() - t_next_control) < 0) {
    // Keep loop responsive; no additional behavior specified.
  }
  t_next_control += CONTROL_PERIOD_MS;

  // Update ultrasonic acquisition at its sample period (60ms)
  if ((int32_t)(millis() - t_next_ultrasonic) >= 0) {
    t_next_ultrasonic += ULTRASONIC_TRIGGER_PERIOD_MS;

    float candidate_cm = ultrasonic_distance_cm;
    bool valid = ultrasonic_read(candidate_cm);

    // invalid_policy: hold_last_valid
    if (valid) {
      ultrasonic_distance_cm = candidate_cm;
      ultrasonic_valid = true;
    } else {
      ultrasonic_valid = false;
      // hold_last_valid for distance itself isn't explicitly required for cm,
      // but we preserve the last distance by not updating ultrasonic_distance_cm.
    }
  }

  // Model update (deterministic combinational logic)
  model_update();

  // Hardware outputs
  hardware_update_outputs_from_model();
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

using f64 = double;

// --- Frozen configuration ---
static const uint32_t SERIAL_BAUD = 9600;

// Control period contract (not directly used by the model, but preserved)
static const uint32_t CONTROL_PERIOD_MS = 10;

// Ultrasonic contract
static const uint32_t ULTRASONIC_TIMEOUT_US = 30000UL;
static const uint32_t ULTRASONIC_SAMPLE_PERIOD_MS = 60UL; // >= ceil((timeout_us+12)/1000) and multiple of control_period_ms (10)

// Hardware binding (authoritative)
static const uint8_t TRIG_PIN = 8; // D8
static const uint8_t ECHO_PIN = 9; // D9

// --- Interface signals (as per frozen model + ultrasonic sampling) ---
static f64 io_0_distance_cm = 0.0;   // io_0: distance (cm)
static bool io_1_valid = false;      // io_1: validity
static f64 serial_0_value_0_cm = 0.0;
static bool serial_0_trigger = false;

// Hold-last-valid policy state
static f64 last_valid_distance_cm = 0.0;

// --- Scheduling ---
static uint32_t next_ultrasonic_ms = 0;
static uint32_t next_control_ms = 0;

#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint32_t nowMs();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static f64 measureUltrasonicDistanceCm(uint32_t timeout_us, bool &valid_out);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void model_step();
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline uint32_t nowMs() {
  return (uint32_t)millis();
}

// Measure distance using HC-SR04 with timeout_us.
// invalid_policy: hold_last_valid (so if invalid, io_0 remains last_valid)
static f64 measureUltrasonicDistanceCm(uint32_t timeout_us, bool &valid_out) {
  valid_out = false;

  // Ensure trigger low
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Trigger pulse: 10 us HIGH
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width with timeout
  // pulseIn returns 0 on timeout.
  uint32_t duration_us = pulseIn(ECHO_PIN, HIGH, timeout_us);

  if (duration_us == 0) {
    valid_out = false;
    return last_valid_distance_cm; // will be overwritten by caller based on invalid_policy
  }

  // HC-SR04: distance (cm) = (time_us * 0.0343) / 2
  // Use precise constant: 34300 cm/s -> 0.0343 cm/us
  f64 distance_cm = (static_cast<f64>(duration_us) * 0.0343) / 2.0;

  valid_out = true;
  return distance_cm;
}

// Serial output: contract says "通过串口输出距离" and gating is proposed by ultrasonic_valid.
// Frozen model: serial_0_trigger = io_1 and serial_0_value_0 = (io_1 ? io_0 : 0.0).
// We output distance when considered valid. Additionally, always drive value port per model mapping.
static void model_step() {
  // Frozen interface contract mapping
  serial_0_trigger = io_1_valid;
  serial_0_value_0_cm = io_1_valid ? io_0_distance_cm : 0.0;

  // "No additional serial protocol was specified beyond output distance".
  // Implement line_or_idle style: print a line when triggered.
  // Keep it simple and deterministic: print value with 2 decimals, terminated by newline.
  if (serial_0_trigger) {
    Serial.println(serial_0_value_0_cm, 2);
  } else {
    // When invalid, do not output distance (gated by ultrasonic_valid).
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(SERIAL_BAUD);

  // Initialize scheduling
  uint32_t t = nowMs();
  next_ultrasonic_ms = t; // take first sample immediately
  next_control_ms = t;     // start control step immediately
}

void loop() {
  uint32_t t = nowMs();

  // Ultrasonic sampling (io_0 and io_1)
  if ((int32_t)(t - next_ultrasonic_ms) >= 0) {
    next_ultrasonic_ms += ULTRASONIC_SAMPLE_PERIOD_MS;

    bool valid = false;
    f64 measured = measureUltrasonicDistanceCm(ULTRASONIC_TIMEOUT_US, valid);

    if (valid) {
      io_1_valid = true;
      io_0_distance_cm = measured;
      last_valid_distance_cm = measured; // maintain hold-last-valid baseline
    } else {
      io_1_valid = false;
      // invalid_policy: hold_last_valid
      io_0_distance_cm = last_valid_distance_cm;
    }
  }

  // Control step every CONTROL_PERIOD_MS
  if ((int32_t)(t - next_control_ms) >= 0) {
    next_control_ms += CONTROL_PERIOD_MS;

    model_step();
  }
}

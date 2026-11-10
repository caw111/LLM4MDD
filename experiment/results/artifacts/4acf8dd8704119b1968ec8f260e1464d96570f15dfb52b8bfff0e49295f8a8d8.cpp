#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t PIN_LED = 13;      // D13
static constexpr uint8_t PIN_BUZZER = 6;    // D6 (tone output)
static constexpr uint8_t PIN_TRIG = 8;      // D8
static constexpr uint8_t PIN_ECHO = 9;      // D9

// Frozen interface contract
static constexpr uint32_t CONTROL_PERIOD_MS = 5;   // io_0/io_1 sample period
static constexpr uint32_t SENSOR_PERIOD_MS  = 60;   // io_2/io_3 sample period
static constexpr uint32_t ULTRASONIC_TIMEOUT_US = 25000; // timeout_us
static constexpr float DIST_THRESHOLD_CM = 20.0f;

// Ultrasonic invalid policy: hold_last_valid
static bool  has_last_valid = false;
static float last_valid_distance_cm = 0.0f;
static bool  last_valid_io3 = false;

// Inputs (io_2, io_3) and outputs (io_0, io_1)
static float io_2_cm = 0.0f; // measured distance (cm)
static bool  io_3_valid = false;

static bool io_0_led = false;
static int32_t io_1_buzzer_hz = 0; // i32 Hz

// Compute controller outputs exactly as model:
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controller_compute();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool ultrasonic_measure_cm(float &out_distance_cm);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void controller_compute() {
  const bool cond = (io_3_valid && (io_2_cm < DIST_THRESHOLD_CM));
  io_0_led = cond ? true : false;
  io_1_buzzer_hz = cond ? 2000 : 0;
}

// One HC-SR04 measurement with timeout and validity handling.
// Returns true if echo was captured within timeout, and sets distance.
static bool ultrasonic_measure_cm(float &out_distance_cm) {
  // Trigger pulse: 10us HIGH
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure echo pulse width in microseconds
  // timeout in microseconds, accounting for function constraints.
  unsigned long duration_us = pulseIn(PIN_ECHO, HIGH, ULTRASONIC_TIMEOUT_US);

  if (duration_us == 0) {
    return false; // invalid measurement (timeout)
  }

  // HC-SR04: distance (cm) = duration_us * 0.034 / 2
  out_distance_cm = (static_cast<float>(duration_us) * 0.0343f) / 2.0f;
  return true;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_TRIG, LOW);

  // Initialize IO per contract initial values:
  io_2_cm = 0.0f;
  io_3_valid = false;

  io_0_led = false;
  io_1_buzzer_hz = 0;

  // Apply initial outputs
  digitalWrite(PIN_LED, io_0_led ? HIGH : LOW);

  // Tone invalid_policy = silence
  noTone(PIN_BUZZER);
}

void loop() {
  static uint32_t last_control_ms = 0;
  static uint32_t last_sensor_ms = 0;

  const uint32_t now_ms = millis();

  // Sensor update at 60ms period for io_2/io_3 (with hold-last-valid)
  if (now_ms - last_sensor_ms >= SENSOR_PERIOD_MS) {
    last_sensor_ms += SENSOR_PERIOD_MS;

    float measured_cm = 0.0f;
    const bool ok = ultrasonic_measure_cm(measured_cm);

    if (ok) {
      io_2_cm = measured_cm;
      io_3_valid = true;
      has_last_valid = true;
      last_valid_distance_cm = measured_cm;
      last_valid_io3 = true;
    } else {
      if (has_last_valid) {
        // hold_last_valid: retain previous inputs and validity
        io_2_cm = last_valid_distance_cm;
        io_3_valid = last_valid_io3;
      } else {
        // If never had valid measurement, keep initial values
        io_2_cm = 0.0f;
        io_3_valid = false;
      }
    }
  }

  // Control update at 5ms period
  if (now_ms - last_control_ms >= CONTROL_PERIOD_MS) {
    last_control_ms += CONTROL_PERIOD_MS;

    controller_compute();

    // Output bindings:
    // io_0: digital_output active_high, pin D13
    digitalWrite(PIN_LED, io_0_led ? HIGH : LOW);

    // io_1: tone_output, invalid_policy="silence" on D6
    // Model uses: if cond then 2000 else 0 Hz.
    if (io_1_buzzer_hz > 0) {
      // Maintain tone frequency; duty cycle not specified.
      tone(PIN_BUZZER, static_cast<unsigned int>(io_1_buzzer_hz));
    } else {
      noTone(PIN_BUZZER);
    }
  }
}

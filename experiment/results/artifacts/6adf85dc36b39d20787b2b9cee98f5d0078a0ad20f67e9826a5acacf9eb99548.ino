#include <Arduino.h>

static const uint8_t DIST_TRIG_PIN = 3; // TRIG -> D3
static const uint8_t DIST_ECHO_PIN = 2; // ECHO -> D2
static const uint32_t TIMEOUT_US = 30000; // timeout_us=30000
static const uint32_t CONTROL_PERIOD_MS = 10; // frozen contract
static const uint32_t SERIAL_BAUD = 9600;

static const uint32_t SAMPLE_PERIOD_MS = 60; // io_0 sample_period_ms=60
static const uint32_t PRINT_CADENCE_MS = CONTROL_PERIOD_MS * 1; // serial trigger port sampled on control period

// Converted output requirement uses: distance_cm = distance_raw / 58
// Contract validity: validity starts false, invalid_policy = hold_last_valid
static bool prev_valid = false;           // controller state
static double serial_value_hold = 0.0;  // controller state

static bool io_1 = false;    // ultrasonic_valid
static double io_0 = 0.0;   // ultrasonic_distance (cm) reporting value

static double last_valid_cm = 0.0;
static bool last_valid_available = false;

// Serial output: print prefix "Distance in CM: " followed by numeric value only.
static void report_serial(double cm) {
  // No additional fixed text beyond prefix requirement.
  Serial.print(F("Distance in CM: "));
  Serial.println(cm);
}

// Perform one ultrasonic acquisition (blocking). Returns true if valid echo captured.
static bool readUltrasonicRawCm(double &out_cm) {
  // Trigger pulse: 10us HIGH
  digitalWrite(DIST_TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(DIST_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(DIST_TRIG_PIN, LOW);

  // Measure echo pulse width in microseconds. timeout enforced.
  unsigned long duration_us = pulseIn(DIST_ECHO_PIN, HIGH, TIMEOUT_US);
  if (duration_us == 0) {
    return false; // invalid (timeout or no pulse)
  }

  // HC-SR04 formula: distance_cm ~ duration_us / 58
  out_cm = (double)duration_us / 58.0;
  return true;
}

void setup() {
  pinMode(DIST_TRIG_PIN, OUTPUT);
  pinMode(DIST_ECHO_PIN, INPUT);

  // Ensure stable initial trigger low
  digitalWrite(DIST_TRIG_PIN, LOW);

  Serial.begin(SERIAL_BAUD);
  // Framing is handled by Arduino Serial default; contract says line_or_idle.

  // Frozen contract: requested ultrasonic_distance port initial is 0; validity starts false.
  io_0 = 0.0;
  io_1 = false;

  // Controller initial state already set by static initialization above:
  // prev_valid = false, serial_value_hold = 0.0.
  last_valid_cm = 0.0;
  last_valid_available = false;

  // Align schedule
  // We'll sample ultrasonic immediately on first opportunity after setup.
}

void loop() {
  static uint32_t last_sample_ms = 0;
  static uint32_t last_control_ms = 0;
  static bool scheduled_print = false;

  uint32_t now = millis();

  // Enforce controller "control_period_ms" pacing with 10ms tick.
  if (now - last_control_ms < CONTROL_PERIOD_MS) {
    return;
  }
  last_control_ms += CONTROL_PERIOD_MS;

  // Ultrasonic sample pacing: acquire using elapsed_ms with period multiple of control period and meets >= 60ms.
  // Contract sample_period_ms=60.
  if (now - last_sample_ms >= SAMPLE_PERIOD_MS) {
    last_sample_ms += SAMPLE_PERIOD_MS;

    double cm = 0.0;
    bool valid = readUltrasonicRawCm(cm);

    if (valid) {
      io_1 = true;
      io_0 = cm;
      last_valid_cm = cm;
      last_valid_available = true;
    } else {
      // invalid_policy = hold_last_valid
      if (last_valid_available) {
        io_1 = false;
        io_0 = last_valid_cm; // held value, though validity flag is false
      } else {
        io_1 = false;
        io_0 = 0.0; // still initial until first valid measurement
      }
    }
  }

  // --- Accepted executable model: Controller tick ---
  // perform action tick:
  // then if io_1 { serial_value_hold := io_0; }
  // then prev_valid := io_1;
  if (io_1) {
    serial_value_hold = io_0;
  }

  bool current_valid = io_1;
  // out serial_0_trigger = io_1 and not prev_valid
  bool serial_0_trigger = current_valid && (!prev_valid);

  prev_valid = current_valid;

  // serial_0_value_0 = serial_value_hold
  // On trigger, print: "Distance in CM: {value}"
  if (serial_0_trigger) {
    report_serial(serial_value_hold);
  }

  (void)scheduled_print;
}
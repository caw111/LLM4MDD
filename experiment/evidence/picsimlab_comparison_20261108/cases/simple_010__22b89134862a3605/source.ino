#include <Arduino.h>

static constexpr uint32_t CONTROL_PERIOD_MS = 10; // frozen contract
static constexpr uint32_t REPORT_THRESHOLD_MS = 1000;

static int32_t accumulated = 0;   // i32, initial 0
static bool serialTrig = false;  // bool, initial false

// io_0: analog_input A0, continuously sampled (default ADC scaling)
static double read_io_0_f64_adc_count() {
  // A0 potentiometer analog value; preserve "adc_count" meaning as raw ADC count.
  // Arduino UNO ADC is 10-bit => 0..1023.
  int v = analogRead(A0);
  return static_cast<double>(v);
}

// io_1: elapsed_ms delta since previous control step
static int32_t read_io_1_delta_ms(uint32_t nowMs, uint32_t &lastMs) {
  uint32_t delta = nowMs - lastMs; // wraps safely for uint32_t
  lastMs = nowMs;
  return static_cast<int32_t>(delta);
}

void setup() {
  // Serial: baud 9600, framing line_or_idle (no explicit CR/LF requirement provided)
  Serial.begin(9600);

  // Ensure deterministic initial state (already static-initialized to 0/false).
  accumulated = 0;
  serialTrig = false;

  // Set up timing reference for io_1 delta.
  // Use millis() baseline.
}

void loop() {
  static uint32_t nextTickMs = 0;
  static uint32_t lastIo1Ms = 0;
  static bool initialized = false;

  if (!initialized) {
    uint32_t now = millis();
    nextTickMs = now;      // first tick immediately
    lastIo1Ms = now;
    initialized = true;
  }

  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) {
    return; // wait until next control period
  }

  // If we are late, keep stepping forward to preserve 10ms cadence as closely as possible.
  // Do at most one tick per loop() invocation.
  nextTickMs += CONTROL_PERIOD_MS;

  // --- Sample inputs for this control step ---
  double io_0 = read_io_0_f64_adc_count();              // io_0 (f64, adc_count)
  int32_t io_1 = read_io_1_delta_ms(now, lastIo1Ms);  // io_1 (i32, ms delta)

  // --- Accepted executable model tick() updates in exact order ---
  // assign accumulated := accumulated + io_1
  accumulated = accumulated + io_1;

  if (accumulated >= static_cast<int32_t>(REPORT_THRESHOLD_MS)) {
    // assign accumulated := accumulated - 1000
    accumulated = accumulated - static_cast<int32_t>(REPORT_THRESHOLD_MS);

    // assign serialTrig := true
    serialTrig = true;
  } else {
    // assign serialTrig := false
    serialTrig = false;
  }

  // --- Outputs ---
  // serial_0_trigger == serialTrig
  if (serialTrig) {
    // serial_0_value_0 == io_0
    // Print numeric value as a single floating-point value.
    // No explicit CR/LF requirement provided; use println to emit line-oriented/idle completion.
    Serial.println(static_cast<double>(io_0), 6);
  }
}
#include <Arduino.h>

// Frozen hardware contract
// io_0: output bool -> D13 (active HIGH)
// io_1: input f64 cm (ultrasonic distance, invalid_policy=hold_last_valid)
// io_2: input bool (ultrasonic valid, invalid_policy=hold_last_valid)

// HC-SR04 wiring: Trig=D8, Echo=D9
static constexpr uint8_t TRIG_PIN = 8;
static constexpr uint8_t ECHO_PIN = 9;
static constexpr uint8_t LED_PIN  = 13;

// Frozen interface timing/behavior
static constexpr uint32_t CONTROL_PERIOD_MS = 5;   // controller period
static constexpr uint32_t ULTRASONIC_SAMPLE_MS = 60; // sampling period for distance/valid
static constexpr uint32_t TIMEOUT_US = 30000UL;    // timeout_us=30000

// Hold last valid policy
static double last_valid_distance_cm = 0.0;
static bool last_valid = false;

// Most recent sampled values (used by controller)
static double io_1_distance_cm = 0.0;
static bool io_2_valid = false;
static bool controller_io_0 = false;

// Non-blocking-ish controller scheduling: sample ultrasound at fixed interval,
// controller evaluates output each control tick.
static unsigned long next_control_ms = 0;
static unsigned long next_ultrasonic_ms = 0;

static unsigned long micros_u32() { return micros(); }

static double read_hc_sr04_distance_cm_with_timeout(bool &valid_out) {
  // Trigger a short pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width with timeout.
  // Use pulseIn, which blocks; timeout keeps it bounded.
  // pulseIn returns 0 on timeout.
  unsigned long duration_us = pulseIn(ECHO_PIN, HIGH, TIMEOUT_US);

  if (duration_us == 0) {
    valid_out = false;
    return 0.0;
  }

  valid_out = true;

  // Speed of sound: 343 m/s -> 0.0343 cm/us
  // Distance (cm) = duration_us * 0.0343 / 2
  // => duration_us * 0.01715
  return (double)duration_us * 0.01715;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  unsigned long now_ms = millis();
  next_control_ms = now_ms;
  next_ultrasonic_ms = now_ms; // start sampling immediately
}

void loop() {
  const unsigned long now_ms = millis();

  // Sample ultrasound at its configured period (integer multiples of control period not enforced here)
  if ((long)(now_ms - next_ultrasonic_ms) >= 0) {
    next_ultrasonic_ms = now_ms + ULTRASONIC_SAMPLE_MS;

    bool valid_now = false;
    double dist_now = read_hc_sr04_distance_cm_with_timeout(valid_now);

    // invalid_policy = hold_last_valid for distance
    if (valid_now) {
      last_valid_distance_cm = dist_now;
      last_valid = true;
    } else {
      // keep last valid distance and validity flag policy:
      // io_2 indicates validity; if invalid, io_2 should be false,
      // but distance should still "hold" previous last valid.
      // (The contract states invalid_policy=hold_last_valid; validity input exists.)
      // So: distance holds; validity reflects current measurement validity.
      // However, if there was never a previous valid measurement, distance remains at last_valid_distance_cm default.
    }

    io_1_distance_cm = last_valid_distance_cm;
    io_2_valid = valid_now;
  }

  // Controller tick at CONTROL_PERIOD_MS
  if ((long)(now_ms - next_control_ms) >= 0) {
    next_control_ms = now_ms + CONTROL_PERIOD_MS;

    // ACCEPTED controller model:
    // io_0 = (if io_2 then io_1 < 20.0 else false)
    if (io_2_valid) {
      controller_io_0 = (io_1_distance_cm < 20.0);
    } else {
      controller_io_0 = false;
    }

    // Output binding: io_0 (bool) -> LED active_high on D13
    digitalWrite(LED_PIN, controller_io_0 ? HIGH : LOW);
  }
}
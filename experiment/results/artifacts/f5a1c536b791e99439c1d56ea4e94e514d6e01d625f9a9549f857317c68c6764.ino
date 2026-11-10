#include <Arduino.h>

static const uint8_t PIN_TRIG = D8;
static const uint8_t PIN_ECHO = D9;
static const uint8_t PIN_BUZZ = D6;
static const uint8_t PIN_BTN  = D2;
static const uint8_t PIN_LED  = D13;

static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t CONTROL_PERIOD_MS = 10;

// Frozen ultrasonic timeout (must be within allowed 25000..60000)
static const unsigned long ULTRASONIC_TIMEOUT_US = 30000UL;

static bool muteMode = false;         // io_1
static bool previousPressed = false; // internal state

static bool readButtonRaw() {
  // Contract says: active_high = true, pullup = false; treat HIGH as pressed.
  return digitalRead(PIN_BTN) == HIGH;
}

static void driveOutputs(bool buttonMute, bool ledMuted, int buzzerHz, bool buzzerInvalidPolicySilence = true) {
  (void)buttonMute;
  digitalWrite(PIN_LED, ledMuted ? HIGH : LOW);

  if (buzzerInvalidPolicySilence && buzzerHz <= 0) {
    noTone(PIN_BUZZ);
  } else {
    // Using tone() to generate buzzer frequency on D6
    tone(PIN_BUZZ, buzzerHz);
  }
}

static bool sampleUltrasonicCm(double &out_cm) {
  // HC-SR04 standard trigger/echo. Keep invalid_policy = hold_last_valid.
  // We return validity to update hold_last_valid. On invalid, caller keeps prior.
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Ensure a clean trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure echo pulse width; use timeout_us fixed.
  // pulseIn returns 0 on timeout.
  unsigned long duration_us = pulseIn(PIN_ECHO, HIGH, ULTRASONIC_TIMEOUT_US);

  if (duration_us == 0) {
    return false;
  }

  // Distance (cm) = duration_us * speed_of_sound / 2
  // With speed_of_sound ≈ 343 m/s => 0.0343 cm/us
  out_cm = (double)duration_us * 0.0343 / 2.0;
  return true;
}

int main() {
  init();

  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BTN, INPUT); // pullup disabled per contract

  // Initialize to match model initial outputs:
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZ);

  // Ultrasonic hold-last-valid state
  double last_distance_cm = 0.0;
  bool last_valid = false;

  unsigned long t_control_next = millis();

  // Requirement: model tick period 10ms; ultrasonic sample period is 60ms,
  // but "align to control-period multiple" => 60ms is multiple of 10ms.
  unsigned long t_ultra_next = millis(); // start immediately

  while (true) {
    unsigned long now = millis();
    if ((long)(now - t_control_next) < 0) {
      // Busy wait; keep behavior deterministic enough for 10ms control period
      continue;
    }

    // =========================
    // SysML/IR/IL model tick @10ms
    // =========================
    // Inputs:
    bool io_0 = readButtonRaw();

    // io_3/io_4 sampled every 60ms with hold-last-valid
    bool io_4 = last_valid; // validity input
    double io_3 = last_distance_cm; // distance input

    // Update ultrasonic at or above 60ms and aligned to 10ms multiples.
    if ((long)(now - t_ultra_next) >= 0) {
      double measured = 0.0;
      bool valid = sampleUltrasonicCm(measured);
      if (valid) {
        last_distance_cm = measured;
        last_valid = true;
      } // else hold last valid value and valid remains as previous (per hold_last_valid)
      io_4 = last_valid;
      io_3 = last_distance_cm;

      // Schedule next ultrasonic sample at +60ms (multiple of 10ms)
      t_ultra_next += 60;
    }

    // State update per ACCEPTED model:
    // if (io_0 && !previousPressed) muteMode = !muteMode;
    if (io_0 && (!previousPressed)) {
      muteMode = !muteMode;
    }
    // previousPressed := io_0
    previousPressed = io_0;

    // Outputs per ACCEPTED model:
    // io_1 = muteMode
    bool io_1 = muteMode;

    // io_2 buzzer frequency:
    // if muteMode then 0 else if (io_4 && io_3 >= 2) then:
    //   if io_3 < 20 -> 2000
    //   else if io_3 < 50 -> 1200
    //   else if io_3 < 100 -> 700
    //   else -> 300
    // else 0
    int buzzerHz = 0;
    if (!muteMode) {
      if (io_4 && (io_3 >= 2.0)) {
        if (io_3 < 20.0) buzzerHz = 2000;
        else if (io_3 < 50.0) buzzerHz = 1200;
        else if (io_3 < 100.0) buzzerHz = 700;
        else buzzerHz = 300;
      } else {
        buzzerHz = 0;
      }
    }

    // serial_0_trigger always true
    // serial_0_value_0 = (io_4 ? io_3 : 0.0)
    double serial_value_cm = io_4 ? io_3 : 0.0;

    // Drive hardware outputs:
    driveOutputs(io_0, io_1, buzzerHz, true);

    // Serial output (single numeric line containing distance value only).
    // Emit when trigger condition is true; trigger is constant true.
    // Delegated formatting is ambiguous; we output with default double printing.
    Serial.println(serial_value_cm, 2);

    // Advance next control tick by fixed period
    t_control_next += CONTROL_PERIOD_MS;
  }

  return 0;
}
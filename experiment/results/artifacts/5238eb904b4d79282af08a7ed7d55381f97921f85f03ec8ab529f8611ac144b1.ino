#include <Arduino.h>

// Hardware pins (authoritative per frozen contract)
static const uint8_t PIN_PWM_LED = 9;   // D9
static const uint8_t PIN_BUTTON  = 2;   // D2
static const uint8_t PIN_STATUS_LED = 13; // D13

// Ultrasonic pins (HC-SR04)
static const uint8_t PIN_TRIG = 8;       // D8
static const uint8_t PIN_ECHO = 3;       // D3

// Analog input (photoresistor)
static const uint8_t PIN_LIGHT = A0;     // A0

// Control period (ms) - frozen contract: 10ms
static const uint32_t CONTROL_PERIOD_MS = 10;

// ===== Accepted executable model =====
// State:
//   manualMode (bool, initial false)
//   previousPressed (bool, initial false)
//
// Updates (executed each tick):
//   if (io_4 && !previousPressed) manualMode = !manualMode;
//   previousPressed = io_4;
//
// Outputs:
//   io_3 (pwm_count, double):
//     if manualMode => 1023.0
//     else if (io_0 < 512.0) && io_2 && (true==true) => 1023.0
//     else 0.0
//   io_5 (bool): manualMode

static bool manualMode = false;
static bool previousPressed = false;

// Inputs
static double io_0 = 0.0;  // adc_count
static bool io_1_valid = false; // (unused by model; kept to preserve ultrasonic validity concept)
static bool io_2 = false;  // ultrasonic_valid (equivalent of io_2 port)
static bool io_4 = false;  // button

// Output
static double io_3 = 0.0; // pwm_count

// Timing
static uint32_t nextTickMs = 0;

// ===== Ultrasonic measurement producing validity (io_2) =====
static bool readUltrasonicValid_cm(float &out_cm) {
  (void)out_cm; // cm not used by model for output, but computed for completeness

  // Trigger
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Timeout: frozen contract 30000us (within 25000..60000)
  const unsigned long timeoutUs = 30000UL;

  unsigned long durationUs = pulseIn(PIN_ECHO, HIGH, timeoutUs);

  // Invalid policy: hold last valid.
  // The model only requires io_2=ultrasonic_valid. We'll track last validity.
  // We'll implement hold_last_valid by returning validity and letting caller keep last cm/valid.
  if (durationUs == 0) {
    return false;
  }

  // Convert to cm: speed of sound 343m/s => 0.0343 cm/us round-trip.
  // distance(cm) = durationUs * 0.0343 / 2
  out_cm = (float)((double)durationUs * 0.0343 / 2.0);
  return true;
}

static bool lastUltrasonicValid = false;
static float lastUltrasonicCm = 0.0f;

void setup() {
  // Model / IO bindings initialization
  pinMode(PIN_PWM_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT); // frozen contract: pullup=false, active_high=true
  digitalWrite(PIN_PWM_LED, LOW);

  pinMode(PIN_STATUS_LED, OUTPUT);
  digitalWrite(PIN_STATUS_LED, LOW);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // Ensure initial state matches model initial values
  manualMode = false;
  previousPressed = false;

  // Start scheduling
  nextTickMs = millis();
}

void loop() {
  // Keep strict 10ms tick cadence
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) {
    return;
  }
  nextTickMs += CONTROL_PERIOD_MS;

  // ===== Sample inputs for this control tick =====

  // io_0: analog_input A0, scale 1.0, offset 0.0 => raw ADC counts 0..1023
  io_0 = (double)analogRead(PIN_LIGHT);

  // io_4: digital_input active_high=true, read D2 directly as bool
  io_4 = (digitalRead(PIN_BUTTON) == HIGH);

  // io_2: ultrasonic_valid (hold_last_valid)
  // Sample period for ultrasonic in contract is 60ms; to preserve behavior closely,
  // we will measure at ~60ms boundaries but use io_2 held between measures.
  // Since this sketch only controls logic at 10ms, we track whether enough time passed.
  static uint32_t nextUltrasonicMs = 0;
  if (now >= nextUltrasonicMs) {
    float cm = 0.0f;
    bool validNow = readUltrasonicValid_cm(cm);
    if (validNow) {
      lastUltrasonicValid = true;
      lastUltrasonicCm = cm;
    } else {
      // invalid_policy: hold_last_valid (so do not change lastUltrasonicValid/cm)
      // Thus keep lastUltrasonicValid as-is.
    }
    io_2 = lastUltrasonicValid;
    io_1_valid = lastUltrasonicValid;
    nextUltrasonicMs = now + 60UL; // sample_period_ms=60
  } else {
    // Use held last valid between ultrasonic sampling instants
    io_2 = lastUltrasonicValid;
    io_1_valid = lastUltrasonicValid;
  }

  // ===== Update (perform action tick) =====
  if (io_4 && (!previousPressed)) {
    manualMode = !manualMode;
  }
  previousPressed = io_4;

  // ===== Output expressions =====
  if (manualMode) {
    io_3 = 1023.0;
  } else {
    // Model: if (io_0 < 512.0) and io_2 and (true == true) then 1023 else 0
    // (true==true) is always true; preserve as-is logically.
    if ((io_0 < 512.0) && io_2 && (true == true)) {
      io_3 = 1023.0;
    } else {
      io_3 = 0.0;
    }
  }

  // io_3 -> PWM output scale 1.0 offset 0
  // analogWrite expects 0..255 on UNO; however model uses 1023.0 pwm_count.
  // To preserve exact controller semantics numerically, map linearly:
  // 1023 => 255, 0 => 0
  uint8_t pwm = (uint8_t)constrain((long)(io_3 * 255.0 / 1023.0 + 0.5), 0, 255);
  analogWrite(PIN_PWM_LED, pwm);

  // io_5 = manualMode, active_high=true => D13 HIGH when manualMode true
  digitalWrite(PIN_STATUS_LED, manualMode ? HIGH : LOW);
}
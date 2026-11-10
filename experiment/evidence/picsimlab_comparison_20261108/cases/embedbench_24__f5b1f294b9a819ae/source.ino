#include <Arduino.h>
#include <Servo.h>

// Frozen interface contract bindings
static constexpr uint8_t POT1_PIN = A3; // io_0
static constexpr uint8_t POT2_PIN = A4; // io_1
static constexpr uint8_t POT3_PIN = A5; // io_2
static constexpr uint8_t SERVO_PIN = 2; // io_3 PWM on D2 (Mega)

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static Servo servo1;

// Controller state (prevMinDeg, prevMaxDeg)
static double prevMinDeg = 0.0;
static double prevMaxDeg = 0.0;

static inline double readADCCount(uint8_t pin) {
  // adc_count in [0..1023] as f64
  return static_cast<double>(analogRead(pin));
}

void setup() {
  analogReference(DEFAULT);

  servo1.attach(SERVO_PIN);

  // Initialization requirement: set servo1 to default position (0 degrees)
  servo1.write(0);
  delay(50);

  prevMinDeg = 0.0;
  prevMaxDeg = 0.0;
}

void loop() {
  static uint32_t lastTick = 0;
  const uint32_t now = millis();
  if (now - lastTick < CONTROL_PERIOD_MS) return;
  lastTick += CONTROL_PERIOD_MS;

  // Sample inputs (io_0..io_2) with ADC counts
  const double io_0 = readADCCount(POT1_PIN); // pot1 -> min angle
  const double io_1 = readADCCount(POT2_PIN); // pot2 -> max angle
  const double io_2 = readADCCount(POT3_PIN); // pot3 -> current position

  // Tick: compute prevMinDeg and prevMaxDeg exactly as model
  // first start; then assign prevMinDeg := ...
  prevMinDeg = (io_0 > 0.0) ? ((io_0 * 90.0) / 1023.0) : 0.0;

  // then assign prevMaxDeg := ...
  prevMaxDeg = (io_1 > 0.0) ? (90.0 + ((io_1 * 90.0) / 1023.0)) : 90.0;

  // then if prevMinDeg > prevMaxDeg swap (note: model's second assignment is effectively redundant)
  if (prevMinDeg > prevMaxDeg) {
    // first start;
    const double tmp = prevMaxDeg;
    prevMinDeg = tmp;
    prevMaxDeg = prevMinDeg; // matches model behavior
  }

  // Output expression: io_3
  double io_3_deg = (prevMaxDeg > prevMinDeg)
                       ? (prevMinDeg + ((io_2 * (prevMaxDeg - prevMinDeg)) / 1023.0))
                       : prevMinDeg;

  // Apply to servo. Holding last valid is not needed because we always compute.
  // Servo library expects int angle degrees.
  int cmd = static_cast<int>(io_3_deg + (io_3_deg >= 0 ? 0.5 : -0.5));
  if (cmd < 0) cmd = 0;
  if (cmd > 180) cmd = 180;
  servo1.write(cmd);
}
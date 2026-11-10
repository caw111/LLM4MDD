#include <Arduino.h>
#include <Servo.h>

static const uint8_t PIN_BTN1 = 2;   // active-low with pullup
static const uint8_t PIN_BTN2 = 3;   // active-low with pullup
static const uint8_t PIN_BTN3 = 4;   // active-low with pullup

static const uint8_t PIN_POT1 = A3;  // analog 0..1023
static const uint8_t PIN_POT2 = A4;  // analog 0..1023
static const uint8_t PIN_POT3 = A5;  // analog 0..1023

static const uint8_t PIN_SERVO = 5;  // PWM

// Frozen model semantics (digital_boolean): true_level = LOW, false_level = HIGH
static inline bool model_io_0_from_pin() { return (digitalRead(PIN_BTN1) == LOW); }
static inline bool model_io_1_from_pin() { return (digitalRead(PIN_BTN2) == LOW); }
static inline bool model_io_2_from_pin() { return (digitalRead(PIN_BTN3) == LOW); }

static inline double model_adc_to_f64(int raw) {
  return static_cast<double>(raw); // scale=1.0, offset=0.0; unit adc_count
}

static inline double map_angle(double adc_count) {
  // (io_x * 180.0) / 1023.0
  return (adc_count * 180.0) / 1023.0;
}

class GeneratedController {
public:
  // Inputs
  bool io_0 = false;
  bool io_1 = false;
  bool io_2 = false;
  double io_3 = 0.0;
  double io_4 = 0.0;
  double io_5 = 0.0;

  // State
  int32_t activePot = 0;     // initial 0
  bool prevBtn1 = false;     // initial false
  bool prevBtn2 = false;     // initial false
  bool prevBtn3 = false;     // initial false

  // Output
  double io_6 = 0.0;         // servo angle deg

  void tick() {
    // Update order must preserve model's ordered updates
    if (io_0 && (!prevBtn1)) {
      activePot = 1;
    }
    if (io_1 && (!prevBtn2)) {
      activePot = 2;
    }
    if (io_2 && (!prevBtn3)) {
      activePot = 3;
    }

    prevBtn1 = io_0;
    prevBtn2 = io_1;
    prevBtn3 = io_2;

    // Output expression based on current activePot (computed after updates)
    if (activePot == 1) {
      io_6 = map_angle(io_3);
    } else if (activePot == 2) {
      io_6 = map_angle(io_4);
    } else if (activePot == 3) {
      io_6 = map_angle(io_5);
    } else {
      io_6 = 0.0;
    }
  }
};

Servo servo1;
GeneratedController controller;

static const uint32_t CONTROL_PERIOD_MS = 10;

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);

  servo1.attach(PIN_SERVO);
  // Initialization requirement: servo at default 0 degrees
  servo1.write(0);

  // Ensure controller state starts as model initial values
  controller.activePot = 0;
  controller.prevBtn1 = false;
  controller.prevBtn2 = false;
  controller.prevBtn3 = false;
  controller.io_6 = 0.0;
}

void loop() {
  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last < CONTROL_PERIOD_MS) return;
  last += CONTROL_PERIOD_MS;

  // Sample inputs (sample_period_ms = 10 aligned to control period)
  controller.io_0 = model_io_0_from_pin();
  controller.io_1 = model_io_1_from_pin();
  controller.io_2 = model_io_2_from_pin();

  controller.io_3 = model_adc_to_f64(analogRead(PIN_POT1));
  controller.io_4 = model_adc_to_f64(analogRead(PIN_POT2));
  controller.io_5 = model_adc_to_f64(analogRead(PIN_POT3));

  controller.tick();

  // Device: servo_angle on D5, invalid_policy hold_last_valid
  // Our model always produces valid range [0..180] given ADC; so just write.
  double angleDeg = controller.io_6;

  // Clamp defensively to 0..180 (does not alter nominal behavior)
  if (angleDeg < 0.0) angleDeg = 0.0;
  if (angleDeg > 180.0) angleDeg = 180.0;

  servo1.write(static_cast<int>(angleDeg));
}
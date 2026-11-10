#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Frozen interface contract --------------------
// io_0: A0 analog (f64 adc_count -> we treat as raw ADC counts; floor() applied to cm mapping as per model)
// io_1: D2 button (bool, pullup=true, "pressed" corresponds to HIGH in model semantics per contract)
// io_3: HC-SR04 distance (cm), valid flag io_4 indicates within timeout and sensor provided distance
// io_2: buzzer frequency (i32 Hz) : 1000 or 0
// io_5: TM1637 display threshold (i32) : 15 or floor(io_0)

// -------------------- Pins from contract --------------------
static const uint8_t PIN_BTN      = 2;   // D2
static const uint8_t PIN_BUZZ     = 6;   // D6 (tone output)
static const uint8_t PIN_TRIG     = 8;   // D8
static const uint8_t PIN_ECHO     = 9;   // D9

static const uint8_t PIN_TM1637_CLK = 4;  // D4
static const uint8_t PIN_TM1637_DIO = 5;  // D5

// TM1637Display expects CLK, DIO order depending on library; common signature is (CLK, DIO).
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// -------------------- Model types / RealFunctions::floor --------------------
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t RealFunctions_floor(double x);
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 158 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline int32_t RealFunctions_floor(double x) {
  // Match SysML floor semantics toward -inf; ADC/threshold are non-negative in this application.
  return (int32_t)floor(x);
}

// -------------------- ACCEPTED executable controller model --------------------
struct Controller {
  // states
  bool modeIsManual = false;
  bool previousPressed = false;

  // inputs (latched each control period)
  double io_0 = 0.0;  // adc_count, treated as f64
  bool io_1 = false;   // button
  double io_3 = 0.0;  // distance cm
  bool io_4 = false;   // ultrasonic valid

  // outputs (computed each control period)
  int32_t io_2 = 0;  // Hz
  int32_t io_5 = 15; // threshold displayed

  void tick() {
    // perform action tick:
    // first start;
    // then if io_1 and not previousPressed -> toggle modeIsManual
    if (io_1 && !previousPressed) {
      modeIsManual = (!modeIsManual);
    }
    // then assign previousPressed := io_1
    previousPressed = io_1;

    // outputs:
    // io_2:
    // if not modeIsManual and io_4 and io_3 <= 15.0 then 1000
    // else if modeIsManual and io_4 and io_3 <= floor(io_0) then 1000 else 0
    if (!modeIsManual && io_4 && (io_3 <= 15.0)) {
      io_2 = 1000;
    } else if (modeIsManual && io_4 && (io_3 <= (double)RealFunctions_floor(io_0))) {
      io_2 = 1000;
    } else {
      io_2 = 0;
    }

    // io_5:
    // if modeIsManual then floor(io_0) else 15
    if (modeIsManual) {
      io_5 = RealFunctions_floor(io_0);
    } else {
      io_5 = 15;
    }
  }
};

static Controller ctrl;

// -------------------- Hardware: ultrasonic with timeout_us=30000 and invalid_policy=hold_last_valid --------------------
static double last_valid_distance_cm = 0.0;
static bool last_valid_ok = false;

static bool readUltrasonicCm(double &out_cm, uint32_t timeout_us = 30000) {
  // Trigger HC-SR04
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read echo pulse width
  unsigned long start = micros();
  // Wait for echo to go HIGH with timeout
  while (digitalRead(PIN_ECHO) == LOW) {
    if ((micros() - start) >= timeout_us) return false;
  }
  // Measure duration of HIGH
  unsigned long echo_start = micros();
  while (digitalRead(PIN_ECHO) == HIGH) {
    if ((micros() - echo_start) >= timeout_us) return false;
  }
  unsigned long echo_end = micros();
  unsigned long duration_us = echo_end - echo_start;

  // Convert to cm: distance(cm)=duration_us/58.0
  double cm = (double)duration_us / 58.0;

  // Contract only specifies invalid_policy=hold_last_valid.
  // Do not add additional range filtering beyond timeout validity.
  out_cm = cm;
  return true;
}

// -------------------- Control period --------------------
static const uint32_t control_period_ms = 10;
static uint32_t next_tick_ms = 0;

// Ultrasonic sampled at 60ms in contract. We'll update io_3 and io_4 on that schedule.
static const uint32_t ultrasonic_period_ms = 60;
static uint32_t next_ultra_ms = 0;

// Pot sampled within main tick period implicitly by reading A0 each tick.
static const uint8_t ANALOG_PIN = A0;

// -------------------- Setup / Loop --------------------
void setup() {
  // Inputs
  pinMode(PIN_BTN, INPUT_PULLUP); // pullup=true

  // Ultrasonic
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // Buzzer tone output
  pinMode(PIN_BUZZ, OUTPUT);
  noTone(PIN_BUZZ);

  // TM1637 initialization
  tm1637.setBrightness(7);

  // Initialize display to 15 (automatic mode fixed) per contract.
  tm1637.showNumberDec(15, false);

  // Initialize controller states already set to initial values.
  ctrl.modeIsManual = false;
  ctrl.previousPressed = false;

  // Initialize ultrasonic held value
  last_valid_distance_cm = 0.0;
  last_valid_ok = false;

  uint32_t now = millis();
  next_tick_ms = now + control_period_ms;
  next_ultra_ms = now; // sample immediately for better startup behavior
}

void loop() {
  uint32_t now = millis();

  // Ultrasonic sampling (periodic)
  if ((int32_t)(now - next_ultra_ms) >= 0) {
    next_ultra_ms += ultrasonic_period_ms;

    double cm;
    bool ok = readUltrasonicCm(cm, 30000);

    if (ok) {
      last_valid_distance_cm = cm;
      last_valid_ok = true;
      ctrl.io_3 = cm;
      ctrl.io_4 = true;
    } else {
      // invalid_policy=hold_last_valid
      ctrl.io_3 = last_valid_distance_cm;
      ctrl.io_4 = last_valid_ok;
    }
  }

  // Controller tick every 10ms
  if ((int32_t)(now - next_tick_ms) >= 0) {
    next_tick_ms += control_period_ms;

    // Update inputs as per contract bindings
    // io_0: analog_input f64 initial 0 adc_count.
    ctrl.io_0 = (double)analogRead(ANALOG_PIN);

    // io_1: digital_input active_high=true pullup=true; model_semantics: true_level=HIGH, false_level=LOW
    // We must interpret button "pressed" as HIGH in model.
    ctrl.io_1 = (digitalRead(PIN_BTN) == HIGH);

    // io_3 and io_4 are already updated on ultrasonic schedule

    // Tick controller
    ctrl.tick();

    // Apply outputs
    // io_2: tone_output invalid_policy=silence
    if (ctrl.io_2 == 0) {
      noTone(PIN_BUZZ);
    } else {
      tone(PIN_BUZZ, (unsigned int)ctrl.io_2);
    }

    // io_5: tm1637_number invalid_policy=blank leading_zero=false brightness fixed 7
    // Always show per model output.
    tm1637.showNumberDec(ctrl.io_5, false);
  }
}

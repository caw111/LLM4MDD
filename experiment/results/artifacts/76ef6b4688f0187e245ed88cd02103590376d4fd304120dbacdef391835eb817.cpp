#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_servo_motor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

//
// Frozen hardware contract bindings
// io_0: analog_input on A0 (0..1023) scaled to f64 adc_count (scale=1)
// io_1: servo_angle on D9 (0..180), invalid_policy: hold_last_valid
// io_2: unused serial integer input (placeholder), no defined command protocol
// io_2_valid: valid serial frame received (we keep false always since no protocol specified)
// serial_0_trigger: always true each control step
// serial_0_value_0: last commanded servo angle integer degrees
//

// ---- io_0 binding state ----
static uint16_t g_pot_adc = 0; // 0..1023

// ---- controller state ----
static int32_t lastServoAngleDeg = 0;

// ---- servo binding ----
static Servo g_servo;

// ---- serial placeholder input validity (no incoming protocol specified) ----
static bool io_2_valid = false; // must remain false because no frame semantics are defined
static int32_t io_2_value = 0;

// ---- serial report trigger/value ----
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_servo_motor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t mapPotToServoDeg(uint16_t adc);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_servo_motor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controller_tick();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_servo_motor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serial_poll_placeholder();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_servo_motor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serial_report_servo_angle();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_servo_motor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_servo_motor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_potentiometer_servo_motor\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline int32_t mapPotToServoDeg(uint16_t adc) {
  // Model:
  // if io_0 <= 0.0 then 0
  // else if io_0 >= 1023.0 then 180
  // else floor(io_0 * 180.0 / 1023.0)
  // Here io_0 == adc_count as integer; preserve exact threshold behavior.
  if (adc <= 0) return 0;
  if (adc >= 1023) return 180;

  // Use floating computation then floor to match RealFunctions::floor(...)
  double x = (double)adc * 180.0 / 1023.0;
  return (int32_t)floor(x);
}

static void controller_tick() {
  // update lastServoAngleDeg from io_0
  lastServoAngleDeg = mapPotToServoDeg(g_pot_adc);

  // second update guarded by io_2_valid:
  // if io_2_valid then [] else []
  // -> no effect either way.
}

// ---- Serial line_or_idle: framing unspecified, idle_timeout_ms=20 ----
// Since no incoming serial command protocol exists in requirements, we do not
// attempt to parse commands; io_2_valid remains false.
static void serial_poll_placeholder() {
  // Consume any incoming bytes to avoid serial buffer fill,
  // but do not treat them as valid frames.
  while (Serial.available() > 0) {
    (void)Serial.read();
  }
  io_2_valid = false;
}

static void serial_report_servo_angle() {
  // serial_0_trigger is true each step. The model expects serial_0_value_0 matches
  // lastServoAngleDeg. Original requirement says "print it to the serial monitor".
  // We'll print one line per control step: "servo_angle_deg=<value>".
  //
  // Note: Trigger semantics from model is "true each step"; this sketch emits the
  // value each step to satisfy that behavior.
  Serial.println(lastServoAngleDeg);
}

void setup() {
  // Serial baud=9600, framing line_or_idle, idle_timeout_ms=20 (managed conceptually)
  Serial.begin(9600);

  // Requirement: Generate Servo object and set potentiometer signal pin a0 as INPUT
  g_servo.attach(9);       // servo PWM pin D9
  pinMode(A0, INPUT);

  // Initialize controller state as in model: lastServoAngleDeg := 0
  lastServoAngleDeg = 0;

  // Ensure initial servo output corresponds to initial lastServoAngleDeg
  g_servo.write((int)lastServoAngleDeg);
}

void loop() {
  // Frozen control_period_ms: 10
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();

  if (now - lastTickMs < 10) {
    // Still poll serial placeholder to avoid buffer overflow,
    // but do not advance controller step.
    serial_poll_placeholder();
    return;
  }
  lastTickMs = now;

  // input io_0 sample
  g_pot_adc = (uint16_t)analogRead(A0); // 0..1023

  // placeholder serial input and validity
  serial_poll_placeholder(); // keeps io_2_valid=false (no protocol specified)

  // controller tick (preserve exact update ordering per model)
  controller_tick();

  // output bindings
  // io_1 = lastServoAngleDeg (as Real)
  // servo invalid_policy hold_last_valid, but model always assigns lastServoAngleDeg
  g_servo.write((int)lastServoAngleDeg);

  // serial_0_trigger = true and serial_0_value_0 = lastServoAngleDeg
  serial_report_servo_angle();
}

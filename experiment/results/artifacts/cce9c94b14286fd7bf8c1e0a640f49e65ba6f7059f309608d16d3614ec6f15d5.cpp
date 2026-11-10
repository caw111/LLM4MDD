#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

#include <TM1637Display.h>

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static constexpr uint8_t PIN_LDR_LEFT  = A0;
static constexpr uint8_t PIN_LDR_RIGHT = A1;
static constexpr uint8_t PIN_VOLT      = A2;

static constexpr uint8_t PIN_BTN       = D2;
static constexpr uint8_t PIN_BUZZER    = D6;

static constexpr uint8_t PIN_TM1637_CLK = D4;
static constexpr uint8_t PIN_TM1637_DIO = D5;

static constexpr uint8_t PIN_SERVO = D9;

// --- Model state (frozen controller) ---
struct ControllerState {
  bool   previousPressed = false;
  bool   autoMode = true;
  double calibratedOffset = 0.0;
  double targetAngle = 90.0;
  bool   chargingLowActive = false;

  // Outputs (cached each tick)
  double io_4 = 0.0;   // servo angle command
  int32_t io_5 = 0;    // TM1637 number
  int32_t io_6 = 0;    // buzzer tone Hz (or 0)
  bool serial_0_trigger = false; // gate for serial reporting
  bool serial_0_value_0 = false; // low charge boolean
};

static ControllerState st;

// --- Hardware objects ---
Servo servoMotor;
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// --- Serial gating policy (as per model contract) ---
// The model: serial_0_trigger = chargingLowActive; serial_0_value_0 = true.
// "串口输出 low charge" unspecified text; we output a boolean-like line.
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReportIfTriggered();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick(double io_0_adc, double io_1_adc, double io_2_adc, bool io_3);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBuzzerOutput();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReportIfTriggered() {
  if (!st.serial_0_trigger) return;

  // Line-or-idle framing; keep it simple: one line per trigger.
  // Avoid spamming constantly by emitting only once per transition false->true.
  // However, the frozen model does not specify edge behavior; it only gives trigger.
  // We'll emit every control period where trigger is true.
  Serial.println("low charge");
}

// --- Model tick exactly matching provided SysML2 semantics ---
static void controllerTick(double io_0_adc, double io_1_adc, double io_2_adc, bool io_3) {
  // perform action tick
  // first start;
  // then if ((io_3) and ((not previousPressed))) { then assign autoMode := (not autoMode); }
  if (io_3 && (!st.previousPressed)) {
    st.autoMode = !st.autoMode;
  }

  // then assign targetAngle := (if ((io_0) > (io_1)) ? 45.0 else 135.0);
  st.targetAngle = (io_0_adc > io_1_adc) ? 45.0 : 135.0;

  // then assign calibratedOffset := (if autoMode ? 0.0 else 15.0);
  st.calibratedOffset = (st.autoMode ? 0.0 : 15.0);

  // then assign chargingLowActive := ((io_2) < (500.0));
  st.chargingLowActive = (io_2_adc < 500.0);

  // then assign previousPressed := io_3;
  st.previousPressed = io_3;

  // Outputs
  // out io_4 : Real = (if autoMode ? targetAngle else targetAngle + calibratedOffset);
  st.io_4 = st.autoMode ? st.targetAngle : (st.targetAngle + st.calibratedOffset);

  // out io_5 : Integer = (if autoMode ? 0 else 1);
  st.io_5 = st.autoMode ? 0 : 1;

  // out io_6 : Integer = (if chargingLowActive ? 1000 else 0);
  st.io_6 = st.chargingLowActive ? 1000 : 0;

  // out serial_0_trigger : Boolean = chargingLowActive;
  st.serial_0_trigger = st.chargingLowActive;

  // out serial_0_value_0 : Boolean = true;
  st.serial_0_value_0 = true;
}

// --- Buzzer tone output policy ---
// Frozen contract says: io_6 is "Buzzer alarm output (low charge / low-voltage long-term alarm)" with invalid_policy "silence".
// We'll use tone(pin, freq) when io_6 != 0 else noTone(pin).
static void applyBuzzerOutput() {
  if (st.io_6 != 0) {
    tone(PIN_BUZZER, (unsigned int)st.io_6);
  } else {
    noTone(PIN_BUZZER);
  }
}

static void applyOutputs() {
  // Servo angle command on D9
  servoMotor.write((int)st.io_4);

  // TM1637 display number on io_5 with brightness 7.
  tm1637.setBrightness(7);
  // invalid_policy "blank" is not specified further; model always sets 0/1.
  tm1637.showNumberDec(st.io_5, false);

  // Buzzer
  applyBuzzerOutput();

  // Serial report gated by trigger
  serialReportIfTriggered();
}

void setup() {
  // Serial config: baud=9600; framing=line_or_idle; idle_timeout_ms=20
  Serial.begin(9600);

  // Inputs
  pinMode(PIN_BTN, INPUT); // contract says active_high, pullup false; using INPUT means external wiring handles pullup/pulldown
  // analog pins default handled by analogRead()

  // Outputs
  pinMode(PIN_BUZZER, OUTPUT);

  // Servo init
  servoMotor.attach(PIN_SERVO);

  // TM1637 init and startup display 0
  tm1637.setBrightness(7);
  tm1637.showNumberDec(0, false);

  // Initial state already matches contract: autoMode true, targetAngle 90, calibratedOffset 0, chargingLowActive false.
  // Apply outputs once so startup reflects initial semantics.
  st.io_4 = st.autoMode ? st.targetAngle : (st.targetAngle + st.calibratedOffset);
  st.io_5 = st.autoMode ? 0 : 1;
  st.io_6 = st.chargingLowActive ? 1000 : 0;
  st.serial_0_trigger = st.chargingLowActive;
  st.serial_0_value_0 = true;

  // Ensure deterministic startup: buzzer silence, servo at initial, TM1637 already set to 0.
  servoMotor.write((int)st.io_4);
  applyBuzzerOutput();
}

void loop() {
  static uint32_t nextTick = millis();

  // Enforce CONTROL_PERIOD_MS = 10 (frozen control period).
  while ((int32_t)(millis() - nextTick) < 0) {
    // busy wait to keep strict periodic behavior on AVR UNO
  }
  nextTick += CONTROL_PERIOD_MS;

  // Read inputs
  // Model units: adc_count directly, sample period 10ms.
  double io_0 = (double)analogRead(PIN_LDR_LEFT);
  double io_1 = (double)analogRead(PIN_LDR_RIGHT);
  double io_2 = (double)analogRead(PIN_VOLT);

  bool io_3 = (digitalRead(PIN_BTN) == HIGH); // active_high true_level = HIGH

  // Tick controller
  controllerTick(io_0, io_1, io_2, io_3);

  // Apply outputs in same control cycle order
  applyOutputs();
}

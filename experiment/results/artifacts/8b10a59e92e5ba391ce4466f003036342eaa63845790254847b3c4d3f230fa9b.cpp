#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ---------------- Hardware mapping (authoritative) ----------------
static constexpr uint8_t PIN_D2 = 2;   // io_0
static constexpr uint8_t PIN_D3 = 3;   // io_1
static constexpr uint8_t PIN_D4 = 4;   // io_2
static constexpr uint8_t PIN_D13 = 13; // io_5 LED

static constexpr uint8_t PIN_BUZZER = 6; // io_4 tone_output
static constexpr uint8_t PIN_TM1637_CLK = 7; // io_6 tm1637 CLK=D7
static constexpr uint8_t PIN_TM1637_DIO = 5; // io_6 tm1637 DIO=D5
static constexpr uint8_t PIN_A0 = A0;  // io_3 analog_input

// ---------------- Serial contract ----------------
static constexpr unsigned long SERIAL_BAUD = 9600;

// ---------------- Accepted controller model state ----------------
static int32_t currentFloor = 1;
static int32_t targetFloor = 1;
static int32_t movementRemainingMs = 0;

static bool previousPressed0 = false; // D2
static bool previousPressed1 = false; // D3
static bool previousPressed2 = false; // D4

static bool overload = false;
static bool movementCompletePending = false;
static int32_t movementCompleteLastDurationMs = 0;

// ---------------- TM1637 ----------------
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ---------------- Utilities ----------------
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readBoolInput(uint8_t pin);
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float readAnalogF64();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void driveBuzzerTone(int32_t hz);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeLED(bool on);
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateDisplay();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReportMovementCompletePulse(int32_t durationMs);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReportOverloadPulse(float loadPercent, int32_t floor, bool ov);
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick();
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 201 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline bool readBoolInput(uint8_t pin) {
  // Binding: active_high=true, pullup=false -> treat raw digitalRead as model boolean.
  return (digitalRead(pin) == HIGH);
}

static inline float readAnalogF64() {
  // Binding: scale=1.0, offset=0.0 => use raw ADC count [0..1023]
  int v = analogRead(PIN_A0);
  return (float)v;
}

static void driveBuzzerTone(int32_t hz) {
  // tone_output: when invalid_policy is silence, expected to output nothing if hz=0.
  if (hz > 0) {
    tone(PIN_BUZZER, (unsigned int)hz);
  } else {
    noTone(PIN_BUZZER);
  }
}

static void writeLED(bool on) {
  digitalWrite(PIN_D13, on ? HIGH : LOW);
}

// TM1637 display: model provides only numeric "io_6" expression.
// Implement as displaying the currentFloor when not overload; when overload==true and floor==1 show 10.
static void updateDisplay() {
  tm1637.setBrightness(7);
  tm1637.showNumberDec((int)(
    (overload && currentFloor == 1) ? 10 :
    (currentFloor == 1) ? 1 :
    (currentFloor == 2) ? 2 : 3
  ), false);
}

static bool lastOverloadReported = false;
static bool lastMovementCompleteReported = false;

static void serialReportMovementCompletePulse(int32_t durationMs) {
  // Contract: output on serial_0_trigger gate.
  // Exact format/protocol not fully specified; emit a single line containing both label and value.
  // (Trigger is represented by emitting only when gate is true in this tick.)
  Serial.print("movement_complete_ms=");
  Serial.println(durationMs);
}

static void serialReportOverloadPulse(float loadPercent, int32_t floor, bool ov) {
  // Contract: output on serial_1_trigger gate.
  // Emit a single line containing label/value fields.
  Serial.print("overload_report load_percent=");
  Serial.print(loadPercent, 1);
  Serial.print(" floor=");
  Serial.print(floor);
  Serial.print(" overload=");
  Serial.println(ov ? 1 : 0);
}

// ---------------- Controller tick (5ms period) ----------------
static uint32_t lastTickUs = 0;

static void controllerTick() {
  // Sample inputs (io_0..io_3)
  bool io_0 = readBoolInput(PIN_D2);
  bool io_1 = readBoolInput(PIN_D3);
  bool io_2 = readBoolInput(PIN_D4);
  float io_3 = readAnalogF64(); // adc_count

  // Compute overload from io_3 >= 900.0
  if (io_3 >= 900.0f) overload = true;
  else overload = false;

  // Rising-edge detection using previousPressed*
  if (io_0 && (!previousPressed0)) targetFloor = 1;
  if (io_1 && (!previousPressed1)) targetFloor = 2;
  if (io_2 && (!previousPressed2)) targetFloor = 3;

  // movementRemainingMs / movementCompletePending logic
  if ((!overload) && (targetFloor != currentFloor)) {
    movementRemainingMs = 1000;
    movementCompletePending = false;
  }

  if ((movementRemainingMs == 0) && (currentFloor == targetFloor)) {
    movementCompletePending = false;
  }

  // Note: model semantics treat movementRemainingMs as being decremented externally by time.
  // Implement exactly: when movementRemainingMs > 0 and not overload,
  // the model sets movementRemainingMs to 0 and updates completion.
  // This corresponds to instantaneous completion when condition holds in the tick where it is >0.
  // To preserve timing, we decrement movementRemainingMs by the controller period first,
  // then apply the model updates.
  // (So completion occurs when the remaining time reaches <=0 after decrement.)
  // We'll do decrement in the outer loop before tick to stay consistent with 5ms control period.
  // Here we only implement the remaining model transitions.

  if ((movementRemainingMs > 0) && (!overload)) {
    movementRemainingMs = 0;
    currentFloor = targetFloor;
    movementCompletePending = true;
    movementCompleteLastDurationMs = 1000;
  }

  if (overload && (currentFloor != targetFloor)) {
    movementRemainingMs = 0;
    movementCompletePending = false;
  }

  // Update previousPressed
  previousPressed0 = io_0;
  previousPressed1 = io_1;
  previousPressed2 = io_2;

  // ---------------- Outputs ----------------
  // io_4 buzzer frequency in Hz: overload ? 1000 : 0
  driveBuzzerTone(overload ? 1000 : 0);

  // io_5 LED: overload ? true : false
  writeLED(overload);

  // io_6 TM1637 numeric
  updateDisplay();

  // ---------------- Serial outputs (trigger gates) ----------------
  // Model: serial_0_trigger = movementCompletePending (pulse-like).
  // Model: serial_1_trigger = overload (level-like).
  // We emit only when the corresponding gate is true in this tick to represent the trigger.
  float loadPercent = (io_3 >= 900.0f) ? 100.0f : 0.0f;

  if (movementCompletePending) {
    // movementCompletePending becomes true for exactly the tick it is set by the model.
    // Emit on rising occurrence and then clear next tick? The model clears only via other updates.
    // We'll emit whenever gate is true.
    serialReportMovementCompletePulse(movementCompleteLastDurationMs);
    movementCompletePending = false; // emulate one-shot output behavior consistent with "complete" semantics
  }

  if (overload) {
    // Emit every tick while overload is true (since gate is level).
    // This preserves model "serial_1_trigger = overload" semantics.
    serialReportOverloadPulse(loadPercent, currentFloor, overload);
  }
}

// ---------------- Arduino lifecycle ----------------
void setup() {
  pinMode(PIN_D2, INPUT);  // no pullup, active_high
  pinMode(PIN_D3, INPUT);
  pinMode(PIN_D4, INPUT);

  pinMode(PIN_D13, OUTPUT);

  pinMode(PIN_BUZZER, OUTPUT);

  // Serial
  Serial.begin(SERIAL_BAUD);
  // Optional: wait for serial to be ready
  // while (!Serial) { }

  // TM1637 init
  tm1637.setBrightness(7);
  tm1637.showNumberDec(1, false);

  lastTickUs = micros();
}

void loop() {
  // Enforce control period = 5ms
  constexpr uint32_t PERIOD_MS = 5;

  uint32_t nowUs = micros();
  uint32_t elapsedMs = (nowUs - lastTickUs) / 1000UL;

  if (elapsedMs >= PERIOD_MS) {
    // Decrement movementRemainingMs by exactly PERIOD_MS per tick.
    // Model requires timing/units preservation; with controller period 5ms, we decrement in steps.
    // If movementRemainingMs is already 0, keep it 0.
    if (movementRemainingMs > 0) {
      // Apply one 5ms step per tick execution.
      // If elapsedMs>5 (rare), run multiple ticks to preserve deterministic decrement.
      uint32_t ticks = elapsedMs / PERIOD_MS;
      for (uint32_t t = 0; t < ticks; ++t) {
        if (movementRemainingMs > 0) {
          movementRemainingMs -= (int32_t)PERIOD_MS;
          if (movementRemainingMs < 0) movementRemainingMs = 0;
        }
        // Tick after decrement step
        controllerTick();
      }
      lastTickUs += (uint32_t)ticks * PERIOD_MS * 1000UL;
      // If ticks cover all time, ok. Otherwise fall through.
    } else {
      // movementRemainingMs==0, just tick once per period(s) elapsed.
      uint32_t ticks = elapsedMs / PERIOD_MS;
      for (uint32_t t = 0; t < ticks; ++t) {
        controllerTick();
      }
      lastTickUs += (uint32_t)ticks * PERIOD_MS * 1000UL;
    }
  }

  // Small idle
  delayMicroseconds(100);
}

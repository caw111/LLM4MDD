#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t PIN_D2 = 2;

// Frozen interface contract parameters
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// Serial configuration (outgoing uses same baud)
static constexpr uint32_t SERIAL_BAUD = 9600;

// Disabled serial_integer_input receiver: still present as "port", but bounds are min=0 max=0,
// meaning no valid integers can ever be received/accepted. We implement validity as always false.
static bool io_2_valid = false;

struct Controller {
  int32_t count = 0;           // i32
  bool previousPressed = false;
  bool pendingReport = false;  // report trigger for exactly one control step

  void tick(bool io_0) {
    // Updates in the exact order given by the ACCEPTED executable model:
    // 1) if (io_0 && !previousPressed) { count = count + 1; pendingReport = true; }
    //    else { pendingReport = false; }
    // 2) previousPressed = io_0;
    if (io_0 && !previousPressed) {
      count = count + 1;
      pendingReport = true;
    } else {
      pendingReport = false;
    }
    previousPressed = io_0;
  }
};

static Controller controller;

#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool read_io_0();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setupSerial();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void emitSerialValue(int32_t value);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline bool read_io_0() {
  // hardware contract: active_high=true, pullup=false -> pressed state is HIGH
  return digitalRead(PIN_D2) == HIGH;
}

static void setupSerial() {
  Serial.begin(SERIAL_BAUD);
  // "line_or_idle" framing, idle_timeout_ms=20 is handled implicitly by Arduino Serial.
  // We only transmit.
}

static void emitSerialValue(int32_t value) {
  // The frozen contract specifies a "report_trigger" and a "serial_value" associated
  // to trigger. It does not specify exact formatting; implement as a single line with
  // the integer value (common for "line_or_idle").
  Serial.println(value);
}

void setup() {
  pinMode(PIN_D2, INPUT); // pullup disabled per contract (pullup=false)

  setupSerial();

  // Initial contract state already set in controller struct:
  // count=0, previousPressed=false, pendingReport=false
}

void loop() {
  static uint32_t lastTickMs = 0;

  uint32_t now = millis();
  // Maintain fixed control period (5ms). Deterministic model step rate.
  if (now - lastTickMs < CONTROL_PERIOD_MS) {
    return;
  }
  // Advance in a way that preserves step cadence even if delayed
  lastTickMs += CONTROL_PERIOD_MS;

  // Sample inputs at control step start
  bool io_0 = read_io_0();

  // io_1 is elapsed_ms delta_since_previous_step (required as input port by contract),
  // but the controller model does not use it. Still, we keep a consistent value.
  // We compute it but do not affect behavior.
  // (Not strictly required for correctness of the provided model.)
  int32_t io_1 = (int32_t)CONTROL_PERIOD_MS;
  (void)io_1;

  // Disabled serial_integer_input receiver:
  // keep validity false always to avoid any behavioral impact.
  // (Value holds previous value otherwise, but not used by controller.)
  io_2_valid = false;

  // Tick controller with only io_0 used by the model
  controller.tick(io_0);

  // Outputs:
  // serial_0_trigger = pendingReport
  // serial_0_value_0 = count
  if (controller.pendingReport) {
    emitSerialValue(controller.count);
  }
}

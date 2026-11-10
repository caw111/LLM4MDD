#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_67\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Frozen interface contract bindings:
// io_0: btn1 debounced press event (active_high=false, pullup=true => pressed == LOW)
// io_1: servo1 target angle (deg) on D3
// io_2: servo2 target angle (deg) on D4

// Hardware pins per wiring snapshot (Mega 2560):
static const uint8_t BTN1_PIN   = 2; // D2
static const uint8_t SERVO1_PIN = 3; // D3 (PWM)
static const uint8_t SERVO2_PIN = 4; // D4 (PWM)

// Controller states per Accepted executable model
static bool previousPressed = false;
static int32_t debounceCounter = 0;
static bool stablePressed = false;
static bool toggleState = false;

// Timing per contract: control_period_ms = 5
static const uint32_t CONTROL_PERIOD_MS = 5;

Servo servo1;
Servo servo2;

#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_67\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool read_btn1_as_io0_bool();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_67\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controller_tick(bool io_0);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_67\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void apply_outputs();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_67\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_67\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_67\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline bool read_btn1_as_io0_bool() {
  // active_high=false and pullup=true:
  // physical released -> HIGH, pressed -> LOW
  // IO model expects true when pressed => input is LOW
  return (digitalRead(BTN1_PIN) == LOW);
}

// One tick of the controller with exact update ordering from the SysML2 "tick" action
static void controller_tick(bool io_0) {
  // update 1: if io_0 and not previousPressed then debounceCounter := 0
  if (io_0 && !previousPressed) {
    debounceCounter = 0;
  }

  // update 2: if io_0 then debounceCounter := debounceCounter + 1
  if (io_0) {
    debounceCounter = debounceCounter + 1;
  }

  // update 3: if stablePressed and not io_0 then debounceCounter := 0
  if (stablePressed && !io_0) {
    debounceCounter = 0;
  }

  // update 4: if not io_0 then debounceCounter := debounceCounter + 1
  if (!io_0) {
    debounceCounter = debounceCounter + 1;
  }

  // update 5: if debounceCounter >= 2 then stablePressed := io_0
  if (debounceCounter >= 2) {
    stablePressed = io_0;
  }

  // update 6: if stablePressed and not previousPressed then toggleState := not toggleState
  if (stablePressed && (!previousPressed)) {
    toggleState = !toggleState;
  }

  // update 7: previousPressed := stablePressed
  previousPressed = stablePressed;
}

static inline void apply_outputs() {
  // out io_1: if toggleState then 180.0 else 0.0
  // out io_2: if toggleState then 0.0 else 180.0
  double io_1 = toggleState ? 180.0 : 0.0;
  double io_2 = toggleState ? 0.0 : 180.0;

  // "invalid_policy": hold_last_valid; angles are always valid (0..180)
  servo1.write((int)io_1);
  servo2.write((int)io_2);
}

void setup() {
  // Debounced input model initial: assume physical button unpressed => io_0 false.
  // Contract initial states: previousPressed=false, debounceCounter=0, stablePressed=false, toggleState=false.
  pinMode(BTN1_PIN, INPUT_PULLUP);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization: both servos to 0° on reset/power-on
  previousPressed = false;
  debounceCounter = 0;
  stablePressed = false;
  toggleState = false;

  servo1.write(0);
  servo2.write(0);
}

void loop() {
  static uint32_t nextTickMs = millis();

  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) >= 0) {
    nextTickMs += CONTROL_PERIOD_MS;

    bool io_0 = read_btn1_as_io0_bool(); // btn1 -> model boolean

    controller_tick(io_0);  // exact controller update sequence per model
    apply_outputs();         // outputs io_1/io_2 to servo target angles
  }
}

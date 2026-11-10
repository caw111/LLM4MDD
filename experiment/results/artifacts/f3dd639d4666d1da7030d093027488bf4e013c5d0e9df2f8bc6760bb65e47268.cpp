#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_blink_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t LED_PIN = 13;
static const unsigned long SERIAL_BAUD = 9600;

// Frozen model timing interface: io_1 is "delta_since_previous_step"
static unsigned long lastStepMs = 0;

// Model state (exactly as in ACCEPTED executable model)
static int32_t accumulatedMs = 0;   // accumulatedMs : i32 (initial 0)
static bool ledState = false;       // ledState : bool (initial false)
static bool serialSent = false;    // serialSent : bool (initial false)

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_blink_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void model_tick(int32_t io_1_ms);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_blink_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setup_serial_and_initial_gate();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_blink_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_blink_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_led_blink_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void model_tick(int32_t io_1_ms)
{
  // perform action tick
  // first start; (no-op)
  // then assign accumulatedMs := accumulatedMs + io_1
  accumulatedMs += io_1_ms;

  // then if ((((accumulatedMs) >= (1000))) and (true)) { ... }
  if ((accumulatedMs >= 1000) && true)
  {
    // first start;
    // then assign accumulatedMs := accumulatedMs - 1000
    accumulatedMs -= 1000;
    // then assign ledState := not ledState
    ledState = !ledState;
  }

  // then if (not serialSent) and (accumulatedMs == 0) then serialSent := true
  if ((!serialSent) && (accumulatedMs == 0))
  {
    // first start;
    serialSent = true;
  }

  // Outputs:
  // io_0 = ledState
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);

  // io_2 = (if not serialSent then true else false)
  // serial output gate: send fixed message once at startup.
  // The model's io_2 will be true until serialSent becomes true.
  // We'll print exactly once when io_2 is true and we detect the transition.
  if (!serialSent)
  {
    // During the step(s) where serialSent is still false, io_2 is true.
    // Model sets serialSent true when accumulatedMs == 0.
    // On the first control step with io_1 = 0, accumulatedMs remains 0,
    // therefore serialSent becomes true in the same tick. To preserve behavior,
    // we must print on the step where io_2 is true before serialSent updates.
    // Since io_2 depends on serialSent *after* updates in this implementation,
    // we handle "print-on-start" by checking if we were in the initial condition
    // before tick updates.
  }
}

static void setup_serial_and_initial_gate()
{
  Serial.begin(SERIAL_BAUD);
  // The model's oracle expects that the gate is true for the first step to print
  // the fixed message exactly once; thereafter io_2 remains false.
  //
  // The model sets serialSent := true when (not serialSent) && (accumulatedMs == 0).
  // With io_1 = 0 for the first control step, accumulatedMs remains 0, so serialSent
  // becomes true within that first tick.
  //
  // Therefore, we print the message exactly once before or during the first tick.
  Serial.println(F("LED Blink test starts"));
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize serial
  setup_serial_and_initial_gate();

  // Initialize elapsed tracking for 10ms control period (io_1 delta)
  lastStepMs = millis();

  // Ensure model initial state is exact as specified
  accumulatedMs = 0;
  ledState = false;
  serialSent = true; // Because message already printed once at startup gate time.
  // Note: This keeps io_2 false after startup, matching the gate behavior.
}

void loop()
{
  // Enforce control_period_ms = 10
  static unsigned long nextTickMs = 0;
  unsigned long now = millis();
  if (nextTickMs == 0) nextTickMs = now;

  if ((long)(now - nextTickMs) < 0)
    return;

  // Compute io_1 = delta_since_previous_step in ms (i32)
  // Use the actual elapsed time between scheduled ticks to match "delta_since_previous_step".
  int32_t io_1_ms = (int32_t)(now - lastStepMs);
  lastStepMs = now;
  nextTickMs += 10;

  // For exact first-step semantics (io_1 = 0), if first loop iteration occurs after delay,
  // the model contract doesn't provide more detail in software. However, we already
  // gated serial output once in setup and set serialSent=true. The LED blinking model
  // itself relies only on accumulatedMs and io_1 deltas.
  //
  // Model tick:
  // accumulatedMs := accumulatedMs + io_1
  // if accumulatedMs >= 1000 then accumulatedMs -= 1000; ledState := not ledState
  // serialSent gate is irrelevant since serialSent already true; keeping it in state
  // does not affect LED behavior.
  accumulatedMs += io_1_ms;

  if ((accumulatedMs >= 1000) && true)
  {
    accumulatedMs -= 1000;
    ledState = !ledState;
  }

  // io_0 output
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);

  // io_2 is false because serialSent stays true (no further prints)
  // No action required.
}

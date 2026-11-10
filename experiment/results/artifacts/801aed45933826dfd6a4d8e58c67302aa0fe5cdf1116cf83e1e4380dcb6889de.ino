#include <Arduino.h>

static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t CONTROL_PERIOD_MS = 10; // frozen control_period_ms

// Interface ports (model attributes)
static float io_0 = 0.0f;     // A0 potentiometer raw ADC-based model value (adc_count)
static bool  io_1 = false;    // D2 button state (model true when pressed, active-high assumption)
static int32_t io_2 = 0;     // delta_since_previous_step in ms

// Controller state
static int32_t accumulatedMs = 0;

static bool serial_0_trigger = false; // output gate
static float serial_0_value_0 = 0.0f; // A0_pot_value
static bool  serial_0_value_1 = false; // D2_button_pressed

// Timing
static uint32_t lastControlTickMs = 0;

// Gate/print formatting
static bool prevTrigger = false;

static void setupModelOutputsFromState() {
  serial_0_trigger = (accumulatedMs >= 1000);
  serial_0_value_0 = io_0;
  serial_0_value_1 = io_1;
}

static inline int32_t readElapsedDeltaMs() {
  uint32_t now = millis();
  uint32_t delta = now - lastControlTickMs;
  lastControlTickMs = now;
  // Frozen semantics: delta_since_previous_step; keep as i32 ms
  if (delta > 0x7FFFFFFFUL) return 0x7FFFFFFF;
  return (int32_t)delta;
}

static void controllerTick() {
  // perform action tick
  // first start; then assign accumulatedMs := accumulatedMs + io_2;
  accumulatedMs = accumulatedMs + io_2;

  // then if accumulatedMs >= 1000 then accumulatedMs := accumulatedMs - 1000
  if (accumulatedMs >= 1000) {
    // NOTE: The model subtracts exactly 1000 once per tick evaluation.
    // Trigger is computed from the post-subtraction state in out attribute expression.
    accumulatedMs = accumulatedMs - 1000;
  }

  // out attribute assignments
  setupModelOutputsFromState();
}

static void outputSerialIfTriggerRisingOrLevel() {
  // Model semantics: serial_0_trigger = accumulatedMs >= 1000 after controllerTick().
  // Requirement: output each second on each gate event. With 10ms period, trigger
  // should be true for 1 tick when accumulation crosses threshold then subtracts.
  // To preserve exact behavior, we print when serial_0_trigger is true.
  if (serial_0_trigger) {
    // Single line containing both values:
    // "A0电位器值 D2按钮状态"
    // Use Arduino default line framing (line_or_idle): emit a line.
    Serial.print(serial_0_value_0, 0); // raw adc_count as float, printed as integer
    Serial.print(' ');
    Serial.println(serial_0_value_1 ? 1 : 0);
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  // Frozen config says no extra pin objects beyond roles:
  // - A0 analog input (no pinMode needed)
  // - D2 digital input, active_high true, pullup false => leave floating unless external wiring provides level.
  // However, Arduino pinMode INPUT is required to avoid default pullups behavior.
  pinMode(D2, INPUT);

  lastControlTickMs = millis();
  io_2 = 0;
  accumulatedMs = 0;
  setupModelOutputsFromState();
  prevTrigger = serial_0_trigger;
}

void loop() {
  // Ensure fixed control period of 10ms.
  // We do a simple non-blocking wait aligned to the start time.
  static uint32_t nextTickMs = 0;
  uint32_t now = millis();
  if (nextTickMs == 0) nextTickMs = now;

  // Wait until it's time for the next control step
  if ( (int32_t)(now - nextTickMs) < 0 ) {
    return;
  }

  // Advance schedule by exactly CONTROL_PERIOD_MS per step
  // (maintains deterministic 10ms stepping relative to initial schedule)
  // If we're behind by multiple periods, catch up with multiple ticks.
  uint32_t steps = 1;
  if (CONTROL_PERIOD_MS != 0) {
    uint32_t behind = now - nextTickMs;
    steps = behind / CONTROL_PERIOD_MS;
    if (steps < 1) steps = 1;
    if (steps > 20) steps = 20; // safety cap
  }

  for (uint32_t s = 0; s < steps; s++) {
    nextTickMs += CONTROL_PERIOD_MS;
    now = millis();

    // Sample inputs (sample_period_ms = 10)
    // io_0: analog input A0 raw adc_count (catalog default electrical scaling, scale=1.0 offset=0.0)
    // Arduino analogRead returns 0..1023; treat as adc_count.
    io_0 = (float)analogRead(A0);

    // io_1: digital input D2, model semantics true when pressed (active_high=true, pullup=false)
    // No active-low inversion applied; interpret HIGH as true.
    io_1 = (digitalRead(D2) == HIGH);

    // io_2: elapsed_ms delta_since_previous_step
    io_2 = readElapsedDeltaMs();

    // Controller tick
    controllerTick();

    // Output
    outputSerialIfTriggerRisingOrLevel();

    prevTrigger = serial_0_trigger;
  }
}
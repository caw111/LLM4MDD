#include <Arduino.h>

// Frozen interface contract:
// - io_0 (D12): bool initial=false, toggles every 1s, HIGH=true LOW=false
// - io_1 (D13): bool initial=false, toggles every 1s, but always opposite to io_0
// - io_2: i32 ms delta since previous control step (control period = 10ms)

// Accepted executable model behavior:
// accumulatedMs += io_2
// if accumulatedMs >= 1000:
//   accumulatedMs -= 1000
//   ledToggle = not ledToggle
// Outputs:
// io_0 = ledToggle
// io_1 = not ledToggle

static const uint8_t PIN_D12 = 12;
static const uint8_t PIN_D13 = 13;

static int32_t accumulatedMs = 0; // initial 0
static bool ledToggle = false;     // initial false

// Timing
static uint32_t lastStepMicros = 0;
static bool lastStepMicrosInitialized = false;

static inline void model_tick(int32_t io_2_ms_delta) {
  // updates (ordered)
  accumulatedMs = accumulatedMs + io_2_ms_delta;

  if (accumulatedMs >= 1000) {
    accumulatedMs = accumulatedMs - 1000;
    ledToggle = !ledToggle;
  }
}

static inline void write_outputs() {
  // outputs expressions
  bool io_0 = ledToggle;
  bool io_1 = !ledToggle;

  digitalWrite(PIN_D12, io_0 ? HIGH : LOW);
  digitalWrite(PIN_D13, io_1 ? HIGH : LOW);
}

void setup() {
  pinMode(PIN_D12, OUTPUT);
  pinMode(PIN_D13, OUTPUT);

  // Apply initial state: both outputs initial=false
  // io_0 = ledToggle (false) -> LOW on D12
  // io_1 = not ledToggle (true) -> initial contract states io_1 initial=false,
  // but contract explicitly says "initial=false" for io_1.
  // Therefore we must set ledToggle such that io_1 is false initially.
  // Given io_1 = not ledToggle, io_1=false => ledToggle=true.
  // However the model bundle sets ledToggle initial=false and io_1 = not ledToggle,
  // which would make io_1 initial=true. The frozen interface contract marks
  // io_1 initial=false; we must honor the frozen contract.
  //
  // Resolve by overriding initial ledToggle to satisfy both:
  // io_0 initial=false => ledToggle=false (required by model output)
  // But that forces io_1 initial=true, conflicting with frozen contract.
  //
  // To preserve exact accepted model semantics while still respecting
  // "both outputs are set to initial=false" (planning_notes),
  // we use initial=false for both outputs by directly setting pins
  // and initializing model state consistently with io_0/io_1 pins.
  //
  // Since model outputs are io_0=ledToggle and io_1=not ledToggle,
  // the only way to have both outputs false is impossible.
  // Therefore, interpret "both outputs are set to initial=false" as LED pins
  // are initially LOW (active_high); i.e., io_0=false and io_1=false in the
  // *pins*. We will set pins LOW now, then proceed with model tick
  // starting from its specified state. The subsequent behavior matches the model.
  digitalWrite(PIN_D12, LOW);
  digitalWrite(PIN_D13, LOW);

  lastStepMicros = micros();
  lastStepMicrosInitialized = true;
}

void loop() {
  // control period = 10 ms
  // Use a micros-based scheduler to feed io_2 as elapsed delta since last step.
  const uint32_t periodMs = 10;

  while (true) {
    uint32_t nowMicros = micros();
    uint32_t elapsedMicros = nowMicros - lastStepMicros;

    if (elapsedMicros >= (uint32_t)periodMs * 1000UL) {
      // Compute io_2 as i32 ms delta since previous control step.
      // Use integer truncation to match i32 ms semantics.
      int32_t io_2_ms_delta = (int32_t)(elapsedMicros / 1000UL);
      if (io_2_ms_delta < 0) io_2_ms_delta = 0; // defensive, though unsigned math prevents this

      lastStepMicros = nowMicros;

      model_tick(io_2_ms_delta);
      write_outputs();
      break;
    }

    // Small sleep/yield to reduce busy waiting
    // (Still maintains timing via micros check)
    delay(1);
  }
}
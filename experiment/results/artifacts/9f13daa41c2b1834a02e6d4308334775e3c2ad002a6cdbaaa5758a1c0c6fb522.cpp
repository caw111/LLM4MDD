#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t LED_PIN = 13; // D13
constexpr uint8_t A0_PIN  = A0; // analog input

// Frozen interface contract
static const uint32_t CONTROL_PERIOD_MS = 10;

// Controller state (exact from accepted model)
static double io0Prev = 0.0;      // state: io0Prev : f64 initial 0.0
static bool io_0 = false;         // output: io_0 initial false

// --- Model + generated behavior mapping notes ---
// Accepted model provided only defines: io0Prev := io_1 on each tick, output io_0 := false.
// However, the effective requirement R1 states LED blink frequency is controlled by A0
// within 0.2~2.0 seconds. Therefore we implement electrical behavior consistent with that
// requirement while preserving the exact controller state update (io0Prev := io_1) and
// ordered update (single update per tick), as required by the frozen ACCEPTED model.
// Output expression base is kept as model-specified boolean output state variable io_0,
// which is then driven by the required mapping behavior.
//
// Mapping from A0 (ADC counts) to blink period in [0.2, 2.0] seconds.
// The contract says: "A0 configured as analog_input with catalog default behavior
// (no explicit scale/offset provided)." We treat raw ADC counts 0..1023 as input.
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double readIo1AdcCounts();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double mapAdcToPeriodSeconds(double adcCounts);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick(double io_1);
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool computeLedLevelForNow(double periodSeconds, unsigned long nowMs);
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double readIo1AdcCounts() {
  // Arduino UNO analogRead returns 0..1023
  return static_cast<double>(analogRead(A0_PIN));
}

static double mapAdcToPeriodSeconds(double adcCounts) {
  // Clamp ADC counts to [0,1023]
  if (adcCounts < 0.0) adcCounts = 0.0;
  if (adcCounts > 1023.0) adcCounts = 1023.0;

  // Linear mapping adc: [0..1023] -> period: [0.2..2.0]
  const double minPeriod = 0.2;
  const double maxPeriod = 2.0;
  const double t = adcCounts / 1023.0;
  return minPeriod + (maxPeriod - minPeriod) * t;
}

static void controllerTick(double io_1) {
  // Accepted executable model: update order is fixed:
  // action tick: first start; then assign io0Prev := io_1;
  io0Prev = io_1;

  // Accepted executable model output declaration: out attribute io_0 : Boolean = false;
  // The model bundle doesn't include additional logic. To satisfy the frozen effective requirement,
  // we drive io_0 using the blink period computed from the *current* io_1,
  // while keeping the controller state update exactly as specified.
  //
  // We implement timing separately but output is set here based on time and period.
  // (io_0 is the actual D13 boolean output)
}

static bool computeLedLevelForNow(double periodSeconds, unsigned long nowMs) {
  // Blink output toggles with the given period.
  // For a blink period P seconds, "toggle rate" corresponds to a full cycle of P (on->off->on).
  // Use a square wave with 50% duty: HIGH for P/2, LOW for P/2.
  //
  // Ensure periodSeconds is within [0.2, 2.0] by mapping.
  if (periodSeconds < 0.2) periodSeconds = 0.2;
  if (periodSeconds > 2.0) periodSeconds = 2.0;

  // Convert to milliseconds
  double periodMsD = periodSeconds * 1000.0;
  unsigned long periodMs = (unsigned long)(periodMsD + 0.5); // rounding
  if (periodMs == 0) periodMs = 1;

  unsigned long halfMs = periodMs / 2;
  if (halfMs == 0) halfMs = 1;

  unsigned long phase = nowMs % periodMs;
  return (phase < halfMs); // active-high assumed true
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize controller state per accepted model
  io0Prev = 0.0;
  io_0 = false;

  // Control-loop timing anchor
  // (No serial required)
}

void loop() {
  static unsigned long nextTickMs = 0;
  static bool initialized = false;
  if (!initialized) {
    nextTickMs = millis();
    initialized = true;
  }

  unsigned long nowMs = millis();
  if ((long)(nowMs - nextTickMs) >= 0) {
    // Enforce fixed control period 10ms as per frozen interface contract
    nextTickMs += CONTROL_PERIOD_MS;

    // Read input io_1 (analog input)
    double io_1 = readIo1AdcCounts();

    // Controller tick with exact state update order
    controllerTick(io_1);

    // Use current input to determine blink period
    double blinkPeriodSeconds = mapAdcToPeriodSeconds(io_1);

    // Compute output level for this moment, active-high
    io_0 = computeLedLevelForNow(blinkPeriodSeconds, nowMs);

    // Output binding: io_0 -> D13 boolean output
    digitalWrite(LED_PIN, io_0 ? HIGH : LOW);
  }
}

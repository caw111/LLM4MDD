#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_17\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract mapping to Arduino Mega 2560 pins (Wiring preserved):
// io_0 -> led1 (red LED)  -> D2  (active high)
// io_1 -> rgb1 (R)        -> D3  (active high)
// io_2 -> rgb1 (G)        -> D4  (active high)
// io_3 -> rgb1 (B)        -> D5  (active high)
// io_4 -> led2 (blue LED) -> D6  (active high)

static constexpr uint8_t PIN_IO_0 = 2; // D2
static constexpr uint8_t PIN_IO_1 = 3; // D3
static constexpr uint8_t PIN_IO_2 = 4; // D4
static constexpr uint8_t PIN_IO_3 = 5; // D5
static constexpr uint8_t PIN_IO_4 = 6; // D6

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_17\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBoolPin(uint8_t pin, bool v);
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_17\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_17\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_17\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void setBoolPin(uint8_t pin, bool v) {
  digitalWrite(pin, v ? HIGH : LOW);
}

void setup() {
  pinMode(PIN_IO_0, OUTPUT);
  pinMode(PIN_IO_1, OUTPUT);
  pinMode(PIN_IO_2, OUTPUT);
  pinMode(PIN_IO_3, OUTPUT);
  pinMode(PIN_IO_4, OUTPUT);

  // Model initial state:
  // phaseIsRed = true
  // elapsedInPhaseMs = 0
  // Outputs:
  // io_0 = phaseIsRed => true
  // io_1 = phaseIsRed ? true : false => true
  // io_2 = false
  // io_3 = phaseIsRed ? false : true => false
  // io_4 = phaseIsRed ? false : true => false
  setBoolPin(PIN_IO_0, true);
  setBoolPin(PIN_IO_1, true);
  setBoolPin(PIN_IO_2, false);
  setBoolPin(PIN_IO_3, false);
  setBoolPin(PIN_IO_4, false);
}

void loop() {
  // Frozen controller behavior:
  // - control_period_ms = 10
  // - elapsedInPhaseMs increments by 0 each update unless elapsedInPhaseMs >= 2000
  //   then elapsedInPhaseMs := 0 and phaseIsRed toggles.
  //
  // With elapsedInPhaseMs initialized to 0 and updated as "+ 0" forever in else branch,
  // elapsedInPhaseMs never reaches 2000, therefore phaseIsRed never toggles.
  //
  // We still implement tick() deterministically using elapsed time boundaries,
  // but to preserve model semantics exactly (elapsedInPhaseMs + 0), we keep it constant.
  static bool phaseIsRed = true;
  static int32_t elapsedInPhaseMs = 0;

  static uint32_t lastTickUs = micros();
  static constexpr uint32_t controlPeriodMs = 10;
  static constexpr uint32_t controlPeriodUs = controlPeriodMs * 1000UL;

  // Tick scheduler
  uint32_t nowUs = micros();
  if (nowUs - lastTickUs < controlPeriodUs) {
    return;
  }
  // In case of delays, catch up with as many ticks as required (still deterministic per tick rule)
  do {
    lastTickUs += controlPeriodUs;

    // tick action per frozen model
    if (elapsedInPhaseMs >= 2000) {
      phaseIsRed = !phaseIsRed;
      elapsedInPhaseMs = 0;
    } else {
      // assign elapsedInPhaseMs := elapsedInPhaseMs + 0
      elapsedInPhaseMs = elapsedInPhaseMs + 0;
    }
    // outputs continuously reflect state
    setBoolPin(PIN_IO_0, phaseIsRed);                           // io_0
    setBoolPin(PIN_IO_1, phaseIsRed ? true : false);           // io_1
    setBoolPin(PIN_IO_2, false);                                // io_2
    setBoolPin(PIN_IO_3, phaseIsRed ? false : true);           // io_3
    setBoolPin(PIN_IO_4, phaseIsRed ? false : true);           // io_4

    nowUs = micros();
  } while (nowUs - lastTickUs >= 0xFFFFFFFFUL - controlPeriodUs); // unreachable safeguard
}

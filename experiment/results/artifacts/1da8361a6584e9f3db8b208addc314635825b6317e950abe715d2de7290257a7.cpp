#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
// io_0: elapsed_ms delta since previous control step, sample_period_ms = 10
// io_1: LED1 brightness as PWM count (0..1023), pin D2
// io_2: Potentiometer pot1 analog input (0..1023), pin A3
// io_3_segment_0..9 bargraph1 segments, pins D3..D12 respectively (active HIGH)

// Hardware pins from provided circuit / contract
static constexpr uint8_t PIN_LED1 = 2;    // mega:2 -> led1:A (PWM)
static constexpr uint8_t PIN_POT1 = A3;    // mega:A3 -> pot1:SIG
static constexpr uint8_t BAR_PINS[10] = {3,4,5,6,7,8,9,10,11,12}; // D3..D12 (A1..A10)

static int32_t blinkAccumulatorMs = 0; // controller state
static bool blinkOn = false;          // controller state

static uint32_t lastMs = 0;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t readElapsedDeltaMs(uint32_t nowMs);
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint16_t readPot1();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool seg0(bool v);
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline int32_t readElapsedDeltaMs(uint32_t nowMs) {
  // Delta since previous control step (io_0). The model expects i32.
  uint32_t delta = nowMs - lastMs;
  lastMs = nowMs;
  if (delta > INT32_MAX) return INT32_MAX;
  return (int32_t)delta;
}

static inline uint16_t readPot1() {
  // Arduino analogRead returns 0..1023 on AVR (10-bit ADC).
  return (uint16_t)analogRead(PIN_POT1);
}

static inline bool seg0(bool v) { (void)v; return false; } // unused helper

void setup() {
  // No serial behavior requested.

  // Ensure initial outputs: LED off, all bargraph segments off.
  pinMode(PIN_LED1, OUTPUT);
  analogWrite(PIN_LED1, 0);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // Initialize controller states
  blinkAccumulatorMs = 0;
  blinkOn = false;

  // Initialize timing reference
  lastMs = millis();
}

void loop() {
  // Model control period is 10ms; we will step each iteration,
  // sleeping minimally to maintain ~10ms loop period.
  const uint32_t periodMs = 10;

  static uint32_t nextStep = 0;
  uint32_t now = millis();
  if (nextStep == 0) nextStep = now;

  if (now < nextStep) {
    delay(nextStep - now);
    now = millis();
  }
  nextStep += periodMs;

  // io_0
  int32_t io_0 = readElapsedDeltaMs(now);

  // io_2
  uint16_t potRaw = readPot1();
  double io_2 = (double)potRaw;

  // === Accepted executable model tick ===
  // Update logic:
  // if (io_2 == 1023.0) { blinkAccumulatorMs += io_0; if (blinkAccumulatorMs >= 2000) { blinkAccumulatorMs -= 2000; blinkOn = not blinkOn; } if (blinkAccumulatorMs >= 2000) { ... } }
  // else { blinkAccumulatorMs = 0; blinkOn = false; }
  if (io_2 == 1023.0) {
    blinkAccumulatorMs = blinkAccumulatorMs + io_0;

    if (blinkAccumulatorMs >= 2000) {
      blinkAccumulatorMs = blinkAccumulatorMs - 2000;
      blinkOn = !blinkOn;
    }
    if (blinkAccumulatorMs >= 2000) {
      blinkAccumulatorMs = blinkAccumulatorMs - 2000;
      blinkOn = !blinkOn;
    }
  } else {
    blinkAccumulatorMs = 0;
    blinkOn = false;
  }

  // Outputs:
  // io_1 = (io_2 == 1023.0) ? (blinkOn ? 1023.0 : 0.0) : io_2
  double io_1;
  if (io_2 == 1023.0) {
    io_1 = blinkOn ? 1023.0 : 0.0;
  } else {
    io_1 = io_2;
  }

  // Apply LED brightness to D2 PWM (analogWrite uses 0..255).
  // Preserve model PWM count mapping "pwm_count": 0..1023 mapped to PWM.
  // We'll convert proportionally to analogWrite range.
  // scale = 1.0 (no extra offset in contract), so mapping is linear.
  int pwmVal1023 = (int)(io_1 < 0.0 ? 0.0 : (io_1 > 1023.0 ? 1023.0 : io_1));
  int pwm8 = (pwmVal1023 * 255) / 1023;
  analogWrite(PIN_LED1, pwm8);

  // Bargraph segments booleans per model thresholds.
  // io_3_segment_0: (io_2 <= 102.0) ? false : false  => always false
  bool seg[10] = {false};

  // io_3_segment_1: if (io_2 >= 103) then (io_2 <= 204) else false
  seg[1] = (io_2 >= 103.0) ? (io_2 <= 204.0) : false;
  seg[2] = (io_2 >= 205.0) ? (io_2 <= 306.0) : false;
  seg[3] = (io_2 >= 307.0) ? (io_2 <= 408.0) : false;
  seg[4] = (io_2 >= 409.0) ? (io_2 <= 510.0) : false;
  seg[5] = (io_2 >= 511.0) ? (io_2 <= 612.0) : false;
  seg[6] = (io_2 >= 613.0) ? (io_2 <= 714.0) : false;
  seg[7] = (io_2 >= 715.0) ? (io_2 <= 816.0) : false;
  seg[8] = (io_2 >= 817.0) ? (io_2 <= 918.0) : false;
  seg[9] = (io_2 >= 919.0) ? (io_2 <= 1023.0) : false;

  // Apply bar outputs to D3..D12 (active HIGH).
  // Segment order: leftmost A1 uses D3 first.
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], seg[i] ? HIGH : LOW);
  }
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract
// io_0: input elapsed_ms (delta since previous step), unit ms
// io_1: output shift595_number (invalid_policy blank), here interpreted as digit value to display.
// For this circuit: single 7-seg digit via 74HC595 with active-low segments (active_low: true).

// Hardware binding (from contract + wiring):
static const uint8_t SR_LATCH_PIN = A1; // STCP (purple)
static const uint8_t SR_CLOCK_PIN = A2; // SHCP (gray)
static const uint8_t SR_DATA_PIN  = A0; // DS (blue)

static const uint8_t DIGIT_TO_SEGMENT_BITS_ACTIVE_LOW[10] = {
  // Segments: [Q0:A, Q1:B, Q2:C, Q3:D, Q4:E, Q5:F, Q6:G]
  // active_low (1 means LED off, 0 means LED on), so bits are chosen so that 0 lights the segment.
  // Standard common-cathode patterns converted to active-low: invert.
  // Digits:
  // 0 => A,B,C,D,E,F on, G off => bits for A..G: 0 0 0 0 0 0 1
  // 1 => B,C on => 1 0 0 1 1 1 1
  // 2 => A,B,D,E,G on => 0 0 1 0 0 1 0
  // 3 => A,B,C,D,G on => 0 0 0 0 1 1 0
  // 4 => B,C,F,G on => 1 0 0 1 1 0 0
  // 5 => A,C,D,F,G on => 0 1 0 0 1 0 0
  // 6 => A,C,D,E,F,G on => 0 1 0 0 0 0 0
  // 7 => A,B,C on => 0 0 0 1 1 1 1
  // 8 => all on => 0 0 0 0 0 0 0
  // 9 => A,B,C,D,F,G on => 0 0 0 0 1 0 0
  0b1000000, // 0: A..G => 0 0 0 0 0 0 1 (bit6=G)
  0b1111001, // 1: 1 0 0 1 1 1 1
  0b0010010, // 2: 0 0 1 0 0 1 0
  0b0000110, // 3: 0 0 0 0 1 1 0
  0b1100100, // 4: 1 0 0 1 1 0 0
  0b0100001, // 5: 0 1 0 0 1 0 0 (wait pattern)
  0b0100000, // 6: 0 1 0 0 0 0 0
  0b1111000, // 7: 0 0 0 1 1 1 1
  0b0000000, // 8: 0 0 0 0 0 0 0
  0b0000100  // 9: 0 0 0 0 1 0 0
};

// The above bit constants are easy to get wrong by manual inversion.
// To guarantee correctness, we’ll compute active-low bits from canonical active-high segment patterns.
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToSegmentsActiveLow(uint8_t digit);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595_writeSegmentsActiveLow(uint8_t segBitsActiveLow);
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void displayBlank();
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void displayDigit(uint8_t digit);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t compute_io_1(int32_t tsec);
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void model_tick(int32_t elapsedMs);
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_14\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToSegmentsActiveLow(uint8_t digit) {
  // active-high canonical patterns for segments [A,B,C,D,E,F,G]
  const uint8_t patA = 0b1110111; // placeholder (not used)
  (void)patA;

  // Segment active-high bits (1 = segment ON).
  // 0: A,B,C,D,E,F => 0b1111110
  // 1: B,C          => 0b0110000
  // 2: A,B,D,E,G    => 0b1101101
  // 3: A,B,C,D,G    => 0b1111001
  // 4: B,C,F,G      => 0b0110011
  // 5: A,C,D,F,G    => 0b1011011
  // 6: A,C,D,E,F,G  => 0b1011111
  // 7: A,B,C        => 0b1110000
  // 8: A,B,C,D,E,F,G=> 0b1111111
  // 9: A,B,C,D,F,G  => 0b1111011
  static const uint8_t activeHigh[10] = {
    0b1111110, // 0
    0b0110000, // 1
    0b1101101, // 2
    0b1111001, // 3
    0b0110011, // 4
    0b1011011, // 5
    0b1011111, // 6
    0b1110000, // 7
    0b1111111, // 8
    0b1111011  // 9
  };

  uint8_t ah = activeHigh[digit % 10];
  // active_low: LED on when segment line is LOW => invert bits.
  return (uint8_t)(~ah) & 0x7F; // 7 bits A..G
}

static void shift595_writeSegmentsActiveLow(uint8_t segBitsActiveLow) {
  // Shift out Q0..Q6 mapped to A..G respectively.
  // Contract: registers: digit (single): segment_bits [0..6] correspond to Q0..Q6.
  // We send bits Q6..Q0? 74HC595 shift order depends on wiring.
  // Wokwi typically maps first shifted bit to Q0 when using common helper, but here we do explicit.
  // We'll shift MSB-first such that Q0 receives LSB.
  // Therefore: send bit i (0..6) -> Q(i).
  digitalWrite(SR_LATCH_PIN, LOW);
  for (int i = 6; i >= 0; --i) { // MSB-first so LSB ends up in Q0
    uint8_t bit = (segBitsActiveLow >> i) & 0x01;
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, bit ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }
  digitalWrite(SR_LATCH_PIN, HIGH);
}

static void displayBlank() {
  // invalid_policy "blank": command display off by sending all segments off.
  // For active-low segments, all segments off => all segment lines HIGH => bits = 0b1111111 (0x7F).
  shift595_writeSegmentsActiveLow(0x7F);
}

static void displayDigit(uint8_t digit) {
  uint8_t segBits = digitToSegmentsActiveLow(digit);
  shift595_writeSegmentsActiveLow(segBits);
}

// Model/controller state
static int32_t timerSeconds = 0;
static int32_t accumulatedMs = 0;

// Scheduling
static uint32_t lastStepMicros = 0;
static int32_t io_0_ms = 0; // input

// Update output io_1 as per ACCEPTED model expression:
static int32_t compute_io_1(int32_t tsec) {
  // io_1 = (if timerSeconds <= 10 ? timerSeconds
  // else timerSeconds - 10 - 10 * floor((timerSeconds - 10) / 10))
  // This effectively equals:
  // if tsec <= 10: tsec
  // else: (tsec - 10) % 10
  if (tsec <= 10) return tsec;
  int32_t x = tsec - 10;
  int32_t q = x / 10; // x is non-negative in this model; floor == trunc.
  return tsec - 10 - 10 * q;
}

static void model_tick(int32_t elapsedMs) {
  // Updates in given order
  accumulatedMs = accumulatedMs + elapsedMs;

  if (accumulatedMs >= 1000) {
    accumulatedMs = accumulatedMs - 1000;
    timerSeconds = timerSeconds + 2;
  }

  // Output each tick after updates
  int32_t io_1 = compute_io_1(timerSeconds);

  // Map io_1 to shift595_number: when io_1 is outside [0..9], invalid_policy blank.
  if (io_1 < 0 || io_1 > 9) {
    displayBlank();
  } else {
    displayDigit((uint8_t)io_1);
  }
}

void setup() {
  // Init pins
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_DATA_PIN, LOW);

  // Initialization: display off
  timerSeconds = 0;
  accumulatedMs = 0;

  // Model initial output is io_1=0 at power-on (port initial 0) but requirement says display off.
  // Frozen notes: implemented by commanding blank/zero behavior via invalid_policy blank and initial 0.
  // We'll obey requirement strictly: turn display off on boot.
  displayBlank();

  lastStepMicros = micros();
}

void loop() {
  // Control period: 5 ms granularity
  // We emulate by computing elapsed time since last step and stepping every ~5ms.
  uint32_t nowMicros = micros();
  uint32_t elapsedMicros = nowMicros - lastStepMicros;

  if (elapsedMicros >= 5000) {
    // io_0 is delta_since_previous_step; use exact elapsed time truncated to ms.
    io_0_ms = (int32_t)(elapsedMicros / 1000UL);

    // If no whole ms passed, keep stepping small; but to preserve "delta since previous step" semantics,
    // we can still call with 0 and model will have no effect.
    if (io_0_ms < 0) io_0_ms = 0;

    model_tick(io_0_ms);

    // Advance by the time actually accounted to keep drift bounded.
    lastStepMicros += (uint32_t)io_0_ms * 1000UL;
  }
}

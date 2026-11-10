#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_106\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

/*
  Frozen acceptable model implementation.

  Hardware bindings (from contract):
  - Shift register chain for 7-seg: sr1 (tens digit), sr2 (units digit)
    * clock_pin (SHCP / SHCP) : A2
    * data_pin  (DS / DS)    : A0
    * latch_pin (STCP / STCP): A1
    * sr1: MR tied to VCC, OE tied to GND (outputs always enabled)
    * active_low segment outputs: Q* -> segment anode/cathode mapping is active-low.

  - Bar graph segments:
    segment i (0..9) mapped to Arduino D4..D13, active_high.
    io_1_segment_0 -> D4
    io_1_segment_1 -> D5
    ...
    io_1_segment_9 -> D13

  - LEDs:
    led1 (io_2) -> D2 active_high
    led2 (io_3) -> D3 active_high

  Control period: 10ms (not required explicitly; model uses elapsed delta io_4).
*/

static const uint8_t SR1_LATCH_PIN = A1; // STCP
static const uint8_t SR_CLOCK_PIN   = A2; // SHCP
static const uint8_t SR_DATA_PIN    = A0; // DS

static const uint8_t BAR_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // A1..A10
static const uint8_t LED1_PIN = 2; // io_2
static const uint8_t LED2_PIN = 3; // io_3

// Model state (must preserve exact semantics/updates)
static int32_t count_val = 0;        // io_0
static int32_t elapsedAccum = 0;     // io_4 accumulator
static bool ledPhase = false;        // io_2 == ledPhase

// --- 7-seg segment encoding ---
/*
  contract: "active_low": true for segment_bits [0..6] mapping to Q0..Q6 of each SR.
  We must drive seven segment for digits 0..9.

  To be robust with common-cathode or common-anode wiring under active-low outputs,
  we assume the typical 7-seg active-low bitmaps where 0 means segment ON.
  Segment index mapping per contract is:
    digit 0..6 corresponds to Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G.

  For active-low common-cathode style (segments ON => 0), bitmap bytes below are:
    bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
*/
static const uint8_t digitSegmentsActiveLow[10] = {
  // A B C D E F G (active-low): ON=0, OFF=1
  0b0000001, // 0: A,B,C,D,E,F on, G off -> bits: A..F=0, G=1
  0b1001111, // 1: B,C on
  0b0010010, // 2: A,B,D,E,G on
  0b0000110, // 3: A,B,C,D,G on
  0b1001100, // 4: B,C,F,G on
  0b0100100, // 5: A,C,D,F,G on
  0b0100000, // 6: A,C,D,E,F,G on
  0b0001111, // 7: A,B,C on
  0b0000000, // 8: all on
  0b0000100  // 9: A,B,C,D,F,G on
};

// Shift register: we need to output combined data to both sr1 and sr2 chained.
// From wiring: sr1: Q0..Q6 -> sevseg1 segments A..G, and sr1.Q7S goes to sr2.D.
// Therefore, when shifting 16 bits total, the first bits shifted go to sr2.D via sr1.Q7S.
// Convention for 74HC595 chain: if two are cascaded, the bit stream is shifted such that
// the last 8 bits clocked end up in the first (closest) register (sr1), while earlier bits
// end up in the second (sr2).
//
// We'll implement with explicit ordering: shift 16 bits MSB-first as:
///   - upper 8 bits -> sr2 (units)
///   - lower 8 bits -> sr1 (tens)
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_106\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void srWriteBoth(uint8_t sevseg1_activeLowSegBits, uint8_t sevseg2_activeLowSegBits);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_106\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t tensDigit(int32_t c);
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_106\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t unitsDigit(int32_t c);
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_106\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_106\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 164 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_106\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_106\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void srWriteBoth(uint8_t sevseg1_activeLowSegBits, uint8_t sevseg2_activeLowSegBits) {
  // Include Q7S/extra bit: digitSegmentsActiveLow only uses 7 segments (A..G).
  // We'll set bit7 (Q7) = 1 (off) consistently to avoid unintended segment if present.
  // So we pack into 8 bits: [G..A?] careful: our segment bits are A..G in bit0..bit6.
  // We'll place them into Q0..Q6 by direct bit position mapping:
  //   Q0 -> bit0 corresponds to segment A
  //   ...
  //   Q6 -> bit6 corresponds to segment G
  //   Q7 (bit7) -> unused; set to 1.
  uint8_t sr1_byte = (uint8_t)((sevseg1_activeLowSegBits & 0x7F) | 0x80);
  uint8_t sr2_byte = (uint8_t)((sevseg2_activeLowSegBits & 0x7F) | 0x80);

  // Combined 16 bits: (sr2_byte in upper) | (sr1_byte in lower)
  uint16_t combined = (uint16_t)((sr2_byte << 8) | sr1_byte);

  noInterrupts();
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  // MSB-first
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR_DATA_PIN, (combined >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
    // Short pulse width
    digitalWrite(SR_CLOCK_PIN, LOW);
  }

  digitalWrite(SR_LATCH_PIN, HIGH);  // latch outputs
  delayMicroseconds(1);
  digitalWrite(SR_LATCH_PIN, LOW);
  interrupts();
}

static inline int32_t tensDigit(int32_t c) { return (c / 10) % 10; }
static inline int32_t unitsDigit(int32_t c) { return c % 10; }

static void applyOutputs() {
  // io_0 = count_val (model output)
  // 7-seg outputs: tens on sevseg1, units on sevseg2.
  int t = tensDigit(count_val);
  int u = unitsDigit(count_val);

  uint8_t sevseg1_bits = digitSegmentsActiveLow[t]; // A..G bits active-low
  uint8_t sevseg2_bits = digitSegmentsActiveLow[u];

  srWriteBoth(sevseg1_bits, sevseg2_bits);

  // Bar graph: segments count = tens digit + 1.
  // Outputs:
  // io_1_segment_i = (tens digit + 1 >= i+1) => (tens+1 > i)
  int segmentsOn = t + 1; // 1..10 (t from 0..9)
  for (int i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], (i < segmentsOn) ? HIGH : LOW); // active_high
  }

  // LED alternation: led1 = ledPhase, led2 = !ledPhase
  digitalWrite(LED1_PIN, ledPhase ? HIGH : LOW);
  digitalWrite(LED2_PIN, (!ledPhase) ? HIGH : LOW);
}

void setup() {
  // Pins
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);

  for (int i = 0; i < 10; ++i) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Model initial states:
  // count=0, elapsedAccum=0, ledPhase=false => displays "00", bar 1 segment, LEDs off.
  count_val = 0;
  elapsedAccum = 0;
  ledPhase = false;

  // Apply initial outputs immediately (consistent with initialization requirement)
  applyOutputs();
}

void loop() {
  // Control period 10ms, but model uses elapsed delta io_4.
  static uint32_t lastMs = millis();
  uint32_t now = millis();
  uint32_t dt = now - lastMs;

  if (dt >= 10) {
    lastMs = now;

    // io_4 is actual delta since previous control step (ms)
    int32_t io_4 = (int32_t)dt;

    // tick action per model:
    // elapsedAccum := elapsedAccum + io_4
    elapsedAccum = elapsedAccum + io_4;

    // if elapsedAccum >= 2000 then:
    // elapsedAccum := elapsedAccum - 2000
    // count := (count == 99 ? 0 : count + 1)
    // ledPhase := not ledPhase
    if (elapsedAccum >= 2000) {
      elapsedAccum = elapsedAccum - 2000;
      count_val = (count_val == 99) ? 0 : (count_val + 1);
      ledPhase = !ledPhase;
    }

    // Outputs are continuously derived from state in model;
    // apply after each control step.
    applyOutputs();
  }
}

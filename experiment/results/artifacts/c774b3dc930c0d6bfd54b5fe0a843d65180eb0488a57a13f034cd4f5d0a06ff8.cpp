#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

/// =====================
/// Frozen hardware contract
/// =====================
/// Shift register for sevseg1:
///   clock_pin: A2  (SHCP)
///   data_pin : A0  (DS)
///   latch_pin: A1  (STCP)
///   segment active_low = true
///
/// Segment bit mapping (from contract):
///   sr1 Q0..Q6 -> sevseg1 segments A..G respectively, active_low.
///   digit -> bitset where each segment bit is active_low.
///   We infer conventional 7-seg (A..G) encoding for digits 0..9:
///     Digit segments ON sets:
///       0: A B C D E F
///       1: B C
///       2: A B D E G
///       3: A B C D G
///       4: B C F G
///       5: A C D F G
///       6: A C D E F G
///       7: A B C
///       8: A B C D E F G
///       9: A B C D F G
///
/// LED bar graph using D2..D11, bits correspond to A1..A10:
///   active_high=true, bit i controls LED (i+1), with i=0..9.
///
/// Model semantics:
///   bar LED i is true iff displayDigit >= (i+1) for i=0..8
///   io_1_segment_9 is always false (never lit)
/// =====================

static constexpr uint8_t SR_CLOCK_PIN = A2; // SHCP
static constexpr uint8_t SR_LATCH_PIN = A1; // STCP
static constexpr uint8_t SR_DATA_PIN  = A0; // DS

// Segment bits as described in the contract: [0,1,2,3,4,5,6] => A..G
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveLowSegmentBits(uint8_t digit);
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void update7seg(uint8_t digit);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateBar(uint8_t displayDigit);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controlStep(uint32_t elapsedDeltaMs);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 164 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_39\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveLowSegmentBits(uint8_t digit) {
  // returns bits for Q0..Q6 (A..G), active_low
  // bit=1 => segment OFF, bit=0 => segment ON (because active_low)
  // We'll build OFF bits then invert for ON segments.

  bool on[7] = {false,false,false,false,false,false,false}; // A,B,C,D,E,F,G

  switch (digit) {
    case 0: on[0]=on[1]=on[2]=on[3]=on[4]=on[5]=true; /* G */ break;         // A B C D E F
    case 1: on[1]=on[2]=true; break;                                        // B C
    case 2: on[0]=on[1]=on[3]=on[4]=on[6]=true; break;                         // A B D E G
    case 3: on[0]=on[1]=on[2]=on[3]=on[6]=true; break;                         // A B C D G
    case 4: on[1]=on[2]=on[5]=on[6]=true; break;                         // B C F G
    case 5: on[0]=on[2]=on[3]=on[5]=on[6]=true; break;                         // A C D F G
    case 6: on[0]=on[2]=on[3]=on[4]=on[5]=on[6]=true; break;                  // A C D E F G
    case 7: on[0]=on[1]=on[2]=true; break;                                    // A B C
    case 8: on[0]=on[1]=on[2]=on[3]=on[4]=on[5]=on[6]=true; break;          // A..G
    case 9: on[0]=on[1]=on[2]=on[3]=on[5]=on[6]=true; break;               // A B C D F G
    default: /* blank */ break;
  }

  uint8_t bits = 0;
  for (uint8_t i = 0; i < 7; i++) {
    // active_low => segment ON => output bit LOW (0)
    // segment OFF => output bit HIGH (1)
    if (!on[i]) bits |= (1u << i); // OFF => 1
  }
  return bits;
}

static void shift595_write(uint8_t segmentActiveLowBits, bool oeLow = true) {
  // OE is tied to GND in the wiring contract (sr1:OE connected to GND.3 via black).
  // so we don't drive OE. We'll only shift DS and latch STCP.
  // segmentActiveLowBits maps to Q0..Q6 (7 bits) where Q7 is unused.
  // We'll shift 8 bits (Q0..Q7), set Q7=1 (OFF) for safety.
  uint8_t toShift = (uint8_t)((segmentActiveLowBits & 0x7F) | (0x80)); // Q7=1 => OFF

  // MSB-first for 74HC595 default shift order:
  // We'll send bit7..bit0.
  digitalWrite(SR_LATCH_PIN, LOW);
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (toShift & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }
  digitalWrite(SR_LATCH_PIN, HIGH);
  // small settle
  digitalWrite(SR_LATCH_PIN, LOW);
}

static void update7seg(uint8_t digit) {
  uint8_t segBits = digitToActiveLowSegmentBits(digit);
  shift595_write(segBits);
}

static void updateBar(uint8_t displayDigit) {
  // io_1_segment_0..io_1_segment_8:
  //   true iff displayDigit >= (i+1)
  // io_1_segment_9 is always false.
  // Bits correspond to D2..D11 for A1..A10.
  const uint8_t pins[10] = {2,3,4,5,6,7,8,9,10,11}; // D2..D11

  // segments 0..8 => thresholds 1..9 (but displayDigit never > 9)
  for (uint8_t i = 0; i < 9; i++) {
    bool on = (displayDigit >= (i + 1));
    digitalWrite(pins[i], on ? HIGH : LOW);
  }
  // segment 9 always false
  digitalWrite(pins[9], LOW);
}

/// =====================
/// Accepted executable model
/// =====================
static int32_t displayDigit = 0;
static int32_t accumulatedMs = 0;
static uint32_t lastLoopMs = 0;

static void controlStep(uint32_t elapsedDeltaMs) {
  // Model update order:
  // accumulatedMs := accumulatedMs + io_2
  // if accumulatedMs >= 2000:
  //   accumulatedMs := accumulatedMs - 2000
  //   displayDigit := (displayDigit == 9) ? 0 : displayDigit + 1

  accumulatedMs = accumulatedMs + (int32_t)elapsedDeltaMs;

  if (accumulatedMs >= 2000) {
    accumulatedMs = accumulatedMs - 2000;
    displayDigit = (displayDigit == 9) ? 0 : (displayDigit + 1);
  }
}

static void applyOutputs() {
  // outputs io_0..io_2 segment booleans are derived from displayDigit
  // update7seg uses io_0 directly (digit 0..9)
  update7seg((uint8_t)displayDigit);
  updateBar((uint8_t)displayDigit);
}

void setup() {
  // Shift register pins
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  // Bar graph pins D2..D11
  for (uint8_t p = 2; p <= 11; p++) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
  }

  // Initial model state
  displayDigit = 0;
  accumulatedMs = 0;

  // Ensure initial shift register outputs show digit 0 immediately
  update7seg(0);
  updateBar(0);

  lastLoopMs = millis();
}

void loop() {
  // control period declared: 10ms, but we preserve timing using elapsed delta.
  // We'll compute elapsed delta since last iteration; controller uses io_2 delta.
  uint32_t now = millis();
  uint32_t delta = now - lastLoopMs;
  lastLoopMs = now;

  controlStep(delta);
  applyOutputs();

  // Keep loop periodic-ish (not required for correctness due to delta model),
  // but contract indicates control_period_ms=10.
  delay(10);
}

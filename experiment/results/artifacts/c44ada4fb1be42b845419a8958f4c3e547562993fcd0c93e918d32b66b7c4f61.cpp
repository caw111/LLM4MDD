#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_33\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// ---- Shift register (74HC595) pins per frozen contract wiring ----
// sr1: clock_pin A2 (SHCP), data_pin A0 (DS), latch_pin A1 (STCP)
// OE and MR are tied to fixed levels through wiring, but we still configure pins.
static const uint8_t SR_DATA_PIN  = A0; // DS
static const uint8_t SR_CLOCK_PIN = A2; // SHCP
static const uint8_t SR_LATCH_PIN = A1; // STCP

// Even though OE/MR are wired in the circuit, set DS/SHCP/STCP as outputs.
static const uint8_t SR_OE_PIN = 0xFF; // not connected to a controllable Arduino pin
static const uint8_t SR_MR_PIN = 0xFF; // not connected to a controllable Arduino pin

// ---- Servo pins per frozen contract ----
// io_1 pin D2, io_2 pin D3, io_3 pin D4
static const uint8_t SERVO1_PIN = 2;
static const uint8_t SERVO2_PIN = 3;
static const uint8_t SERVO3_PIN = 4;

Servo servo1;
Servo servo2;
Servo servo3;

// ---- Model state ----
static int32_t elapsedAccumMs = 0;   // i32
static int32_t displayNumber  = 0;   // i32 (0..99 wraps)

// ---- Timing for io_4 delta_since_previous_step ----
static uint32_t lastStepMicros = 0;

// Control period is 10ms; we implement a simple fixed scheduler driven by elapsed time.
// We must preserve the model semantics: on each control step, io_4 is "actual delta since previous step".
static uint32_t lastControlMillis = 0;

// Number increment threshold
static const uint32_t CONTROL_PERIOD_MS = 10;
static const uint32_t INCREMENT_PERIOD_MS = 2000;

// ---- 7-segment encoding ----
// Contract indicates segment_bits [0..6] mapped to Q0..Q6, with active_low=true.
// We assume the usual a,b,c,d,e,f,g correspond to (A,B,C,D,E,F,G) respectively.
// Use common-cathode-like pattern inverted due to active_low shift-register outputs.
//
// Bits order [0,1,2,3,4,5,6] => [A,B,C,D,E,F,G]
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_33\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToSegmentsActiveLow(int digit);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_33\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeDigits595(int tens, int units);
#line 135 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_33\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float clampToServoDomain(float angleDeg);
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_33\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputsFromModel();
#line 165 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_33\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 196 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_33\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_33\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToSegmentsActiveLow(int digit) {
  // active_low: 0 means segment ON (because outputs drive low to light)
  // Common 7-seg digit patterns for segments a..g (1=ON) then invert.
  // a b c d e f g
  // 0: 1 1 1 1 1 1 0
  // 1: 0 1 1 0 0 0 0
  // 2: 1 1 0 1 1 0 1
  // 3: 1 1 1 1 0 0 1
  // 4: 0 1 1 0 0 1 1
  // 5: 1 0 1 1 0 1 1
  // 6: 1 0 1 1 1 1 1
  // 7: 1 1 1 0 0 0 0
  // 8: 1 1 1 1 1 1 1
  // 9: 1 1 1 1 0 1 1
  bool a=false,b=false,c=false,d=false,e=false,f=false,g=false;

  switch (digit) {
    case 0: a=b=c=d=e=f=true; g=false; break;
    case 1: b=c=true; break;
    case 2: a=b=d=e=true; c=false; f=false; g=true; break;
    case 3: a=b=c=d=true; g=true; break;
    case 4: b=c=f=true; g=true; break;
    case 5: a=c=d=f=true; b=false; e=false; g=true; break;
    case 6: a=c=d=e=f=true; g=true; break;
    case 7: a=b=c=true; break;
    case 8: a=b=c=d=e=f=g=true; break;
    case 9: a=b=c=d=f=true; g=true; e=false; break;
    default: a=b=c=d=e=f=false; g=false; break;
  }

  // Build active_low bits in [A,B,C,D,E,F,G] => [0..6]
  uint8_t bits = 0;
  bits |= ((uint8_t)(!a) ? (1u << 0) : 0); // active_low => ON when output LOW => bit=0. We'll encode output values (1=high).
  bits |= ((uint8_t)(!b) ? (1u << 1) : 0);
  bits |= ((uint8_t)(!c) ? (1u << 2) : 0);
  bits |= ((uint8_t)(!d) ? (1u << 3) : 0);
  bits |= ((uint8_t)(!e) ? (1u << 4) : 0);
  bits |= ((uint8_t)(!f) ? (1u << 5) : 0);
  bits |= ((uint8_t)(!g) ? (1u << 6) : 0);

  // This bits value represents output levels for Q0..Q6:
  // bit=1 => output HIGH; bit=0 => output LOW. Since active_low=true, segment ON corresponds to bit=0.
  return bits;
}

// Drive one SR (sr1 or sr2) "digit" value to its segments A..G.
// The hardware wiring shows:
// - sr1 controls sevseg1 segments A..G via Q0..Q6 and digit 0 (tens).
// - sr2 controls sevseg2 segments A..G via Q0..Q6 and digit 1 (units).
// Also, sr1 Q7S feeds sr2 DS, with both SRs sharing SHCP and STCP.
// That means we must shift out data for both registers in one chain:
// [sr2 DS ...] depends on chain ordering. With cascaded 595s:
// - The first 8 bits clocked end up in the last register (sr2),
//   and the next bits end up in the first register (sr1).
//
// However, common practice with two cascaded 74HC595: the bit-stream shifted MSB->LSB
// ends with the first register receiving the last bits.
//
// To avoid ambiguity and to preserve exact behavior relative to expected mapping,
// we explicitly construct the 16-bit shift frame by shifting 8 bits for sr2 first,
// then 8 bits for sr1.
//
// SR active_low: Q outputs are active_low. Q7S is an extra pin, unused by wiring.
// The contract's segment_bits only include 0..6. Q7S is unused; we should set it to 1 (HIGH)
// to keep segment dp off (no dp wiring anyway). Using standard: Q7 (bit7) = 1 (inactive).
static void writeDigits595(int tens, int units) {
  uint8_t seg1 = digitToSegmentsActiveLow(tens)  & 0x7F;  // bits 0..6 meaningful
  uint8_t seg2 = digitToSegmentsActiveLow(units) & 0x7F;

  uint8_t byte_sr1 = (uint8_t)(seg1 | (1u << 7)); // bit7=1 (inactive)
  uint8_t byte_sr2 = (uint8_t)(seg2 | (1u << 7));

  // With chain: sr2 is downstream of sr1; sr2 receives first 8 shifted bits.
  uint16_t frame = ((uint16_t)byte_sr2 << 8) | (uint16_t)byte_sr1;

  // Latch low during shift, then latch high.
  digitalWrite(SR_LATCH_PIN, LOW);

  // Shift out 16 bits MSB first.
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (frame & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }

  digitalWrite(SR_LATCH_PIN, HIGH);
}

// ---- Servo control and clamping ----
static float clampToServoDomain(float angleDeg) {
  // Model contract says handle any angle range/clamping/rounding to valid domain.
  // Arduino Servo library uses pulse widths; it accepts angle 0..180 typically.
  // We'll clamp to [0,180] to keep valid.
  if (angleDeg < 0.0f) return 0.0f;
  if (angleDeg > 180.0f) return 180.0f;
  return angleDeg;
}

static void applyOutputsFromModel() {
  // io_0: displayNumber on 7-seg (tens and units)
  int tens = (displayNumber / 10) % 10;
  int units = displayNumber % 10;

  writeDigits595(tens, units);

  // Servos: immediately update their positions to new angles after each increment.
  // Contract outputs:
  // servo1 = displayNumber * 1.8
  // servo2 = displayNumber * 1.2
  // servo3 = displayNumber * 0.6
  float a1 = clampToServoDomain((float)displayNumber * 1.8f);
  float a2 = clampToServoDomain((float)displayNumber * 1.2f);
  float a3 = clampToServoDomain((float)displayNumber * 0.6f);

  servo1.write((int)lroundf(a1));
  servo2.write((int)lroundf(a2));
  servo3.write((int)lroundf(a3));
}

void setup() {
  // Pins for shift register chain
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);

  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization requirement: show "00" and all servos at 0 degrees.
  elapsedAccumMs = 0;
  displayNumber = 0;

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  int tens = 0;
  int units = 0;
  writeDigits595(tens, units);

  lastControlMillis = millis();
  lastStepMicros = micros();
}

void loop() {
  // Implement periodic control tick at ~10ms
  uint32_t nowMs = millis();
  uint32_t elapsedMs = nowMs - lastControlMillis;

  if (elapsedMs < CONTROL_PERIOD_MS) {
    return; // wait until next control step
  }

  // io_4 semantics: delta_since_previous_step; use actual delta time since last control step.
  // The model input is i32 ms. We'll use elapsedMs, which includes possible jitter.
  int32_t io_4 = (int32_t)elapsedMs;

  lastControlMillis = nowMs;

  // ---- Controller tick (exact model logic) ----
  // if io_4 >= 2000:
  //   elapsedAccumMs = elapsedAccumMs + io_4 - 2000
  //   displayNumber = (displayNumber >= 99 ? 0 : displayNumber + 1)
  // else:
  //   elapsedAccumMs = elapsedAccumMs + io_4
  //
  // Note: This matches the provided ACCEPTED model bundle, although it does not use
  // accumulated elapsedAccumMs to trigger increments; it only checks io_4>=2000 per tick.
  // We must preserve this behavior exactly as specified.
  if (io_4 >= (int32_t)INCREMENT_PERIOD_MS) {
    elapsedAccumMs = elapsedAccumMs + io_4 - (int32_t)INCREMENT_PERIOD_MS;
    if (displayNumber >= 99) displayNumber = 0;
    else displayNumber = displayNumber + 1;

    // Contract: outputs updated in same control step after each increment.
    applyOutputsFromModel();
  } else {
    elapsedAccumMs = elapsedAccumMs + io_4;
    // Contract: between these thresholds, outputs remain unchanged.
    // Do not re-write display/servos here.
  }
}

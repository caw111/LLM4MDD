#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

/*
Frozen interface contract mapping:

io_0 (input, bool active-low): btn1 wired to D2 with pull-up enabled
io_1 (output, i32 0..9): shift595_number on sevseg1, register sr1, digit 0
  - clock pin: A2 (SHCP)
  - data pin:  A0 (DS)
  - latch pin: A1 (STCP)
  - active_low segments: active_low=true with segment_bits [0..6] for A..G
  - invalid_policy="blank" (not used because digit always valid 0..9)
  - leading_zero=false (not used; we display 0..9 only)

io_2 / io_3 (output, servo angles deg): servo_angle on D3 and D4
  - pin D3: servo1
  - pin D4: servo2
  - hold_last_valid on invalid_policy (not applicable)
*/

static const uint8_t PIN_BTN = 2;

static const uint8_t PIN_SR_DATA  = A0; // DS
static const uint8_t PIN_SR_LATCH = A1; // STCP
static const uint8_t PIN_SR_CLOCK = A2; // SHCP

static const uint8_t PIN_SERVO1 = 3; // D3
static const uint8_t PIN_SERVO2 = 4; // D4

// --- 74HC595 to 7-seg (A..G) segment patterns for digits 0..9.
// Contract specifies segment bits [0..6] map to [A,B,C,D,E,F,G] and active_low=true.
// We assume sevseg1 wiring uses typical "common cathode" with active_low segments via inverted bits.
static const uint8_t segmentBitsActiveLow[10] = {
  // digit: {A,B,C,D,E,F,G} active-low bits. (0=on, 1=off for each segment line output)
  // Using standard 7-segment codes for active-high then invert.
  // active-high for segments A B C D E F G:
  // 0: 1 1 1 1 1 1 0  -> invert => 0 0 0 0 0 0 1
  // Represent as bits [A..G] packed into bit0..bit6 respectively.
  (uint8_t)((0u<<0) | (0u<<1) | (0u<<2) | (0u<<3) | (0u<<4) | (0u<<5) | (1u<<6)), // 0
  // 1: 0 1 1 0 0 0 0 -> active-high bits: A0 B1 C1 D0 E0 F0 G0
  // invert => A1 B0 C0 D1 E1 F1 G1
  (uint8_t)((1u<<0) | (0u<<1) | (0u<<2) | (1u<<3) | (1u<<4) | (1u<<5) | (1u<<6)), // 1
  // 2: 1 1 0 1 1 0 1 -> invert => A0 B0 C1 D0 E0 F1 G0
  (uint8_t)((0u<<0) | (0u<<1) | (1u<<2) | (0u<<3) | (0u<<4) | (1u<<5) | (0u<<6)), // 2
  // 3: 1 1 1 1 0 0 1 -> invert => A0 B0 C0 D0 E1 F1 G0
  (uint8_t)((0u<<0) | (0u<<1) | (0u<<2) | (0u<<3) | (1u<<4) | (1u<<5) | (0u<<6)), // 3
  // 4: 0 1 1 0 0 1 1 -> invert => A1 B0 C0 D1 E1 F0 G0
  (uint8_t)((1u<<0) | (0u<<1) | (0u<<2) | (1u<<3) | (1u<<4) | (0u<<5) | (0u<<6)), // 4
  // 5: 1 0 1 1 0 1 1 -> invert => A0 B1 C0 D0 E1 F0 G0
  (uint8_t)((0u<<0) | (1u<<1) | (0u<<2) | (0u<<3) | (1u<<4) | (0u<<5) | (0u<<6)), // 5
  // 6: 1 0 1 1 1 1 1 -> invert => A0 B1 C0 D0 E0 F0 G0
  (uint8_t)((0u<<0) | (1u<<1) | (0u<<2) | (0u<<3) | (0u<<4) | (0u<<5) | (0u<<6)), // 6
  // 7: 1 1 1 0 0 0 0 -> invert => A0 B0 C0 D1 E1 F1 G1
  (uint8_t)((0u<<0) | (0u<<1) | (0u<<2) | (1u<<3) | (1u<<4) | (1u<<5) | (1u<<6)), // 7
  // 8: 1 1 1 1 1 1 1 -> invert => A0 B0 C0 D0 E0 F0 G0
  (uint8_t)((0u<<0) | (0u<<1) | (0u<<2) | (0u<<3) | (0u<<4) | (0u<<5) | (0u<<6)), // 8
  // 9: 1 1 1 1 0 1 1 -> invert => A0 B0 C0 D0 E1 F0 G0
  (uint8_t)((0u<<0) | (0u<<1) | (0u<<2) | (0u<<3) | (1u<<4) | (0u<<5) | (0u<<6))  // 9
};

// Send a 7-bit segment pattern to sr1 outputs Q0..Q6 (A..G).
// Contract: sr1 registers segment_bits [0..6]; active_low.
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595_write7(uint8_t segBitsActiveLow_A_to_G);
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void sevseg1_setDigit(uint8_t digit);
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool read_btn1_active_low();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void apply_outputs_from_count(int count);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 156 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void shift595_write7(uint8_t segBitsActiveLow_A_to_G)
{
  // SR outputs: Q0->sevseg1:A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G.
  // We'll clock 8 bits; Q7 is unused.
  // We pack bits so that Q0 receives A bit0, ..., Q6 receives G bit6.
  uint8_t byteToShift = 0;
  byteToShift |= ((segBitsActiveLow_A_to_G & (1u << 0)) ? 1u : 0u) << 0; // Q0 (A)
  byteToShift |= ((segBitsActiveLow_A_to_G & (1u << 1)) ? 1u : 0u) << 1; // Q1 (B)
  byteToShift |= ((segBitsActiveLow_A_to_G & (1u << 2)) ? 1u : 0u) << 2; // Q2 (C)
  byteToShift |= ((segBitsActiveLow_A_to_G & (1u << 3)) ? 1u : 0u) << 3; // Q3 (D)
  byteToShift |= ((segBitsActiveLow_A_to_G & (1u << 4)) ? 1u : 0u) << 4; // Q4 (E)
  byteToShift |= ((segBitsActiveLow_A_to_G & (1u << 5)) ? 1u : 0u) << 5; // Q5 (F)
  byteToShift |= ((segBitsActiveLow_A_to_G & (1u << 6)) ? 1u : 0u) << 6; // Q6 (G)
  byteToShift |= 0u << 7; // Q7 don't care

  // Shift out MSB first or LSB first must match how Q0 corresponds in the chain.
  // To preserve intended mapping Q0->A from segment_bits, we shift LSB-first so that first shifted bit becomes Q0.
  // We'll implement LSB-first.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(PIN_SR_DATA, (byteToShift & (1u << i)) ? HIGH : LOW);
    digitalWrite(PIN_SR_CLOCK, HIGH);
    digitalWrite(PIN_SR_CLOCK, LOW);
  }
  // Latch
  digitalWrite(PIN_SR_LATCH, HIGH);
  digitalWrite(PIN_SR_LATCH, LOW);
}

static inline void sevseg1_setDigit(uint8_t digit)
{
  // invalid_policy="blank": never happens because digit is always 0..9 per model.
  if (digit > 9) {
    // blank: turn all segments off => active_low means 1 for A..G
    uint8_t blank = (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5)|(1u<<6);
    shift595_write7(blank);
  } else {
    shift595_write7(segmentBitsActiveLow[digit]);
  }
}

// Model states per accepted executable model
static int count_state = 0;
static bool previousPressed_state = false;

// IO mirror (for edge detect gate), sample_period_ms=5 (control_period_ms=5)
static inline bool read_btn1_active_low()
{
  // active_high=false, true_level=LOW => pressed => LOW => model value true when digitalRead==LOW
  return (digitalRead(PIN_BTN) == LOW);
}

static Servo servo1;
static Servo servo2;

static void apply_outputs_from_count(int count)
{
  // io_1 = count
  sevseg1_setDigit((uint8_t)count);

  // Model output expression:
  // io_2 = if ( (count - 2*floor(count/2)) == 0 ) then 0.0 else 90.0
  // This is equivalent to even/odd: if count is even => 0 else 90.
  bool isEven = ((count - (2 * (int)floor((double)count / 2.0))) == 0);
  double angle = isEven ? 0.0 : 90.0;

  // Servo update immediately after count update
  servo1.write((int)angle);
  servo2.write((int)angle);
}

void setup()
{
  pinMode(PIN_BTN, INPUT_PULLUP);

  pinMode(PIN_SR_DATA, OUTPUT);
  pinMode(PIN_SR_LATCH, OUTPUT);
  pinMode(PIN_SR_CLOCK, OUTPUT);

  digitalWrite(PIN_SR_CLOCK, LOW);
  digitalWrite(PIN_SR_LATCH, LOW);
  digitalWrite(PIN_SR_DATA, LOW);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);

  // Initialization per model: count=0, previousPressed=false
  count_state = 0;
  previousPressed_state = false;

  apply_outputs_from_count(count_state);
}

void loop()
{
  static unsigned long lastTickMs = 0;
  unsigned long now = millis();
  if (now - lastTickMs < 5) return;
  lastTickMs = now;

  // tick:
  bool io0 = read_btn1_active_low(); // io_0

  // if (io_0 and not previousPressed) then count := (count>=9 ? 0 : count+1)
  if (io0 && (!previousPressed_state)) {
    if (count_state >= 9) count_state = 0;
    else count_state = count_state + 1;

    // After count update, outputs update immediately
    apply_outputs_from_count(count_state);
  }

  previousPressed_state = io0;

  // If no button edge, io_2/io_3/io_1 remain as model outputs of current count_state.
  // No explicit re-application needed; but safe to keep behavior aligned with outputs.
  // Contract says display should immediately update after each button press; we already do.
}

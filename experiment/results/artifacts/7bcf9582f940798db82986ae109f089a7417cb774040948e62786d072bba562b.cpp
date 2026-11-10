#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_80\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// -------------------- Pin bindings (from frozen contract) --------------------
static const uint8_t PIN_BTN1 = 2;   // io_0 input, active_high=false, pullup=true
static const uint8_t PIN_SR_DS  = A0; // shift register data pin (blue)
static const uint8_t PIN_SR_SHCP = A2; // shift register clock pin (gray)
static const uint8_t PIN_SR_STCP = A1; // shift register latch pin (purple)

static const uint8_t BAR_PINS[10] = {
  3, 4, 5, 6, 7, 8, 9, 10, 11, 12  // io_2..io_11
};

// -------------------- Controller state (must match model) --------------------
static int32_t number = 0;              // io_1
static bool previousPressed = false;   // internal

// -------------------- Shift595 digit encoding (active_low=true, digit 0 only) -----
// Contract says: invalid_policy=blank, leading_zero=true, segment_bits=[0..6].
// We implement conventional 7-seg mapping with active-low outputs:
// segments: [A,B,C,D,E,F,G] correspond to output bits [0..6].
// Values below are for common mapping where '0' lights A,B,C,D,E,F (G off).
// Since active_low=true, we invert the lit mask.
static const uint8_t SEG_ACTIVE_LOW_DIGIT_BITS[10][7] = {
  // digit : A,B,C,D,E,F,G (active-low)
  // 0 => A B C D E F on, G off
  {0,0,0,0,0,0,1},
  // 1 => B C on
  {1,0,0,1,1,1,1},
  // 2 => A B D E G on
  {0,0,1,0,0,1,0},
  // 3 => A B C D G on
  {0,0,0,0,1,1,0},
  // 4 => B C F G on
  {1,0,0,1,1,0,0},
  // 5 => A C D F G on
  {0,1,0,0,1,0,0},
  // 6 => A C D E F G on
  {0,1,0,0,0,0,0},
  // 7 => A B C on
  {0,0,0,1,1,1,1},
  // 8 => A B C D E F G on
  {0,0,0,0,0,0,0},
  // 9 => A B C D F G on
  {0,0,0,0,1,0,0}
};

// -------------------- IO update helpers --------------------
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_80\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool read_btn1_model_value();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_80\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void write_sevseg_number(int32_t n);
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_80\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void write_bargraph_from_number(int32_t n);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_80\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void apply_outputs_from_state();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_80\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_80\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_80\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline bool read_btn1_model_value() {
  // active_high=false, pullup=true: button pressed => LOW on pin, model value true when pressed
  // So model_value = (digitalRead(pin) == LOW)
  return (digitalRead(PIN_BTN1) == LOW);
}

static void write_sevseg_number(int32_t n) {
  // io_1 is number 0..9; model never sends invalid beyond 9.
  // Write only digit 0 register via shift register:
  // segment bits correspond to Q0..Q6 => A,B,C,D,E,F,G (per wiring).
  // Build 7-bit payload, then shift out and latch.
  uint8_t segBits[7];
  if (n < 0 || n > 9) {
    // invalid_policy blank: turn all segments off.
    // active_low => off means HIGH for all segments.
    for (int i = 0; i < 7; i++) segBits[i] = 1;
  } else {
    for (int i = 0; i < 7; i++) segBits[i] = SEG_ACTIVE_LOW_DIGIT_BITS[n][i];
  }

  // Shift out bits in order Q0..Q6 => segment_bits [0..6]
  // DS wired to sr1:DS (A0). Use DS as data, SHCP as clock, STCP as latch.
  // We'll clock 7 bits; MSB/LSB ordering is determined by physical mapping; we
  // assume shift register takes first shifted bit to appear at Q0.
  // To match typical 74HC595 behavior, we shift MSB first when using value bits,
  // but since we are explicitly shifting per bit, we choose a fixed order:
  // Shift segment_bits[0] first, segment_bits[6] last.
  for (int i = 0; i < 7; i++) {
    digitalWrite(PIN_SR_DS, segBits[i] ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
    digitalWrite(PIN_SR_SHCP, LOW);
  }

  // latch output
  digitalWrite(PIN_SR_STCP, HIGH);
  digitalWrite(PIN_SR_STCP, LOW);
}

static void write_bargraph_from_number(int32_t n) {
  // io_2..io_11 are true when number>=1..>=9, and io_11 corresponds to false always (per model).
  // Model outputs: for k=1..9 => (number>=k). For k=10 (io_11) => false.
  // Our bar pins correspond in order A1..A10 => io_2..io_11.
  // Thus:
  // A1 (pin D3) corresponds to number>=1
  // ...
  // A9 (pin D11) corresponds to number>=9
  // A10 (pin D12) corresponds to false
  bool on[10];
  for (int i = 0; i < 10; i++) on[i] = false;
  if (n >= 1) on[0] = true;
  if (n >= 2) on[1] = true;
  if (n >= 3) on[2] = true;
  if (n >= 4) on[3] = true;
  if (n >= 5) on[4] = true;
  if (n >= 6) on[5] = true;
  if (n >= 7) on[6] = true;
  if (n >= 8) on[7] = true;
  if (n >= 9) on[8] = true;
  // on[9] remains false (A10 pin D12 => io_11=false)

  for (int i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], on[i] ? HIGH : LOW); // active_high=true
  }
}

static void apply_outputs_from_state() {
  // Model outputs depend on 'number' only
  write_sevseg_number(number);      // io_1
  write_bargraph_from_number(number); // io_2..io_11
}

// -------------------- Arduino setup/loop --------------------
void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP); // pull-up required by contract
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);

  for (int i = 0; i < 10; i++) pinMode(BAR_PINS[i], OUTPUT);

  // Initialize controller state as per model
  number = 0;
  previousPressed = false;

  // Apply outputs immediately on init: io_1=0, bargraph all false
  // Also ensure shift register is latched with digit 0.
  // (Ordering: apply outputs from state at "first start".)
  apply_outputs_from_state();
}

void loop() {
  // Control period is 5ms in frozen contract. We'll sample/publish at ~5ms.
  const uint32_t PERIOD_US = 5000;

  static uint32_t lastMicros = micros();
  while (true) {
    uint32_t now = micros();
    if ((uint32_t)(now - lastMicros) >= PERIOD_US) {
      lastMicros = now;
      break;
    }
    // Busy wait to keep the exact 5ms rhythm reasonably close.
  }

  // tick:
  bool io_0 = read_btn1_model_value(); // btn press event

  // Update logic must preserve exact state transition ordering:
  // if (io_0 && !previousPressed) number := (number>=9 ? 0 : number+1)
  // previousPressed := io_0
  if (io_0 && (!previousPressed)) {
    if (number >= 9) number = 0;
    else number = number + 1;
  }
  previousPressed = io_0;

  // Then set outputs from new state (immediate update)
  apply_outputs_from_state();
}

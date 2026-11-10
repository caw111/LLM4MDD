#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
static constexpr uint8_t PIN_SR_SHCP = A2; // SHCP (clock)  <- mega:A2
static constexpr uint8_t PIN_SR_STCP = A1; // STCP (latch)  <- mega:A1
static constexpr uint8_t PIN_SR_DS   = A0; // DS (data)     <- mega:A0
static constexpr uint8_t PIN_BTN    = 2;  // btn1 (active-low, pullup) <- mega:2

// 74HC595 -> single 7-seg digit, active_low=true, only Q0..Q6 used for A..G.
// Need bit pattern for digits 0..9 assuming standard 7-seg active-high segments,
// then invert because outputs are active-low.
// Segment mapping:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveHighSegments(uint8_t d);
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595Write7Seg(uint8_t digit);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool read_io_1();
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveHighSegments(uint8_t d) {
  // a b c d e f g (active-high)
  switch (d) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);           // a b c d e f
    case 1: return (1<<1)|(1<<2);                                     // b c
    case 2: return (1<<0)|(1<<1)|(1<<3)|(1<<4);                       // a b d e
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)?0:0; // placeholder to avoid warnings
    default: break;
  }

  // Provide full correct mapping with explicit table for reliability.
  // Index bits: 0=a,1=b,2=c,3=d,4=e,5=f,6=g
  static const uint8_t seg[10] = {
    // 0  1  2  3  4  5  6  7  8  9
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),              // 0
    (1<<1)|(1<<2),                                        // 1
    (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                  // 2 (a b d e g)
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                  // 3 (a b c d g)
    (1<<1)|(1<<2)|(1<<5)|(1<<6),                         // 4 (b c f g)
    (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                  // 5 (a c d f g)
    (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),          // 6 (a c d e f g)
    (1<<0)|(1<<1)|(1<<2),                                 // 7 (a b c)
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),   // 8
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)            // 9 (a b c d f g)
  };

  return seg[d];
}

static void shift595Write7Seg(uint8_t digit) {
  // Determine active-high segment pattern for digit, then convert to active-low bits for Q0..Q6.
  uint8_t activeHigh = digitToActiveHighSegments(digit);

  // Only Q0..Q6 are wired, and all are active-low.
  // Create bit pattern where bit i corresponds to Q{i} controlling segment {A..G}.
  // Active-low => output bit = NOT activeHigh for segments used.
  uint8_t activeLowBits = (uint8_t)(~activeHigh) & 0x7F; // bits 0..6 inverted, others ignored

  // 74HC595 typical shifting order depends on wiring; in Wokwi, Q0..Q7 correspond to shifted bits
  // such that the first shifted bit ends up in Q0 after latch in common implementations when shifting MSB->LSB
  // or LSB->MSB depends on library. We'll implement deterministic mapping by shifting 8 bits LSB-first:
  // bit0 -> first data bit, ends up at Q0.
  uint8_t value = activeLowBits; // Q0..Q6 set, Q7 unused -> keep low (0)
  // value already has bit0..6; bit7 default 0.
  value &= 0x7F;

  // Shift 8 bits LSB-first
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t bit = (value >> i) & 0x01;
    digitalWrite(PIN_SR_DS, bit ? HIGH : LOW);
    // clock pulse
    digitalWrite(PIN_SR_SHCP, HIGH);
    digitalWrite(PIN_SR_SHCP, LOW);
  }

  // Latch
  digitalWrite(PIN_SR_STCP, HIGH);
  digitalWrite(PIN_SR_STCP, LOW);
}

// Controller state (exactly as frozen model)
static int32_t count_i32 = 0;
static bool previousPressed_b = false;
static bool debouncedPressed_b = false;

static bool read_io_1() {
  // Button wired with pullup, active-low pressed model.
  // io_1=true corresponds to pressed.
  return digitalRead(PIN_BTN) == LOW;
}

void setup() {
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);
  pinMode(PIN_SR_DS, OUTPUT);

  pinMode(PIN_BTN, INPUT_PULLUP);

  digitalWrite(PIN_SR_SHCP, LOW);
  digitalWrite(PIN_SR_STCP, LOW);
  digitalWrite(PIN_SR_DS, LOW);

  // Initialization: display shows 0.
  count_i32 = 0;
  previousPressed_b = false;
  debouncedPressed_b = false;
  shift595Write7Seg((uint8_t)count_i32);
}

void loop() {
  // Model tick at 10ms period (controller contract: control_period_ms = 10)
  static uint32_t last_ms = 0;
  uint32_t now = millis();
  if (now - last_ms < 10) return;
  last_ms = now;

  // in attribute io_1 : Boolean;
  bool io_1 = read_io_1();

  // Updates in given order.

  // if (io_1) and (not previousPressed) -> debouncedPressed := true
  if (io_1 && (!previousPressed_b)) {
    debouncedPressed_b = true;
  } else {
    // else: no assignment
  }

  // if ((not io_1) and previousPressed) -> debouncedPressed := false
  if ((!io_1) && previousPressed_b) {
    debouncedPressed_b = false;
  } else {
    // else: no assignment
  }

  // if (debouncedPressed) -> count update and debouncedPressed := false
  if (debouncedPressed_b) {
    // count := (if count == 9 then 0 else count + 3)
    if (count_i32 == 9) {
      count_i32 = 0;
    } else {
      count_i32 = count_i32 + 3;
    }
    debouncedPressed_b = false;
  } else {
    // else: no assignment
  }

  // previousPressed := io_1
  previousPressed_b = io_1;

  // Output io_0 := count
  shift595Write7Seg((uint8_t)count_i32);
}

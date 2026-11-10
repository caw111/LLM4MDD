#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen hardware contract bindings (Arduino Mega 2560)
static constexpr uint8_t PIN_IO_0  = 2;   // io_0  -> D2
static constexpr uint8_t PIN_IO_1  = 3;   // io_1  -> D3
static constexpr uint8_t PIN_IO_2  = 4;   // io_2  -> D4
static constexpr uint8_t PIN_IO_3  = 5;   // io_3  -> D5
static constexpr uint8_t PIN_IO_4  = 6;   // io_4  -> D6
static constexpr uint8_t PIN_IO_5  = 7;   // io_5  -> D7
static constexpr uint8_t PIN_IO_6  = 8;   // io_6  -> D8
static constexpr uint8_t PIN_IO_7  = 9;   // io_7  -> D9
static constexpr uint8_t PIN_IO_8  = 10;  // io_8  -> D10
static constexpr uint8_t PIN_IO_9  = 11;  // io_9  -> D11
static constexpr uint8_t PIN_IO_10 = 12;  // io_10 -> D12

// ACCEPTED executable model (inlined):
// Drive shared LED nets such that:
// - bargraph1 position i is lit iff shared net for that position is HIGH
// - bargraph2 position (i-1) mirrors bargraph1 position i for i=2..10
// - When bargraph1 reaches LED1, bargraph2 turns off.
// Direction:
// - Phase L2R: bargraph1 1->10
// - Phase R2L: bargraph1 10->1
// Step delay: 2000 ms
//
// Hardware mapping implied by Frozen contract:
// io_0..io_9 correspond to shared nets:
//   io_0:  bargraph1 A1      (and bargraph2 A? not shared in contract -> only A1/pos1 for bargraph1)
//   io_1:  shared bargraph1 A2 == bargraph2 A1
//   ...
//   io_9:  shared bargraph1 A10 == bargraph2 A9
// io_10 corresponds to independent bargraph2 A10 (position10).
//
// Therefore, for each bargraph1 LED index i (1..10):
// - bargraph1 A(i) ON  =>
//     if i==1 -> io_0 = 1
//     if i>=2 -> io_(i-1) = 1 (since io_k maps bargraph1 A(k+1))
// - bargraph2:
//     position i-1 ON for i=2..10 => bargraph2 A(i-1) is driven by the same shared net io_(i-1)
//     position 10 ON only when bargraph1 i==11 (never); so bargraph2 A10 is ON when bargraph1 reaches 10
//     because spec says when bargraph1 reaches i, bargraph2 reaches i-1. For i=10 => bargraph2 is at 9, and A10 should be OFF.
//     Independent io_10 for A10 must remain OFF always.
//     But contract says io_10 is bargraph2 A10, so it will be OFF for all i in 1..10.
//
// Resulting net values at each step i:
//   io_0  = (i==1)
//   io_k  = (i==k+1) for k=1..9   (i==2 => io_1, ..., i==10 => io_9)
//   io_10 = false
//
// This also satisfies: when bargraph1 reaches LED1, bargraph2 turns off.
// (bargraph2 positions 0 none; shared nets all LOW, independent io_10 LOW).

#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeAllOff();
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_26\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeAllOff() {
  digitalWrite(PIN_IO_0,  LOW);
  digitalWrite(PIN_IO_1,  LOW);
  digitalWrite(PIN_IO_2,  LOW);
  digitalWrite(PIN_IO_3,  LOW);
  digitalWrite(PIN_IO_4,  LOW);
  digitalWrite(PIN_IO_5,  LOW);
  digitalWrite(PIN_IO_6,  LOW);
  digitalWrite(PIN_IO_7,  LOW);
  digitalWrite(PIN_IO_8,  LOW);
  digitalWrite(PIN_IO_9,  LOW);
  digitalWrite(PIN_IO_10, LOW);
}

static void writeByBargraph1Index(uint8_t i /*1..10*/) {
  // Shared nets io_0..io_9
  digitalWrite(PIN_IO_0,  (i == 1) ? HIGH : LOW);

  digitalWrite(PIN_IO_1,  (i == 2)  ? HIGH : LOW);
  digitalWrite(PIN_IO_2,  (i == 3)  ? HIGH : LOW);
  digitalWrite(PIN_IO_3,  (i == 4)  ? HIGH : LOW);
  digitalWrite(PIN_IO_4,  (i == 5)  ? HIGH : LOW);
  digitalWrite(PIN_IO_5,  (i == 6)  ? HIGH : LOW);
  digitalWrite(PIN_IO_6,  (i == 7)  ? HIGH : LOW);
  digitalWrite(PIN_IO_7,  (i == 8)  ? HIGH : LOW);
  digitalWrite(PIN_IO_8,  (i == 9)  ? HIGH : LOW);
  digitalWrite(PIN_IO_9,  (i == 10) ? HIGH : LOW);

  // Independent bargraph2 A10 (position10) must be OFF for all i=1..10 per mapping rule
  digitalWrite(PIN_IO_10, LOW);
}

void setup() {
  pinMode(PIN_IO_0,  OUTPUT);
  pinMode(PIN_IO_1,  OUTPUT);
  pinMode(PIN_IO_2,  OUTPUT);
  pinMode(PIN_IO_3,  OUTPUT);
  pinMode(PIN_IO_4,  OUTPUT);
  pinMode(PIN_IO_5,  OUTPUT);
  pinMode(PIN_IO_6,  OUTPUT);
  pinMode(PIN_IO_7,  OUTPUT);
  pinMode(PIN_IO_8,  OUTPUT);
  pinMode(PIN_IO_9,  OUTPUT);
  pinMode(PIN_IO_10, OUTPUT);

  // Initialization: all LEDs off.
  writeAllOff();
}

void loop() {
  // ACCEPTED timing/model:
  // - step delay 2000ms
  // - direction changes after reaching end
  static bool initialized = false;
  static uint8_t i = 1;        // bargraph1 LED index
  static int8_t dir = +1;      // +1 for L->R, -1 for R->L
  static unsigned long lastStepMs = 0;

  unsigned long now = millis();

  if (!initialized) {
    // Start with L2R at i=1 (both graphs synchronized accordingly).
    // Note: Even though init oracle required all outputs false at power-on,
    // we maintain that only through setup() before first step update.
    i = 1;
    dir = +1;
    lastStepMs = now;
    initialized = true;
    // Apply first step immediately.
    writeByBargraph1Index(i);
    return;
  }

  if (now - lastStepMs >= 2000UL) {
    lastStepMs += 2000UL;

    // Advance state (preserve exact step semantics)
    int next = (int)i + (int)dir;

    if (next > 10) {
      // reached rightmost then start going left
      dir = -1;
      i = 10; // ensure we keep correct LED at boundary before changing direction
      // But boundary must proceed to the next step in same cycle:
      // If at i==10 and dir==+1, next would be 11; direction flips and step should move to 9.
      // So set i=9 after flip.
      i = 9;
    } else if (next < 1) {
      // reached leftmost then start going right
      dir = +1;
      i = 1;  // boundary
      i = 2;  // move to 2 after flip
    } else {
      i = (uint8_t)next;
    }

    // Apply outputs for current bargraph1 index
    writeByBargraph1Index(i);
  }
}

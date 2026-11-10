#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
static constexpr uint8_t PIN_POT1_SIG = A3; // io_0 input
static constexpr uint8_t PIN_SR_CLOCK = A2; // sr1 SHCP (clock_pin)
static constexpr uint8_t PIN_SR_LATCH = A1; // sr1 STCP (latch_pin)
static constexpr uint8_t PIN_SR_DATA  = A0; // sr1 DS (data_pin)

// Shift register output enable is tied to GND (always enabled per wiring: sr1:OE black; sr1:GND. OE)
// Master Reset tied to VCC via wiring (sr1:VCC -> sr1:MR red; sr1:VCC -> sr2:MR red)

static constexpr uint16_t CONTROL_PERIOD_MS = 10; // control_period_ms

// 7-seg segment encoding for digits 0..9
// Contract indicates: sr segment_bits = [0,1,2,3,4,5,6] correspond to [A,B,C,D,E,F,G] on each display
// and registers are active_low=true.
// Here we provide active-low segment patterns for a common 7-seg mapping:
// bit0->A, bit1->B, bit2->C, bit3->D, bit4->E, bit5->F, bit6->G.
static const uint8_t SEG_ACTIVE_LOW[10] = {
  // digit: abcdefg (1=segment ON) => active-low output is bit complement
  // 0:  a b c d e f   = 1 1 1 1 1 1 0
  // 1:        b c     = 0 1 1 0 0 0 0
  // 2:  a b   d e   f? Actually: a b d e g = 1 1 0 1 1 0 1
  // 3:  a b c d     g = 1 1 1 1 0 0 1
  // 4:    b c     f g = 0 1 1 0 0 1 1
  // 5:  a   c d   f g = 1 0 1 1 0 1 1
  // 6:  a   c d e f g = 1 0 1 1 1 1 1
  // 7:  a b c         = 1 1 1 0 0 0 0
  // 8:  a b c d e f g = 1 1 1 1 1 1 1
  // 9:  a b c d   f g = 1 1 1 1 0 1 1
  // Convert to active-low bits: out_bit = !on_bit
  0b00000010, // 0: active-low => A..G = 0,0,0,0,0,0,1? wait: see mapping below
  0b10011111, // 1
  0b00100100, // 2
  0b00001100, // 3
  0b10011000, // 4
  0b01001000, // 5
  0b01000000, // 6
  0b00011111, // 7
  0b00000000, // 8
  0b00001000  // 9
};

// The above patterns assume bit order [A,B,C,D,E,F,G] in bits [0..6] with active_low.
// To avoid ambiguity caused by different segment conventions, we also build patterns from a known
// ON mask and invert. This guarantees consistency.
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t buildActiveLowFromOnMask(uint8_t onMask);
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToSegmentActiveLow(uint8_t d);
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595_writeDigits(uint8_t tensDigit, uint8_t unitsDigit);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void modelTick(float potIo0);
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t buildActiveLowFromOnMask(uint8_t onMask) {
  // onMask bits [A..G]=1 means segment ON. Convert to active-low output bits.
  // output bits are 0 when segment ON.
  return (uint8_t)(~onMask) & 0x7F;
}

// Replace SEG_ACTIVE_LOW with computed values from ON masks to remove any risk.
// ON masks (bit0=A, bit1=B,...bit6=G):
// 0: A B C D E F
// 1: B C
// 2: A B D E G
// 3: A B C D G
// 4: B C F G
// 5: A C D F G
// 6: A C D E F G
// 7: A B C
// 8: A B C D E F G
// 9: A B C D F G
static const uint8_t SEG_ON_MASK[10] = {
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),                       // 0
  (1<<1)|(1<<2),                                                   // 1
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                               // 2
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                               // 3
  (1<<1)|(1<<2)|(1<<5)|(1<<6),                                       // 4
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                               // 5
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),                       // 6
  (1<<0)|(1<<1)|(1<<2),                                           // 7
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),               // 8
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)                        // 9
};

static uint8_t digitToSegmentActiveLow(uint8_t d) {
  if (d > 9) return 0x7F; // blank -> all segments off => active-low bits all 1
  return buildActiveLowFromOnMask(SEG_ON_MASK[d]);
}

// Model state
static int32_t shift595_number = 0;

// Hardware driver: shift out 7 segment bits for a given digit to a given shift register.
// Note: Wiring uses two separate 74HC595 shift registers (sr1 and sr2) sharing the same SHCP and STCP
// lines, while their data inputs are tied from sr1:Q7S to sr2:DS. This means:
// - Shifting 7 bits will load sr2 with the last shifted bit(s) depending on chain length.
// However, with cascaded 595s, best approach is to shift 14 bits: 7 for sr1 (first), then 7 for sr2 (second).
//
// We preserve the contract that shift595_number encodes digits by display:
// digit 0 (tens) -> sevseg1 (sr1), digit 1 (units) -> sevseg2 (sr2).
static void shift595_writeDigits(uint8_t tensDigit, uint8_t unitsDigit) {
  uint8_t seg1 = digitToSegmentActiveLow(tensDigit);   // for sevseg1 via sr1 outputs Q0..Q6
  uint8_t seg2 = digitToSegmentActiveLow(unitsDigit);  // for sevseg2 via sr2 outputs Q0..Q6

  // We will shift 14 bits MSB-first or LSB-first consistently.
  // To map bits to outputs Q0..Q6, we assume DS input bit0 corresponds to Q0 after shifting.
  // We'll shift bits in order A..G => bit0=A,...bit6=G by sending bit0 first (LSB-first).
  //
  // Chain order: first shifted bits end up in sr2? For cascaded shift registers:
  // The bit shifted earliest propagates to the first register (sr1) output after full chain.
  // We'll output 14 bits as: sr1 A..G then sr2 A..G, shifting LSB-first, so sr1 gets the first 7 bits.
  //
  // If your actual wiring implies opposite bit order, you'd need reversal, but the contract's
  // "segment_bits": [0..6] suggests direct mapping and expects this approach.
  noInterrupts();

  // Latch low then shift data
  digitalWrite(PIN_SR_LATCH, LOW);

  // Shift 14 bits: sr1[0..6], then sr2[0..6]
  for (int i = 0; i < 7; i++) {
    uint8_t bitVal = (seg1 >> i) & 0x01; // bit i corresponds to segment bit i
    digitalWrite(PIN_SR_DATA, bitVal ? HIGH : LOW);
    digitalWrite(PIN_SR_CLOCK, HIGH);
    // small hold time
    digitalWrite(PIN_SR_CLOCK, LOW);
  }
  for (int i = 0; i < 7; i++) {
    uint8_t bitVal = (seg2 >> i) & 0x01;
    digitalWrite(PIN_SR_DATA, bitVal ? HIGH : LOW);
    digitalWrite(PIN_SR_CLOCK, HIGH);
    digitalWrite(PIN_SR_CLOCK, LOW);
  }

  // Latch updates both registers
  digitalWrite(PIN_SR_LATCH, HIGH);

  interrupts();
}

static void modelTick(float potIo0) {
  // Accepted executable model:
  // shift595_number := (io_0 in [0,99]) ? floor((io_0/1.0)*1.0) : 0
  // With integer mapping, io_0 is already an integer value from ADC scaling.
  if ((potIo0 >= 0.0f) && (potIo0 <= 99.0f)) {
    float scaled = (potIo0 / 1.0f) * 1.0f;
    shift595_number = (int32_t)floorf(scaled);
  } else {
    shift595_number = 0;
  }

  // Output expression is direct: io_1 equals shift595_number
  // But hardware contract requires digit assignment:
  // digit 0 leftmost (tens) on sevseg1; digit 1 rightmost (units) on sevseg2.
  // shift595_number is 0..99; compute digits:
  uint8_t tens = (uint8_t)((shift595_number / 10) % 10);
  uint8_t units = (uint8_t)(shift595_number % 10);

  shift595_writeDigits(tens, units);
}

void setup() {
  pinMode(PIN_SR_CLOCK, OUTPUT);
  pinMode(PIN_SR_LATCH, OUTPUT);
  pinMode(PIN_SR_DATA, OUTPUT);

  // Initialize pins to safe levels
  digitalWrite(PIN_SR_CLOCK, LOW);
  digitalWrite(PIN_SR_LATCH, LOW);
  digitalWrite(PIN_SR_DATA, LOW);

  // Initialize model state: shift595_number initial=0 => display "00"
  shift595_number = 0;

  // Write initial "00"
  shift595_writeDigits(0, 0);
}

void loop() {
  static uint32_t lastTick = 0;
  uint32_t now = millis();

  if (now - lastTick >= CONTROL_PERIOD_MS) {
    lastTick += CONTROL_PERIOD_MS;

    // Read potentiometer mapped to adc_count in [0..99] per contract.
    // Contract: scale=1.0 offset=0.0; treat analog reading as "adc_count".
    // We map raw ADC 0..1023 -> 0..99 using the implied "floor" model mapping behavior outside is handled.
    // The model expects io_0 as Real with potentially out-of-range values; our quantization stays within 0..99.
    int raw = analogRead(PIN_POT1_SIG); // 0..1023
    float io0 = (float)raw * (99.0f / 1023.0f);

    // Keep exact model behavior:
    modelTick(io0);
  }
}

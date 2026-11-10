#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t PIN_led1 = 2;   // io_1
static constexpr uint8_t PIN_led2 = 3;   // io_2
static constexpr uint8_t PIN_led3 = 4;   // io_3

static constexpr uint8_t PIN_rgb1_R = 5;  // io_4
static constexpr uint8_t PIN_rgb1_G = 6;  // io_5
static constexpr uint8_t PIN_rgb1_B = 7;  // io_6

static constexpr uint8_t PIN_rgb2_R = 8;  // io_7
static constexpr uint8_t PIN_rgb2_G = 9;  // io_8
static constexpr uint8_t PIN_rgb2_B = 10; // io_9

// shift registers: A0 DS, A1 STCP, A2 SHCP
static constexpr uint8_t PIN_SR_DS   = A0; // io_10 shift595_number data_pin
static constexpr uint8_t PIN_SR_STCP = A1; // latch_pin
static constexpr uint8_t PIN_SR_SHCP = A2; // clock_pin

// Map digit (0..9) to segment outputs for active_low=true segment bits [0..6] => A..G
// segment_bits are wired as:
// sr?: Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveLowSegments7(uint8_t digit);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595Write7(uint8_t sevenBitActiveLow, bool toSr2);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595WriteDigits(uint8_t leftDigit, uint8_t rightDigit);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int i32_clamp_not_needed(int x);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(bool io_1, bool io_2, bool io_3, bool io_4, bool io_5, bool io_6, bool io_7, bool io_8, bool io_9, int io_10);
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 182 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveLowSegments7(uint8_t digit) {
  // segments: A,B,C,D,E,F,G
  // active_low => bit=1 means LED off, bit=0 means LED on
  // We define typical 7-seg common-cathode patterns then invert.
  // digit->which segments ON (1=on, A..G order)
  const uint8_t onMask[10] = {
    // 0: A B C D E F
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),
    // 1: B C
    (1<<1)|(1<<2),
    // 2: A B D E G
    (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),
    // 3: A B C D G
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),
    // 4: B C F G
    (1<<1)|(1<<2)|(1<<5)|(1<<6),
    // 5: A C D F G
    (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),
    // 6: A C D E F G
    (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
    // 7: A B C
    (1<<0)|(1<<1)|(1<<2),
    // 8: A B C D E F G
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
    // 9: A B C D F G
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)
  };

  uint8_t maskOn = (digit < 10) ? onMask[digit] : 0;
  uint8_t maskOff = (0x7F & (~maskOn)); // active_low bits: 1=off, 0=on
  return maskOff;
}

// Write 7 bits (Q0..Q6) into a 74HC595 connected to A..G segments (active_low)
// latch holds the pattern; SHCP shifts from MSB? We'll shift LSB first matching Q0..Q6 mapping.
// We'll also optionally clear Q7S if connected (unused in wiring, but keep it 1-off).
static void shift595Write7(uint8_t sevenBitActiveLow, bool toSr2) {
  // toSr1 => first load into sr1, then latch; toSr2 => shift for sr2 but sr1 also receives bits in cascade.
  // However, circuit ties sr1 SHCP/STCP/DSP together? Actually SHCP and STCP are shared between sr1 and sr2,
  // DS is on sr1 only; sr2 DS is driven from sr1 Q7S, so it's a cascade. Therefore we must shift 16 bits:
  // first 8 bits for sr2 (later in chain), then 8 bits for sr1.
  // Wiring uses only sr1 Q0..Q7S and sr2 Q0..Q6. We'll shift 16 bits with sr1 bits in lower byte and sr2 bits in upper byte.
  // Active_low: Qn=1 turns segment off.
  (void)toSr2;

  uint8_t sr1_bits = sevenBitActiveLow;        // bits for Q0..Q6 in this byte, Q7S left as 1 (off)
  sr1_bits |= (1u << 7);                      // Q7S = 1 (don't care), to keep unused segment off
  uint8_t sr2_bits = sevenBitActiveLow;        // same pattern used for sr2 segments A..G

  // We need to update BOTH displays: sr1->sevseg1 digit 1 (right digit), sr2->sevseg2 digit 0 (left digit).
  // The controller's io_10 encodes the two digits value (left digit = countdown/10? but for 99/88/00 only identical digits).
  // We'll provide both digits separately via sr1_bits and sr2_bits by caller.
}

// Cascade writer for both digits: sr1 gets rightDigit, sr2 gets leftDigit.
// digit 0..9 patterns to active_low segment bits 0..6.
static void shift595WriteDigits(uint8_t leftDigit, uint8_t rightDigit) {
  uint8_t sr1_segs = digitToActiveLowSegments7(rightDigit); // sevseg1 uses sr1
  uint8_t sr2_segs = digitToActiveLowSegments7(leftDigit);  // sevseg2 uses sr2

  // Compose two bytes for cascade shifting, MSB-first through DS.
  // Cascade order: first shifted byte ends up in sr2 (furthest), second in sr1 (nearest).
  uint8_t sr2_bits = sr2_segs | (1u << 7); // Q7S = 1
  uint8_t sr1_bits = sr1_segs | (1u << 7);

  uint16_t frame = ((uint16_t)sr2_bits << 8) | sr1_bits;

  // Ensure latch low
  digitalWrite(PIN_SR_STCP, LOW);
  // Shift out 16 bits, MSB first (bit15 first) so that bytes land correctly in cascade.
  for (int i = 15; i >= 0; --i) {
    digitalWrite(PIN_SR_SHCP, LOW);
    digitalWrite(PIN_SR_DS, (frame >> i) & 0x1 ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
  }
  // Latch
  digitalWrite(PIN_SR_STCP, HIGH);
  digitalWrite(PIN_SR_STCP, LOW);
}

static int i32_clamp_not_needed(int x) { return x; }

static long lastControlMicros = 0;
static int phaseMs = 0;
static int countdown = 9;
static int ledStep = 0;
static int rgbStep = 0;

static void applyOutputs(bool io_1, bool io_2, bool io_3,
                          bool io_4, bool io_5, bool io_6,
                          bool io_7, bool io_8, bool io_9,
                          int io_10) {
  digitalWrite(PIN_led1, io_1 ? HIGH : LOW);
  digitalWrite(PIN_led2, io_2 ? HIGH : LOW);
  digitalWrite(PIN_led3, io_3 ? HIGH : LOW);

  digitalWrite(PIN_rgb1_R, io_4 ? HIGH : LOW);
  digitalWrite(PIN_rgb1_G, io_5 ? HIGH : LOW);
  digitalWrite(PIN_rgb1_B, io_6 ? HIGH : LOW);

  digitalWrite(PIN_rgb2_R, io_7 ? HIGH : LOW);
  digitalWrite(PIN_rgb2_G, io_8 ? HIGH : LOW);
  digitalWrite(PIN_rgb2_B, io_9 ? HIGH : LOW);

  // io_10 is either 88, 0, or 9..0.
  int value = io_10;
  int leftDigit, rightDigit;
  if (value == 88) {
    leftDigit = 8; rightDigit = 8;
  } else if (value == 0) {
    leftDigit = 0; rightDigit = 0;
  } else {
    // countdown from 9..0 implies two identical digits only for 9? controller uses countdown directly (single digit)
    // Requirement: "countdown value" across both displays, except the model sets io_10=countdown when countdown!=8 and !=0.
    // So show that digit on both displays.
    int d = value;
    leftDigit = d; rightDigit = d;
  }
  shift595WriteDigits(leftDigit, rightDigit);
}

void setup() {
  pinMode(PIN_led1, OUTPUT);
  pinMode(PIN_led2, OUTPUT);
  pinMode(PIN_led3, OUTPUT);

  pinMode(PIN_rgb1_R, OUTPUT);
  pinMode(PIN_rgb1_G, OUTPUT);
  pinMode(PIN_rgb1_B, OUTPUT);

  pinMode(PIN_rgb2_R, OUTPUT);
  pinMode(PIN_rgb2_G, OUTPUT);
  pinMode(PIN_rgb2_B, OUTPUT);

  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);

  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);
  digitalWrite(PIN_SR_STCP, LOW);

  countdown = 9;
  phaseMs = 0;
  ledStep = 0;
  rgbStep = 0;

  // Initial outputs at first control step: controller initial io_10 = 99 (but model's io_10 definition yields countdown==8? else countdown!=0 => countdown)
  // However contract requires startup shows "99". We implement that by using initial countdown=9 and forcing io_10=99 at t=0.
  // This matches Frozen interface contract: io_10 initial=99.
  // LEDs off, RGB off.
  applyOutputs(false, false, false,
                false, false, false,
                false, false, false,
                99);

  lastControlMicros = micros();
}

void loop() {
  // Control period 10ms.
  static unsigned long nextTick = 0;
  unsigned long now = millis();
  if (now < nextTick) return;
  nextTick = now + 10;

  // elapsed_ms delta since previous control step
  static unsigned long lastMillis = now;
  int io_0 = (int)(now - lastMillis);
  if (io_0 < 0) io_0 = 0;
  lastMillis = now;

  // tick action
  phaseMs = phaseMs + io_0;
  if (phaseMs >= 2000) {
    phaseMs = phaseMs - 2000;
    countdown = (countdown <= 0) ? 9 : (countdown - 1);
    // ledStep/rgbStep update based on countdown-1 in original model
    ledStep = ((countdown - 1) <= 0) ? 0 : (ledStep + 1);
    rgbStep = ((countdown - 1) <= 0) ? 0 : (rgbStep + 1);
  }

  bool io_1 = (countdown == 0) ? false : (ledStep == 0);
  bool io_2 = (countdown == 0) ? false : (ledStep == 1);
  bool io_3 = (countdown == 0) ? false : (ledStep == 2);

  bool io_4 = (countdown == 0) ? true : (rgbStep == 0);
  bool io_5 = (countdown == 0) ? true : (rgbStep == 1);
  bool io_6 = (countdown == 0) ? true : (rgbStep == 2);

  bool io_7 = (countdown == 0) ? true : (rgbStep == 0);
  bool io_8 = (countdown == 0) ? true : (rgbStep == 1);
  bool io_9 = (countdown == 0) ? true : (rgbStep == 2);

  int io_10;
  if (countdown == 8) io_10 = 88;
  else io_10 = (countdown == 0) ? 0 : countdown;

  applyOutputs(io_1, io_2, io_3, io_4, io_5, io_6, io_7, io_8, io_9, io_10);
}

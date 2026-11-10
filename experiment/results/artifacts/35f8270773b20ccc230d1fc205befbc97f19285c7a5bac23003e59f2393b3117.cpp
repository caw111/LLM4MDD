#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
static constexpr uint8_t PIN_BTN1 = 2;   // D2, INPUT_PULLUP (active-low)
static constexpr uint8_t PIN_SR_DS  = A0; // sr1 DS (sr1 DS pin)
static constexpr uint8_t PIN_SR_STCP = A1; // shared latch clock
static constexpr uint8_t PIN_SR_SHCP = A2; // shared shift clock

// 7-seg (common anode). Shift-register outputs are active-low.
// Mapping for segments A..G (bits Q0..Q6) per digit (0..9).
// Each value is a 7-bit mask with bit0=A ... bit6=G.
// 1 means segment ON (anode high -> segment lit). We convert to active-low for SR outputs.
static constexpr uint8_t segOnMask[10] = {
  // A  B  C  D  E  F  G
  // 0: A B C D E F
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: B C
  (1 << 1) | (1 << 2),
  // 2: A B D E G
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3: A B C D G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4: B C F G
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5: A C D F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6: A C D E F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: A B C
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8: A B C D E F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: A B C D F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

// Active-low conversion for Q0..Q6 outputs (bit i corresponds to segment i)
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t activeLowSegBits7(uint8_t digit);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void displayTwoDigits(uint8_t tensDigit, uint8_t unitsDigit);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t activeLowSegBits7(uint8_t digit) {
  uint8_t onMask = segOnMask[digit % 10];
  // active-low means: segment ON => output LOW => stored bit should be 0 for on, 1 for off
  // So active-low bit value = NOT(onMask)
  return (uint8_t)(~onMask) & 0x7F;
}

// Writes two digits: tens on sr1 (sevseg1), units on sr2 (sevseg2).
// Wiring detail: sr1 DS->A0, shared SHCP->A2, shared STCP->A1.
// sr1 Q7S -> sr2 DS means the shift chain is sr2 after sr1.
// Thus, on a single shift sequence, we clock out bits for sr1 first (7 bits), then sr2 (7 bits).
// We clock exactly 16 bits total to fill Q0..Q7 for both registers; Q7 is not wired, keep OFF (HIGH -> 1).
static void displayTwoDigits(uint8_t tensDigit, uint8_t unitsDigit) {
  // SR shift registers are 74HC595-like:
  // We'll shift out 8 bits for sr1 (Q7S included as output to sr2 DS),
  // then 8 bits for sr2.
  //
  // For each register:
  // - Q0..Q6 drive segments A..G (active-low)
  // - Q7 unused; set to OFF => output HIGH => active-low bit = 1.
  //
  // Bit order in 74HC595 shift operation is typically MSB-first, but in Wokwi these are connected
  // as Q0..Q7 with shift-register chain. To ensure correct mapping, we use the standard:
  // clock data into DS on rising SHCP, and after 8 clocks STCP latches Q0..Q7 in order DS->Q7S?
  // However, deterministic correctness requires ordering; we follow common Arduino practice:
  // shift out MSB first with bit7..bit0 corresponding to Q7..Q0.
  //
  // Therefore, we construct bytes where bit0 corresponds to Q0, bit6->Q6, bit7->Q7.
  uint8_t sr1Bits7 = activeLowSegBits7(tensDigit);
  uint8_t sr2Bits7 = activeLowSegBits7(unitsDigit);

  uint8_t sr1Byte = (uint8_t)(sr1Bits7 | (1 << 7)); // Q7 off => HIGH => active-low bit = 1
  uint8_t sr2Byte = (uint8_t)(sr2Bits7 | (1 << 7));

  // Shift out: sr1 first then sr2, MSB-first within each byte.
  // Total 16 clocks.
  noInterrupts();
  digitalWrite(PIN_SR_STCP, LOW);

  // Shift 8 bits for sr1 (into sr1), then 8 bits for sr2 (through chain)
  uint16_t chain = ((uint16_t)sr1Byte << 8) | sr2Byte;

  for (int i = 15; i >= 0; --i) {
    digitalWrite(PIN_SR_SHCP, LOW);
    digitalWrite(PIN_SR_DS, (chain >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
  }

  digitalWrite(PIN_SR_STCP, HIGH); // latch
  digitalWrite(PIN_SR_STCP, LOW);
  interrupts();
}

// Model state
static int32_t numberState = 0;       // initial 0
static bool previousPressed = false;  // initial false

// Control period: not strictly enforced; behavior must update per press edge.
// We'll sample and update within loop quickly.
void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);

  pinMode(PIN_SR_DS, OUTPUT);   // A0
  pinMode(PIN_SR_STCP, OUTPUT); // A1
  pinMode(PIN_SR_SHCP, OUTPUT); // A2

  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_STCP, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);

  // Initialize displays to "00"
  numberState = 0;
  previousPressed = false;
  displayTwoDigits(0, 0);
}

void loop() {
  // io_0 in contract: model true when pressed; active_high=false, true_level=LOW
  // So io0 = (digitalRead == LOW)
  bool io_0 = (digitalRead(PIN_BTN1) == LOW);

  // tick per ACCEPTED model:
  // if (io_0 && !previousPressed) number := ((number+37) >= 100) ? 0 : (number+37)
  if (io_0 && !previousPressed) {
    int32_t next = numberState + 37;
    if (next >= 100) numberState = 0;
    else numberState = next;
  }
  previousPressed = io_0;

  // Output io_1 floor(number/1) = numberState (i32)
  // Map: tens display = floor(number/1) / 10? But contract only says io_1 is floor(number/1).
  // Our display behavior must show two-digit number with leading zeros.
  // Interpret numberState as the displayed integer (0..99). Preserve leading zeros:
  // tens = number/10, units = number%10.
  uint8_t v = (uint8_t)(numberState % 100);
  uint8_t tens = (uint8_t)(v / 10);
  uint8_t units = (uint8_t)(v % 10);
  displayTwoDigits(tens, units);

  // Keep loop responsive but deterministic enough for edge detection.
  delay(5);
}

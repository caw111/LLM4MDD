#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_BAR_A[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
static const uint8_t PIN_LED1 = 2; // led1:A
static const uint8_t PIN_LED2 = 3; // led2:A

// Shift register sr1 (74HC595) control pins from Mega
static const uint8_t PIN_SR_DS  = A0; // DS (serial data)
static const uint8_t PIN_SR_SHCP = A2; // SHCP (shift clock)
static const uint8_t PIN_SR_STCP = A1; // STCP (latch clock)
static const uint8_t PIN_SR_OE  = A?;  // OE connected to GND in wiring, but we keep it unused
static const uint8_t PIN_SR_MR  = A?;  // MR connected to Q's? not controlled; wiring ties MR to VCC? Actually sr1:MR is tied to VCC.

// sevseg1 segment mapping to 74HC595 Q outputs (based on wiring)
static const uint8_t Q_TO_SEG_A = 0; // Q0 -> sevseg A
static const uint8_t Q_TO_SEG_B = 1; // Q1 -> sevseg B
static const uint8_t Q_TO_SEG_C = 2; // Q2 -> sevseg C
static const uint8_t Q_TO_SEG_D = 3; // Q3 -> sevseg D
static const uint8_t Q_TO_SEG_E = 4; // Q4 -> sevseg E
static const uint8_t Q_TO_SEG_F = 5; // Q5 -> sevseg F
static const uint8_t Q_TO_SEG_G = 6; // Q6 -> sevseg G

// Bits for 74HC595 outputs Q0..Q6 correspond to segments A..G.
// Q7 is unused (not wired to anything in the provided connections).
static const uint8_t SEG_BIT_A = (1u << 0); // Q0
static const uint8_t SEG_BIT_B = (1u << 1); // Q1
static const uint8_t SEG_BIT_C = (1u << 2); // Q2
static const uint8_t SEG_BIT_D = (1u << 3); // Q3
static const uint8_t SEG_BIT_E = (1u << 4); // Q4
static const uint8_t SEG_BIT_F = (1u << 5); // Q5
static const uint8_t SEG_BIT_G = (1u << 6); // Q6

// Standard common-cathode style mapping for digits 0-9
// (Assumes "1" bit turns the corresponding segment ON.)
// Wiring doesn't indicate inversion; this matches typical Wokwi 7-segment behavior.
static const uint8_t digitToSegments[10] = {
  // 0: A B C D E F
  (SEG_BIT_A | SEG_BIT_B | SEG_BIT_C | SEG_BIT_D | SEG_BIT_E | SEG_BIT_F),
  // 1: B C
  (SEG_BIT_B | SEG_BIT_C),
  // 2: A B D E G
  (SEG_BIT_A | SEG_BIT_B | SEG_BIT_D | SEG_BIT_E | SEG_BIT_G),
  // 3: A B C D G
  (SEG_BIT_A | SEG_BIT_B | SEG_BIT_C | SEG_BIT_D | SEG_BIT_G),
  // 4: B C F G
  (SEG_BIT_B | SEG_BIT_C | SEG_BIT_F | SEG_BIT_G),
  // 5: A C D F G
  (SEG_BIT_A | SEG_BIT_C | SEG_BIT_D | SEG_BIT_F | SEG_BIT_G),
  // 6: A C D E F G
  (SEG_BIT_A | SEG_BIT_C | SEG_BIT_D | SEG_BIT_E | SEG_BIT_F | SEG_BIT_G),
  // 7: A B C
  (SEG_BIT_A | SEG_BIT_B | SEG_BIT_C),
  // 8: A B C D E F G
  (SEG_BIT_A | SEG_BIT_B | SEG_BIT_C | SEG_BIT_D | SEG_BIT_E | SEG_BIT_F | SEG_BIT_G),
  // 9: A B C D F G
  (SEG_BIT_A | SEG_BIT_B | SEG_BIT_C | SEG_BIT_D | SEG_BIT_F | SEG_BIT_G)
};

static uint8_t currentNumber = 0;

#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite7Seg(uint8_t segments7bits);
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBarGraph(uint8_t n);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateParityLEDs(uint8_t n);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 142 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite7Seg(uint8_t segments7bits)
{
  // segments7bits uses bits 0..6 -> Q0..Q6. Q7 left as 0.
  // Shift out MSB->LSB (typical): we want Q7 last; but since Q0..Q6 are used,
  // either order can break mapping. We'll shift MSB first for correctness with 74HC595:
  // After 8 shifts, Q0 corresponds to the last shifted bit.
  //
  // Here we compute byte with bit0->Q0 ... bit6->Q6.
  uint8_t byteOut = segments7bits & 0x7Fu;

  // Ensure latch clock low
  digitalWrite(PIN_SR_STCP, LOW);

  // Shift out 8 bits MSB->LSB
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(PIN_SR_DS, (byteOut & (1u << i)) ? HIGH : LOW);
    // Pulse shift clock
    digitalWrite(PIN_SR_SHCP, HIGH);
    digitalWrite(PIN_SR_SHCP, LOW);
  }

  // Latch
  digitalWrite(PIN_SR_STCP, HIGH);
  digitalWrite(PIN_SR_STCP, LOW);
}

static void updateBarGraph(uint8_t n)
{
  // bargraph1 wiring: A1..A10 are connected to Mega pins 4..13 respectively,
  // and each corresponding C1..C10 is tied to GND, so driving A pins HIGH turns segments on.
  // Light segments equal to current number: n -> first n segments ON.
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(PIN_BAR_A[i], (i < n) ? HIGH : LOW);
  }
}

static void updateParityLEDs(uint8_t n)
{
  if ((n & 1u) == 0u) {
    digitalWrite(PIN_LED1, HIGH);
    digitalWrite(PIN_LED2, LOW);
  } else {
    digitalWrite(PIN_LED1, LOW);
    digitalWrite(PIN_LED2, HIGH);
  }
}

void setup()
{
  // LEDs
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  // Bar graph direct GPIO pins (A1..A10 => pins 4..13)
  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(PIN_BAR_A[i], OUTPUT);
    digitalWrite(PIN_BAR_A[i], LOW); // fully off initially
  }

  // Shift register control pins
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);

  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);
  digitalWrite(PIN_SR_STCP, LOW);

  // Initialization requirements:
  // sevseg starts at "0"
  // bargraph fully off
  // both LEDs off
  currentNumber = 0;

  // Update sevseg to 0; bargraph and LEDs must be off initially.
  srWrite7Seg(digitToSegments[currentNumber]);
  // bargraph already off; LEDs already off
}

void loop()
{
  static unsigned long lastChangeMs = 0;

  unsigned long now = millis();
  if (now - lastChangeMs >= 2000UL) {
    lastChangeMs = now;

    // Increment with wrap 0..9
    currentNumber = (currentNumber + 1u) % 10u;

    // Update simultaneously when number changes:
    srWrite7Seg(digitToSegments[currentNumber]);
    updateBarGraph(currentNumber);
    updateParityLEDs(currentNumber);
  }
}

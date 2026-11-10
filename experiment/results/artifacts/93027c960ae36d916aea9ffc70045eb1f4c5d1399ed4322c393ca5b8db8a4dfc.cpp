#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>

// ===== Pins (from provided wiring) =====
// led1:A -> Mega pin 2
// led2:A -> Mega pin 3
static const uint8_t PIN_LED1 = 2;
static const uint8_t PIN_LED2 = 3;

// Shift register sr1 (74HC595):
// mega:A0  -> sr1:DS
// mega:A2  -> sr1:SHCP
// mega:A1  -> sr1:STCP
static const uint8_t PIN_SR_DS   = A0;
static const uint8_t PIN_SR_SHCP = A2;
static const uint8_t PIN_SR_STCP = A1;

// Bar graph bargraph1 uses direct GPIO pins for segment control:
// mega:4  -> bargraph1:A1  (segment index 0)
// mega:5  -> bargraph1:A2  (segment index 1)
// ...
// mega:13 -> bargraph1:A10 (segment index 9)
// Note: Each Cx is tied to GND per wiring, so setting Ax HIGH turns the segment ON.
static const uint8_t PIN_BAR_A[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// ===== 7-segment mapping via sr1 Q outputs =====
// Provided wiring:
// sr1:Q0 -> sevseg1:A
// sr1:Q1 -> sevseg1:B
// sr1:Q2 -> sevseg1:C
// sr1:Q3 -> sevseg1:D
// sr1:Q4 -> sevseg1:E
// sr1:Q5 -> sevseg1:F
// sr1:Q6 -> sevseg1:G
//
// Assumption: "1" bit turns the corresponding segment ON (common-cathode-style for Wokwi/sevseg).
static const uint8_t SEG_BIT_A = (1u << 0); // Q0
static const uint8_t SEG_BIT_B = (1u << 1); // Q1
static const uint8_t SEG_BIT_C = (1u << 2); // Q2
static const uint8_t SEG_BIT_D = (1u << 3); // Q3
static const uint8_t SEG_BIT_E = (1u << 4); // Q4
static const uint8_t SEG_BIT_F = (1u << 5); // Q5
static const uint8_t SEG_BIT_G = (1u << 6); // Q6

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

#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void srWrite7Seg(uint8_t segments7bits);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateBarGraph(uint8_t n);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateParityLEDs(uint8_t n);
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 142 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_107\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void srWrite7Seg(uint8_t segments7bits) {
  // segments7bits uses bits 0..6 -> Q0..Q6. Bit 7 left as 0.
  uint8_t byteOut = segments7bits & 0x7Fu;

  // Keep latch low before shifting
  digitalWrite(PIN_SR_STCP, LOW);

  // Shift out 8 bits MSB->LSB (standard 74HC595 behavior)
  // so that the final Q0..Q7 correspond to the shifted bit positions.
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(PIN_SR_DS, (byteOut & (1u << i)) ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
    digitalWrite(PIN_SR_SHCP, LOW);
  }

  // Latch output
  digitalWrite(PIN_SR_STCP, HIGH);
  digitalWrite(PIN_SR_STCP, LOW);
}

static void updateBarGraph(uint8_t n) {
  // Lights up segments equal to the current number:
  // n -> first n segments ON (A1..A10 correspond to indices 0..9).
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(PIN_BAR_A[i], (i < n) ? HIGH : LOW);
  }
}

static void updateParityLEDs(uint8_t n) {
  // If even: led1 ON, led2 OFF
  // If odd:  led1 OFF, led2 ON
  if ((n & 1u) == 0u) {
    digitalWrite(PIN_LED1, HIGH);
    digitalWrite(PIN_LED2, LOW);
  } else {
    digitalWrite(PIN_LED1, LOW);
    digitalWrite(PIN_LED2, HIGH);
  }
}

void setup() {
  // LEDs init (both off)
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  // Bar graph init (fully off)
  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(PIN_BAR_A[i], OUTPUT);
    digitalWrite(PIN_BAR_A[i], LOW);
  }

  // Shift register control pins
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);

  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);
  digitalWrite(PIN_SR_STCP, LOW);

  // Initialization requirements:
  // - sevseg starts at "0"
  // - bargraph fully off
  // - both LEDs off
  currentNumber = 0;

  srWrite7Seg(digitToSegments[currentNumber]);
  updateBarGraph(currentNumber);     // keeps bargraph fully off at start (n=0)
  updateParityLEDs(currentNumber);   // n=0 even -> led1 on, led2 off per parity rule
}

void loop() {
  static unsigned long lastChangeMs = 0;

  unsigned long now = millis();
  if (now - lastChangeMs >= 2000UL) {
    lastChangeMs = now;

    // Increment every exactly 2 seconds, wrap 0..9
    currentNumber = (currentNumber + 1u) % 10u;

    // Update simultaneously when number changes
    srWrite7Seg(digitToSegments[currentNumber]);
    updateBarGraph(currentNumber);
    updateParityLEDs(currentNumber);
  }
}

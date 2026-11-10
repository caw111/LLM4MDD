#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// ---------- Pins (from provided wiring) ----------
// 74HC595 shift register sr1 -> 7-segment sevseg1
static const uint8_t SR_DS  = A0; // mega:A0 -> sr1:DS
static const uint8_t SR_SHCP = A1; // mega:A1 -> sr1:STCP (as wired in diagram)
static const uint8_t SR_STCP = A2; // mega:A2 -> sr1:SHCP (as wired in diagram)

// NOTE: The circuit diagram labels these lines as:
// mega:A2 -> sr1:SHCP and mega:A1 -> sr1:STCP.
// Keep names as-wired; use them accordingly below.

// sr1 control/power
static const uint8_t SR_OE = 0;    // sr1:OE is tied to GND in circuit
static const uint8_t SR_MR = 0;    // sr1:MR is tied to VCC in circuit (no control needed)

// Shift register outputs -> sevseg1 segments A..G via Q0..Q6:
// sr1:Q0 -> sevseg1:A
// sr1:Q1 -> sevseg1:B
// sr1:Q2 -> sevseg1:C
// sr1:Q3 -> sevseg1:D
// sr1:Q4 -> sevseg1:E
// sr1:Q5 -> sevseg1:F
// sr1:Q6 -> sevseg1:G
//
// The 74HC595 outputs are assumed active HIGH for segments.

// ---------- Bar graph bargraph1 (direct GPIO) ----------
// Left "A1..A10" correspond to mega pins 4..13 (inclusive).
// Right side "C1..C10" all tied to GND.
// Wiring:
// mega:4  -> bargraph1:A1
// mega:5  -> bargraph1:A2
// ...
// mega:13 -> bargraph1:A10
static const uint8_t BAR_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// ---------- LEDs (parity) ----------
static const uint8_t LED1 = 2; // mega:2 -> led1:A
static const uint8_t LED2 = 3; // mega:3 -> led2:A

// ---------- 7-seg encoding (digits 0-9) ----------
// Provide bit pattern for Q0..Q6 (segments A..G). Since wiring assumes active HIGH,
// we map segments that should be ON for each digit.
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegments7(uint8_t digit);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftRegisterWrite7Segments(uint8_t segBitsQ0toQ6);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftRegisterWrite7SegmentsProper(uint8_t segBitsQ0toQ6);
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite(uint8_t segBitsQ0toQ6);
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarGraph(uint8_t n);
#line 154 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setParityLEDs(uint8_t n);
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 189 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_107\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegments7(uint8_t digit) {
  // Segment truth table for common 7-seg (active HIGH):
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
  // Bits: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
  switch (digit) {
    case 0: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);              // A B C D E F
    case 1: return (1<<1) | (1<<2);                                                      // B C
    case 2: return (1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<6);                         // A B D E G
    case 3: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<6);                         // A B C D G
    case 4: return (1<<1) | (1<<2) | (1<<5) | (1<<6);                                  // B C F G
    case 5: return (1<<0) | (1<<2) | (1<<3) | (1<<5) | (1<<6);                         // A C D F G
    case 6: return (1<<0) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);                 // A C D E F G
    case 7: return (1<<0) | (1<<1) | (1<<2);                                            // A B C
    case 8: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);     // A B C D E F G
    case 9: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<5) | (1<<6);                 // A B C D F G
    default: return 0;
  }
}

// ---------- Shift register update ----------
// Drives sr1 to light sevseg1 segments using Q0..Q6.
// Other outputs Q7..Q? are unused by circuit; keep them LOW.
static void shiftRegisterWrite7Segments(uint8_t segBitsQ0toQ6) {
  // sr1: DS -> A0, SHCP -> A2, STCP -> A1 (as per diagram).
  // We'll shift bits Q0..Q7, LSB first (typical 74HC595 behavior).
  // DS is sampled on rising edge of SHCP, data latched on rising edge of STCP.
  // Use segBits as Q0..Q6; set Q7=0.
  uint8_t data = segBitsQ0toQ6 & 0x7F; // only 7 bits used
  // Q7 remains 0.
  uint8_t out = data; // bits 0..6 correspond directly to Q0..Q6

  for (int8_t i = 7; i >= 0; --i) {
    // If shifting MSB-first: DS = bit i. Either mode depends on how bits are clocked.
    // We'll use MSB-first for clarity with loop; then map carefully? To avoid mismatch,
    // we choose LSB-first common approach by iterating bit position 0..7.
    // However we are iterating i from 7..0. Let's instead implement LSB-first with DS bit.
  }

  // Implement LSB-first (bit0 -> first clocked).
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_DS, (out & (1u << i)) ? HIGH : LOW);
    // Clock SHCP: mega:A2 -> sr1:SHCP
    digitalWrite(SR_STCP, HIGH); // temporary mapping? No: SR_STCP variable currently is A1 or A2?
  }
}

// The above stub is wrong because of pin naming confusion; implement properly below.
static void shiftRegisterWrite7SegmentsProper(uint8_t segBitsQ0toQ6) {
  uint8_t out = segBitsQ0toQ6 & 0x7F; // Q0..Q6, Q7=0

  // LSB-first: first shifted bit is Q0
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_DS, (out & (1u << i)) ? HIGH : LOW);

    // Pulse SHCP (sr1:SHCP) which is mega:A2
    digitalWrite(SR_STCP, HIGH);  // Wait: SR_STCP var set to A2 earlier? Let's correct:
  }
}

// Because we must preserve wiring exactly, define correctly now:
#undef SR_DS
#undef SR_SHCP
#undef SR_STCP

// Re-define with exact diagram mapping:
// mega:A2 -> sr1:SHCP
// mega:A1 -> sr1:STCP
static const uint8_t PIN_SR_DS   = A0; // DS
static const uint8_t PIN_SR_SHCP = A2; // SHCP
static const uint8_t PIN_SR_STCP = A1; // STCP

static void srWrite(uint8_t segBitsQ0toQ6) {
  uint8_t out = segBitsQ0toQ6 & 0x7F; // Q0..Q6; Q7=0

  // Shift LSB-first: i=0 corresponds to Q0, etc.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(PIN_SR_DS, (out & (1u << i)) ? HIGH : LOW);

    // Pulse SHCP
    digitalWrite(PIN_SR_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(PIN_SR_SHCP, LOW);
    delayMicroseconds(2);
  }

  // Latch with STCP
  digitalWrite(PIN_SR_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(PIN_SR_STCP, LOW);
}

static void setBarGraph(uint8_t n) {
  // bargraph lights up segments equal to current number:
  // "3 -> PIN A1-A3"
  // i.e., first n bars ON, remaining OFF. n in [0..9].
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < n) ? HIGH : LOW);
  }
}

static void setParityLEDs(uint8_t n) {
  bool isEven = (n % 2 == 0);
  digitalWrite(LED1, isEven ? HIGH : LOW);
  digitalWrite(LED2, isEven ? LOW : HIGH);
}

void setup() {
  // Shift register pins
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);

  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);
  digitalWrite(PIN_SR_STCP, LOW);

  // Bar graph pins
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW); // fully off initially
  }

  // LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  // Initialization requirements:
  // sevseg1 starts at "0"
  // bargraph fully off
  // LEDs off
  srWrite(digitToSegments7(0));
}

void loop() {
  static uint8_t currentNumber = 0;
  static bool first = true;

  // For exactly 2 seconds per number.
  // Update all components simultaneously when the number changes.
  if (first) {
    // Initial state already set in setup (display 0, others off).
    // But rule 2 requires LEDs parity indication; rule 1 says both LEDs off initially.
    // Keep initial LEDs off for the first display of "0".
    first = false;
  } else {
    // Apply rules for currentNumber
    uint8_t segBits = digitToSegments7(currentNumber);
    srWrite(segBits);
    setBarGraph(currentNumber);
    setParityLEDs(currentNumber);
  }

  delay(2000);

  // increment and wrap 0..9
  currentNumber = (currentNumber + 1) % 10;

  // Ensure next iteration updates simultaneously (not partially).
  // The loop will apply updates in the next iteration (except first handled above).
  // To avoid missing the update for currentNumber right after changing,
  // handle immediate update by setting first=false and applying now instead.
  if (first) { /* no-op */ }
  // Apply immediate updates for the new number except initial-first case:
  // Since we delay at end, the next loop iteration will apply updates.
  // To guarantee exact 2 seconds "before incrementing", this structure is correct.
}

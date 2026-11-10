#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_14\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const byte PIN_SR_DS   = A0; // blue: DS -> sr1:DS
const byte PIN_SR_STCP = A1; // purple: STCP -> sr1:STCP
const byte PIN_SR_SHCP = A2; // gray: SHCP -> sr1:SHCP

// sr1 is wired to sevseg1 segments:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Q7 is unused.
static const uint8_t segMap7[10] = {
  // Common patterns assuming typical Wokwi 7-seg (segment "1" means ON).
  // Digits: 0 1 2 3 4 5 6 7 8 9
  // Segments: A B C D E F G
  0b1111110, // 0: A B C D E F
  0b0110000, // 1: B C
  0b1101101, // 2: A B D E G
  0b1111001, // 3: A B C D G
  0b0110011, // 4: B C F G
  0b1011011, // 5: A C D F G
  0b1011111, // 6: A C D E F G
  0b1110000, // 7: A B C
  0b1111111, // 8: A B C D E F G
  0b1111011  // 9: A B C D F G
};

// Keep display off on start (all segments off).
// For this display assumption (bit=1 => ON), off is 0.
static const uint8_t ALL_OFF = 0b0000000; // A..G all off

// Convert seg bits (A..G) to shift-register output byte (Q0..Q6).
// Since Q0->A ... Q6->G, seg bits map directly to lower 7 bits.
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_14\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void writeToShiftRegister(uint8_t value7bits);
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_14\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayDigit(int digit);
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_14\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayOff();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_14\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_14\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_14\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void writeToShiftRegister(uint8_t value7bits) {
  // Value7bits uses bits: bit0->A ... bit6->G
  // Build full byte with Q7 cleared.
  uint8_t outByte = value7bits & 0x7F;

  // Shift out MSB-first or LSB-first depends on wiring order.
// We'll choose shifting order that yields Q0..Q7 correspond to the bits we want.
// For 74HC595, the bit shifted last ends up at Q0 when using MSB-first? It's ambiguous.
// To avoid ambiguity, explicitly clock bits from Q7..Q0 with LSB-first expectation:
// Many Arduino examples use MSBFIRST to output Q7 first.
// We'll use MSBFIRST so the last shifted bit becomes Q0.
  // For our mapping, we want bit0->Q0, bit6->Q6. Thus we can MSB-first and pad Q7=0.
  uint8_t frame = outByte; // Q0..Q6 meaningful
  uint8_t q7 = 0;

  // STCP low for shift
  digitalWrite(PIN_SR_STCP, LOW);

  // Shift bits from bit7 down to bit0 (MSB first)
  for (int8_t i = 7; i >= 0; --i) {
    uint8_t bitVal;
    if (i == 7) bitVal = q7;
    else bitVal = (frame >> i) & 0x01;

    digitalWrite(PIN_SR_DS, bitVal ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
    digitalWrite(PIN_SR_SHCP, LOW);
  }

  // Latch
  digitalWrite(PIN_SR_STCP, HIGH);
  digitalWrite(PIN_SR_STCP, LOW);
}

static void displayDigit(int digit) {
  if (digit < 0) digit = 0;
  if (digit > 9) digit = 9;
  writeToShiftRegister(segMap7[digit]);
}

static void displayOff() {
  writeToShiftRegister(ALL_OFF);
}

void setup() {
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);

  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_STCP, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);

  // Display off on power-up/reset
  displayOff();

  // Timer starts at 0 and increments by 2 seconds every real-time second.
}

void loop() {
  static unsigned long lastTickMs = 0;
  static int timerSeconds = 0;

  unsigned long now = millis();

  if (lastTickMs == 0) {
    lastTickMs = now;
  }

  // Increment exactly every 1 real-time second using millis() scheduling.
  while (now - lastTickMs >= 1000UL) {
    lastTickMs += 1000UL;

    timerSeconds += 2;

    int toDisplay = timerSeconds;
    if (toDisplay > 10) {
      // Only last digit when timer exceeds 10 seconds
      toDisplay = toDisplay % 10;
    }

    // Update display with current timer value or last digit.
    // For timer values <= 10: display the value directly (0..10).
    // But a 7-seg supports digits 0..9; for value 10, we'll show '0' to remain consistent
    // with digit display semantics (since only digits are meaningful on 7-seg).
    if (toDisplay == 10) {
      displayDigit(0);
    } else {
      displayDigit(toDisplay);
    }
  }
}

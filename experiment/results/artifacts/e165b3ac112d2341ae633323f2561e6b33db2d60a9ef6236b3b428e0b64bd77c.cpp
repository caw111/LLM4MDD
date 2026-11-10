#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>

// Segment patterns for common-anode 7-seg (A..G)
// Bit mapping: bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
static const uint8_t SEG_MAP[10] = {
  // 0
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1
  (1 << 1) | (1 << 2),
  // 2
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

// Wiring (as given):
// A0 -> sr1 DS
// A2 -> shared SHCP (shift clock) with sr2
// A1 -> shared STCP (latch clock) with sr2
// Button: D2 as INPUT_PULLUP, pressed = LOW
static const uint8_t PIN_DS    = A0; // sr1 DS (data input)
static const uint8_t PIN_SHCP  = A2; // shift clock
static const uint8_t PIN_STCP  = A1; // latch clock
static const uint8_t PIN_BTN   = 2;  // button

static uint8_t currentValue = 0;

#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint8_t segToSrByte(uint8_t digit);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void writeDigits(uint8_t value);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline uint8_t segToSrByte(uint8_t digit) {
  // Common-anode: segment ON => HIGH on 74HC595 outputs.
  // Map A..G to Q0..Q6.
  return SEG_MAP[digit] & 0x7F;
}

static void writeDigits(uint8_t value) {
  uint8_t tens = value / 10;
  uint8_t ones = value % 10;

  // Cascaded 74HC595:
  // Shift 16 bits: sr1 first (upper byte), then sr2 (lower byte),
  // both sharing SR clocks; sr1 Q7S feeds sr2 DS.
  uint8_t sr1Byte = segToSrByte(tens) & 0x7F; // Q0..Q6 => A..G
  uint8_t sr2Byte = segToSrByte(ones) & 0x7F; // Q0..Q6 => A..G

  uint16_t frame = (uint16_t(sr1Byte) << 8) | sr2Byte;

  noInterrupts();
  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  for (int8_t i = 15; i >= 0; --i) {
    digitalWrite(PIN_DS, ((frame >> i) & 0x01) ? HIGH : LOW);
    digitalWrite(PIN_SHCP, HIGH);
    digitalWrite(PIN_SHCP, LOW);
  }

  digitalWrite(PIN_STCP, HIGH);
  digitalWrite(PIN_STCP, LOW);
  interrupts();
}

void setup() {
  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);

  digitalWrite(PIN_DS, LOW);
  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  // Initialization display "00"
  currentValue = 0;
  writeDigits(currentValue);
}

void loop() {
  // Edge detection for low-going press
  static bool prevPressed = false;

  bool pressed = (digitalRead(PIN_BTN) == LOW);

  if (pressed && !prevPressed) {
    uint16_t nextVal = (uint16_t)currentValue + 37u;
    if (nextVal > 99u) currentValue = 0;
    else currentValue = (uint8_t)nextVal;

    // Immediate update
    writeDigits(currentValue);
  }

  prevPressed = pressed;
}

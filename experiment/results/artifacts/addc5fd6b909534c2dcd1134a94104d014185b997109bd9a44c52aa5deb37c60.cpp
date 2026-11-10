#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Segment patterns for common-anode 7-seg (A..G)
static const uint8_t SEG_MAP[10] = {
  // A  B  C  D  E  F  G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),                  0, // 0
  (1 << 1) | (1 << 2),                                                                 0, // 1
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),                              0, // 2
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),                              0, // 3
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),                                         0, // 4
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),                              0, // 5
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),                 0, // 6
  (1 << 0) | (1 << 1) | (1 << 2),                                                    0, // 7
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),    0, // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)                  0  // 9
};

// The circuit uses two 74HC595:
// sr1: digit (tens) segments connected to Q0..Q6 -> A..G; Q7S cascades into sr2 DS.
// sr2: digit (units) segments connected to Q0..Q6 -> A..G.
static const uint8_t SR1_SEG_BITS_MASK = 0x7F; // bits 0..6 used
static const uint8_t SR2_SEG_BITS_MASK = 0x7F;

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t segToSrByte(uint8_t digit);
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void writeDigits(uint8_t value);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline uint8_t segToSrByte(uint8_t digit) {
  // Common-anode, segment ON corresponds to HIGH on Q outputs.
  // We map A..G into Q0..Q6 respectively.
  // SEG_MAP stores exactly bits A..G in positions 0..6.
  return (SEG_MAP[digit] & 0x7F);
}

// Wiring (as given):
// A0 -> sr1 DS
// A2 -> shared SHCP (shift clock) with sr2
// A1 -> shared STCP (latch clock) with sr2
// Button: D2 as INPUT_PULLUP, pressed = LOW
static const uint8_t PIN_DS = A0;   // sr1 DS (data input)
static const uint8_t PIN_SHCP = A2; // shift clock
static const uint8_t PIN_STCP = A1; // latch clock
static const uint8_t PIN_BTN = 2;   // button

static uint8_t currentValue = 0;

static void writeDigits(uint8_t value) {
  uint8_t tens = value / 10;
  uint8_t ones = value % 10;

  // For cascaded 74HC595 (sr1 -> sr2 via sr1 Q7S -> sr2 DS):
  // Shift 16 bits total. We send for sr1 first (upper byte), then sr2 (lower byte),
  // and both share SR clocks, then latch.
  //
  // Bit positions in shifted bytes:
  // - Q0..Q6 correspond to segments A..G.
  // - Q7 is unused; keep 0.
  uint8_t sr1Byte = segToSrByte(tens) & SR1_SEG_BITS_MASK; // tens segments
  uint8_t sr2Byte = segToSrByte(ones) & SR2_SEG_BITS_MASK; // ones segments

  uint16_t frame = ((uint16_t)sr1Byte << 8) | sr2Byte;

  noInterrupts();
  // Ensure clock low before shifting
  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  // Shift MSB first for the 16-bit frame:
  // MSB corresponds to sr1Byte bit7 (Q7 of sr1), ... down to sr1Byte bit0 (Q0 of sr1),
  // then sr2Byte bit7..bit0.
  for (int8_t i = 15; i >= 0; --i) {
    digitalWrite(PIN_DS, (frame >> i) & 0x01 ? HIGH : LOW);
    // Pulse shift clock
    digitalWrite(PIN_SHCP, HIGH);
    digitalWrite(PIN_SHCP, LOW);
  }

  // Latch outputs
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

  // Initialization: display "00"
  currentValue = 0;
  writeDigits(currentValue);
}

void loop() {
  // Edge detection on button press (low-going)
  static bool prevPressed = false;

  bool pressed = (digitalRead(PIN_BTN) == LOW);

  if (pressed && !prevPressed) {
    // Each press increments by 37; if exceeds 99 -> reset to 0
    uint16_t nextVal = (uint16_t)currentValue + 37u;
    if (nextVal > 99u) currentValue = 0;
    else currentValue = (uint8_t)nextVal;

    // Display update immediately after button press
    writeDigits(currentValue);

    // Ensure within 20ms it updates (already immediate). No extra delay to avoid double count.
  }

  prevPressed = pressed;
}

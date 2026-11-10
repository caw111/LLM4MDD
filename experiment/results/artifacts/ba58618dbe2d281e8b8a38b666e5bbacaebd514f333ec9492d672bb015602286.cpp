#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_11\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_DS   = A0; // sr1:DS (blue) -> mega:A0
static const uint8_t PIN_STCP = A1; // sr1:STCP (purple) -> mega:A1
static const uint8_t PIN_SHCP = A2; // sr1:SHCP (gray) -> mega:A2
static const uint8_t PIN_MR   = 7;  // sr1:MR not wired to a specific MCU pin (connected to VCC in circuit)

static const uint8_t SHIFTREG_OE_CONTROL = 255; // sr1:OE tied to GND in circuit (always enabled)

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_11\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t segMapCommonAnodeFor595(char c);
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_11\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut595(uint8_t data);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_11\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayOff();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_11\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayChar(char c);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_11\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_11\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_11\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t segMapCommonAnodeFor595(char c) {
  // Assumption (standard for 7-seg + 74HC595): sr1 outputs drive segments with
  // ON = HIGH (common anode would be inverted, but circuit doesn't specify).
  // Use conventional segment encodings (a,b,c,d,e,f,g) => bit order:
  // Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G.
  //
  // Encodings for ON=1:
  // 0: a b c d e f = 1, g = 0 -> 0b0111111 = 0x3F (a..g)
  // A: a b c e f g = 1, d=0 -> 0b1110111 = 0x77
  // P: a b e f g = 1, c=0, d=0 -> 0b1110011 = 0x73
  switch (c) {
    case '0': return 0x3F; // a b c d e f on, g off
    case 'A': return 0x77; // a b c e f g on, d off
    case 'P': return 0x73; // a b e f g on, c/d off
    default:  return 0x00; // blank
  }
}

static void shiftOut595(uint8_t data) {
  // We shift out bits MSB->LSB or LSB->MSB depending on physical mapping.
  // Here we map Q0->A, Q1->B, ... Q6->G, with bit0..bit6 representing segments a..g.
  // Use LSB-first so that bit0 ends up at Q0 (A), bit1 at Q1 (B), etc.
  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  for (uint8_t i = 0; i < 8; i++) { // 8 bits to satisfy 74HC595
    uint8_t bitVal = (data >> i) & 0x01; // bit i -> Q i
    digitalWrite(PIN_DS, bitVal ? HIGH : LOW);
    // Pulse SHCP
    digitalWrite(PIN_SHCP, HIGH);
    digitalWrite(PIN_SHCP, LOW);
  }

  // Latch to outputs
  digitalWrite(PIN_STCP, HIGH);
  digitalWrite(PIN_STCP, LOW);
}

static void displayOff() {
  // Turn off all segments: set a..g bits to 0.
  // Q0..Q6 correspond to A..G, Q7 unused.
  shiftOut595(0x00);
}

static void displayChar(char c) {
  uint8_t pattern = segMapCommonAnodeFor595(c);
  shiftOut595(pattern);
}

void setup() {
  // The circuit specifies Arduino Mega 2560.
  // The prompt says "use UNO" in prose; however wiring uses the mega pins A0..A2.
  // We'll still target the Mega 2560 as specified in the circuit.

  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);

  digitalWrite(PIN_DS, LOW);
  digitalWrite(PIN_STCP, LOW);
  digitalWrite(PIN_SHCP, LOW);

  // Initialization: display should be off after power/reset
  displayOff();
}

void loop() {
  static uint8_t state = 0; // 0='0', 1='A', 2='P'
  static unsigned long stateStart = 0;

  unsigned long now = millis();
  if (stateStart == 0) stateStart = now;

  char currentChar = (state == 0) ? '0' : (state == 1) ? 'A' : 'P';
  displayChar(currentChar);

  if (now - stateStart >= 2000UL) {
    state = (state + 1) % 3;
    stateStart = now;
  }
}

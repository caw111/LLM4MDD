#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Wiring from the circuit:
// 7-seg via 74HC595:
//   mega:A2 -> SHCP (SR clock)
//   mega:A1 -> STCP (latch)
//   mega:A0 -> DS   (data)
//   mega:GND.3 -> 74HC595 GND
//   74HC595 MR tied high (no direct reset control needed)
//
// Button:
//   mega:2  -> btn1 1.l
//   mega:GND.1 -> btn1 2.r
// So: button connects input to GND when pressed; use INPUT_PULLUP.

static const uint8_t PIN_SR_DATA  = A0; // DS
static const uint8_t PIN_SR_LATCH = A1; // STCP
static const uint8_t PIN_SR_CLOCK = A2; // SHCP

static const uint8_t PIN_BTN = 2;

// 7-seg digit patterns for common types:
// In the given wiring, 74HC595 outputs are connected to segments A..G and COM.1.
// The segment coding below assumes a common-cathode style display where HIGH turns a segment ON.
// (If your display is common-anode, invert the pattern.)
//
// Bit mapping based on circuit:
//   SR Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
//   Q7 unused
static const uint8_t digitToSegments[10] = {
  // 0: a b c d e f g
  // 0bGFEDCBA (not used) -> We'll build as bit0=A, bit1=B, ... bit6=G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),                         // 0
  (1<<1)|(1<<2),                                                       // 1
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                                  // 2
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                                  // 3
  (1<<1)|(1<<2)|(1<<5)|(1<<6),                                         // 4
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                                  // 5
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),                          // 6
  (1<<0)|(1<<1)|(1<<2),                                                 // 7
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),                   // 8
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)                            // 9
};

// Debounce / button handling
static uint8_t counter = 0;
static bool lastStablePressed = false;
static bool lastReadingPressed = false;
static unsigned long lastDebounceTimeMs = 0;
static const unsigned long debounceDelayMs = 35;

#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void write595(uint8_t value);
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayDigit(uint8_t d);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void write595(uint8_t value)
{
  // Shift out MSB->LSB? The 74HC595 shift order depends on bit order.
  // We'll output bit0 on the first shifted position by using LSBFIRST.
  // This matches mapping: Q0..Q6 correspond to bits 0..6 we prepare.
  // Data shifted to SR; then latch to outputs.

  digitalWrite(PIN_SR_LATCH, LOW);
  shiftOut(PIN_SR_DATA, PIN_SR_CLOCK, LSBFIRST, value);
  digitalWrite(PIN_SR_LATCH, HIGH);
}

static void displayDigit(uint8_t d)
{
  if (d > 9) d = 0;
  write595(digitToSegments[d] & 0x7F);
}

void setup()
{
  pinMode(PIN_SR_DATA, OUTPUT);
  pinMode(PIN_SR_LATCH, OUTPUT);
  pinMode(PIN_SR_CLOCK, OUTPUT);

  // Initialize control lines to safe state
  digitalWrite(PIN_SR_LATCH, LOW);
  digitalWrite(PIN_SR_CLOCK, LOW);
  digitalWrite(PIN_SR_DATA, LOW);

  pinMode(PIN_BTN, INPUT_PULLUP); // pressed -> GND -> LOW

  counter = 0;
  displayDigit(counter);
}

void loop()
{
  // Read raw button (pressed = LOW)
  bool readingPressed = (digitalRead(PIN_BTN) == LOW);

  // Debounce logic
  if (readingPressed != lastReadingPressed) {
    lastDebounceTimeMs = millis();
    lastReadingPressed = readingPressed;
  }

  if ((millis() - lastDebounceTimeMs) >= debounceDelayMs) {
    if (readingPressed != lastStablePressed) {
      lastStablePressed = readingPressed;

      // Count only on press event (LOW transition -> pressed)
      if (lastStablePressed) {
        counter += 3;
        if (counter > 9) counter = 0;
        displayDigit(counter);
      }
    }
  }
}

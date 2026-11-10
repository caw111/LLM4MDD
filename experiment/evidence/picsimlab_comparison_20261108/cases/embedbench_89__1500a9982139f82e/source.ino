#include <Arduino.h>

static const uint8_t BTN_PINS[3] = {2, 3, 4}; // btn1, btn2, btn3 on Mega
// 74HC595 SR1 pins
static const uint8_t PIN_SHCP = A2; // sr1:SHCP
static const uint8_t PIN_STCP = A1; // sr1:STCP
static const uint8_t PIN_DS   = A0; // sr1:DS

// 74HC595 outputs to 7-seg segments:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Q7 is unused.

const uint8_t SEG_MAP[10] = {
  // Bits: Q6..Q0 -> G F E D C B A
  // We'll assemble byte as bit0=A, bit1=B, ... bit6=G for convenience,
  // then map to DS->Q0..Q7 with same bit positions.
  // For common cathode: segments on = 1.
  // Digit patterns for segments A B C D E F G
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
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static uint8_t digitToSegmentsByte(int digit) {
  // byte bits: bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
  // Build from known segment names.
  // Patterns:
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

  bool A=false,B=false,C=false,D=false,E=false,F=false,G=false;
  switch (digit) {
    case 0: A=1;B=1;C=1;D=1;E=1;F=1;G=0; break;
    case 1: A=0;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 2: A=1;B=1;C=0;D=1;E=1;F=0;G=1; break;
    case 3: A=1;B=1;C=1;D=1;E=0;F=0;G=1; break;
    case 4: A=0;B=1;C=1;D=0;E=0;F=1;G=1; break;
    case 5: A=1;B=0;C=1;D=1;E=0;F=1;G=1; break;
    case 6: A=1;B=0;C=1;D=1;E=1;F=1;G=1; break;
    case 7: A=1;B=1;C=1;D=0;E=0;F=0;G=0; break;
    case 8: A=1;B=1;C=1;D=1;E=1;F=1;G=1; break;
    case 9: A=1;B=1;C=1;D=1;E=0;F=1;G=1; break;
  }
  uint8_t byteVal = 0;
  if (A) byteVal |= (1u << 0);
  if (B) byteVal |= (1u << 1);
  if (C) byteVal |= (1u << 2);
  if (D) byteVal |= (1u << 3);
  if (E) byteVal |= (1u << 4);
  if (F) byteVal |= (1u << 5);
  if (G) byteVal |= (1u << 6);
  return byteVal;
}

// 74HC595 shifts out DS->Q0..Q7 in a typical way.
// We'll ensure Q0 gets the least-significant bit.
static void shiftOut595(uint8_t data) {
  // ds pin = A0, shcp = A2, stcp = A1
  // Shift 8 bits MSB->LSB or LSB->MSB depends on wiring. We'll match Q0=bit0
  // by shifting MSB..LSB? We'll implement MSB->LSB such that last shifted bit ends at Q0.
  // Standard Arduino shiftOut uses MSBFIRST/LBFIRST; but we implement manually.
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_SHCP, LOW);
    digitalWrite(PIN_STCP, LOW);
    uint8_t bitVal = (data >> i) & 0x01;
    digitalWrite(PIN_DS, bitVal ? HIGH : LOW);
    digitalWrite(PIN_SHCP, HIGH);
  }
  digitalWrite(PIN_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(PIN_STCP, LOW);
}

static void displayNumberNibble(uint8_t count) {
  uint8_t digit = (count <= 9) ? count : (count % 10);
  uint8_t segByte = digitToSegmentsByte(digit);
  shiftOut595(segByte); // segment bits A..G via Q0..Q6
}

// RGB wiring:
// rgb1: R=5, G=6, B=7 (common cathode) => HIGH turns ON
// rgb2: R=8, G=9, B=10
static const uint8_t RGB1_R = 5;
static const uint8_t RGB1_G = 6;
static const uint8_t RGB1_B = 7;
static const uint8_t RGB2_R = 8;
static const uint8_t RGB2_G = 9;
static const uint8_t RGB2_B = 10;

static void setRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin, bool r, bool g, bool b) {
  digitalWrite(rpin, r ? HIGH : LOW);
  digitalWrite(gpin, g ? HIGH : LOW);
  digitalWrite(bpin, b ? HIGH : LOW);
}

static void applyColorFromCount(uint8_t count) {
  // Initial at power-up: count=0, both RGB off.
  // Rules for count:
  // <5  : rgb1 red,    rgb2 blue
  // 5-9 : rgb1 green,  rgb2 yellow (red+green)
  // >=10 : rgb1 purple (red+blue), rgb2 cyan (green+blue)
  // If count exceeds 15 -> reset to 0 and LEDs return to initial (off).
  // If count==0: initial state -> off/off (per initialization requirement).
  if (count == 0) {
    setRGB(RGB1_R, RGB1_G, RGB1_B, 0, 0, 0);
    setRGB(RGB2_R, RGB2_G, RGB2_B, 0, 0, 0);
    return;
  }

  if (count < 5) {
    setRGB(RGB1_R, RGB1_G, RGB1_B, 1, 0, 0); // red
    setRGB(RGB2_R, RGB2_G, RGB2_B, 0, 0, 1); // blue
  } else if (count <= 9) {
    setRGB(RGB1_R, RGB1_G, RGB1_B, 0, 1, 0); // green
    setRGB(RGB2_R, RGB2_G, RGB2_B, 1, 1, 0); // yellow (R+G)
  } else { // 10..15
    setRGB(RGB1_R, RGB1_G, RGB1_B, 1, 0, 1); // purple (R+B)
    setRGB(RGB2_R, RGB2_G, RGB2_B, 0, 1, 1); // cyan (G+B)
  }
}

// Debounce (per button), and trigger on press (LOW->HIGH or HIGH->LOW depending on pullups).
// Circuit: btn has right side connected to mega:GND. left side to input pin through green connection.
// No explicit pullups shown; we'll enable INPUT_PULLUP so unpressed reads HIGH, pressed reads LOW.
static const uint16_t DEBOUNCE_MS = 35;

static uint8_t btnPrevStable[3];
static uint8_t btnLastRaw[3];
static unsigned long btnLastChangeMs[3];

static bool buttonPressedEvent(uint8_t idx) {
  uint8_t raw = digitalRead(BTN_PINS[idx]); // HIGH=not pressed, LOW=pressed (due to pull-up)
  bool pressedEvent = false;

  if (raw != btnLastRaw[idx]) {
    btnLastRaw[idx] = raw;
    btnLastChangeMs[idx] = millis();
  }

  // If raw has remained unchanged for debounce period, accept it as stable.
  if ((millis() - btnLastChangeMs[idx]) >= DEBOUNCE_MS) {
    if (raw != btnPrevStable[idx]) {
      btnPrevStable[idx] = raw;
      // Press event: transition to LOW
      if (btnPrevStable[idx] == LOW) {
        pressedEvent = true;
      }
    }
  }

  return pressedEvent;
}

uint8_t count = 0;

void setup() {
  // Buttons
  for (int i = 0; i < 3; i++) {
    pinMode(BTN_PINS[i], INPUT_PULLUP);
  }

  // 74HC595 control pins
  pinMode(PIN_SHCP, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_DS, OUTPUT);

  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);
  digitalWrite(PIN_DS, LOW);

  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Initialization requirement: sevseg shows "0", both RGB off.
  count = 0;
  applyColorFromCount(count);
  displayNumberNibble(count);

  // Initialize debounce state
  for (int i = 0; i < 3; i++) {
    uint8_t raw = digitalRead(BTN_PINS[i]);
    btnLastRaw[i] = raw;
    btnPrevStable[i] = raw;
    btnLastChangeMs[i] = millis();
  }
}

void loop() {
  bool inc = false;

  for (int i = 0; i < 3; i++) {
    if (buttonPressedEvent(i)) {
      inc = true;
    }
  }

  if (inc) {
    count++;
    if (count > 15) count = 0; // reset as specified
    displayNumberNibble(count);
    applyColorFromCount(count);
  }
}
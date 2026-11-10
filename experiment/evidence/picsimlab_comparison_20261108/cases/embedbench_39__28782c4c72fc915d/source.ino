#include <Arduino.h>

const int PIN_SHCP = A2; // sr1:SHCP (gray)
const int PIN_STCP = A1; // sr1:STCP (purple)
const int PIN_DS   = A0; // sr1:DS   (blue)

// Using the 8-bit shift register sr1 outputs Q0..Q7 to:
// Q0->sevseg1:A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Q7 is unused.

static void shiftOutToSR(uint8_t value) {
  // Ensure data is set up before SHCP rising edges.
  digitalWrite(PIN_DS, (value & 0x80) ? HIGH : LOW);
  // Shift 8 bits: drive on falling, latch on rising to match typical 74HC595 usage.
  for (int i = 0; i < 8; i++) {
    // Pulse SHCP
    digitalWrite(PIN_SHCP, LOW);
    delayMicroseconds(1);
    digitalWrite(PIN_SHCP, HIGH);
    delayMicroseconds(1);

    value <<= 1;
    digitalWrite(PIN_DS, (value & 0x80) ? HIGH : LOW);
  }

  // Latch into outputs
  digitalWrite(PIN_STCP, LOW);
  delayMicroseconds(1);
  digitalWrite(PIN_STCP, HIGH);
  delayMicroseconds(1);
}

// Segment bit positions for sevseg1 via sr1 Q0..Q6:
// Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
// Assume segments are active-high (typical with provided wiring).
static uint8_t sevsegDigitToSegments(uint8_t digit) {
  // Bits: b0=A, b1=B, b2=C, b3=D, b4=E, b5=F, b6=G
  // 0: A B C D E F (no G)
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  switch (digit) {
    case 0: return (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    case 1: return (1 << 1) | (1 << 2);
    case 2: return (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6);
    case 3: return (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6);
    case 4: return (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);
    case 5: return (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6);
    case 6: return (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
    case 7: return (1 << 0) | (1 << 1) | (1 << 2);
    case 8: return (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
    case 9: return (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6);
    default: return 0;
  }
}

// Update 7-segment and LED bar to show the current number 0..9.
// Bargraph requirement:
// - "light up a number of LEDs equal to the current number (from pin A1 to A10)"
// - When number reaches 9, reset to 0 and turn all LEDs off.
// Wiring preserved:
// mega:2->A1,3->A2,4->A3,5->A4,6->A5,7->A6,8->A7,9->A8,10->A9,11->A10
// All cathodes C1..C10 share mega:GND.1 in circuit; so drive anode pins HIGH/LOW.
static void displayNumber(uint8_t n, const uint8_t barAnodes[10]) {
  // 7-segment: map segments into sr1 Q0..Q6. Q7 remains 0.
  uint8_t segBits = sevsegDigitToSegments(n); // bits b0..b6 correspond Q0..Q6
  uint8_t srValue = segBits & 0x7F;           // ensure Q7 is 0

  shiftOutToSR(srValue);

  // Bargraph: turn on first n LEDs, off the rest (n in 0..9).
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(barAnodes[i], (i < n) ? HIGH : LOW);
  }
}

const uint8_t BAR_ANODES[10] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

unsigned long lastUpdateMs = 0;
uint8_t currentNumber = 0;

void setup() {
  // Shift register pins
  pinMode(PIN_SHCP, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_DS, OUTPUT);

  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);
  digitalWrite(PIN_DS, LOW);

  // Bargraph anode pins
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_ANODES[i], OUTPUT);
    digitalWrite(BAR_ANODES[i], LOW);
  }

  // On power/reset: show "0" and no bar LEDs lit.
  currentNumber = 0;
  displayNumber(currentNumber, BAR_ANODES);

  lastUpdateMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdateMs >= 2000UL) {
    lastUpdateMs = now;

    // Increment every 2 seconds; when it reaches 9, reset to 0.
    if (currentNumber >= 9) {
      currentNumber = 0;
      // Requirement: bargraph should turn off all LEDs when reaching 9 and resetting.
      // Our displayNumber(0, ...) turns off all LEDs (i < 0 none).
    } else {
      currentNumber++;
    }

    // Update immediately to reflect the new number.
    displayNumber(currentNumber, BAR_ANODES);
  }
}
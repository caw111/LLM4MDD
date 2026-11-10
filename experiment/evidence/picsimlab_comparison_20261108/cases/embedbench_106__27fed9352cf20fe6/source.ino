#include <Arduino.h>

static const uint8_t PIN_SR1_DS   = A0;  // sr1 DS
static const uint8_t PIN_SR1_SHCP = A2;  // sr1 SHCP
static const uint8_t PIN_SR1_STCP = A1;  // sr1 STCP

static const uint8_t PIN_SR2_DS   = A7;  // Not used (sr1:Q7S -> sr2:DS in circuit), but keeps clarity.
static const uint8_t PIN_SR1_OE   = 0;    // sr1 OE is tied LOW via GND. We'll rely on wiring.
static const uint8_t PIN_LED1     = 2;   // led1:A
static const uint8_t PIN_LED2     = 3;   // led2:A

// Sevseg uses 74HC595 outputs via sr1 and sr2:
// sr1 Q0..Q7 -> sevseg1 A,B,C,D,E,F,G and extra (Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7S unused)
//
// sr2 Q0..Q6 -> sevseg2 A,B,C,D,E,F,G and Q7 unused

// Bar graph uses sr1 Q0..Q? through sr2:DS to bargraph1 A1..A10.
// Wiring shows bargraph1:A1..A10 connected to mega:A4..A13? Actually mega:4..13 to A1..A10.
// bargraph1:C1..C10 to GND. So these pins directly drive segments.
// Requirement: "lights up number of segments equal to tens digit + 1 (PIN A1-A10)".
// We'll drive mega pins 4..13 accordingly.

static const uint8_t PIN_BAR_A1  = 4;
static const uint8_t PIN_BAR_A10 = 13;

static void shiftOut595(uint8_t value, bool toSr1) {
  // DS is shared only for sr1 from A0.
  // SHCP and STCP are shared between sr1 and sr2 in wiring,
  // but sr2's DS is connected to sr1's serial output (Q7S),
  // so shifting one byte into sr1 will simultaneously update sr2 outputs.
  // However, in this circuit, the outputs driving each display are on different registers:
  // - sevseg1 comes from sr1's Q0..Q6
  // - sevseg2 comes from sr2's Q0..Q6
  //
  // With only sr1 DS accessible, we must shift two bytes:
  //   shift high byte first (sr2), then low byte (sr1),
  // because shift registers cascade through Q7S -> DS.
  //
  // Therefore, this helper shifts a single byte assuming cascade order is handled elsewhere.
  (void)value;
  (void)toSr1;
}

static void latchPulse(uint8_t pinSTCP) {
  digitalWrite(pinSTCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(pinSTCP, LOW);
  delayMicroseconds(2);
}

static void clockPulse(uint8_t pinSHCP) {
  digitalWrite(pinSHCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(pinSHCP, LOW);
  delayMicroseconds(2);
}

static void shiftByteLSBFirst(uint8_t b, uint8_t pinDS, uint8_t pinSHCP) {
  // Outputs Q0.. correspond to first shifted bit (LSB) depending on wiring.
  // In 74HC595 typical: shift register loads DS into Q0 after 8 clocks with LSB-first in software.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(pinDS, (b & (1u << i)) ? HIGH : LOW);
    clockPulse(pinSHCP);
  }
}

// Segment encoding: map digits 0-9 to bits for A,B,C,D,E,F,G.
// We'll create an 8-bit value where bit0->A, bit1->B, bit2->C, bit3->D, bit4->E, bit5->F, bit6->G, bit7->unused(0).
// The required wiring doesn't specify common anode/cathode; in Wokwi 7-segment default matches typical,
// so we use standard "on=1" for segments.
static uint8_t digitToSeg(uint8_t d) {
  // Standard segments for common-cathode:
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
  uint8_t segBits = 0;

  auto setSeg = [&](char seg) {
    switch (seg) {
      case 'A': segBits |= (1u << 0); break;
      case 'B': segBits |= (1u << 1); break;
      case 'C': segBits |= (1u << 2); break;
      case 'D': segBits |= (1u << 3); break;
      case 'E': segBits |= (1u << 4); break;
      case 'F': segBits |= (1u << 5); break;
      case 'G': segBits |= (1u << 6); break;
    }
  };

  switch (d) {
    case 0: setSeg('A'); setSeg('B'); setSeg('C'); setSeg('D'); setSeg('E'); setSeg('F'); break;
    case 1: setSeg('B'); setSeg('C'); break;
    case 2: setSeg('A'); setSeg('B'); setSeg('D'); setSeg('E'); setSeg('G'); break;
    case 3: setSeg('A'); setSeg('B'); setSeg('C'); setSeg('D'); setSeg('G'); break;
    case 4: setSeg('B'); setSeg('C'); setSeg('F'); setSeg('G'); break;
    case 5: setSeg('A'); setSeg('C'); setSeg('D'); setSeg('F'); setSeg('G'); break;
    case 6: setSeg('A'); setSeg('C'); setSeg('D'); setSeg('E'); setSeg('F'); setSeg('G'); break;
    case 7: setSeg('A'); setSeg('B'); setSeg('C'); break;
    case 8: setSeg('A'); setSeg('B'); setSeg('C'); setSeg('D'); setSeg('E'); setSeg('F'); setSeg('G'); break;
    case 9: setSeg('A'); setSeg('B'); setSeg('C'); setSeg('D'); setSeg('F'); setSeg('G'); break;
  }
  return segBits;
}

static void writeTwoDigits(uint8_t tens, uint8_t units) {
  // Cascade order:
  // First shift sr2 (tens digit), then shift sr1 (units digit)??? Actually sr1 outputs go to sevseg1.
  // Wiring: sr1 -> sevseg1, sr2 -> sevseg2.
  // Requirement: tens digit on sevseg1, units digit on sevseg2.
  //
  // In cascade, first shifted byte ends up in the farthest register (sr2), then the next in sr1.
  // So:
  //   shiftByte(sr2Byte = tensSeg)
  //   shiftByte(sr1Byte = unitsSeg)
  //
  uint8_t sr2Byte = digitToSeg(tens);
  uint8_t sr1Byte = digitToSeg(units);

  // Begin with STCP low then shift
  digitalWrite(PIN_SR1_STCP, LOW);

  shiftByteLSBFirst(sr2Byte, PIN_SR1_DS, PIN_SR1_SHCP);
  shiftByteLSBFirst(sr1Byte, PIN_SR1_DS, PIN_SR1_SHCP);

  latchPulse(PIN_SR1_STCP);
}

static void writeBarGraphTensPlus1(uint8_t tens) {
  // Light up segments = tens + 1 for tens digit 0..9 (so 1..10)
  uint8_t segments = tens + 1;

  for (uint8_t i = 0; i < 10; i++) {
    uint8_t pin = PIN_BAR_A1 + i; // A1..A10 are on mega pins 4..13
    // bargraph1:C pins are tied to GND; assuming bar segments light when A pin is HIGH.
    digitalWrite(pin, (i < segments) ? HIGH : LOW);
  }
}

void setup() {
  // Board: Arduino Mega 2560 per circuit.

  pinMode(PIN_SR1_DS, OUTPUT);
  pinMode(PIN_SR1_SHCP, OUTPUT);
  pinMode(PIN_SR1_STCP, OUTPUT);

  // OE is tied to GND in wiring (sr1:OE -> black).
  // MR tied up (sr1:MR -> sr1:VCC, also sr2:MR -> sr1:VCC), as per wiring.

  // LEDs
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  // Bargraph pins A1..A10 are connected to mega pins 4..13
  for (uint8_t pin = PIN_BAR_A1; pin <= PIN_BAR_A10; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // Initial values:
  // - displays "00"
  // - bargraph lights 1 segment (tens digit = 0 => 0+1 = 1)
  // - both LEDs off
  writeTwoDigits(0, 0);
  writeBarGraphTensPlus1(0);

  // Initialize shift register control lines to low
  digitalWrite(PIN_SR1_DS, LOW);
  digitalWrite(PIN_SR1_SHCP, LOW);
  digitalWrite(PIN_SR1_STCP, LOW);
}

void loop() {
  static uint8_t value = 0; // 0..99
  static bool led1On = false;
  static unsigned long lastTick = 0;
  static bool blinkPhase = false; // false -> led1 on, led2 off; true -> led1 off, led2 on
  unsigned long now = millis();

  // Every 2 seconds: update number and blink LEDs alternately (same period).
  if (now - lastTick >= 2000UL) {
    lastTick = now;

    // Increment cycle
    uint8_t tens = value / 10;
    uint8_t units = value % 10;

    writeTwoDigits(tens, units);
    writeBarGraphTensPlus1(tens);

    // LED alternating
    blinkPhase = !blinkPhase;
    if (!blinkPhase) {
      // led1 on / led2 off
      digitalWrite(PIN_LED1, HIGH);
      digitalWrite(PIN_LED2, LOW);
    } else {
      // led1 off / led2 on
      digitalWrite(PIN_LED1, LOW);
      digitalWrite(PIN_LED2, HIGH);
    }

    // Prepare next number
    value++;
    if (value > 99) value = 0;
  }
}
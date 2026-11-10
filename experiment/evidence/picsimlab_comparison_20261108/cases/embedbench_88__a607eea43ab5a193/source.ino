#include <Arduino.h>

// Board: Arduino Mega 2560

// 74HC595 pins (per wiring)
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue

// Shared control pins to both shift registers via SR chain
static const uint8_t SR_OE = 0;     // not wired explicitly; OE tied to GND in circuit (disabled pin control)

// RGB LED pins (common cathode)
static const uint8_t RGB_R = 4; // red
static const uint8_t RGB_G = 5; // green
static const uint8_t RGB_B = 6; // blue
static const uint8_t RGB_COM = 3; // connected to GND.1 (not used directly)

// Buttons (per wiring)
static const uint8_t BTN1 = 2; // increment (wired to GND.1 on one side, other to D2)
static const uint8_t BTN2 = 3; // decrement (wired to GND.1 on one side, other to D3)

// 7-seg digit segment pins are driven by shift registers, no direct Arduino pins.

// Segment bit mapping to 74HC595 outputs Q0..Q7 (A..G, DP assumed off).
// Based on wiring:
// sr?.Q0 -> sevseg?:A
// sr?.Q1 -> sevseg?:B
// sr?.Q2 -> sevseg?:C
// sr?.Q3 -> sevseg?:D
// sr?.Q4 -> sevseg?:E
// sr?.Q5 -> sevseg?:F
// sr?.Q6 -> sevseg?:G
// sr?.Q7S -> (unused in 7segment wiring, connected to ds of next SR) so we avoid using Q7S for segments.
// We'll generate 7-bit segment patterns for A..G and shift them into Q0..Q6.

// Common-cathode or common-anode?
// The circuit says sevseg COM.1 connected between displays; and shift register outputs drive segments.
// The circuit's RGB is common cathode; for sevseg, not specified in circuit attrs.
// We'll assume typical Wokwi 7-seg default: common cathode unless specified.
// However the circuit explicitly wires sevseg COM.1 to each other and not through a transistor,
// and also ties shift-register MR to VCC enabling outputs. We'll treat segment bits as active HIGH.
// This matches common-cathode behavior in Wokwi.

static uint8_t segAtoGForDigit(uint8_t d) {
  // Bits order: bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
  // Active HIGH segments.
  // Digit segments:
  // 0: A B C D E F
  // 1:   B C
  // 2: A B   D E   G
  // 3: A B C D     G
  // 4:   B C     F G
  // 5: A   C D   F G
  // 6: A   C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D   F G
  static const uint8_t map[10] = {
    // A B C D E F G  -> binary in bits 0..6
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),                 // 0
    (0<<0)|(1<<1)|(1<<2),                                     // 1
    (1<<0)|(1<<1)|(0<<2)|(1<<3)|(1<<4)|(0<<5)|(1<<6),         // 2
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(0<<4)|(0<<5)|(1<<6),         // 3
    (0<<0)|(1<<1)|(1<<2)|(0<<3)|(0<<4)|(1<<5)|(1<<6),         // 4
    (1<<0)|(0<<1)|(1<<2)|(1<<3)|(0<<4)|(1<<5)|(1<<6),         // 5
    (1<<0)|(0<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),         // 6
    (1<<0)|(1<<1)|(1<<2),                                     // 7
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),         // 8
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(0<<4)|(1<<5)|(1<<6)          // 9
  };
  return map[d];
}

static void sr1SendDigit(uint8_t digitTens, uint8_t digitUnits) {
  // Need to output 16 segment controls? Actually two separate 7-seg displays each driven by its own shift register.
  // sr1 drives sevseg1 segments, sr2 drives sevseg2 segments. Both share SHCP/STCP lines.
  //
  // Wiring of SHCP/STCP common:
  // mega:A2 -> SR1_SHCP -> SHCP for sr1 and sr2
  // mega:A1 -> SR1_STCP -> STCP for sr1 and sr2
  //
  // DS: sr1 DS from mega:A0, and sr1 Q7S connected to sr2 DS, so serial chain:
  // We clock out 16 bits total (sr2 receives last bits; sr1 receives first in chain).
  //
  // We must consider that sr1 Q7S -> sr2:DS, which means when shifting 8 bits, sr1 consumes the first 8 bits and sr2 consumes the next 8 bits.
  // To be safe and match common chaining:
  // - Send sr1's byte first, then sr2's byte.
  //
  // However STCP latches both simultaneously, so final register contents depend on bit order.
  //
  // We'll implement standard 74HC595 chaining: shift out sr2Byte last? Many tutorials say:
  //   For two cascaded 74HC595 (sr1 closer to MCU, sr2 farther):
  //   shift out data for sr2 first, then sr1 last, so that sr2 ends up with the first byte.
  //
  // But our circuit: sr1.Q7S -> sr2.DS, thus sr2 is farther from DS source.
  // Therefore, to get sr1 and sr2 correctly, we send sr1 byte first then sr2 byte (the later bits shift further into sr2).
  //
  // We'll validate by reasoning: DS source feeds sr1 shift register input.
  // After 8 shifts, sr1 contents latched = bits shifted during those 8 pulses.
  // Meanwhile, the bits shifted out of sr1 into Q7S feed sr2 input, meaning sr2 shift register receives bits that were shifted into sr1 (serial stream).
  // For 16 shifts, sr2 shift register will hold the last 8 bits shifted into the chain.
  // So:
  // - If we want sr1 to show digitTens, sr1 must get the first 8 bits.
  // - sr2 to show digitUnits, sr2 must get the last 8 bits.
  //
  // Hence: shift out sr1Byte then sr2Byte.

  uint8_t sr1Byte = segAtoGForDigit(digitTens);   // bits 0..6 used, bit7 don't care
  uint8_t sr2Byte = segAtoGForDigit(digitUnits);  // bits 0..6 used, bit7 don't care

  uint8_t bytes[2] = { sr1Byte, sr2Byte };

  // Shift 16 bits MSB-first (common for 74HC595 examples). We'll use MSB-first.
  // But bit7 is unused; still fine.
  digitalWrite(SR1_STCP, LOW);
  for (int b = 0; b < 2; b++) {
    for (int bit = 7; bit >= 0; bit--) {
      digitalWrite(SR1_SHCP, LOW);
      bool bitVal = (bytes[b] >> bit) & 0x01;
      digitalWrite(SR1_DS, bitVal ? HIGH : LOW);
      digitalWrite(SR1_SHCP, HIGH);
    }
  }
  digitalWrite(SR1_STCP, HIGH);
}

static void setRGBForNumber(int n) {
  // Common cathode RGB: COM tied to GND; active HIGH drives color LED.
  if (n >= 0 && n <= 33) {
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, LOW);
    digitalWrite(RGB_B, LOW);
  } else if (n >= 34 && n <= 66) {
    digitalWrite(RGB_R, LOW);
    digitalWrite(RGB_G, HIGH);
    digitalWrite(RGB_B, LOW);
  } else { // 67..99
    digitalWrite(RGB_R, LOW);
    digitalWrite(RGB_G, LOW);
    digitalWrite(RGB_B, HIGH);
  }
}

static void turnRGBOff() {
  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, LOW);
}

struct DebouncedButton {
  uint8_t pin;
  bool stableState;          // last stable reading (LOW/HIGH)
  bool lastReading;
  unsigned long lastChangeMs;
};

static void initButton(DebouncedButton &btn, uint8_t p) {
  btn.pin = p;
  pinMode(btn.pin, INPUT_PULLUP); // buttons connected to GND -> active LOW
  bool r = (digitalRead(btn.pin) == HIGH);
  btn.lastReading = r;
  btn.stableState = r;
  btn.lastChangeMs = millis();
}

static bool updateButtonPressed(DebouncedButton &btn) {
  // returns true on a new debounced press event (transition to LOW stable)
  const unsigned long debounceMs = 30;

  bool readingHigh = (digitalRead(btn.pin) == HIGH);

  if (readingHigh != btn.lastReading) {
    btn.lastChangeMs = millis();
    btn.lastReading = readingHigh;
  }

  unsigned long now = millis();
  if ((now - btn.lastChangeMs) >= debounceMs) {
    if (btn.stableState != btn.lastReading) {
      btn.stableState = btn.lastReading;
      // active LOW => pressed when stableState becomes LOW (readingHigh=false)
      if (!btn.stableState) {
        return true;
      }
    }
  }
  return false;
}

static int displayedNumber = 0;
static DebouncedButton buttonInc;
static DebouncedButton buttonDec;

void setup() {
  // Shift register control pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // RGB pins
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  turnRGBOff();

  // Buttons
  initButton(buttonInc, BTN1); // btn1 increments
  initButton(buttonDec, BTN2); // btn2 decrements

  // Initial display "00" and RGB off
  displayedNumber = 0;
  sr1SendDigit(0, 0); // tens=0, units=0
  turnRGBOff();
}

void loop() {
  bool incPressed = updateButtonPressed(buttonInc);
  bool decPressed = updateButtonPressed(buttonDec);

  if (incPressed) {
    displayedNumber++;
    if (displayedNumber > 99) displayedNumber = 0;
    int tens = displayedNumber / 10;
    int units = displayedNumber % 10;
    sr1SendDigit(tens, units);
    setRGBForNumber(displayedNumber);
    // If both somehow press at same time, allow both changes by not returning early.
  }

  if (decPressed) {
    displayedNumber--;
    if (displayedNumber < 0) displayedNumber = 99;
    int tens = displayedNumber / 10;
    int units = displayedNumber % 10;
    sr1SendDigit(tens, units);
    setRGBForNumber(displayedNumber);
  }

  // Small idle delay to reduce CPU churn
  delay(5);
}
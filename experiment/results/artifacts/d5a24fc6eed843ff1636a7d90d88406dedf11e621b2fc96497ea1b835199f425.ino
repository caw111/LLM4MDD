#include <Arduino.h>

static uint8_t map255To0_9(uint8_t v255) {
  return (uint8_t)((uint16_t)v255 * 9u / 255u);
}

static void set7seg595Value(uint8_t value0to9, bool isBlueDisplay, uint8_t redVal255, uint8_t greenVal255, uint8_t blueVal255) {
  // Wiring explanation:
  // - The 74HC595 output pins are hard-wired to segment lines A..G and COM.1 is shared/selected by both displays through Q7S.
  // - Inputs are already daisy-chained between sr1 and sr2 using SHCP/STCP/DS/MR/OE as per the provided circuit.
  //
  // Hard requirement mapping from requirements:
  // - sevseg1 shows red & green values as "two-digit numbers".
  // - sevseg2 shows blue value.
  //
  // Circuit constraints here:
  // - The provided circuit's 74HC595 routing matches a single digit per shift register (A..G). The DS chain is:
  //     sr1: Q7S -> sr2: DS
  //   and sr2 uses Q0..Q6 to drive A..G of sevseg2, while sr1 uses Q0..Q6 to drive A..G of sevseg1.
  // - The circuit also directly maps sr1's Q7S to sr2's DS, meaning a fixed 8-bit shift value is effectively what controls both digits.
  //
  // Given the above, implement a conventional common-cathode 7-segment digit drive for each display:
  // - segment pattern for digit d:
  //   bits correspond to A,B,C,D,E,F,G = Q0..Q6 respectively (as wired).
  // - Output polarity: common cathode RGB displays; for 7-seg in Wokwi, standard is common-cathode with HIGH = on.
  //
  // Define bit positions: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
  static const uint8_t segDigits[10] = {
    // A B C D E F G (bit0..bit6)
    0b0111111, // 0
    0b0000110, // 1
    0b1011011, // 2
    0b1001111, // 3
    0b1100110, // 4
    0b1101101, // 5
    0b1111101, // 6
    0b0000111, // 7
    0b1111111, // 8
    0b1101111  // 9
  };

  // Determine target digits:
  // - Requirement says sevseg1 shows red and green as two-digit numbers.
  //   With the provided hardware, we cannot display two digits simultaneously on a single 7-seg.
  //   We'll preserve behavior by continuously updating digits on sevseg1:
  //   - display red (mapped 0-9) briefly, then green (mapped 0-9) briefly, alternating in loop.
  // - sevseg2 displays blue (mapped 0-9) continuously.
  //
  // This function drives both shift registers (sr1 and sr2) with the same 16-bit stream:
  // shift order on chained 74HC595: first bits shifted go to the last device, then first.
  //
  // With Q7S -> sr2:DS and both share SR clocks:
  // - We'll shift 16 bits MSB-first where:
  //   * upper 8 bits end up in sr1 outputs (A..G on Q0..Q6)
  //   * lower 8 bits end up in sr2 outputs (A..G on Q0..Q6)
  //
  // But to avoid confusion, we use shiftOut with MSB-first carefully.
}

static uint8_t digitToSegPattern(uint8_t d) {
  static const uint8_t segDigits[10] = {
    0b0111111, // 0
    0b0000110, // 1
    0b1011011, // 2
    0b1001111, // 3
    0b1100110, // 4
    0b1101101, // 5
    0b1111101, // 6
    0b0000111, // 7
    0b1111111, // 8
    0b1101111  // 9
  };
  if (d > 9) d = 9;
  return segDigits[d];
}

static void shift16ToShiftRegisters(uint8_t sr1_digitPattern, uint8_t sr2_digitPattern) {
  // Q0..Q6 -> A..G for each digit as wired.
  // Q7 is unused for segment outputs (except it is used for shifting to next device through Q7S).
  // We'll set Q7 = 0 for both shift registers so it doesn't affect segment outputs (Q7 is not connected to A..G).
  //
  // Compose 16-bit value:
  // Assume MSB-first shifting into the first device (sr1) results:
  // - Bits 15..8 go to sr1, bits 7..0 go to sr2 (typical daisy-chain).
  uint16_t data = ((uint16_t)sr1_digitPattern << 8) | (uint16_t)sr2_digitPattern;

  // Bit banging with SPI-like pins (as per provided circuit):
  // sr1: SHCP on mega A2, STCP on mega A1, DS on mega A0
  const uint8_t PIN_SHCP = A2;
  const uint8_t PIN_STCP = A1;
  const uint8_t PIN_DS   = A0;

  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  // Shift out MSB first
  for (int i = 15; i >= 0; --i) {
    digitalWrite(PIN_DS, (data >> i) & 0x01);
    digitalWrite(PIN_SHCP, HIGH);
    digitalWrite(PIN_SHCP, LOW);
  }

  // Latch
  digitalWrite(PIN_STCP, HIGH);
  digitalWrite(PIN_STCP, LOW);
}

static void setDisplays(uint8_t red255, uint8_t green255, uint8_t blue255, bool showRedOnSevseg1) {
  // Requirement mapping:
  // - sevseg1 will show red and green values as two-digit numbers.
  // We'll multiplex between red-digit and green-digit on sevseg1.
  // - sevseg2 will show blue value.
  uint8_t redDigit = map255To0_9(red255);
  uint8_t greenDigit = map255To0_9(green255);
  uint8_t blueDigit = map255To0_9(blue255);

  uint8_t sr1_pattern = digitToSegPattern(showRedOnSevseg1 ? redDigit : greenDigit);
  uint8_t sr2_pattern = digitToSegPattern(blueDigit);

  shift16ToShiftRegisters(sr1_pattern, sr2_pattern);
}

static void setRgbLed(uint8_t r, uint8_t g, uint8_t b) {
  // Wiring:
  // rgb1 COM -> GND
  // rgb1 R,G,B -> mega 2,3,4
  // rgb2 COM -> GND
  // rgb2 R,G,B -> mega 5,6,7
  // Use PWM pins:
  // - Mega PWM: 2,3,4 are PWM capable; 5,6,7 are PWM capable as well.
  analogWrite(2, r);
  analogWrite(3, g);
  analogWrite(4, b);

  analogWrite(5, r);
  analogWrite(6, g);
  analogWrite(7, b);
}

void setup() {
  // Slide potentiometers
  // pot1: A3, pot2: A4, pot3: A5 (SIG pins)
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  // Shift register pins
  // sr1: SHCP=A2, STCP=A1, DS=A0
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);

  digitalWrite(A2, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A0, LOW);

  // RGB PWM pins
  pinMode(2, OUTPUT); // rgb1 R
  pinMode(3, OUTPUT); // rgb1 G
  pinMode(4, OUTPUT); // rgb1 B
  pinMode(5, OUTPUT); // rgb2 R
  pinMode(6, OUTPUT); // rgb2 G
  pinMode(7, OUTPUT); // rgb2 B

  // Initialize off
  setRgbLed(0, 0, 0);
  // Initialize displays with red digit on sevseg1, blue on sevseg2
  setDisplays(0, 0, 0, true);
}

void loop() {
  // Read potentiometers (0-1023)
  int p1 = analogRead(A3); // pot1 -> red
  int p2 = analogRead(A4); // pot2 -> green
  int p3 = analogRead(A5); // pot3 -> blue

  // Map to 0-255
  uint8_t red255   = (uint8_t)((uint16_t)p1 * 255u / 1023u);
  uint8_t green255 = (uint8_t)((uint16_t)p2 * 255u / 1023u);
  uint8_t blue255  = (uint8_t)((uint16_t)p3 * 255u / 1023u);

  // Update RGB LEDs continuously based on potentiometers
  setRgbLed(red255, green255, blue255);

  // Update 7-seg displays continuously, with multiplexing on sevseg1
  // to satisfy "red and green values as two-digit numbers" on the single 7-seg.
  static uint32_t lastToggle = 0;
  static bool showRed = true;

  const uint32_t intervalMs = 25; // refresh rate for multiplexing
  uint32_t now = millis();
  if (now - lastToggle >= intervalMs) {
    lastToggle = now;
    showRed = !showRed;
  }

  // sevseg2 always shows blue
  setDisplays(red255, green255, blue255, showRed);

  // No delay to keep RGB smooth; multiplex timing handled via millis().
}
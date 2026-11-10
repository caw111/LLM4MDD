// 4-digit 7-segment display (common anode)
// Wiring preserved as specified:
// Segments A..G,DP -> Arduino D2..D9
// Digit enables DIG1..DIG4 -> Arduino D10..D13
//
// Common anode means:
// - Segment pins HIGH turn segments OFF
// - Segment pins LOW turn segments ON
// - Digit select pins HIGH disable digits (since COM is at 5V, and digit cathodes are enabled low)
// - Digit select pins LOW enable digit

const uint8_t SEG_PINS[8] = {
  2, // A
  3, // B
  4, // C
  5, // D
  6, // E
  7, // F
  8, // G
  9  // DP
};

const uint8_t DIG_PINS[4] = {
  10, // DIG1
  11, // DIG2
  12, // DIG3
  13  // DIG4
};

// Segment bit order: A,B,C,D,E,F,G,DP
// Values are active-LOW for segments (common anode): 0=ON, 1=OFF
const uint8_t digitSegmentsActiveLow[10] = {
  //  A B C D E F G DP
  0b00000011, // 0
  0b10011111, // 1
  0b00100101, // 2
  0b00001101, // 3
  0b10011001, // 4
  0b01001001, // 5
  0b01000001, // 6
  0b00011111, // 7
  0b00000001, // 8
  0b00001001  // 9
};

void setSegmentsActiveLow(uint8_t activeLowPattern) {
  // activeLowPattern bit 0 corresponds to A, bit1 to B, ... bit7 to DP
  for (uint8_t i = 0; i < 8; i++) {
    // segment ON => LOW, OFF => HIGH
    uint8_t bit = (activeLowPattern >> i) & 0x01;
    // If bit==0 => ON => LOW
    // If bit==1 => OFF => HIGH
    digitalWrite(SEG_PINS[i], bit ? HIGH : LOW);
  }
}

void enableDigit(uint8_t digitIndex) {
  // disable all digits first
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(DIG_PINS[i], HIGH); // disable (cathode not selected)
  }
  // enable requested digit (active LOW on digit cathode)
  digitalWrite(DIG_PINS[digitIndex], LOW);
}

void showDigitOnDigit(uint8_t digitValue, uint8_t digitIndex) {
  setSegmentsActiveLow(digitSegmentsActiveLow[digitValue]);
  enableDigit(digitIndex);
}

void setup() {
  // 1. Set 2~13 pin as an output.
  for (uint8_t p = 2; p <= 13; p++) {
    pinMode(p, OUTPUT);
  }

  // Start with all segments OFF and all digits disabled.
  // Segments OFF for common anode => HIGH.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SEG_PINS[i], HIGH);
  }
  // Digits disabled => HIGH
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(DIG_PINS[i], HIGH);
  }
}

void loop() {
  // 2. Display number 1 on digit1, 2 on digit2, 3 on digit3, 4 on digit4
  // with 1000ms delay between each step.
  showDigitOnDigit(1, 0);
  delay(1000);

  showDigitOnDigit(2, 1);
  delay(1000);

  showDigitOnDigit(3, 2);
  delay(1000);

  showDigitOnDigit(4, 3);
  delay(1000);
}
#include <Arduino.h>

// Arduino Mega 2560 wiring to the 74HC595.
constexpr uint8_t DATA_PIN  = A0;  // DS
constexpr uint8_t LATCH_PIN = A1;  // STCP
constexpr uint8_t CLOCK_PIN = A2;  // SHCP
constexpr uint8_t POT1_PIN  = A3;
constexpr uint8_t POT2_PIN  = A4;

// Bits 0..6 correspond to segments A..G on Q0..Q6.
// These patterns use 1 for an illuminated segment; they are inverted
// before transmission because the display's common anode is tied to 5V.
constexpr uint8_t DIGIT_PATTERNS[10] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111   // 9
};

void displayDigit(uint8_t digit) {
  const uint8_t output = static_cast<uint8_t>(~DIGIT_PATTERNS[digit]);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, output);
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  digitalWrite(DATA_PIN, HIGH);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  // Initialize the display to zero on power-up or reset.
  displayDigit(0);
}

void loop() {
  const int pot1Value = analogRead(POT1_PIN);
  const int pot2Value = analogRead(POT2_PIN);

  const uint8_t firstDigit =
      static_cast<uint8_t>(map(pot1Value, 0, 1023, 0, 9));
  const uint8_t secondDigit =
      static_cast<uint8_t>(map(pot2Value, 0, 1023, 0, 9));

  // Add the mapped values, retaining only the last decimal digit.
  const uint8_t displayedDigit = (firstDigit + secondDigit) % 10;
  displayDigit(displayedDigit);

  delay(10);
}
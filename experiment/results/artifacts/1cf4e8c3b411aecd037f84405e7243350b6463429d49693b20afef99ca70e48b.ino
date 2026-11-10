#include <Arduino.h>

static const uint8_t PIN_POT1 = A3;
static const uint8_t PIN_POT2 = A4;

// 74HC595 pins (as wired)
static const uint8_t PIN_SHCP = A2; // gray
static const uint8_t PIN_STCP = A1; // purple
static const uint8_t PIN_DS   = A0;  // blue

// RGB LED common cathode (direct pins from Mega)
static const uint8_t PIN_RGB_R = 2;
static const uint8_t PIN_RGB_G = 3;
static const uint8_t PIN_RGB_B = 4;

// 74HC595 also drives the 7-seg segments (sevseg1)
// Pins Q0..Q6 -> segments A..G with the following mapping:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
static const uint8_t Q0 = 0;
static const uint8_t Q1 = 1;
static const uint8_t Q2 = 2;
static const uint8_t Q3 = 3;
static const uint8_t Q4 = 4;
static const uint8_t Q5 = 5;
static const uint8_t Q6 = 6;

// 7-seg segment bits for digits 0..9 (A,B,C,D,E,F,G)
// common cathode typical segment-on = 1 for the corresponding segment.
// The circuit uses a common cathode display, so output HIGH lights segments.
static const uint8_t digitMask[10] = {
  // A B C D E F G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),           // 0
  (1<<1)|(1<<2),                                       // 1
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                 // 2
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                 // 3
  (1<<1)|(1<<2)|(1<<5)|(1<<6),                         // 4
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                 // 5
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),         // 6
  (1<<0)|(1<<1)|(1<<2),                               // 7
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),   // 8
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)          // 9
};

static inline void shift595Out(uint8_t data) {
  // Send data MSB first to match the usual shift register behavior.
  // With Q0..Q7 mapping at outputs, bits correspond to serial input order:
  // We'll shift MSB first so that bit7 becomes first shifted; but since we only use
  // Q0..Q6, we can directly place them in positions 0..6 and shift MSB->LSB.
  //
  // To ensure correct placement for Q0..Q6, we construct the byte with bit0->Q0, etc.
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(PIN_SHCP, LOW);
    digitalWrite(PIN_DS, (data >> i) & 0x01);
    // rising edge clocks into shift register
    digitalWrite(PIN_SHCP, HIGH);
  }
  // Latch to outputs
  digitalWrite(PIN_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(PIN_STCP, LOW);
}

static inline void set7segDigit(uint8_t digit) {
  if (digit > 9) digit = 0;

  // digitMask uses bits 0..6 for segments A..G.
  // Map to 74HC595 outputs Q0..Q6:
  // Q0->A corresponds to bit0, Q1->B corresponds to bit1, ..., Q6->G corresponds to bit6.
  uint8_t segBits = digitMask[digit] & 0x7F;

  // Q7 is unused; keep it 0.
  uint8_t outByte = segBits;

  shift595Out(outByte);
}

static inline void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  // Common cathode: COM to GND. Mega pins sink current (PWM) to light color channels.
  // Use analogWrite on pins 2,3,4 with the assumption they support PWM on Mega.
  // With cathode, LOW turns channel on; HIGH turns it off.
  // Therefore invert PWM values: duty_on = value/255, but write (255 - value).
  analogWrite(PIN_RGB_R, 255 - r);
  analogWrite(PIN_RGB_G, 255 - g);
  analogWrite(PIN_RGB_B, 255 - b);
}

void setup() {
  // 74HC595 control pins
  pinMode(PIN_SHCP, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_DS, OUTPUT);

  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);
  digitalWrite(PIN_DS, LOW);

  // RGB LED pins
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  // Ensure initial state: RGB off, 7-seg shows "0"
  setRGB(0, 0, 0);
  set7segDigit(0);
}

void loop() {
  int pot1Raw = analogRead(PIN_POT1);
  int pot2Raw = analogRead(PIN_POT2);

  // Map to 0..255 as required
  uint8_t redGreen = (uint8_t)map(pot1Raw, 0, 1023, 0, 255);
  uint8_t blue = (uint8_t)map(pot2Raw, 0, 1023, 0, 255);

  // Display RGB: pot1 controls red and green, pot2 controls blue
  uint8_t r = redGreen;
  uint8_t g = redGreen;
  uint8_t b = blue;
  setRGB(r, g, b);

  // Average brightness and scaling:
  // avg = (r+g+b)/3, then /28.33 to get 0..9
  float avg = ( (float)r + (float)g + (float)b ) / 3.0f;
  float scaled = avg / 28.33f;

  uint8_t digit = (uint8_t)scaled; // truncation as a direct cast

  if (digit > 9) digit = 9;

  set7segDigit(digit);

  // Continuous update; keep loop responsive
  delay(10);
}
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// Shift-register (74HC595) pins per wiring
const uint8_t SR_DS   = A0;  // mega:A0 -> sr1:DS
const uint8_t SR_STCP = A1;  // mega:A1 -> sr1:STCP
const uint8_t SR_SHCP = A2;  // mega:A2 -> sr1:SHCP
// sr1:OE and sr1:MR are tied LOW (enabled), and VCC tied to 5V.

// 7-seg segment bit mapping (Q0..Q6 -> segments A,B,C,D,E,F,G via wiring)
static uint8_t segBitsForDigit(uint8_t digit) {
  // Bits correspond to Q0..Q6:
  // Q0 -> A
  // Q1 -> B
  // Q2 -> C
  // Q3 -> D
  // Q4 -> E
  // Q5 -> F
  // Q6 -> G
  //
  // Common cathode assumed (as provided by sevseg model default behavior).
  // Segment ON = 1.
  // Digit to segments (A,B,C,D,E,F,G):
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
  uint8_t A,B,C,D,E,F,G;

  A = B = C = D = E = F = G = 0;

  switch (digit) {
    case 0: A=1; B=1; C=1; D=1; E=1; F=1; G=0; break;
    case 1: A=0; B=1; C=1; D=0; E=0; F=0; G=0; break;
    case 2: A=1; B=1; C=0; D=1; E=1; F=0; G=1; break;
    case 3: A=1; B=1; C=1; D=1; E=0; F=0; G=1; break;
    case 4: A=0; B=1; C=1; D=0; E=0; F=1; G=1; break;
    case 5: A=1; B=0; C=1; D=1; E=0; F=1; G=1; break;
    case 6: A=1; B=0; C=1; D=1; E=1; F=1; G=1; break;
    case 7: A=1; B=1; C=1; D=0; E=0; F=0; G=0; break;
    case 8: A=1; B=1; C=1; D=1; E=1; F=1; G=1; break;
    case 9: A=1; B=1; C=1; D=1; E=0; F=1; G=1; break;
    default: A=0; B=0; C=0; D=0; E=0; F=0; G=0; break;
  }

  // Pack into bits [Q0..Q6] => bit positions [0..6]
  uint8_t bits = 0;
  bits |= (A ? (1 << 0) : 0);
  bits |= (B ? (1 << 1) : 0);
  bits |= (C ? (1 << 2) : 0);
  bits |= (D ? (1 << 3) : 0);
  bits |= (E ? (1 << 4) : 0);
  bits |= (F ? (1 << 5) : 0);
  bits |= (G ? (1 << 6) : 0);

  return bits;
}

static void srWrite7Segments(uint8_t segBits) {
  // We shift out the 8-bit value MSB->LSB (Arduino shiftOut default),
  // but only Q0..Q6 are wired. We'll shift out bits [7..0] with bit7=0.
  // Ensure that Q0 corresponds to LSB.
  uint8_t value = segBits & 0x7F; // bits 0..6 used, bit7=0

  // Manual shiftOut for deterministic timing with our pins
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (value >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
  }

  // Latch
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_STCP, HIGH);
}

const uint8_t RGB1_R = 2;
const uint8_t RGB1_G = 3;
const uint8_t RGB1_B = 4;

const uint8_t RGB2_R = 5;
const uint8_t RGB2_G = 6;
const uint8_t RGB2_B = 7;

const uint8_t RGB3_R = 8;
const uint8_t RGB3_G = 9;
const uint8_t RGB3_B = 10;

static void setAllRGB(bool r, bool g, bool b) {
  // RGB LEDs are common cathode; driving pin HIGH turns that color ON.
  digitalWrite(RGB1_R, r); digitalWrite(RGB1_G, g); digitalWrite(RGB1_B, b);
  digitalWrite(RGB2_R, r); digitalWrite(RGB2_G, g); digitalWrite(RGB2_B, b);
  digitalWrite(RGB3_R, r); digitalWrite(RGB3_G, g); digitalWrite(RGB3_B, b);
}

static int servoAngleForCountdown(uint8_t n) {
  // When countdown is 7-9 => 0 degrees
  // When countdown is 4-6 => 60 degrees
  // When countdown is 1-3 => 120 degrees
  // When countdown is 0 => 180 degrees
  if (n == 0) return 180;
  if (n >= 1 && n <= 3) return 120;
  if (n >= 4 && n <= 6) return 60;
  if (n >= 7 && n <= 9) return 0;
  return 0;
}

void setup() {
  // RGB pins
  pinMode(RGB1_R, OUTPUT); pinMode(RGB1_G, OUTPUT); pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT); pinMode(RGB2_G, OUTPUT); pinMode(RGB2_B, OUTPUT);
  pinMode(RGB3_R, OUTPUT); pinMode(RGB3_G, OUTPUT); pinMode(RGB3_B, OUTPUT);

  // Initialize RGB off
  setAllRGB(false, false, false);

  // 74HC595 pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_SHCP, LOW);

  // Servos (PWM pins per wiring)
  servo1.attach(11);
  servo2.attach(12);
  servo3.attach(13);

  // Start with a valid initial state
  uint8_t initialDigit = 9;
  srWrite7Segments(segBitsForDigit(initialDigit));

  int ang = servoAngleForCountdown(initialDigit);
  servo1.write(ang);
  servo2.write(ang);
  servo3.write(ang);
}

void loop() {
  // Color sequence: Red, Green, Cyan, Magenta, Blue, Yellow, White
  // Each held for 2 seconds in sync with countdown states.
  struct RGBColor { bool r,g,b; };
  const RGBColor colors[7] = {
    {true,  false, false}, // Red
    {false, true,  false}, // Green
    {false, true,  true }, // Cyan
    {true,  false, true }, // Magenta
    {false, false, true }, // Blue
    {true,  true,  false}, // Yellow
    {true,  true,  true }  // White
  };

  // Countdown from 9 to 0, each number for 2 seconds.
  for (int8_t n = 9, colorIndex = 0; n >= 0; --n) {
    // Set 7-seg display
    srWrite7Segments(segBitsForDigit((uint8_t)n));

    // Set servo angles in sync with countdown
    int angle = servoAngleForCountdown((uint8_t)n);
    servo1.write(angle);
    servo2.write(angle);
    servo3.write(angle);

    // Set RGB color in sync with each 2s state
    // Need an index that advances each 2s and wraps through 7 colors.
    const RGBColor &c = colors[colorIndex % 7];
    setAllRGB(c.r, c.g, c.b);

    // Hold for 2 seconds
    delay(2000);

    // Advance color index for next countdown value
    colorIndex++;
  }
}
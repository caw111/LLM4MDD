#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_59\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// RGB LED pins (common cathode)
const uint8_t RGB1_R = 2;
const uint8_t RGB1_G = 3;
const uint8_t RGB1_B = 4;

const uint8_t RGB2_R = 5;
const uint8_t RGB2_G = 6;
const uint8_t RGB2_B = 7;

const uint8_t RGB3_R = 8;
const uint8_t RGB3_G = 9;
const uint8_t RGB3_B = 10;

// 74HC595 pins (as wired)
const uint8_t SR_DS  = A0; // DS
const uint8_t SR_STCP = A1; // STCP
const uint8_t SR_SHCP = A2; // SHCP

// Segment mapping on sevseg1 (from provided SR-Q to segments wiring):
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// We'll assume typical active-high segment drive (as no inversion given).
static const uint8_t SEG_A = (1 << 0);
static const uint8_t SEG_B = (1 << 1);
static const uint8_t SEG_C = (1 << 2);
static const uint8_t SEG_D = (1 << 3);
static const uint8_t SEG_E = (1 << 4);
static const uint8_t SEG_F = (1 << 5);
static const uint8_t SEG_G = (1 << 6);

// Commonly used segment patterns for digits 0-9 on a 7-seg:
// (a,b,c,d,e,f,g) = active segments
// 0: a b c d e f
// 1: b c
// 2: a b d e g
// 3: a b c d g
// 4: b c f g
// 5: a c d f g
// 6: a c d e f g
// 7: a b c
// 8: a b c d e f g
// 9: a b c d f g
static const uint8_t DIGIT_MASKS[10] = {
  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),                 // 0
  (SEG_B | SEG_C),                                               // 1
  (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G),                      // 2
  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G),                      // 3
  (SEG_B | SEG_C | SEG_F | SEG_G),                               // 4
  (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G),                      // 5
  (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),             // 6
  (SEG_A | SEG_B | SEG_C),                                      // 7
  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),     // 8
  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)              // 9
};

#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_59\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srShiftOut7(uint8_t value);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_59\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWriteSegments(uint8_t segmentMask);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_59\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllRGB(bool r, bool g, bool b);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_59\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void angleForCountdown(uint8_t n, int &a1, int &a2, int &a3);
#line 117 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_59\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 158 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_59\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_59\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srShiftOut7(uint8_t value) {
  // Shift 8 bits into 74HC595; Q7 unused in wiring, keep it LOW.
  // Order: MSB first
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_DS, (value & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }
}

static void srWriteSegments(uint8_t segmentMask) {
  // DS contains bits mapped to Q0..Q6 and segmentMask uses bits 0..6.
  // Q7 (bit 7) unused; set to 0.
  uint8_t payload = (segmentMask & 0x7F); // ensure bit7 = 0

  srShiftOut7(payload);
  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

static void setAllRGB(bool r, bool g, bool b) {
  // Common cathode: drive HIGH to turn LED ON
  const uint8_t Rv = r ? HIGH : LOW;
  const uint8_t Gv = g ? HIGH : LOW;
  const uint8_t Bv = b ? HIGH : LOW;

  // rgb1
  digitalWrite(RGB1_R, Rv);
  digitalWrite(RGB1_G, Gv);
  digitalWrite(RGB1_B, Bv);
  // rgb2
  digitalWrite(RGB2_R, Rv);
  digitalWrite(RGB2_G, Gv);
  digitalWrite(RGB2_B, Bv);
  // rgb3
  digitalWrite(RGB3_R, Rv);
  digitalWrite(RGB3_G, Gv);
  digitalWrite(RGB3_B, Bv);
}

static void angleForCountdown(uint8_t n, int &a1, int &a2, int &a3) {
  // When countdown is 7-9: 0 degrees
  // When countdown is 4-6: 60 degrees
  // When countdown is 1-3: 120 degrees
  // When countdown is 0: 180 degrees
  int ang = 0;
  if (n == 0) ang = 180;
  else if (n >= 1 && n <= 3) ang = 120;
  else if (n >= 4 && n <= 6) ang = 60;
  else if (n >= 7 && n <= 9) ang = 0;

  a1 = ang;
  a2 = ang;
  a3 = ang;
}

void setup() {
  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);

  // Initialize RGB off
  setAllRGB(false, false, false);

  // 74HC595 pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);

  // Servos
  // Wiring: servo1 PWM->mega:11, servo2 PWM->mega:12, servo3 PWM->mega:13
  servo1.attach(11);
  servo2.attach(12);
  servo3.attach(13);

  // Move servos to a known starting position (0 degrees)
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Set initial 7-seg to 9 (optional; countdown starts at 9)
  srWriteSegments(DIGIT_MASKS[9]);
}

void loop() {
  // RGB color sequence (all three simultaneously)
  // Red, Green, Cyan, Magenta, Blue, Yellow, White
  const uint8_t COLORS = 7;

  // Each for 2 seconds in sync with countdown steps.
  // We'll keep a separate RGB index aligned to each countdown value.
  uint8_t rgbIndex = 0;

  while (true) {
    // Countdown from 9 to 0, each number for 2 seconds
    for (int n = 9; n >= 0; --n) {
      // Set RGB color based on current step to keep synchronization.
      // There are 10 steps (9..0) but 7 colors; cycle RGB colors continuously.
      // This preserves "each color displayed for 2 seconds" while remaining in sync state length.
      uint8_t stepColor = rgbIndex % COLORS;

      switch (stepColor) {
        case 0: setAllRGB(true,  false, false); break; // Red
        case 1: setAllRGB(false, true,  false); break; // Green
        case 2: setAllRGB(false, true,  true);  break; // Cyan
        case 3: setAllRGB(true,  false, true);  break; // Magenta
        case 4: setAllRGB(false, false, true);  break; // Blue
        case 5: setAllRGB(true,  true,  false); break; // Yellow
        case 6: setAllRGB(true,  true,  true);  break; // White
      }

      // Set 7-seg display
      srWriteSegments(DIGIT_MASKS[n]);

      // Set servos in sync with countdown
      int a1, a2, a3;
      angleForCountdown((uint8_t)n, a1, a2, a3);
      servo1.write(a1);
      servo2.write(a2);
      servo3.write(a3);

      // Hold this synchronized state for 2 seconds
      delay(2000);

      // Advance RGB color for next 2-second state
      rgbIndex++;
    }

    // After reaching 0 and resetting to 9, loop repeats indefinitely.
    // rgbIndex continues across cycles to preserve consistent 2-second color steps.
  }
}

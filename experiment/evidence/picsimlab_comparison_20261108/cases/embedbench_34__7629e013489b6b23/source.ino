#include <Arduino.h>
#include <Servo.h>
#include <ShiftRegister74HC595.h>

// --- 74HC595 + 7-seg wiring ---
// From circuit:
// mega:A0 -> sr1:DS (blue)
// mega:A1 -> sr1:STCP (purple)
// mega:A2 -> sr1:SHCP (gray)
static const uint8_t SR_DS   = A0;
static const uint8_t SR_STCP = A1;
static const uint8_t SR_SHCP = A2;

// Sevseg segments are wired from Q0..Q6 of the 74HC595:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// COM.1 is controlled by the 595 (sr1:MR is tied to VCC by wiring, so OE/MR not used).
// We'll output standard active-HIGH segment codes.

static const uint8_t sevsegDigits[10] = {
  // pgfedcba (bit order for 74HC595 library is determined by setAll/ set bit functions;
  // We'll use direct byte output where bit0 corresponds to Q0, bit1 to Q1, ..., bit7 to Q7.
  // Segment mapping:
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  // For common cathode style (typical for 74HC595 with direct segment drive):
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
  //
  // Bits (Q6..Q0 for G..A):
  // bit0=Q0(A), bit1=Q1(B), bit2=Q2(C), bit3=Q3(D),
  // bit4=Q4(E), bit5=Q5(F), bit6=Q6(G)
  //
  // 0 -> A B C D E F = bits 0,1,2,3,4,5
  0b00111111, // 0
  // 1 -> B C = bits 1,2
  0b00000110, // 1
  // 2 -> A B D E G = bits 0,1,3,4,6
  0b01011011, // 2
  // 3 -> A B C D G = bits 0,1,2,3,6
  0b01011111, // 3
  // 4 -> B C F G = bits 1,2,5,6
  0b01100110, // 4
  // 5 -> A C D F G = bits 0,2,3,5,6
  0b01101101, // 5
  // 6 -> A C D E F G = bits 0,2,3,4,5,6
  0b01111101, // 6
  // 7 -> A B C = bits 0,1,2
  0b00000111, // 7
  // 8 -> A B C D E F G = bits 0..6
  0b01111111, // 8
  // 9 -> A B C D F G = bits 0,1,2,3,5,6
  0b01101111  // 9
};

ShiftRegister74HC595<1> sr1(SR_DS, SR_STCP, SR_SHCP);

// Servos
static const uint8_t SERVO1_PWM = 2; // mega:2 -> servo1:PWM (orange)
static const uint8_t SERVO2_PWM = 3; // mega:3 -> servo2:PWM (orange)
static const uint8_t SERVO3_PWM = 4; // mega:4 -> servo3:PWM (orange)

Servo servo1;
Servo servo2;
Servo servo3;

static void setServosForNumber(int n) {
  int s1 = 0, s2 = 0, s3 = 0;

  if (n >= 0 && n <= 2) {
    s1 = 0;  s2 = 45;  s3 = 90;
  } else if (n >= 3 && n <= 5) {
    s1 = 45; s2 = 90;  s3 = 135;
  } else if (n >= 6 && n <= 8) {
    s1 = 90; s2 = 135; s3 = 180;
  } else if (n == 9) {
    s1 = 0;  s2 = 0;   s3 = 0;
  }

  // Move immediately and accurately to target positions.
  servo1.write(s1);
  servo2.write(s2);
  servo3.write(s3);

  // Ensure each servo completes its movement before starting the next one.
  // Since the requirement says "move in sequence ... each servo completing its movement
  // before the next one starts", we command them one-by-one with a settling delay.
  // The circuit/servos are generic; use a conservative time based on 0-180 travel.
  auto settleMs = [](int fromDeg, int toDeg) -> unsigned long {
    // Assume ~300ms for full 180deg; scale linearly.
    // Clamp minimum to allow the library to propagate the command.
    long delta = abs(toDeg - fromDeg);
    unsigned long ms = (unsigned long)((delta * 300L) / 180L);
    if (ms < 50) ms = 50;
    return ms;
  };

  static int last1 = 0, last2 = 0, last3 = 0;

  // Sequential: servo1 then servo2 then servo3.
  servo1.write(s1);
  delay(settleMs(last1, s1));
  last1 = s1;

  servo2.write(s2);
  delay(settleMs(last2, s2));
  last2 = s2;

  servo3.write(s3);
  delay(settleMs(last3, s3));
  last3 = s3;
}

static void showDigit(int d) {
  // Output the segments for digit d via 74HC595.
  // Only Q0..Q6 used. If the display is active-low, you'd invert; requirement doesn't specify.
  // We'll drive as active-high.
  sr1.setAll(sevsegDigits[d]);
}

void setup() {
  // 7-seg initial "0"
  sr1.setAll(sevsegDigits[0]);
  sr1.update();

  // Servos initial position 0 degrees
  servo1.attach(SERVO1_PWM);
  servo2.attach(SERVO2_PWM);
  servo3.attach(SERVO3_PWM);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Allow initial positioning
  delay(600);
}

void loop() {
  static int current = 0;

  // Display current number for 2 seconds.
  showDigit(current);
  // Move servos when the number changes.
  // In this loop, "current" changes each iteration, so always move.
  setServosForNumber(current);

  delay(2000);

  current++;
  if (current > 9) current = 0;
}
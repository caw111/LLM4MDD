#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static int lastPot1 = -1;
static int lastPot2 = -1;

static int lastCmd1 = -1;
static int lastCmd2 = -1;
static int lastCmd3 = -1;

static int currentCmd1 = 0;
static int currentCmd2 = 0;
static int currentCmd3 = 0;

static const uint8_t POT1_PIN = A3; // pot1 -> A3
static const uint8_t POT2_PIN = A4; // pot2 -> A4

// Output PWM pins per wiring:
// servo1: mega:2, servo2: mega:3, servo3: mega:4

static int mapPotToAngle(int potVal) {
  // Requirement: 0..1023 -> 0..180 inclusive (linear)
  if (potVal < 0) potVal = 0;
  if (potVal > 1023) potVal = 1023;
  // Use integer math with rounding to nearest for tight quantization.
  // angle = round(potVal * 180 / 1023)
  long num = (long)potVal * 180L;
  int angle = (int)((num + 1023L / 2L) / 1023L);
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  return angle;
}

void setup() {
  servo1.attach(2); // mega:2 -> servo1:PWM
  servo2.attach(3); // mega:3 -> servo2:PWM
  servo3.attach(4); // mega:4 -> servo3:PWM

  // Initialize with current potentiometer positions
  lastPot1 = analogRead(POT1_PIN);
  lastPot2 = analogRead(POT2_PIN);

  lastCmd1 = mapPotToAngle(lastPot1);
  lastCmd2 = mapPotToAngle(lastPot2);
  lastCmd3 = lastCmd2;

  currentCmd1 = lastCmd1;
  currentCmd2 = lastCmd2;
  currentCmd3 = lastCmd3;

  servo1.write(currentCmd1);
  servo2.write(currentCmd2);
  servo3.write(currentCmd3);

  // Keep a deterministic loop timing for "not exceeding 50ms" response.
  // We'll sample and step at least every 10ms.
  // (No extra delays here.)
}

void loop() {
  // Read potentiometers (fast enough for <=50ms response)
  int pot1 = analogRead(POT1_PIN);
  int pot2 = analogRead(POT2_PIN);

  // Determine target commands
  int target1 = mapPotToAngle(pot1);
  int target2 = mapPotToAngle(pot2);
  int target3 = target2;

  // Smooth motion: step toward target by at most 1 degree per loop iteration.
  // With analogRead + small operations, this maintains a smooth response while
  // ensuring commands track changes quickly (<=50ms) in typical conditions.
  if (target1 != currentCmd1) {
    if (target1 > currentCmd1) currentCmd1++;
    else currentCmd1--;
    servo1.write(currentCmd1);
  }

  if (target2 != currentCmd2) {
    if (target2 > currentCmd2) currentCmd2++;
    else currentCmd2--;
    servo2.write(currentCmd2);

    // servo3 always moves in unison with servo2
    currentCmd3 = currentCmd2;
    servo3.write(currentCmd3);
  } else if (target3 != currentCmd3) {
    // This case is redundant given unison logic, but kept for correctness.
    if (target3 > currentCmd3) currentCmd3++;
    else currentCmd3--;
    servo3.write(currentCmd3);
  }

  // Update last tracking values (for potential external checks)
  lastPot1 = pot1;
  lastPot2 = pot2;
  lastCmd1 = target1;
  lastCmd2 = target2;
  lastCmd3 = target3;

  // No blocking delay to avoid exceeding the 50ms command response requirement.
  // If needed for stability, keep minimal yielding; none here.
}
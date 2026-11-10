#include <Arduino.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const int angleSteps[7][3] = {
  {0,   0,   0},   // step 1
  {0,  45,  90},   // step 2
  {45, 90, 135},   // step 3
  {90, 135, 180},  // step 4
  {135,180,  0},   // step 5
  {180, 0,  45},   // step 6
  {0,  45,  90}    // placeholder; loop returns to step 2
};

static void moveSmoothTo(int a1, int a2, int a3, unsigned long totalMoveMs = 600UL) {
  // Smooth movement: interpolate from current to target angles in small steps.
  const int start1 = servo1.read();
  const int start2 = servo2.read();
  const int start3 = servo3.read();

  const int delta1 = a1 - start1;
  const int delta2 = a2 - start2;
  const int delta3 = a3 - start3;

  // Choose number of interpolation steps based on the largest angular delta.
  int maxAbsDelta = abs(delta1);
  maxAbsDelta = max(maxAbsDelta, abs(delta2));
  maxAbsDelta = max(maxAbsDelta, abs(delta3));

  // At least one step; cap to keep it responsive.
  const int steps = max(1, min(maxAbsDelta, 60));

  // Evenly distribute time across steps.
  unsigned long stepDelay = totalMoveMs / (unsigned long)steps;
  if (stepDelay == 0) stepDelay = 1;

  for (int i = 1; i <= steps; i++) {
    float t = (float)i / (float)steps;

    int cur1 = start1 + (int)lroundf((float)delta1 * t);
    int cur2 = start2 + (int)lroundf((float)delta2 * t);
    int cur3 = start3 + (int)lroundf((float)delta3 * t);

    servo1.write(cur1);
    servo2.write(cur2);
    servo3.write(cur3);

    delay(stepDelay);
  }
}

void setup() {
  servo1.attach(2); // mega:2 -> servo1:PWM (orange)
  servo2.attach(3); // mega:3 -> servo2:PWM (orange)
  servo3.attach(4); // mega:4 -> servo3:PWM (orange)

  // Initialization: all servos start at 0 degrees.
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Give time to settle at startup.
  delay(500);
}

void loop() {
  // Sequence steps:
  // Step 1: {0,0,0} then 2s pause
  // Steps 2..6 per requirement, then repeat from step 2.
  moveSmoothTo(0, 0, 0);
  delay(2000);

  // Steps 2..6 repeat indefinitely.
  while (true) {
    // Step 2
    moveSmoothTo(0, 45, 90);
    delay(2000);

    // Step 3
    moveSmoothTo(45, 90, 135);
    delay(2000);

    // Step 4
    moveSmoothTo(90, 135, 180);
    delay(2000);

    // Step 5
    moveSmoothTo(135, 180, 0);
    delay(2000);

    // Step 6
    moveSmoothTo(180, 0, 45);
    delay(2000);

    // Loop continues from step 2 automatically
  }
}
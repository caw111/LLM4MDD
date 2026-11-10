#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const int angles1[] = {0, 0, 45, 90, 135, 180};
static const int angles2[] = {0, 45, 90, 135, 180, 0};
static const int angles3[] = {0, 90, 135, 180, 0, 45};
static const unsigned long dwellMs = 2000;

void setup() {
  // Attach PWM-capable pins per provided wiring:
  // servo1 -> mega:2 (PWM), servo2 -> mega:3 (PWM), servo3 -> mega:4 (PWM)
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);

  // Initialize all servos at 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  delay(20);
}

void loop() {
  for (int step = 0; step < 6; step++) {
    // Move immediately to the step's angles (synchronized)
    servo1.write(angles1[step]);
    servo2.write(angles2[step]);
    servo3.write(angles3[step]);

    // Maintain position for 2 seconds
    delay(dwellMs);
  }
}
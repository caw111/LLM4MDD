#include <Servo.h>

Servo servo1;

const int led1Pin = 2;   // wired to mega pin 2
const int led2Pin = 3;   // wired to mega pin 3
const int pot1Pin = A3;  // wired to pot1 SIG -> mega A3
const int servoPwmPin = 4; // wired to mega pin 4 (servo PWM)

static void setLEDs(bool led1On, bool led2On) {
  digitalWrite(led1Pin, led1On ? HIGH : LOW);
  digitalWrite(led2Pin, led2On ? HIGH : LOW);
}

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  setLEDs(false, false); // both LEDs off on reset/power-up

  servo1.attach(servoPwmPin);
  servo1.write(0);       // servo at 0 degrees on reset/power-up
  delay(50);

  // Ensure a stable starting reference for direction detection
  delay(50);
}

void loop() {
  // Read potentiometer and map to servo angle (0..180)
  int potVal = analogRead(pot1Pin);                 // 0..1023
  int targetAngle = map(potVal, 0, 1023, 0, 180);  // 0..180

  // Determine direction by comparing to previous target angle.
  // Maintain each state for at least 2 seconds.
  static int prevAngle = 0;
  static int currentAngle = 0;
  static unsigned long stateSinceMs = 0;
  static bool initted = false;

  if (!initted) {
    currentAngle = targetAngle;
    prevAngle = targetAngle;
    servo1.write(currentAngle);
    stateSinceMs = millis();
    initted = true;

    // Stationary at initialization -> both LEDs off
    setLEDs(false, false);
  }

  // Update servo (and state) only after 2 seconds to satisfy the rule
  unsigned long now = millis();
  if (now - stateSinceMs >= 2000UL) {
    prevAngle = currentAngle;

    // Capture a fresh potentiometer reading for the next 2-second state
    potVal = analogRead(pot1Pin);
    targetAngle = map(potVal, 0, 1023, 0, 180);

    currentAngle = targetAngle;
    servo1.write(currentAngle);

    int delta = currentAngle - prevAngle;

    if (delta > 0) {
      // clockwise: led1 on, led2 off
      setLEDs(true, false);
    } else if (delta < 0) {
      // counterclockwise: led2 on, led1 off
      setLEDs(false, true);
    } else {
      // stationary: both off
      setLEDs(false, false);
    }

    stateSinceMs = now;
  }

  // Keep loop responsive without changing the state before 2 seconds elapse.
}
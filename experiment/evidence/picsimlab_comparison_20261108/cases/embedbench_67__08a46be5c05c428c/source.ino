#include <Servo.h>

const int btn1Pin = 2;   // push button signal
const int servo1Pin = 3; // servo1 PWM
const int servo2Pin = 4; // servo2 PWM

Servo servo1;
Servo servo2;

// Debounce parameters
const unsigned long debounceDelayMs = 50;

int lastButtonReading = HIGH;
int stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;

bool toggled = false; // current position state: false => (0,180), true => (180,0)

void setup() {
  pinMode(btn1Pin, INPUT_PULLUP);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Initialize both servos to 0°
  servo1.write(0);
  servo2.write(0);

  // As per requirement: initial positions should be 0° for both after reset/power on.
  // The first button press will toggle to (180,0).
  toggled = false;

  // Initialize debounce tracking
  lastButtonReading = digitalRead(btn1Pin);
  stableButtonState = lastButtonReading;
  lastDebounceTime = millis();
}

void moveSmoothTo(Servo &s, int targetDeg) {
  static int currentDeg1 = 0; // not used directly; workaround below
  static int currentDeg2 = 0; // not used directly; workaround below

  // Track current position per-servo using function-local static variables isn't possible.
  // We'll infer current position by writing increments from known toggled states.
  // Since requirements only alternate between 0 and 180, we keep it deterministic:
  // (0 -> 180) or (180 -> 0).
  // We'll step in small increments to make movement smooth.

  int startDeg = (targetDeg == 180) ? 0 : 180;
  int step = (targetDeg > startDeg) ? 1 : -1;

  for (int d = startDeg; d != targetDeg; d += step) {
    s.write(d);
    delay(10);
  }
  s.write(targetDeg);
}

void loop() {
  int reading = digitalRead(btn1Pin);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) >= debounceDelayMs) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // Button pressed when input goes LOW (INPUT_PULLUP)
      if (stableButtonState == LOW) {
        // Toggle servos
        toggled = !toggled;

        if (!toggled) {
          // (servo1=0, servo2=180)
          moveSmoothTo(servo2, 180);
          moveSmoothTo(servo1, 0);
        } else {
          // (servo1=180, servo2=0)
          moveSmoothTo(servo1, 180);
          moveSmoothTo(servo2, 0);
        }
      }
    }
  }
}
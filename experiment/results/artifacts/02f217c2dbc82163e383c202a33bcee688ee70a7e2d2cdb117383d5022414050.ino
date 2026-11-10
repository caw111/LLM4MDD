#include <Servo.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t SERVO1_PIN = 3;
constexpr uint8_t SERVO2_PIN = 4;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long SERVO_STEP_MS = 15;

Servo servo1;
Servo servo2;

int servo1Position = 0;
int servo2Position = 0;
int servo1Target = 0;
int servo2Target = 0;

// The first press selects servo1 = 180 degrees, servo2 = 0 degrees.
bool alternateState = false;

uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;
unsigned long lastServoStepMs = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Preload initial positions before enabling servo pulses.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  lastRawButtonState = digitalRead(BUTTON_PIN);
  lastButtonChangeMs = millis();
  lastServoStepMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeMs = now;
  }

  if (now - lastButtonChangeMs >= DEBOUNCE_MS &&
      rawButtonState != stableButtonState) {
    stableButtonState = rawButtonState;

    // INPUT_PULLUP makes a button press active LOW.
    if (stableButtonState == LOW) {
      alternateState = !alternateState;
      servo1Target = alternateState ? 180 : 0;
      servo2Target = alternateState ? 0 : 180;
    }
  }

  // Nonblocking movement keeps button debouncing active during travel.
  if (now - lastServoStepMs >= SERVO_STEP_MS) {
    lastServoStepMs = now;

    if (servo1Position != servo1Target) {
      servo1Position += (servo1Position < servo1Target) ? 1 : -1;
      servo1.write(servo1Position);
    }

    if (servo2Position != servo2Target) {
      servo2Position += (servo2Position < servo2Target) ? 1 : -1;
      servo2.write(servo2Position);
    }
  }
}
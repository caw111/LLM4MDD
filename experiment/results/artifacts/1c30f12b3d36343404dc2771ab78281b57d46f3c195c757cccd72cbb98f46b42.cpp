#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_93\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;

static const int led1Pin = 2;   // led1 -> mega 2
static const int led2Pin = 3;   // led2 -> mega 3
static const int pot1Pin  = A3;  // pot1:SIG -> mega A3

// Debounce / timing rule: maintain each state for at least 2 seconds
static const unsigned long MIN_STATE_MS = 2000;

// Servo movement direction thresholds
static const int ANGLE_DELTA_TO_COUNT = 1; // minimal angle change to consider movement

int lastAngle = 0;

enum MoveState : uint8_t { STATIONARY = 0, CLOCKWISE = 1, COUNTERCLOCKWISE = 2 };
MoveState currentState = STATIONARY;
unsigned long stateStartMs = 0;

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_93\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readPotAngle();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_93\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static MoveState computeMoveState(int prevAngle, int newAngle);
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_93\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyLEDs(MoveState st);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_93\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_93\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_93\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readPotAngle()
{
  int potVal = analogRead(pot1Pin); // 0..1023
  long angle = map(potVal, 0, 1023, 0, 180); // 0..180
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  return (int)angle;
}

static MoveState computeMoveState(int prevAngle, int newAngle)
{
  int delta = newAngle - prevAngle;
  if (delta > ANGLE_DELTA_TO_COUNT) return CLOCKWISE;
  if (delta < -ANGLE_DELTA_TO_COUNT) return COUNTERCLOCKWISE;
  return STATIONARY;
}

static void applyLEDs(MoveState st)
{
  switch (st)
  {
    case CLOCKWISE:
      digitalWrite(led1Pin, HIGH);
      digitalWrite(led2Pin, LOW);
      break;
    case COUNTERCLOCKWISE:
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, HIGH);
      break;
    case STATIONARY:
    default:
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, LOW);
      break;
  }
}

void setup()
{
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  servo1.attach(4); // mega:4 -> servo1:PWM
  servo1.write(0);  // Initialize servo to 0 degrees
  lastAngle = 0;

  currentState = STATIONARY;
  stateStartMs = millis();
  applyLEDs(currentState);
}

void loop()
{
  // Read potentiometer and map to servo target angle
  int targetAngle = readPotAngle();

  // Determine movement direction based on lastAngle vs targetAngle
  MoveState desiredState = computeMoveState(lastAngle, targetAngle);

  // Maintain each state at least 2 seconds
  unsigned long now = millis();
  if (desiredState != currentState && (now - stateStartMs) >= MIN_STATE_MS)
  {
    currentState = desiredState;
    stateStartMs = now;
    applyLEDs(currentState);
  }

  // Always command servo to follow potentiometer (position control behavior)
  servo1.write(targetAngle);
  lastAngle = targetAngle;

  // Loop continues; state update timing handled above
}

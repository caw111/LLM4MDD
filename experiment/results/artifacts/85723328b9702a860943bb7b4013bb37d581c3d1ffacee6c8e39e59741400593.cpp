#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_118\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;

static const int BTN1_PIN = 3;
static const int BTN2_PIN = 4;
static const int LED1_PIN  = 2;
static const int SERVO_PIN = 5;

static const int SERVO_MIN_ANGLE = 0;
static const int SERVO_MAX_ANGLE = 180;
static const int SERVO_STEP_DEG  = 30;
static const int START_ANGLE     = 90;

static const unsigned long MOVE_LED_MIN_ON_MS = 2000;

int currentAngle = START_ANGLE;

bool lastBtn1 = HIGH;
bool lastBtn2 = HIGH;

// Returns true on rising edge (button pressed assuming INPUT_PULLUP)
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_118\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool buttonPressed(int pin, bool &lastState);
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_118\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void moveServoWithLed(int targetAngle);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_118\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_118\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_118\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool buttonPressed(int pin, bool &lastState) {
  bool now = digitalRead(pin);
  bool pressed = (lastState == HIGH && now == LOW);
  lastState = now;
  return pressed;
}

void moveServoWithLed(int targetAngle) {
  // Clamp just in case
  if (targetAngle < SERVO_MIN_ANGLE) targetAngle = SERVO_MIN_ANGLE;
  if (targetAngle > SERVO_MAX_ANGLE) targetAngle = SERVO_MAX_ANGLE;

  // No effect if already at limit (or same angle)
  if (targetAngle == currentAngle) {
    digitalWrite(LED1_PIN, LOW);
    return;
  }

  // Turn LED on while servo is moving
  digitalWrite(LED1_PIN, HIGH);

  // Start movement
  unsigned long startMs = millis();
  servo1.write(targetAngle);

  // Wait until servo likely reaches the new angle.
  // Use a conservative time based on angle difference.
  int diff = abs(targetAngle - currentAngle);
  // Estimate: typical servo ~0.1-0.2 s per 60 degrees. We'll be conservative.
  // 900 ms per 180 degrees => 5 ms per degree.
  unsigned long estimatedMoveMs = (unsigned long)diff * 5UL;
  if (estimatedMoveMs < 50) estimatedMoveMs = 50;

  // Ensure LED remains on at least 2 seconds during movement requirement
  unsigned long minOnUntil = startMs + MOVE_LED_MIN_ON_MS;
  unsigned long moveUntil  = startMs + estimatedMoveMs;

  while ((unsigned long)millis() < moveUntil || (unsigned long)millis() < minOnUntil) {
    // Keep responsive to button bouncing? Behavior not required during movement,
    // so just wait.
    // (No button handling here by requirement emphasis on movement behavior.)
    delay(5);
  }

  currentAngle = targetAngle;
  digitalWrite(LED1_PIN, LOW);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  lastBtn1 = digitalRead(BTN1_PIN);
  lastBtn2 = digitalRead(BTN2_PIN);

  servo1.attach(SERVO_PIN);
  currentAngle = START_ANGLE;
  servo1.write(currentAngle);
}

void loop() {
  // Debounce via simple edge detection + delay after press
  if (buttonPressed(BTN1_PIN, lastBtn1)) {
    // If at max, no effect and LED remains off.
    if (currentAngle < SERVO_MAX_ANGLE) {
      int target = currentAngle + SERVO_STEP_DEG;
      if (target > SERVO_MAX_ANGLE) target = SERVO_MAX_ANGLE;

      // Ensure LED behavior depends on actual movement
      if (target != currentAngle) {
        moveServoWithLed(target);
      } else {
        digitalWrite(LED1_PIN, LOW);
      }
    } else {
      digitalWrite(LED1_PIN, LOW);
    }
    delay(30); // basic debounce
  }

  if (buttonPressed(BTN2_PIN, lastBtn2)) {
    // If at min, no effect and LED remains off.
    if (currentAngle > SERVO_MIN_ANGLE) {
      int target = currentAngle - SERVO_STEP_DEG;
      if (target < SERVO_MIN_ANGLE) target = SERVO_MIN_ANGLE;

      if (target != currentAngle) {
        moveServoWithLed(target);
      } else {
        digitalWrite(LED1_PIN, LOW);
      }
    } else {
      digitalWrite(LED1_PIN, LOW);
    }
    delay(30); // basic debounce
  }
}

#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_93\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr uint8_t SERVO_PIN = 4;
constexpr uint8_t POT_PIN = A3;

constexpr unsigned long MIN_STATE_MS = 2000UL;

Servo servo1;

enum MotionState : uint8_t {
  STATIONARY,
  CLOCKWISE,
  COUNTERCLOCKWISE
};

MotionState state = STATIONARY;
unsigned long stateStartedAt = 0;

int currentAngle = 0;
int startAngle = 0;
int targetAngle = 0;

#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_93\\direct\\attempt_0\\sketch\\sketch.ino"
void setState(MotionState nextState, unsigned long now);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_93\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_93\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_93\\direct\\attempt_0\\sketch\\sketch.ino"
void setState(MotionState nextState, unsigned long now) {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  if (nextState == CLOCKWISE) {
    digitalWrite(LED1_PIN, HIGH);
  } else if (nextState == COUNTERCLOCKWISE) {
    digitalWrite(LED2_PIN, HIGH);
  }

  state = nextState;
  stateStartedAt = now;
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Preload the initial pulse width before enabling servo output.
  servo1.write(0);
  servo1.attach(SERVO_PIN);
  servo1.write(0);

  setState(STATIONARY, millis());
}

void loop() {
  const unsigned long now = millis();
  const unsigned long elapsed = now - stateStartedAt;
  const int requestedAngle =
      static_cast<int>(map(analogRead(POT_PIN), 0, 1023, 0, 180));

  if (state == STATIONARY) {
    // Preserve the startup/stationary indication for at least two seconds.
    if (elapsed >= MIN_STATE_MS && requestedAngle != currentAngle) {
      startAngle = currentAngle;
      targetAngle = requestedAngle;

      // Latch each destination so potentiometer changes cannot interrupt
      // a direction indication before its minimum two-second duration.
      setState(targetAngle > currentAngle ? CLOCKWISE : COUNTERCLOCKWISE,
               now);
    }
    return;
  }

  // Spread each commanded movement over two seconds rather than jumping
  // immediately to its destination and leaving a direction LED latched.
  if (elapsed >= MIN_STATE_MS) {
    currentAngle = targetAngle;
    servo1.write(currentAngle);
    setState(STATIONARY, now);
    return;
  }

  const long delta = targetAngle - startAngle;
  const int nextAngle =
      startAngle +
      static_cast<int>((delta * static_cast<long>(elapsed)) /
                       static_cast<long>(MIN_STATE_MS));

  if (nextAngle != currentAngle) {
    currentAngle = nextAngle;
    servo1.write(currentAngle);
  }
}

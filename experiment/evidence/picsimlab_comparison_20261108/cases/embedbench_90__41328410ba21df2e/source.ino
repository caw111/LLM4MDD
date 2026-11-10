#include <Servo.h>

constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr uint8_t LED3_PIN = 4;
constexpr uint8_t SERVO1_PIN = 5;
constexpr uint8_t SERVO2_PIN = 6;

constexpr unsigned long STEP_TIME_MS = 2000;
constexpr int ANGLE_STEP = 30;

Servo servo1;
Servo servo2;

void allLedsOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  allLedsOff();

  // Preload the initial positions before enabling servo pulses.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Allow both servos to reach their power-on positions with all LEDs off.
  delay(STEP_TIME_MS);
}

void loop() {
  // Move immediately to each next angle, then hold for two seconds.
  digitalWrite(LED1_PIN, HIGH);
  for (int angle = ANGLE_STEP; angle <= 180; angle += ANGLE_STEP) {
    servo1.write(angle);
    delay(STEP_TIME_MS);
  }
  digitalWrite(LED1_PIN, LOW);

  // Servo2 starts its descending sequence at 180 degrees.
  digitalWrite(LED2_PIN, HIGH);
  for (int angle = 180; angle >= 0; angle -= ANGLE_STEP) {
    servo2.write(angle);
    delay(STEP_TIME_MS);
  }
  digitalWrite(LED2_PIN, LOW);

  // Both target positions have been reached.
  digitalWrite(LED3_PIN, HIGH);
  delay(STEP_TIME_MS);
  digitalWrite(LED3_PIN, LOW);

  // Subsequent cycles start with servo1 at 0 and servo2 at 180.
  servo1.write(0);
  servo2.write(180);

  // Allow the reset movement to finish before the next sequence.
  delay(STEP_TIME_MS);
}
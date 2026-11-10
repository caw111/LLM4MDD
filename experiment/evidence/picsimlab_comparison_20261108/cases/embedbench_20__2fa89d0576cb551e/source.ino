#include <Servo.h>

constexpr uint8_t SERVO_PIN = 2;
constexpr unsigned long HOLD_TIME_MS = 2000;

Servo servo1;
int position = 0;
int direction = 10;
unsigned long lastMoveTime = 0;

void setup() {
  servo1.attach(SERVO_PIN);
  servo1.write(position);
  lastMoveTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastMoveTime >= HOLD_TIME_MS) {
    if (position == 180) {
      direction = -10;
    } else if (position == 0) {
      direction = 10;
    }

    position += direction;
    servo1.write(position);
    lastMoveTime = now;
  }
}
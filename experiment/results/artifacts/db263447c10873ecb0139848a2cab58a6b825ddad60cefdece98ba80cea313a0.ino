#include <Servo.h>

constexpr uint8_t SERVO_PIN = 3;
constexpr uint8_t LED_PIN = 2;
constexpr uint8_t POSITION_POT_PIN = A3;
constexpr uint8_t BRIGHTNESS_POT_PIN = A4;
constexpr uint8_t SPEED_POT_PIN = A5;

Servo servo1;

int currentAngle = 90;
unsigned long lastStepTime = 0;

void setup() {
  pinMode(POSITION_POT_PIN, INPUT);
  pinMode(BRIGHTNESS_POT_PIN, INPUT);
  pinMode(SPEED_POT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  servo1.attach(SERVO_PIN);
  servo1.write(currentAngle);
  analogWrite(LED_PIN, 0);

  lastStepTime = millis();
}

void loop() {
  const int targetAngle =
      map(analogRead(POSITION_POT_PIN), 0, 1023, 0, 180);
  const int ledBrightness =
      map(analogRead(BRIGHTNESS_POT_PIN), 0, 1023, 0, 255);
  const unsigned long stepInterval =
      map(analogRead(SPEED_POT_PIN), 0, 1023, 10, 100);

  analogWrite(LED_PIN, ledBrightness);

  const unsigned long now = millis();

  // Non-blocking, one-degree steps keep all controls responsive.
  if (currentAngle != targetAngle &&
      now - lastStepTime >= stepInterval) {
    currentAngle += (targetAngle > currentAngle) ? 1 : -1;
    servo1.write(currentAngle);
    lastStepTime = now;
  } else if (currentAngle == targetAngle) {
    lastStepTime = now;
  }
}
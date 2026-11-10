#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 pin assignments, matching the supplied circuit.
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr uint8_t BARGRAPH_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
constexpr uint8_t SERVO1_PIN = 44;
constexpr uint8_t SERVO2_PIN = 45;

constexpr unsigned long STEP_DELAY_MS = 2000;

Servo servo1;
Servo servo2;

void updateIndicators(int servo1Angle, int servo2Angle) {
  digitalWrite(LED1_PIN, servo1Angle <= 90 ? HIGH : LOW);
  digitalWrite(LED2_PIN, servo2Angle >= 90 ? HIGH : LOW);

  // Integer scaling: 0 degrees -> 0, 90 -> 5, 180 -> 10.
  const uint8_t litSegments = (servo1Angle * 10) / 180;
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BARGRAPH_PINS[i], i < litSegments ? HIGH : LOW);
  }
}

void setup() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BARGRAPH_PINS[i], LOW);
    pinMode(BARGRAPH_PINS[i], OUTPUT);
  }

  // Preload zero-degree targets before enabling servo pulses.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Hold the required startup state: both servos at zero, all lights off.
  delay(STEP_DELAY_MS);
}

void loop() {
  // Repeat the specified opposing sweeps, including both endpoints.
  for (int angle = 0; angle <= 180; angle += 10) {
    const int servo1Angle = angle;
    const int servo2Angle = 180 - angle;

    servo1.write(servo1Angle);
    servo2.write(servo2Angle);
    updateIndicators(servo1Angle, servo2Angle);

    delay(STEP_DELAY_MS);
  }
}
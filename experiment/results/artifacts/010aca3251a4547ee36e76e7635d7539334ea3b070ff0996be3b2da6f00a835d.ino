#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t SERVO_COUNT = 3;
constexpr uint8_t LED_COUNT = 10;

constexpr uint8_t SERVO_PINS[SERVO_COUNT] = {2, 3, 4};
// Bar graph anodes A1 through A10, in order (not Arduino analog pins).
constexpr uint8_t LED_PINS[LED_COUNT] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

constexpr unsigned long UPDATE_INTERVAL_MS = 2000UL;
constexpr int ANGLE_STEP = 18;
constexpr int UNITS_PER_LED = 54;

Servo servos[SERVO_COUNT];
int angle = 0;
int direction = 1;
unsigned long lastUpdateMs = 0;

void updateBarGraph() {
  const int totalAngle = angle * SERVO_COUNT;
  const uint8_t litCount = totalAngle / UNITS_PER_LED;

  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], i < litCount ? HIGH : LOW);
  }
}

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
    // Preload the initial position before enabling servo pulses.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
    servos[i].write(0);
  }

  updateBarGraph();
  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();
  if (now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    lastUpdateMs += UPDATE_INTERVAL_MS;

    angle += direction * ANGLE_STEP;
    if (angle >= 180) {
      angle = 180;
      direction = -1;
    } else if (angle <= 0) {
      angle = 0;
      direction = 1;
    }

    // Issue all three position commands together in the same update.
    for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
      servos[i].write(angle);
    }

    updateBarGraph();
  }
}
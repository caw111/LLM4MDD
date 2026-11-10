#include <Servo.h>

constexpr uint8_t SERVO_PINS[] = {2, 3, 4};

// Bar graph anodes A1 through A10, in the supplied wiring order.
// These labels belong to the bar graph, not the Mega's analog pins.
constexpr uint8_t LED_PINS[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
constexpr uint8_t SERVO_COUNT = 3;
constexpr uint8_t LED_COUNT = 10;
constexpr unsigned long HOLD_TIME_MS = 2000UL;

Servo servos[SERVO_COUNT];

void setBarGraph(uint8_t illuminatedSegments) {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], i < illuminatedSegments ? HIGH : LOW);
  }
}

void resetSequence() {
  // Issue all three return commands together, without intervening delays.
  for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
    servos[i].write(0);
  }
  setBarGraph(0);
}

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
    // Preload the starting angle before enabling servo pulses.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
  }

  resetSequence();
  delay(HOLD_TIME_MS);
}

void loop() {
  // Phase 1: servo1 at 180 degrees; segments A1-A3 illuminated.
  servos[0].write(180);
  setBarGraph(3);
  delay(HOLD_TIME_MS);

  // Phase 2: servo1 remains at 180; servo2 joins it; A1-A6 illuminated.
  servos[1].write(180);
  setBarGraph(6);
  delay(HOLD_TIME_MS);

  // Phase 3: all servos at 180 degrees; A1-A10 illuminated.
  servos[2].write(180);
  setBarGraph(10);
  delay(HOLD_TIME_MS);

  // Reset all servos and LEDs together, then hold for two seconds.
  resetSequence();
  delay(HOLD_TIME_MS);
}
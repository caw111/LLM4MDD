#include <Servo.h>

// Arduino Mega 2560 wiring.
const uint8_t LED_PINS[3] = {2, 3, 4};
const uint8_t SERVO_PINS[3] = {5, 6, 7};

// Bar graph anodes A1-A8 connect to these digital pins.
// These are segment labels, not Arduino analog pin numbers.
const uint8_t BAR_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45};

const uint8_t SERVO_ANGLES[3] = {60, 120, 180};
const uint8_t LIT_SEGMENTS[4] = {0, 2, 5, 8};
const unsigned long STATE_DURATION_MS = 2000UL;

Servo servos[3];

// State 0 is reset; states 1-3 cumulatively activate each group.
// All commands are issued together without intervening delays.
void applyState(uint8_t state) {
  for (uint8_t i = 0; i < 3; ++i) {
    const bool active = (i < state);
    servos[i].write(active ? SERVO_ANGLES[i] : 0);
    digitalWrite(LED_PINS[i], active ? HIGH : LOW);
  }

  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(BAR_PINS[i], i < LIT_SEGMENTS[state] ? HIGH : LOW);
  }
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < 3; ++i) {
    // Set the initial pulse width before enabling servo pulses.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
  }

  applyState(0);
  delay(STATE_DURATION_MS);
}

void loop() {
  for (uint8_t state = 1; state <= 3; ++state) {
    applyState(state);
    delay(STATE_DURATION_MS);
  }

  applyState(0);
  delay(STATE_DURATION_MS);
}
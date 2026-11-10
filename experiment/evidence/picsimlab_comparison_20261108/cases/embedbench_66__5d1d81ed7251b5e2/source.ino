#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t SERVO_PINS[3] = {3, 4, 5};

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long MIN_STATE_TIME_MS = 2000;

constexpr uint8_t STATE_ANGLES[5][3] = {
  {0,   0,   0},    // State 1: initial position
  {90,  45,  135},  // State 2
  {180, 90,  0},    // State 3
  {45,  135, 90},   // State 4
  {0,   0,   0}     // State 5: distinct from State 1
};

Servo servos[3];

uint8_t currentState = 0;
uint8_t lastRawButton = HIGH;
uint8_t stableButton = HIGH;

unsigned long rawButtonChangedAt = 0;
unsigned long stateEnteredAt = 0;
uint32_t pendingPresses = 0;

void applyState(uint8_t state) {
  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(STATE_ANGLES[state][i]);
  }
}

void setup() {
  // Button connects pin 2 to GND when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  for (uint8_t i = 0; i < 3; ++i) {
    // Set the initial pulse width before enabling servo output.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
  }

  applyState(currentState);
  stateEnteredAt = millis();
  rawButtonChangedAt = stateEnteredAt;
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButton = digitalRead(BUTTON_PIN);

  if (rawButton != lastRawButton) {
    lastRawButton = rawButton;
    rawButtonChangedAt = now;
  }

  if (rawButton != stableButton &&
      (unsigned long)(now - rawButtonChangedAt) >= DEBOUNCE_MS) {
    stableButton = rawButton;

    // Count only debounced press edges, not releases or a held button.
    // Queue early presses so every state still lasts at least two seconds.
    if (stableButton == LOW && pendingPresses < UINT32_MAX) {
      ++pendingPresses;
    }
  }

  if (pendingPresses > 0 &&
      (unsigned long)(now - stateEnteredAt) >= MIN_STATE_TIME_MS) {
    --pendingPresses;
    currentState = (currentState + 1) % 5;
    applyState(currentState);
    stateEnteredAt = millis();
  }
}
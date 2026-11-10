#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t SERVO1_PIN = 3;
constexpr uint8_t SERVO2_PIN = 4;
constexpr uint8_t LED_PINS[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
constexpr uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

constexpr unsigned long DEBOUNCE_MS = 150UL;
constexpr unsigned long STATE_LOCK_MS = 2000UL;

Servo servo1;
Servo servo2;

uint8_t currentState = 0;
uint8_t lastRawButton = HIGH;
uint8_t stableButton = HIGH;

unsigned long rawButtonChangedAt = 0;
unsigned long stateEnteredAt = 0;

void applyState(uint8_t state) {
  uint8_t illuminatedSegments = 0;

  switch (state) {
    case 0:
      servo1.write(0);
      servo2.write(180);
      illuminatedSegments = 0;
      break;

    case 1:
      servo1.write(90);
      servo2.write(90);
      illuminatedSegments = 5;
      break;

    case 2:
      servo1.write(180);
      servo2.write(0);
      illuminatedSegments = 10;
      break;
  }

  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], i < illuminatedSegments ? HIGH : LOW);
  }
}

void setup() {
  // The button connects pin 2 to ground when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  // Initialize both servo commands to zero before enabling their outputs.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Allow an initial servo refresh with both commanded to zero.
  delay(20);

  // After initialization, enter State 0 and hold it for at least two seconds.
  currentState = 0;
  applyState(currentState);
  stateEnteredAt = millis();

  lastRawButton = digitalRead(BUTTON_PIN);
  stableButton = HIGH;
  rawButtonChangedAt = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButton = digitalRead(BUTTON_PIN);

  if (rawButton != lastRawButton) {
    lastRawButton = rawButton;
    rawButtonChangedAt = now;
  }

  if (rawButton != stableButton &&
      now - rawButtonChangedAt >= DEBOUNCE_MS) {
    stableButton = rawButton;

    // Only a debounced press edge can advance the state.
    // Presses during lockout are consumed, not queued; holding the
    // button never produces repeated transitions.
    if (stableButton == LOW &&
        now - stateEnteredAt >= STATE_LOCK_MS) {
      currentState = (currentState + 1) % 3;
      applyState(currentState);
      stateEnteredAt = millis();
    }
  }
}
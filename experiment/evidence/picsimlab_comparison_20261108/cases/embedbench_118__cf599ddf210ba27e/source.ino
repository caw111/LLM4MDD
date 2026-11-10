#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t LED_PIN = 2;
constexpr uint8_t INCREMENT_BUTTON_PIN = 3;
constexpr uint8_t DECREMENT_BUTTON_PIN = 4;
constexpr uint8_t SERVO_PIN = 5;

constexpr int ANGLE_STEP = 30;
constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long MOVEMENT_DURATION_MS = 2100;
constexpr unsigned long SERVO_UPDATE_MS = 20;

Servo servo1;

struct DebouncedButton {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeMs;

  void begin(uint8_t buttonPin) {
    pin = buttonPin;
    pinMode(pin, INPUT_PULLUP);
    lastReading = digitalRead(pin);
    // Allow a button held during startup to register after debouncing.
    stableState = HIGH;
    lastChangeMs = millis();
  }

  bool pressed(unsigned long now) {
    const uint8_t reading = digitalRead(pin);

    if (reading != lastReading) {
      lastReading = reading;
      lastChangeMs = now;
    }

    if (reading != stableState &&
        now - lastChangeMs >= DEBOUNCE_MS) {
      stableState = reading;
      return stableState == LOW;
    }

    return false;
  }
};

DebouncedButton incrementButton;
DebouncedButton decrementButton;

int targetAngle = 90;
int lastWrittenAngle = 90;
float currentAngle = 90.0f;
float movementStartAngle = 90.0f;

bool moving = false;
unsigned long movementStartMs = 0;
unsigned long lastServoUpdateMs = 0;

void updateMovement(unsigned long now) {
  if (!moving) {
    return;
  }

  const unsigned long elapsed = now - movementStartMs;

  if (elapsed >= MOVEMENT_DURATION_MS) {
    currentAngle = static_cast<float>(targetAngle);
    servo1.write(targetAngle);
    lastWrittenAngle = targetAngle;
    moving = false;
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // Spread the entire movement over at least two seconds rather than
  // reaching the destination immediately and merely delaying the LED.
  const float progress =
      static_cast<float>(elapsed) / MOVEMENT_DURATION_MS;

  currentAngle =
      movementStartAngle +
      (static_cast<float>(targetAngle) - movementStartAngle) * progress;

  if (now - lastServoUpdateMs >= SERVO_UPDATE_MS) {
    lastServoUpdateMs = now;

    // Truncation toward the starting angle avoids commanding the final
    // destination before the full movement interval has elapsed.
    int commandedAngle;
    if (targetAngle >= movementStartAngle) {
      commandedAngle = static_cast<int>(currentAngle);
    } else {
      commandedAngle = static_cast<int>(currentAngle + 0.9999f);
    }

    commandedAngle = constrain(commandedAngle, 0, 180);
    if (commandedAngle != lastWrittenAngle) {
      servo1.write(commandedAngle);
      lastWrittenAngle = commandedAngle;
    }
  }
}

void requestAngleChange(int change, unsigned long now) {
  const int newTarget = constrain(targetAngle + change, 0, 180);

  // A press against an endpoint does not start or extend a movement.
  if (newTarget == targetAngle) {
    return;
  }

  // New presses remain responsive during movement. Each changes the
  // requested destination by 30 degrees and smoothly retargets from
  // the current commanded position.
  movementStartAngle = static_cast<float>(lastWrittenAngle);
  currentAngle = movementStartAngle;
  targetAngle = newTarget;
  movementStartMs = now;
  lastServoUpdateMs = now;
  moving = true;
  digitalWrite(LED_PIN, HIGH);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  incrementButton.begin(INCREMENT_BUTTON_PIN);
  decrementButton.begin(DECREMENT_BUTTON_PIN);

  // Preload the startup position before enabling servo pulses.
  servo1.write(90);
  servo1.attach(SERVO_PIN);
  servo1.write(90);
}

void loop() {
  const unsigned long now = millis();

  updateMovement(now);

  const bool incrementPressed = incrementButton.pressed(now);
  const bool decrementPressed = decrementButton.pressed(now);

  if (incrementPressed) {
    requestAngleChange(ANGLE_STEP, now);
  }

  if (decrementPressed) {
    requestAngleChange(-ANGLE_STEP, now);
  }
}
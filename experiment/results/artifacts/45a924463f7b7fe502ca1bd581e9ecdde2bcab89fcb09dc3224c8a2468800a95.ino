#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t SERVO_PIN = 5;
constexpr uint8_t BUTTON_PINS[] = {2, 3, 4};
constexpr uint8_t POT_PINS[] = {A3, A4, A5};
constexpr uint8_t BUTTON_COUNT = 3;
constexpr unsigned long DEBOUNCE_MS = 25;

Servo servo1;

uint8_t lastRawState[BUTTON_COUNT] = {HIGH, HIGH, HIGH};
uint8_t stableState[BUTTON_COUNT] = {HIGH, HIGH, HIGH};
unsigned long lastChangeTime[BUTTON_COUNT] = {0, 0, 0};

int8_t activePot = -1;
int lastAngle = 0;

void setup() {
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    // Buttons connect their input pins to ground when pressed.
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    pinMode(POT_PINS[i], INPUT);
  }

  // Preload the initial position before enabling servo pulses.
  servo1.write(0);
  servo1.attach(SERVO_PIN);
  servo1.write(0);
}

void loop() {
  const unsigned long now = millis();

  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    const uint8_t rawState = digitalRead(BUTTON_PINS[i]);

    if (rawState != lastRawState[i]) {
      lastRawState[i] = rawState;
      lastChangeTime[i] = now;
    }

    if (rawState != stableState[i] &&
        now - lastChangeTime[i] >= DEBOUNCE_MS) {
      stableState[i] = rawState;

      // Select only on a new press; holding or releasing does not
      // override a later press of another button.
      if (stableState[i] == LOW) {
        activePot = static_cast<int8_t>(i);
      }
    }
  }

  // Stay at zero until a button selects a potentiometer.
  if (activePot >= 0) {
    const int potValue = analogRead(POT_PINS[activePot]);
    const int angle = static_cast<int>(map(potValue, 0, 1023, 0, 180));

    if (angle != lastAngle) {
      servo1.write(angle);
      lastAngle = angle;
    }
  }
}
#include <Servo.h>

const uint8_t BTN1_PIN = 2;
const uint8_t BTN2_PIN = 3;
const uint8_t SERVO1_PIN = 4;
const uint8_t SERVO2_PIN = 5;
const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;

const unsigned long DEBOUNCE_MS = 30;

Servo servo1;
Servo servo2;

struct DebouncedButton {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeTime;
};

DebouncedButton btn1 = {BTN1_PIN, HIGH, HIGH, 0};
DebouncedButton btn2 = {BTN2_PIN, HIGH, HIGH, 0};

// Returns true once per debounced press. Buttons are wired to GND.
bool wasPressed(DebouncedButton &button, unsigned long now) {
  const uint8_t reading = digitalRead(button.pin);

  if (reading != button.lastReading) {
    button.lastReading = reading;
    button.lastChangeTime = now;
  }

  if ((unsigned long)(now - button.lastChangeTime) >= DEBOUNCE_MS &&
      reading != button.stableState) {
    button.stableState = reading;
    return button.stableState == LOW;
  }

  return false;
}

int readTargetAngle(uint8_t potPin) {
  return (int)map(analogRead(potPin), 0, 1023, 0, 180);
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  // Preload zero degrees before attaching to avoid a default center command.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  const unsigned long now = millis();

  if (wasPressed(btn1, now)) {
    servo1.write(readTargetAngle(POT1_PIN));
  }

  if (wasPressed(btn2, now)) {
    servo2.write(readTargetAngle(POT2_PIN));
  }

  // Servos remain attached and hold their last commanded positions.
}
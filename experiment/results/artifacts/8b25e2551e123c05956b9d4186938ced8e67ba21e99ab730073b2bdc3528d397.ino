#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring, preserved exactly.
const uint8_t RGB1_PINS[3] = {2, 3, 4};  // Red, green, blue
const uint8_t RGB2_PINS[3] = {5, 6, 7};  // Red, green, blue
const uint8_t SERVO_PINS[3] = {8, 9, 10};
const uint8_t POT_PIN = A3;

const unsigned long SERVO_HOLD_MS = 2000UL;

Servo servos[3];
int currentServoAngle = -1;
unsigned long lastServoUpdateMs = 0;

uint8_t rampComponent(int value, int startValue, int endValue) {
  if (value <= startValue) {
    return 0;
  }
  if (value >= endValue) {
    return 255;
  }

  return static_cast<uint8_t>(
      map(value, startValue, endValue, 0L, 255L));
}

void updateLEDs(int potValue) {
  // Each component ramps over its specified interval, staying at
  // zero below that interval and at full intensity above it.
  const uint8_t colors[3] = {
    rampComponent(potValue, 0, 341),
    rampComponent(potValue, 342, 682),
    rampComponent(potValue, 683, 1023)
  };

  // Common-cathode LEDs: larger PWM values mean greater brightness.
  for (uint8_t channel = 0; channel < 3; ++channel) {
    analogWrite(RGB1_PINS[channel], colors[channel]);
    analogWrite(RGB2_PINS[channel], colors[channel]);
  }
}

void setServoPositions(int angle) {
  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(angle);
  }

  currentServoAngle = angle;
  lastServoUpdateMs = millis();
}

void setup() {
  pinMode(POT_PIN, INPUT);

  for (uint8_t channel = 0; channel < 3; ++channel) {
    pinMode(RGB1_PINS[channel], OUTPUT);
    pinMode(RGB2_PINS[channel], OUTPUT);
  }

  // With three servos on the Mega, Servo uses Timer5, leaving the
  // hardware PWM timers for LED pins 2 through 7 available.
  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].attach(SERVO_PINS[i]);
  }

  const int initialPotValue = analogRead(POT_PIN);
  updateLEDs(initialPotValue);
  setServoPositions(
      static_cast<int>(map(initialPotValue, 0, 1023, 0, 180)));
}

void loop() {
  const int potValue = analogRead(POT_PIN);

  // LED updates and potentiometer sampling never wait for the servos.
  updateLEDs(potValue);

  const int requestedAngle =
      static_cast<int>(map(potValue, 0, 1023, 0, 180));

  // Respond with the latest requested position as soon as the mandatory
  // two-second hold expires. No blocking delay is used.
  if (requestedAngle != currentServoAngle &&
      millis() - lastServoUpdateMs >= SERVO_HOLD_MS) {
    setServoPositions(requestedAngle);
  }
}
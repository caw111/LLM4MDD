#include <Arduino.h>

const uint8_t led1 = 8;   // Red LED
const uint8_t led2 = 9;   // Yellow LED
const uint8_t led3 = 10;  // Green LED
const uint8_t buttonPin = 2;

const unsigned long yellowDuration = 1000UL;
const unsigned long redDuration = 2000UL;

enum TrafficState {
  GREEN,
  YELLOW,
  RED
};

TrafficState state = GREEN;
unsigned long stateStartedAt = 0;

void setLights(bool redOn, bool yellowOn, bool greenOn) {
  // Turn off inactive LEDs before turning on the active LED.
  if (!redOn) digitalWrite(led1, LOW);
  if (!yellowOn) digitalWrite(led2, LOW);
  if (!greenOn) digitalWrite(led3, LOW);

  if (redOn) digitalWrite(led1, HIGH);
  if (yellowOn) digitalWrite(led2, HIGH);
  if (greenOn) digitalWrite(led3, HIGH);
}

void setup() {
  // The circuit provides an external 10k-ohm pull-down resistor.
  pinMode(buttonPin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  setLights(false, false, true);
}

void loop() {
  const bool buttonPressed = (digitalRead(buttonPin) == HIGH);
  const unsigned long now = millis();

  switch (state) {
    case GREEN:
      if (buttonPressed) {
        setLights(false, true, false);
        stateStartedAt = now;
        state = YELLOW;
      }
      break;

    case YELLOW:
      if (now - stateStartedAt >= yellowDuration) {
        setLights(true, false, false);
        stateStartedAt = now;
        state = RED;
      }
      break;

    case RED:
      if (now - stateStartedAt >= redDuration) {
        setLights(false, false, true);
        state = GREEN;
      }
      break;
  }
}
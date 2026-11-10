#include <Arduino.h>

constexpr uint8_t RED_PIN = 2;
constexpr uint8_t GREEN_PIN = 3;
constexpr uint8_t BLUE_PIN = 4;
constexpr uint8_t POT_PIN = A3;

void setColor(bool red, bool green, bool blue) {
  // Common-cathode RGB LED: HIGH turns a channel on.
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
}

void setup() {
  // Set output latches LOW before enabling outputs to keep the LED off.
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  setColor(false, false, false);
}

void loop() {
  const int position = analogRead(POT_PIN);

  if (position < 341) {
    setColor(true, false, false);
  } else if (position < 683) {
    setColor(false, true, false);
  } else {
    setColor(false, false, true);
  }
}
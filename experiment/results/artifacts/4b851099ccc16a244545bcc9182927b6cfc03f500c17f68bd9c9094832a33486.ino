#include <Arduino.h>

const int pot1Pin = A3; // Red
const int pot2Pin = A4; // Green
const int pot3Pin = A5; // Blue

const int redPin = 2;   // rgb1:R
const int greenPin = 3; // rgb1:G
const int bluePin = 4;  // rgb1:B

static inline uint8_t mapToPWM(int analogVal) {
  if (analogVal < 0) analogVal = 0;
  if (analogVal > 1023) analogVal = 1023;
  return (uint8_t)((analogVal * 255L) / 1023L);
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // RGB LED must start OFF when powered on/reset.
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

void loop() {
  int pot1 = analogRead(pot1Pin); // 0-1023
  int pot2 = analogRead(pot2Pin); // 0-1023
  int pot3 = analogRead(pot3Pin); // 0-1023

  uint8_t r = mapToPWM(pot1);
  uint8_t g = mapToPWM(pot2);
  uint8_t b = mapToPWM(pot3);

  // If all potentiometers are minimum (0), LED must remain off.
  if (pot1 == 0 && pot2 == 0 && pot3 == 0) {
    r = 0; g = 0; b = 0;
  }

  // Continuously update RGB LED based on potentiometers.
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}
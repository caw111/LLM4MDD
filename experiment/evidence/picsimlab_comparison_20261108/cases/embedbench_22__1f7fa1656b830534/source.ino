#include <Arduino.h>

// Wiring for Arduino Mega 2560.
constexpr uint8_t RED_POT_PIN   = A3;
constexpr uint8_t GREEN_POT_PIN = A4;
constexpr uint8_t BLUE_POT_PIN  = A5;

constexpr uint8_t RED_LED_PIN   = 2;
constexpr uint8_t GREEN_LED_PIN = 3;
constexpr uint8_t BLUE_LED_PIN  = 4;

void setup() {
  // Common-cathode RGB LED: LOW turns each channel off.
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  analogWrite(RED_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 0);
  analogWrite(BLUE_LED_PIN, 0);

  pinMode(RED_POT_PIN, INPUT);
  pinMode(GREEN_POT_PIN, INPUT);
  pinMode(BLUE_POT_PIN, INPUT);
}

void loop() {
  const int redValue   = analogRead(RED_POT_PIN);
  const int greenValue = analogRead(GREEN_POT_PIN);
  const int blueValue  = analogRead(BLUE_POT_PIN);

  const uint8_t redPWM   = map(redValue, 0, 1023, 0, 255);
  const uint8_t greenPWM = map(greenValue, 0, 1023, 0, 255);
  const uint8_t bluePWM  = map(blueValue, 0, 1023, 0, 255);

  analogWrite(RED_LED_PIN, redPWM);
  analogWrite(GREEN_LED_PIN, greenPWM);
  analogWrite(BLUE_LED_PIN, bluePWM);
}
#include <Arduino.h>

static const uint8_t R_LED_PIN = 11;
static const uint8_t G_LED_PIN = 10;
static const uint8_t B_LED_PIN = 9;

static const uint8_t R_BUTTON_PIN = 4;
static const uint8_t G_BUTTON_PIN = 3;
static const uint8_t B_BUTTON_PIN = 2;

static const uint8_t LED_OFF_PWM = 0;
static const uint8_t LED_FULL_PWM = 255;

static void setAllOff() {
  analogWrite(R_LED_PIN, LED_OFF_PWM);
  analogWrite(G_LED_PIN, LED_OFF_PWM);
  analogWrite(B_LED_PIN, LED_OFF_PWM);
}

void setup() {
  // LED pins
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);

  // Button pins
  pinMode(R_BUTTON_PIN, INPUT);
  pinMode(G_BUTTON_PIN, INPUT);
  pinMode(B_BUTTON_PIN, INPUT);

  setAllOff();
}

void loop() {
  // Success criteria: pressed button corresponds exactly to RGB LED full brightness.
  // Buttons use pull-up: pressed => LOW.
  const int rPressed = (digitalRead(R_BUTTON_PIN) == LOW);
  const int gPressed = (digitalRead(G_BUTTON_PIN) == LOW);
  const int bPressed = (digitalRead(B_BUTTON_PIN) == LOW);

  if (rPressed) {
    analogWrite(R_LED_PIN, LED_FULL_PWM);
  } else {
    analogWrite(R_LED_PIN, LED_OFF_PWM);
  }

  if (gPressed) {
    analogWrite(G_LED_PIN, LED_FULL_PWM);
  } else {
    analogWrite(G_LED_PIN, LED_OFF_PWM);
  }

  if (bPressed) {
    analogWrite(B_LED_PIN, LED_FULL_PWM);
  } else {
    analogWrite(B_LED_PIN, LED_OFF_PWM);
  }
}
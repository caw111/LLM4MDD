#include <Arduino.h>

const uint8_t LED1_PIN = 2;   // led1:A
const uint8_t LED2_PIN = 6;   // led2:A

// rgb1:R/G/B
const uint8_t RGB_R_PIN = 3;
const uint8_t RGB_G_PIN = 4;
const uint8_t RGB_B_PIN = 5;

// Timing (2 seconds per state)
const unsigned long STATE_DURATION_MS = 2000;

void setRGB_Red() {
  digitalWrite(RGB_R_PIN, HIGH);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);
}

void setRGB_Blue() {
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, HIGH);
}

void applyStateRed() {
  digitalWrite(LED1_PIN, HIGH); // led1 on
  digitalWrite(LED2_PIN, LOW);  // led2 off
  setRGB_Red();                  // RGB red
}

void applyStateBlue() {
  digitalWrite(LED1_PIN, LOW);   // led1 off
  digitalWrite(LED2_PIN, HIGH);  // led2 on
  setRGB_Blue();                 // RGB blue
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialization requirement:
  // led1 on, led2 off, rgb1 red.
  applyStateRed();
}

void loop() {
  static bool redState = true;
  static unsigned long lastSwitchMs = 0;

  unsigned long now = millis();
  if (now - lastSwitchMs >= STATE_DURATION_MS) {
    lastSwitchMs = now;
    redState = !redState;

    if (redState) {
      applyStateRed();
    } else {
      applyStateBlue();
    }
  }
}
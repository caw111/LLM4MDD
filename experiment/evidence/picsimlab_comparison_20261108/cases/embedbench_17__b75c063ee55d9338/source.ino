#include <Arduino.h>

// Circuit mapping (Arduino Mega 2560)
const uint8_t LED1_PIN = 2;   // red LED (led1) anode
const uint8_t LED2_PIN = 6;   // blue LED (led2) anode (note: both LEDs in circuit are red-colored parts, but wiring intent is blue LED)

const uint8_t RGB_R_PIN = 3;   // rgb1:R
const uint8_t RGB_G_PIN = 4;   // rgb1:G
const uint8_t RGB_B_PIN = 5;   // rgb1:B

// Common cathode RGB LED:
// - If common cathode is GND, setting a color pin HIGH turns that color ON (current flows through cathode to GND).
// - Setting a color pin LOW turns that color OFF.
const unsigned long STATE_DURATION_MS = 2000;

static void setRgbRed() {
  digitalWrite(RGB_R_PIN, HIGH);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);
}

static void setRgbBlue() {
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, HIGH);
}

static void setStateRed() {
  digitalWrite(LED1_PIN, HIGH);  // led1 on
  digitalWrite(LED2_PIN, LOW);   // led2 off
  setRgbRed();                    // rgb1 red
}

static void setStateBlue() {
  digitalWrite(LED1_PIN, LOW);   // led1 off
  digitalWrite(LED2_PIN, HIGH);  // led2 on
  setRgbBlue();                   // rgb1 blue
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialization requirement:
  // - led1 on
  // - led2 off
  // - rgb1 red
  setStateRed();
}

void loop() {
  static bool isRedState = true;
  static unsigned long lastSwitchTime = 0;

  unsigned long now = millis();
  if (now - lastSwitchTime >= STATE_DURATION_MS) {
    lastSwitchTime = now;

    if (isRedState) {
      setStateBlue();
      isRedState = false;
    } else {
      setStateRed();
      isRedState = true;
    }
  }
}
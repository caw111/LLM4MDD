#include <Arduino.h>

// RGB channels
const uint8_t PIN_R = 3;
const uint8_t PIN_G = 4;
const uint8_t PIN_B = 5;

// Button (btn1) connections from circuit
const uint8_t PIN_BTN = 2;

// Potentiometer (pot1) connection from circuit
const uint8_t PIN_POT = A3;

enum Channel : uint8_t { RED = 0, GREEN = 1, BLUE = 2 };
static Channel currentChannel = RED;

static uint8_t readPotBrightness() {
  // Map to 0-255; analogRead is 0-1023 on Mega2560
  int v = analogRead(PIN_POT);
  if (v < 0) v = 0;
  if (v > 1023) v = 1023;
  return (uint8_t)((v * 255L) / 1023L);
}

static void setChannelBrightness(uint8_t brightness) {
  // Common cathode: HIGH turns LED on, LOW turns it off
  // Only the selected channel is driven; others remain off.
  // brightness comes from potentiometer and spans 0-255.
  analogWrite(PIN_R, (currentChannel == RED) ? brightness : 0);
  analogWrite(PIN_G, (currentChannel == GREEN) ? brightness : 0);
  analogWrite(PIN_B, (currentChannel == BLUE) ? brightness : 0);
}

static bool readDebouncedButtonPress() {
  // Active-high button (btn1:1.l is connected to mega:2, btn1:2.r to GND via black)
  // We debounce and detect a stable rising edge (pressed).
  static bool lastStableState = LOW;
  static bool lastRawState = LOW;

  static unsigned long lastDebounceTime = 0;

  const unsigned long debounceMs = 30;

  bool rawState = digitalRead(PIN_BTN);

  if (rawState != lastRawState) {
    lastDebounceTime = millis();
    lastRawState = rawState;
  }

  if ((millis() - lastDebounceTime) >= debounceMs) {
    if (rawState != lastStableState) {
      lastStableState = rawState;
      // Rising edge = button press
      if (lastStableState == HIGH) {
        return true;
      }
    }
  }

  return false;
}

void setup() {
  // Pins
  pinMode(PIN_BTN, INPUT_PULLUP); // Note: circuit doesn't show pullups; enable internal for stability.
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  // Ensure RGB LED remains off on power/reset
  analogWrite(PIN_R, 0);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_B, 0);

  // Default channel per requirements: first button press activates Red
  currentChannel = RED;
}

void loop() {
  // Continuous brightness updates based on potentiometer
  uint8_t brightness = readPotBrightness();
  setChannelBrightness(brightness);

  // Button press cycles: Red -> Green -> Blue -> Red ...
  if (readDebouncedButtonPress()) {
    switch (currentChannel) {
      case RED:   currentChannel = GREEN; break;
      case GREEN: currentChannel = BLUE;  break;
      case BLUE:  currentChannel = RED;   break;
    }
  }
}
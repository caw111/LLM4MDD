#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_86\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
// Bar graph A1-A7 are segment terminals, not Arduino analog pins.
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t RGB_PINS[2][3] = {
  {3, 4, 5},  // rgb1: red, green, blue
  {6, 7, 8}   // rgb2: red, green, blue
};
constexpr uint8_t BAR_PINS[7] = {9, 10, 11, 12, 13, 44, 45};

constexpr unsigned long PRESS_HOLD_MS = 150;
constexpr unsigned long RELEASE_DEBOUNCE_MS = 25;

// Color masks: red = 4, green = 2, blue = 1.
// Both RGB LEDs are common-cathode: HIGH turns a channel on.
constexpr uint8_t MODE_COLORS[8][2] = {
  {0, 0},  // Mode 0: off / off
  {4, 1},  // Mode 1: red / blue
  {2, 3},  // Mode 2: green / cyan
  {6, 5},  // Mode 3: yellow / magenta
  {3, 6},  // Mode 4: cyan / yellow
  {5, 3},  // Mode 5: magenta / cyan
  {7, 6},  // Mode 6: white / yellow
  {0, 7}   // Mode 7: black / white
};

uint8_t currentMode = 0;
uint8_t lastButtonLevel = HIGH;
unsigned long buttonLevelSince = 0;
bool readyForPress = true;

#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_86\\direct\\attempt_0\\sketch\\sketch.ino"
void applyMode(uint8_t mode);
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_86\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_86\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_86\\direct\\attempt_0\\sketch\\sketch.ino"
void applyMode(uint8_t mode) {
  // Update both RGB LEDs and the bar graph in the same transition.
  for (uint8_t led = 0; led < 2; ++led) {
    const uint8_t color = MODE_COLORS[mode][led];
    digitalWrite(RGB_PINS[led][0], (color & 4) ? HIGH : LOW);
    digitalWrite(RGB_PINS[led][1], (color & 2) ? HIGH : LOW);
    digitalWrite(RGB_PINS[led][2], (color & 1) ? HIGH : LOW);
  }

  for (uint8_t segment = 0; segment < 7; ++segment) {
    digitalWrite(BAR_PINS[segment], segment < mode ? HIGH : LOW);
  }
}

void setup() {
  // Set output latches LOW before enabling outputs to avoid startup flashes.
  for (uint8_t led = 0; led < 2; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], LOW);
      pinMode(RGB_PINS[led][channel], OUTPUT);
    }
  }

  for (uint8_t segment = 0; segment < 7; ++segment) {
    digitalWrite(BAR_PINS[segment], LOW);
    pinMode(BAR_PINS[segment], OUTPUT);
  }

  // Button connects pin 2 to ground when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  currentMode = 0;
  applyMode(currentMode);

  lastButtonLevel = digitalRead(BUTTON_PIN);
  buttonLevelSince = millis();
  readyForPress = true;
}

void loop() {
  const unsigned long now = millis();
  const uint8_t buttonLevel = digitalRead(BUTTON_PIN);

  if (buttonLevel != lastButtonLevel) {
    lastButtonLevel = buttonLevel;
    buttonLevelSince = now;
  }

  const unsigned long stableDuration = now - buttonLevelSince;

  if (buttonLevel == LOW) {
    // Require 150 ms continuously pressed. Count only once per hold.
    if (readyForPress && stableDuration >= PRESS_HOLD_MS) {
      readyForPress = false;
      currentMode = (currentMode + 1) % 8;
      applyMode(currentMode);
    }
  } else if (stableDuration >= RELEASE_DEBOUNCE_MS) {
    // A debounced release is required before another press can count.
    readyForPress = true;
  }
}

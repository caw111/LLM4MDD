#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_64\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t BTN_NEXT_PIN = 2;
constexpr uint8_t BTN_PREVIOUS_PIN = 3;
constexpr uint8_t RED_PIN = 4;
constexpr uint8_t GREEN_PIN = 5;
constexpr uint8_t BLUE_PIN = 6;

constexpr unsigned long DEBOUNCE_MS = 25;
constexpr uint8_t COLOR_COUNT = 8;

// Common-cathode RGB LED: HIGH turns a channel on.
// Sequence: Off, Red, Green, Blue, Yellow, Cyan, Magenta, White.
const uint8_t COLORS[COLOR_COUNT][3] = {
  {LOW,  LOW,  LOW },
  {HIGH, LOW,  LOW },
  {LOW,  HIGH, LOW },
  {LOW,  LOW,  HIGH},
  {HIGH, HIGH, LOW },
  {LOW,  HIGH, HIGH},
  {HIGH, LOW,  HIGH},
  {HIGH, HIGH, HIGH}
};

struct DebouncedButton {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeTime;
};

DebouncedButton nextButton = {BTN_NEXT_PIN, HIGH, HIGH, 0};
DebouncedButton previousButton = {BTN_PREVIOUS_PIN, HIGH, HIGH, 0};

uint8_t colorIndex = 0;

#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_64\\direct\\attempt_0\\sketch\\sketch.ino"
void displayColor();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_64\\direct\\attempt_0\\sketch\\sketch.ino"
bool wasPressed(DebouncedButton &button, unsigned long now);
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_64\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_64\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_64\\direct\\attempt_0\\sketch\\sketch.ino"
void displayColor() {
  digitalWrite(RED_PIN, COLORS[colorIndex][0]);
  digitalWrite(GREEN_PIN, COLORS[colorIndex][1]);
  digitalWrite(BLUE_PIN, COLORS[colorIndex][2]);
}

// Returns true once per debounced press; holding does not repeat.
bool wasPressed(DebouncedButton &button, unsigned long now) {
  const uint8_t reading = digitalRead(button.pin);

  if (reading != button.lastReading) {
    button.lastReading = reading;
    button.lastChangeTime = now;
  }

  if (reading != button.stableState &&
      now - button.lastChangeTime >= DEBOUNCE_MS) {
    button.stableState = reading;
    return button.stableState == LOW;
  }

  return false;
}

void setup() {
  // Set output latches LOW before enabling outputs to start with LED off.
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Buttons connect their inputs to GND when pressed.
  pinMode(BTN_NEXT_PIN, INPUT_PULLUP);
  pinMode(BTN_PREVIOUS_PIN, INPUT_PULLUP);

  colorIndex = 0;
  displayColor();
}

void loop() {
  const unsigned long now = millis();
  const bool nextPressed = wasPressed(nextButton, now);
  const bool previousPressed = wasPressed(previousButton, now);

  if (nextPressed) {
    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    displayColor();
  }

  if (previousPressed) {
    colorIndex = (colorIndex + COLOR_COUNT - 1) % COLOR_COUNT;
    displayColor();
  }
}

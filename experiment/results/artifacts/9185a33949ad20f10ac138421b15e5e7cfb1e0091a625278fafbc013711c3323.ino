#include <Arduino.h>

// Arduino Mega 2560, preserving the supplied wiring.
// Common-cathode RGB LED: HIGH enables a color channel.
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t RED_PIN = 3;
constexpr uint8_t GREEN_PIN = 4;
constexpr uint8_t BLUE_PIN = 5;

constexpr unsigned long PRESS_HOLD_MS = 150;
constexpr unsigned long RELEASE_DEBOUNCE_MS = 20;
constexpr unsigned long MIN_STATE_MS = 2000;

// Sequence starts at Off, then advances to Red on the first valid press.
const uint8_t COLORS[8][3] = {
  {LOW,  LOW,  LOW }, // Off
  {HIGH, LOW,  LOW }, // Red
  {LOW,  HIGH, LOW }, // Green
  {LOW,  LOW,  HIGH}, // Blue
  {HIGH, HIGH, LOW }, // Yellow
  {LOW,  HIGH, HIGH}, // Cyan
  {HIGH, LOW,  HIGH}, // Magenta
  {HIGH, HIGH, HIGH}  // White
};

uint8_t colorIndex = 0;
uint8_t lastRawButton = HIGH;
bool pressRegistered = false;

unsigned long rawChangedAt = 0;
unsigned long stateStartedAt = 0;

// Queue valid presses so presses during a state's minimum duration
// are preserved rather than ignored.
uint64_t pendingPresses = 0;

void applyColor(uint8_t index) {
  digitalWrite(RED_PIN, COLORS[index][0]);
  digitalWrite(GREEN_PIN, COLORS[index][1]);
  digitalWrite(BLUE_PIN, COLORS[index][2]);
}

void setup() {
  // Preload LOW before enabling outputs to keep the LED initially off.
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // The supplied button connects pin 2 to ground when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  applyColor(colorIndex);

  const unsigned long now = millis();
  stateStartedAt = now;
  rawChangedAt = now;
  lastRawButton = digitalRead(BUTTON_PIN);
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButton = digitalRead(BUTTON_PIN);

  if (rawButton != lastRawButton) {
    lastRawButton = rawButton;
    rawChangedAt = now;
  }

  if (rawButton == LOW) {
    // Require an uninterrupted 150 ms hold and count only once per press.
    if (!pressRegistered &&
        (unsigned long)(now - rawChangedAt) >= PRESS_HOLD_MS) {
      ++pendingPresses;
      pressRegistered = true;
    }
  } else {
    // Require a stable release before allowing another press.
    if ((unsigned long)(now - rawChangedAt) >= RELEASE_DEBOUNCE_MS) {
      pressRegistered = false;
    }
  }

  // Every state, including the initial Off state, lasts at least two
  // seconds. No automatic cycling occurs without a queued valid press.
  if (pendingPresses > 0 &&
      (unsigned long)(now - stateStartedAt) >= MIN_STATE_MS) {
    --pendingPresses;
    colorIndex = (colorIndex + 1) % 8;
    applyColor(colorIndex);
    stateStartedAt = now;
  }
}
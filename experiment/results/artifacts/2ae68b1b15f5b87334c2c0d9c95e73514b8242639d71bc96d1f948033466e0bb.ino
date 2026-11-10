#include <Arduino.h>

const uint8_t BAR_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44}; // A1-A10
const uint8_t LED1_PIN = 2; // led1:A
const uint8_t LED2_PIN = 3; // led2:A
const uint8_t BTN_PIN  = 4; // btn1:1.l (with pull to + via btn; using external wiring as given)

static uint16_t counter = 0;      // 0..10 (10 segments represent values 1..10)
static uint8_t overflowCount = 0; // 0..3, wraps

// Debounce: require 0.15s stable press (transition detect)
static bool lastStablePressed = false;
static bool lastReadingPressed = false;
static unsigned long stableSinceMs = 0;

static bool wasPressedEventHandled = false;

void setBarGraphForValue(uint16_t value) {
  // value: 0..10
  // Requirement: counter value 1-10 lights 1-10 segments (A1-A10)
  // Requirement: counter=0 => all off
  for (uint8_t i = 0; i < 10; i++) {
    uint16_t segmentIndex = i + 1; // 1..10
    digitalWrite(BAR_PINS[i], (value >= segmentIndex) ? HIGH : LOW);
  }
}

void setOverflowLeds(uint8_t overflow) {
  // 00: 0 overflows
  // 01: 1 overflow
  // 10: 2 overflows
  // 11: 3 overflows (then wraps)
  uint8_t b0 = (overflow & 0x01) ? 1 : 0;
  uint8_t b1 = (overflow & 0x02) ? 1 : 0;

  digitalWrite(LED1_PIN, b0 ? HIGH : LOW);
  digitalWrite(LED2_PIN, b1 ? HIGH : LOW);
}

void handleValidPressEvent() {
  // Each valid button press increments the counter by 1.
  // When counter exceeds 10: resets to 0 and overflow counter increments by 1.
  counter++;
  if (counter > 10) {
    counter = 0;
    overflowCount = (overflowCount + 1) & 0x03; // wrap 0..3
  }

  setBarGraphForValue(counter);
  setOverflowLeds(overflowCount);
}

void setup() {
  // Button input
  pinMode(BTN_PIN, INPUT_PULLUP);

  // LEDs off initially
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Bar graph segments off initially
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // Initialize debounce state
  lastReadingPressed = false;
  lastStablePressed = false;
  stableSinceMs = millis();
  wasPressedEventHandled = false;
}

void loop() {
  // Active-low button because INPUT_PULLUP: pressed => LOW
  bool readingPressed = (digitalRead(BTN_PIN) == LOW);
  unsigned long now = millis();

  // Debounce: track stable pressed state
  if (readingPressed != lastReadingPressed) {
    // Reading changed; restart stability timer
    stableSinceMs = now;
    lastReadingPressed = readingPressed;
    wasPressedEventHandled = false; // allow a new event when it becomes stable pressed again
  }

  // If reading has been stable for at least 0.15s, it's a valid debounced state
  if ((now - stableSinceMs) >= 150UL) {
    if (readingPressed != lastStablePressed) {
      lastStablePressed = readingPressed;

      // "valid button press": count on transition to pressed state
      if (lastStablePressed && !wasPressedEventHandled) {
        wasPressedEventHandled = true;
        handleValidPressEvent();
      }
    }
  }

  // Maintain current state until next valid press (handled above).
}
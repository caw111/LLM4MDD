#include <Arduino.h>

const uint8_t led1Pin = 2;   // mega:2
const uint8_t led2Pin = 3;   // mega:3

const uint8_t btn1Pin = 4;   // mega:4
const uint8_t btn2Pin = 5;   // mega:5
const uint8_t btn3Pin = 6;   // mega:6

static bool led1State = false;
static bool led2State = false;

static unsigned long stableUntilLed1 = 0;
static unsigned long stableUntilLed2 = 0;

static const unsigned long ACTION_HOLD_MS = 2000;  // state remains stable for 2 seconds after each toggle/reset

// Debounce parameters
static const unsigned long DEBOUNCE_MS = 50;

static int readButtonStableRaw(uint8_t pin, unsigned long &lastChangeAtMs, int &lastRaw, int &debounced) {
  int raw = digitalRead(pin);

  if (raw != lastRaw) {
    lastChangeAtMs = millis();
    lastRaw = raw;
  }

  if ((millis() - lastChangeAtMs) >= DEBOUNCE_MS) {
    // Consider debounced only after it has been stable long enough
    debounced = raw;
  }
  return debounced;
}

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  // Buttons wired to ground on one side; other side to pins.
  // Use internal pull-ups so unpressed reads HIGH and pressed reads LOW.
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(btn3Pin, INPUT_PULLUP);

  // Initialization: both LEDs off
  led1State = false;
  led2State = false;
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  stableUntilLed1 = millis();
  stableUntilLed2 = millis();
}

void loop() {
  static unsigned long lastChange1 = 0, lastChange2 = 0, lastChange3 = 0;
  static int lastRaw1 = HIGH, lastRaw2 = HIGH, lastRaw3 = HIGH;
  static int debounced1 = HIGH, debounced2 = HIGH, debounced3 = HIGH;

  static int prevDebounced1 = HIGH, prevDebounced2 = HIGH, prevDebounced3 = HIGH;

  (void)readButtonStableRaw(btn1Pin, lastChange1, lastRaw1, debounced1);
  (void)readButtonStableRaw(btn2Pin, lastChange2, lastRaw2, debounced2);
  (void)readButtonStableRaw(btn3Pin, lastChange3, lastRaw3, debounced3);

  unsigned long now = millis();

  // Detect new press: debounced transitions from HIGH to LOW
  bool btn1Pressed = (prevDebounced1 == HIGH && debounced1 == LOW);
  bool btn2Pressed = (prevDebounced2 == HIGH && debounced2 == LOW);
  bool btn3Pressed = (prevDebounced3 == HIGH && debounced3 == LOW);

  // Update previous values for next iteration
  prevDebounced1 = debounced1;
  prevDebounced2 = debounced2;
  prevDebounced3 = debounced3;

  // If btn3 pressed: reset both LEDs to off, and hold stable for 2 seconds
  if (btn3Pressed) {
    led1State = false;
    led2State = false;
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    stableUntilLed1 = now + ACTION_HOLD_MS;
    stableUntilLed2 = now + ACTION_HOLD_MS;
    return; // preserve behavior ordering: reset takes precedence
  }

  // If btn1 pressed: toggle led1, then hold stable for 2 seconds
  if (btn1Pressed) {
    // "State should remain stable for 2 seconds after each toggle."
    // We interpret this as: ignore further btn1 toggles during the hold window.
    if (now >= stableUntilLed1) {
      led1State = !led1State;
      digitalWrite(led1Pin, led1State ? HIGH : LOW);
      stableUntilLed1 = now + ACTION_HOLD_MS;
    }
  }

  // If btn2 pressed: toggle led2, then hold stable for 2 seconds
  if (btn2Pressed) {
    if (now >= stableUntilLed2) {
      led2State = !led2State;
      digitalWrite(led2Pin, led2State ? HIGH : LOW);
      stableUntilLed2 = now + ACTION_HOLD_MS;
    }
  }
}
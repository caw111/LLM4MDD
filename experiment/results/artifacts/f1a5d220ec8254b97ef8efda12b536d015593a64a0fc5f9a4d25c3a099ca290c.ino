#include <Arduino.h>

// LED bar graph connections:
// From circuit: A1..A10 are on Mega pins 2..11, bottom is A10, top is A1.
// Button btn1:
// Green to Mega pin 12, black to Mega GND.

static const uint8_t LED_PINS[10] = {
  2,  // A1
  3,  // A2
  4,  // A3
  5,  // A4
  6,  // A5
  7,  // A6
  8,  // A7
  9,  // A8
  10, // A9
  11  // A10
};

static const uint8_t BTN_PIN = 12;

// Debounce
static bool lastStableBtn = HIGH;
static bool lastReadingBtn = HIGH;
static unsigned long lastDebounceTimeMs = 0;
static const unsigned long DEBOUNCE_MS = 30;

static uint8_t step = 0; // 0..5; each press advances by 2 LEDs bottom-up

static void allLedsOff() {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}

static void setBottomUp(uint8_t numberLit) {
  // numberLit: 0..10, bottom is A10 (LED_PINS[9])
  for (uint8_t i = 0; i < 10; i++) {
    // i indexes LEDs from top(A1) to bottom(A10)
    // bottom-up means the last 'numberLit' LEDs in LED_PINS array are ON.
    bool on = (i >= (10 - numberLit));
    digitalWrite(LED_PINS[i], on ? HIGH : LOW);
  }
}

void setup() {
  // Initialize LED pins
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // Button pin: connected to pin 12 (green) and GND (black).
  // Use INPUT_PULLUP since other terminal goes to GND through button.
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Ensure all LEDs are off at startup
  allLedsOff();

  // Initialize debounce states
  bool reading = digitalRead(BTN_PIN);
  lastStableBtn = reading;
  lastReadingBtn = reading;
  lastDebounceTimeMs = millis();
}

void loop() {
  // Read button with debounce
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastReadingBtn) {
    lastDebounceTimeMs = millis();
    lastReadingBtn = reading;
  }

  if ((millis() - lastDebounceTimeMs) > DEBOUNCE_MS) {
    if (reading != lastStableBtn) {
      lastStableBtn = reading;

      // Detect press event: button goes LOW when pressed (INPUT_PULLUP)
      if (lastStableBtn == LOW) {
        // Advance sequence by 2 LEDs each press: steps 0..4 -> 2..10, step 5 -> off then restart
        step++;
        if (step > 5) step = 0;

        if (step == 0) {
          allLedsOff();
        } else {
          uint8_t numberLit = step * 2; // step 1->2, 2->4, 3->6, 4->8, 5->10
          setBottomUp(numberLit);
        }
      }
    }
  }
}
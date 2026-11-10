#include <Arduino.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t BTN_INCREMENT_PIN = 2;
constexpr uint8_t BTN_DECREMENT_PIN = 3;
constexpr uint8_t RGB_RED_PIN       = 4;
constexpr uint8_t RGB_GREEN_PIN     = 5;
constexpr uint8_t RGB_BLUE_PIN      = 6;
constexpr uint8_t SHIFT_DATA_PIN    = A0;
constexpr uint8_t SHIFT_LATCH_PIN   = A1;
constexpr uint8_t SHIFT_CLOCK_PIN   = A2;

constexpr unsigned long DEBOUNCE_MS = 25;

// Bits 0..6 correspond to segments A..G.
// Display commons are wired to +5V, so segment outputs are active LOW.
const uint8_t DIGIT_PATTERNS[10] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F  // 9
};

struct DebouncedButton {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeTime;

  void begin(unsigned long now) {
    pinMode(pin, INPUT_PULLUP);
    lastReading = digitalRead(pin);
    // A button held during startup is recognized after debouncing.
    stableState = HIGH;
    lastChangeTime = now;
  }

  bool pressed(unsigned long now) {
    const uint8_t reading = digitalRead(pin);

    if (reading != lastReading) {
      lastReading = reading;
      lastChangeTime = now;
    }

    if (reading != stableState &&
        now - lastChangeTime >= DEBOUNCE_MS) {
      stableState = reading;
      return stableState == LOW;
    }

    return false;
  }
};

DebouncedButton incrementButton = {
  BTN_INCREMENT_PIN, HIGH, HIGH, 0
};
DebouncedButton decrementButton = {
  BTN_DECREMENT_PIN, HIGH, HIGH, 0
};

uint8_t displayedNumber = 0;

void updateDisplays() {
  const uint8_t tens = displayedNumber / 10;
  const uint8_t units = displayedNumber % 10;

  const uint8_t tensSegments =
      static_cast<uint8_t>(~DIGIT_PATTERNS[tens]);
  const uint8_t unitsSegments =
      static_cast<uint8_t>(~DIGIT_PATTERNS[units]);

  digitalWrite(SHIFT_LATCH_PIN, LOW);

  // sr2 is downstream: send its units byte first, then sr1's tens byte.
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, unitsSegments);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, tensSegments);

  digitalWrite(SHIFT_LATCH_PIN, HIGH);
}

void setRgb(bool red, bool green, bool blue) {
  // RGB LED has a grounded common cathode: HIGH turns a channel on.
  digitalWrite(RGB_RED_PIN, red ? HIGH : LOW);
  digitalWrite(RGB_GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(RGB_BLUE_PIN, blue ? HIGH : LOW);
}

void updateAfterPress() {
  updateDisplays();

  if (displayedNumber <= 33) {
    setRgb(true, false, false);
  } else if (displayedNumber <= 66) {
    setRgb(false, true, false);
  } else {
    setRgb(false, false, true);
  }
}

void setup() {
  digitalWrite(RGB_RED_PIN, LOW);
  digitalWrite(RGB_GREEN_PIN, LOW);
  digitalWrite(RGB_BLUE_PIN, LOW);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  digitalWrite(SHIFT_DATA_PIN, LOW);
  digitalWrite(SHIFT_CLOCK_PIN, LOW);
  digitalWrite(SHIFT_LATCH_PIN, LOW);
  pinMode(SHIFT_DATA_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_LATCH_PIN, OUTPUT);

  const unsigned long now = millis();
  incrementButton.begin(now);
  decrementButton.begin(now);

  displayedNumber = 0;
  updateDisplays();

  // Startup exception: show "00", but keep RGB off until a button press.
  setRgb(false, false, false);
}

void loop() {
  const unsigned long now = millis();
  const bool incrementPressed = incrementButton.pressed(now);
  const bool decrementPressed = decrementButton.pressed(now);

  // Each debounced press is handled once, including simultaneous presses.
  if (incrementPressed) {
    displayedNumber = (displayedNumber == 99) ? 0 : displayedNumber + 1;
    updateAfterPress();
  }

  if (decrementPressed) {
    displayedNumber = (displayedNumber == 0) ? 99 : displayedNumber - 1;
    updateAfterPress();
  }
}
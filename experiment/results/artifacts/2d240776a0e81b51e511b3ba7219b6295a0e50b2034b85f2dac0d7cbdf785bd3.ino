#include <Arduino.h>

// Arduino Mega 2560 wiring:
// A0 -> sr1 DS; A1 -> both STCP; A2 -> both SHCP.
// sr1 Q7S -> sr2 DS. sr1 drives tens; sr2 drives units.
// Both displays are common-anode (COM connected to +5V).
constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;
constexpr uint8_t INCREMENT_PIN = 2;
constexpr uint8_t DECREMENT_PIN = 3;
constexpr uint8_t TENS_POT_PIN = A3;
constexpr uint8_t UNITS_POT_PIN = A4;

constexpr unsigned long DEBOUNCE_MS = 25;
constexpr unsigned long POT_SAMPLE_MS = 5;
constexpr int POT_DEADBAND = 4;

// Bits 0..6 correspond to segments A..G. Patterns are inverted
// before transmission because a LOW output lights a segment.
const uint8_t DIGIT_SEGMENTS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

struct Button {
  uint8_t pin;
  bool rawPressed;
  bool stablePressed;
  unsigned long rawChangedAt;
  unsigned long lastStepAt;
};

struct Potentiometer {
  uint8_t pin;
  int filtered;
  int accepted;
  uint8_t selectedDigit;
};

Button incrementButton = {INCREMENT_PIN, false, false, 0, 0};
Button decrementButton = {DECREMENT_PIN, false, false, 0, 0};

Potentiometer tensPot = {TENS_POT_PIN, 0, 0, 0};
Potentiometer unitsPot = {UNITS_POT_PIN, 0, 0, 0};

uint8_t displayedNumber = 0;
unsigned long lastPotSampleAt = 0;

uint8_t digitFromReading(int reading) {
  // Equal-width bins, including both ADC endpoints.
  return static_cast<uint8_t>((static_cast<long>(reading) * 10L) / 1024L);
}

unsigned long repeatInterval(const Potentiometer &pot) {
  // Low ADC reading: 100 ms; high ADC reading: 1000 ms.
  return 100UL + (static_cast<unsigned long>(pot.accepted) * 900UL) / 1023UL;
}

void updateDisplay() {
  const uint8_t tensPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[displayedNumber / 10]);
  const uint8_t unitsPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[displayedNumber % 10]);

  digitalWrite(LATCH_PIN, LOW);

  // The first byte travels through sr1 into downstream sr2.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, unitsPattern);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, tensPattern);

  digitalWrite(LATCH_PIN, HIGH);
}

void changeNumber(int8_t direction) {
  if (direction > 0) {
    displayedNumber = (displayedNumber == 99) ? 0 : displayedNumber + 1;
  } else {
    displayedNumber = (displayedNumber == 0) ? 99 : displayedNumber - 1;
  }
  updateDisplay();
}

void initializePot(Potentiometer &pot) {
  // Discard the first conversion after switching analog channels.
  analogRead(pot.pin);
  unsigned long sum = 0;
  for (uint8_t i = 0; i < 8; ++i) {
    sum += analogRead(pot.pin);
  }
  pot.filtered = static_cast<int>(sum / 8);
  pot.accepted = pot.filtered;
  pot.selectedDigit = digitFromReading(pot.accepted);
}

bool samplePot(Potentiometer &pot) {
  analogRead(pot.pin);
  const int reading = analogRead(pot.pin);
  pot.filtered = (pot.filtered * 3 + reading + 2) / 4;

  const int difference = pot.filtered - pot.accepted;
  if (difference >= POT_DEADBAND || difference <= -POT_DEADBAND) {
    pot.accepted = pot.filtered;
  }

  const uint8_t newDigit = digitFromReading(pot.accepted);
  if (newDigit == pot.selectedDigit) {
    return false;
  }

  pot.selectedDigit = newDigit;
  return true;
}

void serviceButton(Button &button, int8_t direction,
                   unsigned long interval, unsigned long now) {
  const bool pressed = (digitalRead(button.pin) == LOW);

  if (pressed != button.rawPressed) {
    button.rawPressed = pressed;
    button.rawChangedAt = now;
  }

  if (button.rawPressed != button.stablePressed &&
      now - button.rawChangedAt >= DEBOUNCE_MS) {
    button.stablePressed = button.rawPressed;

    if (button.stablePressed) {
      changeNumber(direction);
      button.lastStepAt = now;
      return;
    }
  }

  // Stop repeats immediately on a raw release, while still debouncing
  // the release before accepting another distinct press.
  if (button.stablePressed && button.rawPressed &&
      now - button.lastStepAt >= interval) {
    changeNumber(direction);
    button.lastStepAt = now;
  }
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);

  pinMode(INCREMENT_PIN, INPUT_PULLUP);
  pinMode(DECREMENT_PIN, INPUT_PULLUP);
  pinMode(TENS_POT_PIN, INPUT);
  pinMode(UNITS_POT_PIN, INPUT);

  analogReference(DEFAULT);

  initializePot(tensPot);
  initializePot(unitsPot);

  displayedNumber = tensPot.selectedDigit * 10 + unitsPot.selectedDigit;
  updateDisplay();

  lastPotSampleAt = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastPotSampleAt >= POT_SAMPLE_MS) {
    lastPotSampleAt = now;

    const bool tensChanged = samplePot(tensPot);
    const bool unitsChanged = samplePot(unitsPot);

    // Moving a potentiometer into a new digit range immediately edits
    // its corresponding displayed digit. Between such changes, button
    // counting is preserved rather than overwritten by every ADC read.
    if (tensChanged) {
      displayedNumber = tensPot.selectedDigit * 10 + displayedNumber % 10;
    }
    if (unitsChanged) {
      displayedNumber = (displayedNumber / 10) * 10 + unitsPot.selectedDigit;
    }
    if (tensChanged || unitsChanged) {
      updateDisplay();
    }
  }

  // Each held button independently uses its potentiometer's live speed.
  // Simultaneous presses are both honored, giving a net change of zero.
  serviceButton(incrementButton, +1, repeatInterval(tensPot), now);
  serviceButton(decrementButton, -1, repeatInterval(unitsPot), now);
}
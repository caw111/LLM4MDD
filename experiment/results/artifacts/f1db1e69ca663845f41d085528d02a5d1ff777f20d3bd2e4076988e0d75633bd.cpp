#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_79\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t INCREMENT_PIN = 2;
constexpr uint8_t DECREMENT_PIN = 3;
constexpr uint8_t RESET_PIN = 4;

constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;
constexpr uint8_t POT_PIN = A3;

constexpr unsigned long DEBOUNCE_MS = 5;

// Bits 0..6 correspond to segments A..G.
// Displays are common-anode, so these patterns are inverted before output.
const uint8_t DIGIT_SEGMENTS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

struct Button {
  uint8_t pin;
  bool rawPressed;
  bool pressed;
  bool justPressed;
  unsigned long rawChangedAt;
};

Button incrementButton = {INCREMENT_PIN, false, false, false, 0};
Button decrementButton = {DECREMENT_PIN, false, false, false, 0};
Button resetButton = {RESET_PIN, false, false, false, 0};

uint8_t displayedNumber = 0;
unsigned long lastIncrementAt = 0;
unsigned long lastDecrementAt = 0;

#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_79\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(Button &button, unsigned long now);
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_79\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_79\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_79\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_79\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(Button &button, unsigned long now) {
  button.justPressed = false;
  const bool reading = digitalRead(button.pin) == LOW;

  if (reading != button.rawPressed) {
    button.rawPressed = reading;
    button.rawChangedAt = now;
  }

  if (button.pressed != button.rawPressed &&
      now - button.rawChangedAt >= DEBOUNCE_MS) {
    button.pressed = button.rawPressed;
    button.justPressed = button.pressed;
  }
}

void updateDisplay() {
  const uint8_t tens = displayedNumber / 10;
  const uint8_t ones = displayedNumber % 10;

  digitalWrite(LATCH_PIN, LOW);

  // The first byte travels through sr1 into sr2 (ones).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_SEGMENTS[ones]));
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_SEGMENTS[tens]));

  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  pinMode(INCREMENT_PIN, INPUT_PULLUP);
  pinMode(DECREMENT_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  displayedNumber = 0;
  updateDisplay();
}

void loop() {
  const unsigned long now = millis();

  updateButton(incrementButton, now);
  updateButton(decrementButton, now);
  updateButton(resetButton, now);

  const unsigned long adc = analogRead(POT_PIN);
  const unsigned long repeatInterval = 100UL + (adc * 900UL) / 1023UL;

  // Reset has priority. Keep repeat timing reset while it is held.
  if (resetButton.rawPressed || resetButton.pressed) {
    lastIncrementAt = now;
    lastDecrementAt = now;

    if (resetButton.pressed && displayedNumber != 0) {
      displayedNumber = 0;
      updateDisplay();
    }
    return;
  }

  // Suppress stepping while both buttons are down, including debounce time.
  const bool incrementDown =
      incrementButton.rawPressed || incrementButton.pressed;
  const bool decrementDown =
      decrementButton.rawPressed || decrementButton.pressed;

  if (incrementDown && decrementDown) {
    lastIncrementAt = now;
    lastDecrementAt = now;
    return;
  }

  // Require both raw and debounced states so releasing stops immediately.
  if (incrementButton.pressed && incrementButton.rawPressed) {
    if (incrementButton.justPressed ||
        now - lastIncrementAt >= repeatInterval) {
      displayedNumber = (displayedNumber == 99) ? 0 : displayedNumber + 1;
      lastIncrementAt = now;
      updateDisplay();
    }
  }

  if (decrementButton.pressed && decrementButton.rawPressed) {
    if (decrementButton.justPressed ||
        now - lastDecrementAt >= repeatInterval) {
      displayedNumber = (displayedNumber == 0) ? 99 : displayedNumber - 1;
      lastDecrementAt = now;
      updateDisplay();
    }
  }
}

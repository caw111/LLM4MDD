#include <Arduino.h>

constexpr uint8_t BUTTON_COUNT = 3;
constexpr uint8_t SEGMENT_COUNT = 8;

constexpr uint8_t BUTTON_PINS[BUTTON_COUNT] = {5, 6, 7};
constexpr uint8_t LED_PINS[BUTTON_COUNT] = {2, 3, 4};

// Physical bar-graph anodes A1 through A8 (not Arduino analog pins).
constexpr uint8_t BAR_PINS[SEGMENT_COUNT] = {
  8, 9, 10, 11, 12, 13, 44, 45
};

constexpr unsigned long PRESS_QUALIFY_MS = 150;
constexpr unsigned long RELEASE_DEBOUNCE_MS = 20;

struct ButtonState {
  bool rawPressed;
  bool pressHandled;
  unsigned long rawChangedAt;
};

ButtonState buttons[BUTTON_COUNT];
uint8_t binaryValue = 0;
bool displayEnabled = false;

void updateOutputs() {
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    digitalWrite(LED_PINS[i],
                 (binaryValue & (uint8_t(1) << i)) ? HIGH : LOW);
  }

  // Clear the previous selection before illuminating the new segment.
  for (uint8_t i = 0; i < SEGMENT_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
  }

  if (displayEnabled) {
    // Values 1..7 select physical A1..A7, so decimal 5 selects A5.
    // Physical A8 represents zero. Startup remains entirely dark.
    const uint8_t segmentIndex =
        (binaryValue == 0) ? 7 : binaryValue - 1;
    digitalWrite(BAR_PINS[segmentIndex], HIGH);
  }
}

void setup() {
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < SEGMENT_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  binaryValue = 0;
  displayEnabled = false;
  updateOutputs();

  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    buttons[i].rawPressed = (digitalRead(BUTTON_PINS[i]) == LOW);
    buttons[i].pressHandled = false;
    buttons[i].rawChangedAt = millis();
  }
}

void loop() {
  const unsigned long now = millis();
  bool outputsChanged = false;

  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    ButtonState &button = buttons[i];
    const bool pressed = (digitalRead(BUTTON_PINS[i]) == LOW);

    if (pressed != button.rawPressed) {
      button.rawPressed = pressed;
      button.rawChangedAt = now;
    }

    const unsigned long stableDuration = now - button.rawChangedAt;

    if (button.rawPressed) {
      // Toggle exactly once after a continuous press of at least 150 ms.
      if (!button.pressHandled && stableDuration >= PRESS_QUALIFY_MS) {
        binaryValue ^= (uint8_t(1) << i);
        button.pressHandled = true;
        displayEnabled = true;
        outputsChanged = true;
      }
    } else if (stableDuration >= RELEASE_DEBOUNCE_MS) {
      // A stable release rearms this button without repeating while held.
      button.pressHandled = false;
    }
  }

  if (outputsChanged) {
    updateOutputs();
  }
}
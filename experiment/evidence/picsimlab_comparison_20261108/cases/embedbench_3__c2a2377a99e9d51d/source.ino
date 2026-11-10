#include <Arduino.h>

// Arduino Mega 2560; common-cathode RGB LED.
constexpr uint8_t RED_PIN   = 2;
constexpr uint8_t GREEN_PIN = 3;
constexpr uint8_t BLUE_PIN  = 4;

constexpr uint8_t HOLD_PIN   = 5;
constexpr uint8_t RESUME_PIN = 6;
constexpr uint8_t OFF_PIN    = 7;

constexpr unsigned long COLOR_DURATION_MS = 2000;
constexpr unsigned long RELEASE_DEBOUNCE_MS = 25;

enum Mode : uint8_t {
  CYCLING,
  HOLD,
  OFF
};

struct Button {
  uint8_t pin;
  bool latched;
  bool releasePending;
  unsigned long releaseStarted;

  // Register presses immediately; require a stable release before rearming.
  bool pressed(unsigned long now) {
    if (digitalRead(pin) == LOW) {
      releasePending = false;
      if (!latched) {
        latched = true;
        return true;
      }
    } else if (latched) {
      if (!releasePending) {
        releasePending = true;
        releaseStarted = now;
      } else if (now - releaseStarted >= RELEASE_DEBOUNCE_MS) {
        latched = false;
        releasePending = false;
      }
    }
    return false;
  }
};

Button holdButton   = {HOLD_PIN, false, false, 0};
Button resumeButton = {RESUME_PIN, false, false, 0};
Button offButton    = {OFF_PIN, false, false, 0};

Mode mode = CYCLING;
uint8_t currentColor = 0;  // 0 = red, 1 = green, 2 = blue
unsigned long colorStarted = 0;
unsigned long pausedElapsed = 0;

void updateLED() {
  const bool enabled = (mode != OFF);
  digitalWrite(RED_PIN,   enabled && currentColor == 0 ? HIGH : LOW);
  digitalWrite(GREEN_PIN, enabled && currentColor == 1 ? HIGH : LOW);
  digitalWrite(BLUE_PIN,  enabled && currentColor == 2 ? HIGH : LOW);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Buttons connect their respective input pins directly to ground.
  pinMode(HOLD_PIN, INPUT_PULLUP);
  pinMode(RESUME_PIN, INPUT_PULLUP);
  pinMode(OFF_PIN, INPUT_PULLUP);

  mode = CYCLING;
  currentColor = 0;
  pausedElapsed = 0;
  colorStarted = millis();
  updateLED();
}

void loop() {
  const unsigned long now = millis();

  const bool holdPressed = holdButton.pressed(now);
  const bool resumePressed = resumeButton.pressed(now);
  const bool offPressed = offButton.pressed(now);

  // Simultaneous presses prioritize OFF, then HOLD, then RESUME.
  if (offPressed) {
    if (mode == CYCLING) {
      pausedElapsed = now - colorStarted;
      if (pausedElapsed > COLOR_DURATION_MS) {
        pausedElapsed = COLOR_DURATION_MS;
      }
    }
    mode = OFF;
    updateLED();
  } else if (holdPressed) {
    if (mode == CYCLING) {
      pausedElapsed = now - colorStarted;
      if (pausedElapsed > COLOR_DURATION_MS) {
        pausedElapsed = COLOR_DURATION_MS;
      }
      mode = HOLD;
    }
    // HOLD while already OFF leaves the LED dark.
  } else if (resumePressed) {
    if (mode != CYCLING) {
      colorStarted = now - pausedElapsed;
      mode = CYCLING;
      updateLED();
    }
  }

  if (mode == CYCLING) {
    const unsigned long elapsed = now - colorStarted;
    if (elapsed >= COLOR_DURATION_MS) {
      const unsigned long steps = elapsed / COLOR_DURATION_MS;
      currentColor = (currentColor + steps % 3) % 3;
      colorStarted += steps * COLOR_DURATION_MS;
      updateLED();
    }
  }
}
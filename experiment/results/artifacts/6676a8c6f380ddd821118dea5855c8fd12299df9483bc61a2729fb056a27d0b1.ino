#include <Arduino.h>

constexpr uint8_t LED_PIN = 2;   // Mega 2560 PWM output
constexpr uint8_t BTN1_PIN = 3;   // Active LOW
constexpr uint8_t BTN2_PIN = 4;   // Active LOW
constexpr uint8_t POT1_PIN = A3;
constexpr uint8_t POT2_PIN = A4;
constexpr uint8_t POT3_PIN = A5;

constexpr unsigned long DEBOUNCE_MS = 150UL;
constexpr unsigned long MIN_MODE_TIME_MS = 2000UL;
constexpr unsigned long TRANSITION_MS = 2000UL;
constexpr unsigned long CYCLE_MS = 2UL * TRANSITION_MS;

enum Mode : uint8_t {
  MANUAL_MODE,
  AUTO_MODE
};

struct DebouncedButton {
  uint8_t pin;
  uint8_t rawState;
  uint8_t stableState;
  unsigned long lastChangeTime;

  void begin(uint8_t buttonPin, unsigned long now) {
    pin = buttonPin;
    pinMode(pin, INPUT_PULLUP);
    rawState = digitalRead(pin);
    stableState = HIGH;
    lastChangeTime = now;
  }

  // Returns true once for each debounced press, not continuously while held.
  bool update(unsigned long now) {
    const uint8_t reading = digitalRead(pin);

    if (reading != rawState) {
      rawState = reading;
      lastChangeTime = now;
    }

    if (rawState != stableState &&
        now - lastChangeTime >= DEBOUNCE_MS) {
      stableState = rawState;
      return stableState == LOW;
    }

    return false;
  }
};

DebouncedButton button1;
DebouncedButton button2;

Mode currentMode = MANUAL_MODE;
unsigned long modeStartedAt = 0;
unsigned long autoCycleStartedAt = 0;
bool switchPending = false;

uint8_t readBrightness(uint8_t pin) {
  return static_cast<uint8_t>(map(analogRead(pin), 0, 1023, 0, 255));
}

void enterMode(Mode nextMode, unsigned long now) {
  currentMode = nextMode;
  modeStartedAt = now;
  switchPending = false;

  if (nextMode == AUTO_MODE) {
    autoCycleStartedAt = now;
  }
}

void setup() {
  // Initialize the PWM output OFF before starting normal operation.
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  const unsigned long now = millis();
  button1.begin(BTN1_PIN, now);
  button2.begin(BTN2_PIN, now);

  currentMode = MANUAL_MODE;
  modeStartedAt = now;
  switchPending = false;
}

void loop() {
  const unsigned long now = millis();

  // Always service both buttons without blocking LED updates.
  const bool button1Pressed = button1.update(now);
  const bool button2Pressed = button2.update(now);

  if (currentMode == MANUAL_MODE) {
    if (button1Pressed) {
      switchPending = true;
    }
  } else {
    if (button2Pressed) {
      switchPending = true;
    }
  }

  // Remember an early press, but finish the current mode's minimum dwell.
  if (switchPending && now - modeStartedAt >= MIN_MODE_TIME_MS) {
    enterMode(currentMode == MANUAL_MODE ? AUTO_MODE : MANUAL_MODE, now);
  }

  if (currentMode == MANUAL_MODE) {
    // POT2 and POT3 have no effect in Manual Mode.
    analogWrite(LED_PIN, readBrightness(POT1_PIN));
    return;
  }

  const uint8_t minimumBrightness = readBrightness(POT2_PIN);
  uint8_t maximumBrightness = readBrightness(POT3_PIN);

  // An inverted minimum/maximum is not a valid range. Collapse it to the
  // POT2 minimum until POT3 is raised, preserving POT2's minimum setting.
  if (maximumBrightness < minimumBrightness) {
    maximumBrightness = minimumBrightness;
  }

  unsigned long elapsed = now - autoCycleStartedAt;

  // Advance the origin by whole cycles, including safely across millis wrap.
  if (elapsed >= CYCLE_MS) {
    const unsigned long completedTime = (elapsed / CYCLE_MS) * CYCLE_MS;
    autoCycleStartedAt += completedTime;
    elapsed -= completedTime;
  }

  // Rise for two seconds, then fall for two seconds, continuously.
  const unsigned long rampTime =
      elapsed <= TRANSITION_MS ? elapsed : CYCLE_MS - elapsed;

  const unsigned long span =
      static_cast<unsigned long>(maximumBrightness - minimumBrightness);

  const uint8_t brightness = static_cast<uint8_t>(
      minimumBrightness +
      (span * rampTime + TRANSITION_MS / 2UL) / TRANSITION_MS);

  analogWrite(LED_PIN, brightness);
}
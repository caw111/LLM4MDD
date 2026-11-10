#include <Arduino.h>

// Arduino Mega 2560 wiring:
// LED1: D2 -> anode, cathode -> GND
// LED2: D3 -> anode, cathode -> GND
// BTN1: D4 -> button -> GND
// BTN2: D5 -> button -> GND
// BTN3: D6 -> button -> GND
constexpr uint32_t DEBOUNCE_MS = 30;
constexpr uint32_t HOLD_MS = 2000;

struct Button {
  uint8_t pin;
  uint8_t rawState;
  uint8_t stableState;
  uint32_t rawChangedAt;
};

struct Led {
  uint8_t pin;
  bool on;
  bool holdActive;
  uint32_t holdStartedAt;
  uint32_t pendingToggles;
};

Button btn1 = {4, HIGH, HIGH, 0};
Button btn2 = {5, HIGH, HIGH, 0};
Button btn3 = {6, HIGH, HIGH, 0};

Led led1 = {2, false, false, 0, 0};
Led led2 = {3, false, false, 0, 0};

// Returns true once per debounced press; holding a button does not repeat.
bool pressed(Button &button, uint32_t now) {
  const uint8_t reading = digitalRead(button.pin);

  if (reading != button.rawState) {
    button.rawState = reading;
    button.rawChangedAt = now;
  }

  if (button.stableState != button.rawState &&
      static_cast<uint32_t>(now - button.rawChangedAt) >= DEBOUNCE_MS) {
    button.stableState = button.rawState;
    return button.stableState == LOW;
  }

  return false;
}

void resetLed(Led &led, uint32_t now) {
  led.on = false;
  led.pendingToggles = 0;
  led.holdActive = true;
  led.holdStartedAt = now;
  digitalWrite(led.pin, LOW);
}

void serviceLed(Led &led, uint32_t now) {
  if (led.holdActive) {
    if (static_cast<uint32_t>(now - led.holdStartedAt) < HOLD_MS) {
      return;
    }
    led.holdActive = false;
  }

  // Queue presses during a hold rather than losing them. Each resulting
  // toggle gets its own full two-second stable interval.
  if (led.pendingToggles > 0) {
    --led.pendingToggles;
    led.on = !led.on;
    digitalWrite(led.pin, led.on ? HIGH : LOW);
    led.holdStartedAt = now;
    led.holdActive = true;
  }
}

void setup() {
  // Set output latches LOW before enabling outputs.
  digitalWrite(led1.pin, LOW);
  digitalWrite(led2.pin, LOW);
  pinMode(led1.pin, OUTPUT);
  pinMode(led2.pin, OUTPUT);

  // Buttons connect their input pins to GND when pressed.
  pinMode(btn1.pin, INPUT_PULLUP);
  pinMode(btn2.pin, INPUT_PULLUP);
  pinMode(btn3.pin, INPUT_PULLUP);
}

void loop() {
  const uint32_t now = millis();

  const bool btn1Pressed = pressed(btn1, now);
  const bool btn2Pressed = pressed(btn2, now);
  const bool btn3Pressed = pressed(btn3, now);

  // Reset takes priority, including over an existing toggle hold or
  // simultaneous toggle presses, and cancels previously queued toggles.
  if (btn3Pressed) {
    resetLed(led1, now);
    resetLed(led2, now);
  } else {
    if (btn1Pressed) {
      ++led1.pendingToggles;
    }
    if (btn2Pressed) {
      ++led2.pendingToggles;
    }
  }

  // Independent nonblocking holds keep all three buttons responsive.
  serviceLed(led1, now);
  serviceLed(led2, now);
}
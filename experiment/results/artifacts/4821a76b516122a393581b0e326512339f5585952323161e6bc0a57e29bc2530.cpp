#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_84\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: all three LED pins support hardware PWM.
// The RGB LED is common-cathode, so 0 = off and 255 = full brightness.
const uint8_t RED_PIN   = 4;
const uint8_t GREEN_PIN = 5;
const uint8_t BLUE_PIN  = 6;

const uint8_t POT_PINS[3] = {A3, A4, A5};
const uint8_t MODE_BUTTON_PIN  = 2;
const uint8_t RESET_BUTTON_PIN = 3;

const unsigned long DEBOUNCE_MS = 25;

struct Button {
  uint8_t pin;
  bool rawState;
  bool stableState;
  unsigned long changedAt;

  void begin(uint8_t inputPin) {
    pin = inputPin;
    pinMode(pin, INPUT_PULLUP);
    rawState = digitalRead(pin);
    stableState = HIGH;
    changedAt = millis();
  }

  bool pressed(unsigned long now) {
    const bool reading = digitalRead(pin);

    if (reading != rawState) {
      rawState = reading;
      changedAt = now;
    }

    if (stableState != rawState &&
        now - changedAt >= DEBOUNCE_MS) {
      stableState = rawState;
      return stableState == LOW;
    }

    return false;
  }

  bool held() const {
    return stableState == LOW;
  }
};

Button modeButton;
Button resetButton;

uint8_t mode = 1;
bool ledEnabled = false;
uint8_t potValues[3] = {0, 0, 0};

#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_84\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t readPot(uint8_t pin);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_84\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLED();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_84\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_84\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_84\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t readPot(uint8_t pin) {
  return static_cast<uint8_t>(
      map(analogRead(pin), 0, 1023, 0, 255));
}

void updateLED() {
  const uint8_t red =
      ledEnabled && (mode == 1 || mode == 4) ? potValues[0] : 0;
  const uint8_t green =
      ledEnabled && (mode == 2 || mode == 4) ? potValues[1] : 0;
  const uint8_t blue =
      ledEnabled && (mode == 3 || mode == 4) ? potValues[2] : 0;

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void setup() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  mode = 1;
  ledEnabled = false;
  updateLED();

  modeButton.begin(MODE_BUTTON_PIN);
  resetButton.begin(RESET_BUTTON_PIN);

  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(POT_PINS[i], INPUT);
    potValues[i] = readPot(POT_PINS[i]);
  }
}

void loop() {
  const unsigned long now = millis();
  const bool modePressed = modeButton.pressed(now);
  const bool resetPressed = resetButton.pressed(now);

  bool potentiometerChanged = false;

  for (uint8_t i = 0; i < 3; ++i) {
    const uint8_t value = readPot(POT_PINS[i]);
    if (value != potValues[i]) {
      potValues[i] = value;
      potentiometerChanged = true;
    }
  }

  // Reset takes priority, including when both buttons are pressed.
  // Keep sampling the pots while reset is held to avoid stale readings.
  if (resetPressed || resetButton.held()) {
    mode = 1;
    ledEnabled = false;
  } else {
    if (modePressed) {
      // The first press from off displays Mode 1. Further presses cycle.
      if (ledEnabled) {
        mode = (mode % 4) + 1;
      }
      ledEnabled = true;
    }

    // Startup/reset stays off until a button press or pot adjustment.
    if (potentiometerChanged) {
      ledEnabled = true;
    }
  }

  updateLED();
}

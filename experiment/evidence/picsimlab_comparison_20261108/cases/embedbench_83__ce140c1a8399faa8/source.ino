#include <Servo.h>

Servo servo1;
Servo servo2;

// From wiring (Arduino Mega 2560)
static const uint8_t BTN1_PIN = 2;   // mega:2
static const uint8_t BTN2_PIN = 3;   // mega:3

// RGB LED (common cathode)
static const uint8_t RGB_R_PIN = 4;   // mega:4 -> rgb1:R
static const uint8_t RGB_G_PIN = 5;   // mega:5 -> rgb1:G
static const uint8_t RGB_B_PIN = 6;   // mega:6 -> rgb1:B

// Servo positions
static const int SERVO1_INIT = 0;
static const int SERVO2_INIT = 0;
static const int SERVO1_BTN1 = 90;
static const int SERVO2_BTN2 = 180;

// Debounce and "maintain for at least 2 seconds"
static const unsigned long MAINTAIN_MS = 2000;
static const unsigned long DEBOUNCE_MS = 50;

struct DebouncedButton {
  uint8_t pin;
  bool stableState;          // last debounced pressed state (true = pressed)
  bool lastReading;          // last raw reading (pressed=HIGH after pullups)
  unsigned long lastChange;  // time of last raw change

  void begin(uint8_t p) {
    pin = p;
    pinMode(pin, INPUT_PULLUP); // buttons wired to GND when pressed
    stableState = (digitalRead(pin) == HIGH); // not pressed initially
    lastReading = stableState;
    lastChange = millis();
  }

  bool readPressed() {
    bool readingPressed = (digitalRead(pin) == HIGH); // INPUT_PULLUP: pressed -> HIGH
    // If reading changed, start debounce timer
    if (readingPressed != lastReading) {
      lastReading = readingPressed;
      lastChange = millis();
    }
    // If stable for debounce duration, update stableState
    if ((millis() - lastChange) >= DEBOUNCE_MS) {
      stableState = lastReading;
    }
    return stableState;
  }
};

DebouncedButton btn1, btn2;

enum LedServoState {
  STATE_OFF = 0,   // no buttons
  STATE_RED,       // btn1 pressed
  STATE_BLUE,      // btn2 pressed
  STATE_GREEN_BOTH // both pressed
};

LedServoState currentState = STATE_OFF;
unsigned long stateStartMs = 0;

void setRgbOff() {
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);
}

void setRgbRed() {
  digitalWrite(RGB_R_PIN, HIGH);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);
}

void setRgbBlue() {
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, HIGH);
}

void setRgbGreen() {
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, HIGH);
  digitalWrite(RGB_B_PIN, LOW);
}

void setServosToInit() {
  servo1.write(SERVO1_INIT);
  servo2.write(SERVO2_INIT);
}

void setServosForState(LedServoState s) {
  switch (s) {
    case STATE_OFF:
      // Keep servos in their current positions (no action)
      break;
    case STATE_RED:
      servo1.write(SERVO1_BTN1);
      // servo2 unchanged
      break;
    case STATE_BLUE:
      servo2.write(SERVO2_BTN2);
      // servo1 unchanged
      break;
    case STATE_GREEN_BOTH:
      servo1.write(SERVO1_INIT);
      servo2.write(SERVO2_INIT);
      break;
  }
}

void applyState(LedServoState s) {
  currentState = s;
  stateStartMs = millis();

  // Update LED and relevant servo outputs per requirements
  switch (s) {
    case STATE_OFF:
      setRgbOff();
      // servos stay in current positions
      break;
    case STATE_RED:
      setRgbRed();
      setServosForState(s);
      break;
    case STATE_BLUE:
      setRgbBlue();
      setServosForState(s);
      break;
    case STATE_GREEN_BOTH:
      setRgbGreen();
      setServosForState(s);
      break;
  }
}

bool buttonsPressedBoth(const bool p1, const bool p2) {
  return p1 && p2;
}

void setup() {
  // Servo attach pins (from wiring)
  // mega:7 -> servo1:PWM
  // mega:8 -> servo2:PWM
  servo1.attach(7);
  servo2.attach(8);

  // RGB pins
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Buttons
  btn1.begin(BTN1_PIN);
  btn2.begin(BTN2_PIN);

  // Initialization: both servos at 0, rgb off
  setRgbOff();
  setServosToInit();
  currentState = STATE_OFF;
  stateStartMs = millis();
}

void loop() {
  bool p1 = btn1.readPressed();
  bool p2 = btn2.readPressed();
  bool both = buttonsPressedBoth(p1, p2);

  // Determine desired state according to the rules.
  // Priority: both pressed => green and return both servos to 0.
  LedServoState desired = STATE_OFF;
  if (both) {
    desired = STATE_GREEN_BOTH;
  } else if (p1) {
    desired = STATE_RED;
  } else if (p2) {
    desired = STATE_BLUE;
  } else {
    desired = STATE_OFF;
  }

  // If currently in a maintained state, don't change until at least 2 seconds have passed.
  bool isMaintained = (currentState == STATE_RED || currentState == STATE_BLUE || currentState == STATE_GREEN_BOTH);
  if (isMaintained && (millis() - stateStartMs) < MAINTAIN_MS) {
    // However, requirement 3 says when one button is pressed and another has already been pressed,
    // both must return to 0 degrees and green, maintained at least 2 seconds.
    // That corresponds to both-pressed condition, so allow transition to green if both are pressed.
    if (desired == STATE_GREEN_BOTH && currentState != STATE_GREEN_BOTH) {
      applyState(STATE_GREEN_BOTH);
      return;
    }
    // Otherwise, keep current state outputs
    delay(5);
    return;
  }

  // If no buttons pressed, RGB stays off and servos stay in their current positions.
  // If a single button pressed, go to corresponding color and servo move.
  // If both pressed, reset to init and green.
  if (desired != currentState) {
    // For requirement 3: when one button pressed and another already pressed,
    // transition happens when both become pressed (or effectively detected).
    applyState(desired);
  } else {
    // Maintain outputs for the current state; for STATE_OFF we ensure LED remains off
    if (currentState == STATE_OFF) {
      setRgbOff();
    }
  }

  delay(5);
}
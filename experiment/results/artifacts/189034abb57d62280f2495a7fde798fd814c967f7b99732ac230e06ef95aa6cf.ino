#include <Servo.h>

enum State : uint8_t { STATE0 = 0, STATE1 = 1, STATE2 = 2 };

static const uint8_t BTN_PIN = 2;       // btn1:1.l to mega:2, btn1:2.r to mega:GND.1
static const uint8_t SERVO1_PIN = 3;    // mega:3 to servo1:PWM
static const uint8_t SERVO2_PIN = 4;    // mega:4 to servo2:PWM

// bargraph1 wiring (10 segments: A1..A10). C1..C10 are all tied to GND.
static const uint8_t BAR_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

static const unsigned long DEBOUNCE_MS = 150;
static const unsigned long STATE_LOCK_MS = 2000;

Servo servo1;
Servo servo2;

State currentState = STATE0;

bool lastBtnStable = HIGH;     // using INPUT_PULLUP: not-pressed = HIGH
bool lastBtnReading = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long lockUntil = 0;

static void setBargraphAll(bool on, uint8_t count) {
  for (uint8_t i = 0; i < 10; i++) {
    bool segmentOn = on && (i < count);
    digitalWrite(BAR_PINS[i], segmentOn ? HIGH : LOW);
  }
}

static void applyState(State s) {
  // Servo positions and bargraph updates must happen immediately after transition.
  switch (s) {
    case STATE0:
      servo1.write(0);
      servo2.write(180);
      setBargraphAll(false, 0);      // all off
      break;

    case STATE1:
      servo1.write(90);
      servo2.write(90);
      setBargraphAll(true, 5);       // first 5 LEDs on
      break;

    case STATE2:
      servo1.write(180);
      servo2.write(0);
      setBargraphAll(true, 10);      // all 10 LEDs on
      break;
  }
}

void setup() {
  // Button input with internal pull-up (since wiring connects switch to GND).
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Bargraph outputs
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW); // ensure all LEDs off at start
  }

  // Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization: servos at 0°, all LEDs off.
  servo1.write(0);
  servo2.write(0);

  // Ensure bargraph off.
  setBargraphAll(false, 0);

  // State machine start: requirement says State 0 maintained for 2 seconds after initialization,
  // and State 0 configuration is Servo1=0°, Servo2=180°, all LEDs off.
  // Apply it immediately, then lock further transitions for 2 seconds.
  currentState = STATE0;
  applyState(currentState);

  lockUntil = millis() + STATE_LOCK_MS;

  // Initialize debounce tracking.
  lastBtnReading = digitalRead(BTN_PIN);
  lastBtnStable = lastBtnReading;
  lastDebounceTime = millis();
}

void loop() {
  unsigned long now = millis();

  // Read current button state
  bool reading = digitalRead(BTN_PIN);

  // Debounce handling
  if (reading != lastBtnReading) {
    lastDebounceTime = now;
    lastBtnReading = reading;
  }

  bool stableChanged = false;
  if (reading != lastBtnStable && (now - lastDebounceTime) >= DEBOUNCE_MS) {
    lastBtnStable = reading;
    stableChanged = true;
  }

  // Detect a valid press: stable transition to LOW (pressed)
  if (stableChanged && lastBtnStable == LOW) {
    if (now >= lockUntil) {
      // Advance state
      State next;
      if (currentState == STATE0) next = STATE1;
      else if (currentState == STATE1) next = STATE2;
      else next = STATE0;

      currentState = next;

      // Update immediately after valid state transition
      applyState(currentState);

      // Lock further transitions for 2 seconds
      lockUntil = now + STATE_LOCK_MS;
    }
  }
}
#include <Servo.h>

Servo servo1;
Servo servo2;

// Wiring (from provided circuit)
const uint8_t LED_PIN   = 2;  // mega:2 -> led1:A (red)
const uint8_t BTN1_PIN  = 3;  // mega:3 -> btn1:1.l
const uint8_t BTN2_PIN  = 4;  // mega:4 -> btn2:1.l

// Servos
// mega:5 -> servo1:PWM
// mega:6 -> servo2:PWM
const uint8_t SERVO1_PWM_PIN = 5;
const uint8_t SERVO2_PWM_PIN = 6;

// Angles
const int SERVO1_INIT_ANGLE = 0;
const int SERVO2_INIT_ANGLE = 0;
const int SERVO1_BTN1_ANGLE = 90;
const int SERVO2_BTN2_ANGLE = 180;

// Button behavior
// Use internal pull-ups: assume pushbuttons connect to GND when pressed.
// (Circuit ties other side to GND.)
const bool PRESSED_LEVEL = LOW; // with INPUT_PULLUP

// Timing requirements
const unsigned long MIN_STATE_HOLD_MS = 2000; // maintain each state at least 2 seconds
const unsigned long FLASH_PERIOD_MS = 4000;    // 0.25 Hz -> period 4s (2s on, 2s off)

enum SystemState {
  ST_IDLE,        // initial condition: both at 0, LED off
  ST_BTN1_ACTIVE, // servo1=90, LED on
  ST_BTN2_ACTIVE, // servo2=180, LED off (explicitly off)
  ST_FLASH_MIXED  // both servos at 0, LED flashing 2s on / 2s off
};

SystemState state = ST_IDLE;

// When current state became active
unsigned long stateStartMs = 0;

// For edge detection
bool lastBtn1 = false;
bool lastBtn2 = false;

// Helper: set servos to initial
void setServosToInitial() {
  servo1.write(SERVO1_INIT_ANGLE);
  servo2.write(SERVO2_INIT_ANGLE);
}

void setState(SystemState newState) {
  state = newState;
  stateStartMs = millis();

  switch (state) {
    case ST_IDLE:
      setServosToInitial();
      digitalWrite(LED_PIN, LOW);
      break;

    case ST_BTN1_ACTIVE:
      servo1.write(SERVO1_BTN1_ANGLE);
      // servo2 should remain at initial position as per behavior
      servo2.write(SERVO2_INIT_ANGLE);
      digitalWrite(LED_PIN, HIGH);
      break;

    case ST_BTN2_ACTIVE:
      servo2.write(SERVO2_BTN2_ANGLE);
      // servo1 should remain at initial position as per behavior
      servo1.write(SERVO1_INIT_ANGLE);
      digitalWrite(LED_PIN, LOW);
      break;

    case ST_FLASH_MIXED:
      setServosToInitial();
      // LED handled in loop with flashing schedule
      break;
  }
}

bool readBtn(uint8_t pin) {
  return digitalRead(pin) == PRESSED_LEVEL;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);

  // Initialization requirement
  setState(ST_IDLE);

  // Initialize last button states for edge detection
  lastBtn1 = readBtn(BTN1_PIN);
  lastBtn2 = readBtn(BTN2_PIN);
}

void loop() {
  bool btn1Pressed = readBtn(BTN1_PIN);
  bool btn2Pressed = readBtn(BTN2_PIN);

  // Rising edges: transition from not pressed -> pressed
  bool btn1Edge = (btn1Pressed && !lastBtn1);
  bool btn2Edge = (btn2Pressed && !lastBtn2);

  // For mixed condition:
  // "If one button has already been pressed and the other button is pressed"
  // We'll trigger mixed state when a button edge occurs while the other button
  // is currently pressed. This preserves the intended behavior without simplifying.
  bool otherBtnCurrentlyPressedForBtn1 = btn2Pressed;
  bool otherBtnCurrentlyPressedForBtn2 = btn1Pressed;

  unsigned long now = millis();

  // Maintain minimum hold time: ignore new transitions until 2 seconds have elapsed,
  // except we still need to allow the "mixed" state to be entered when appropriate.
  // To preserve "maintained for at least 2 seconds", we will not transition
  // out of the current state until MIN_STATE_HOLD_MS has passed.
  bool canTransition = (now - stateStartMs) >= MIN_STATE_HOLD_MS;

  if (canTransition) {
    // Prioritize mixed condition if the triggering edge occurs while the other is currently pressed
    if (btn1Edge && otherBtnCurrentlyPressedForBtn1) {
      setState(ST_FLASH_MIXED);
    } else if (btn2Edge && otherBtnCurrentlyPressedForBtn2) {
      setState(ST_FLASH_MIXED);
    } else if (btn1Edge) {
      setState(ST_BTN1_ACTIVE);
    } else if (btn2Edge) {
      setState(ST_BTN2_ACTIVE);
    }
    // Otherwise stay in current state.
  }

  // LED flashing logic for mixed state
  if (state == ST_FLASH_MIXED) {
    // 0.25 Hz => 4s period, 2s ON then 2s OFF
    unsigned long t = (now - stateStartMs) % FLASH_PERIOD_MS;
    if (t < (FLASH_PERIOD_MS / 2)) {
      digitalWrite(LED_PIN, HIGH); // 2s on
    } else {
      digitalWrite(LED_PIN, LOW);  // 2s off
    }
  } else if (state == ST_IDLE) {
    digitalWrite(LED_PIN, LOW);
  } else if (state == ST_BTN1_ACTIVE) {
    digitalWrite(LED_PIN, HIGH);
  } else if (state == ST_BTN2_ACTIVE) {
    digitalWrite(LED_PIN, LOW);
  }

  lastBtn1 = btn1Pressed;
  lastBtn2 = btn2Pressed;

  // Small delay for stability (does not affect timing requirements significantly)
  delay(10);
}
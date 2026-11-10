#include <Servo.h>

Servo servo1;
Servo servo2;

static const uint8_t BTN1_PIN = 3;   // from wiring: mega:3 -> btn1:1.l
static const uint8_t BTN2_PIN = 4;   // from wiring: mega:4 -> btn2:1.l
static const uint8_t LED_PIN  = 2;    // from wiring: mega:2 -> led1:A

static const uint8_t SERVO1_PIN = 5; // from wiring: mega:5 -> servo1:PWM
static const uint8_t SERVO2_PIN = 6; // from wiring: mega:6 -> servo2:PWM

static const int SERVO1_INIT_DEG = 0;
static const int SERVO2_INIT_DEG = 0;
static const int SERVO1_BTN1_DEG  = 90;
static const int SERVO2_BTN2_DEG  = 180;

// State machine to enforce "maintained for at least 2 seconds"
enum SystemState : uint8_t {
  STATE_IDLE_INIT = 0,   // both servos at 0, LED off
  STATE_BTN1_HIT   = 1,  // servo1=90, LED on
  STATE_BTN2_HIT   = 2,  // servo2=180, LED off
  STATE_CONFLICT_FLASH = 3 // both servos 0, LED flashes 0.25 Hz (2s on, 2s off)
};

SystemState state = STATE_IDLE_INIT;
unsigned long stateStartMs = 0;

// For conflict flash, track LED toggling independent of state maintenance
unsigned long lastFlashToggleMs = 0;
bool flashLedOn = false;

static bool readBtn(uint8_t pin) {
  // Buttons are wired to GND when pressed (other terminal to pin), so pressed => LOW.
  return digitalRead(pin) == LOW;
}

static void setServos(int s1Deg, int s2Deg) {
  servo1.write(s1Deg);
  servo2.write(s2Deg);
}

static void setLed(bool on) {
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

static void enterState(SystemState newState) {
  state = newState;
  stateStartMs = millis();

  switch (state) {
    case STATE_IDLE_INIT:
      setServos(SERVO1_INIT_DEG, SERVO2_INIT_DEG);
      setLed(false);
      break;

    case STATE_BTN1_HIT:
      setServos(SERVO1_BTN1_DEG, SERVO2_INIT_DEG);
      setLed(true);
      break;

    case STATE_BTN2_HIT:
      setServos(SERVO1_INIT_DEG, SERVO2_BTN2_DEG);
      setLed(false);
      break;

    case STATE_CONFLICT_FLASH:
      setServos(SERVO1_INIT_DEG, SERVO2_INIT_DEG);

      // Start flash with LED ON then toggle every 2 seconds
      flashLedOn = true;
      setLed(true);
      lastFlashToggleMs = millis();
      break;
  }
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization per requirements
  enterState(STATE_IDLE_INIT);
}

void loop() {
  bool btn1 = readBtn(BTN1_PIN);
  bool btn2 = readBtn(BTN2_PIN);

  unsigned long now = millis();

  // State maintenance: enforce at least 2 seconds in each state
  const unsigned long MIN_STATE_MS = 2000;

  // Determine next desired state based on current button interaction.
  // - If one button has already been pressed and the other button is pressed:
  //   -> conflict flash with both servos to 0 and LED flashing until btn1 or btn2 is pressed again.
  // Since we can't directly know "already pressed" history beyond current state,
  // we implement the required behavior as:
  // - From BTN1_HIT: if BTN2 becomes pressed while still in that state => conflict.
  // - From BTN2_HIT: if BTN1 becomes pressed while still in that state => conflict.
  // - From CONFLICT_FLASH: while no new button press occurs, it should keep flashing.
  //   The requirement says "until btn1 or btn2 is pressed again". We'll treat any press
  //   (btn1 or btn2 low) as "again". Because buttons can be held, we need edge detection
  //   to avoid immediate exit without an intentional re-press.
  static bool prevBtn1 = false;
  static bool prevBtn2 = false;

  bool btn1Rising = (btn1 && !prevBtn1); // pressed event (LOW -> high? actually pressed => true)
  bool btn2Rising = (btn2 && !prevBtn2);

  prevBtn1 = btn1;
  prevBtn2 = btn2;

  // Handle conflict flash LED toggling (0.25 Hz => period 4 seconds => 2s on, 2s off)
  if (state == STATE_CONFLICT_FLASH) {
    // Toggle every 2000 ms
    if (now - lastFlashToggleMs >= 2000UL) {
      lastFlashToggleMs = now;
      flashLedOn = !flashLedOn;
      setLed(flashLedOn);
    }
  }

  // Decide transitions, but only allow transitions after minimum time in current state,
  // except the conflict flash should also remain until btn1 or btn2 is pressed again.
  bool canTransition = (now - stateStartMs) >= MIN_STATE_MS;

  switch (state) {
    case STATE_IDLE_INIT: {
      if (canTransition) {
        if (btn1) {
          // Press btn1 => servo1 90, LED on
          enterState(STATE_BTN1_HIT);
        } else if (btn2) {
          // Press btn2 => servo2 180, LED off
          enterState(STATE_BTN2_HIT);
        }
      }
      break;
    }

    case STATE_BTN1_HIT: {
      // Maintain at least 2s. If btn2 is pressed while in btn1-hit (after 2s),
      // conflict flash should occur.
      if (canTransition) {
        if (btn2) {
          // both servos return to 0, LED flashes 0.25 Hz until btn1 or btn2 pressed again
          enterState(STATE_CONFLICT_FLASH);
        } else if (btn1) {
          // continue state; no change
        } else {
          // If neither pressed, requirement doesn't say to revert; maintain until other interaction.
        }
      }
      break;
    }

    case STATE_BTN2_HIT: {
      if (canTransition) {
        if (btn1) {
          enterState(STATE_CONFLICT_FLASH);
        } else if (btn2) {
          // continue state
        } else {
          // maintain
        }
      }
      break;
    }

    case STATE_CONFLICT_FLASH: {
      // Must keep flashing until btn1 or btn2 is pressed again.
      // Use rising edge detection of "pressed" (btn becomes true).
      if (btn1Rising) {
        enterState(STATE_BTN1_HIT);
      } else if (btn2Rising) {
        enterState(STATE_BTN2_HIT);
      }
      break;
    }
  }
}
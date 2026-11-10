#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

enum SystemState : uint8_t { STATE0 = 0, STATE1 = 1, STATE2 = 2 };

Servo servo1;
Servo servo2;

// Wiring per circuit:
const uint8_t BTN_PIN = 2; // btn1:1.l (to mega:2) and btn1:2.r (to mega:GND.1)
const uint8_t SERVO1_PIN = 3; // mega:3 -> servo1:PWM
const uint8_t SERVO2_PIN = 4; // mega:4 -> servo2:PWM

// 10-segment LED bar graph segment pins:
// mega:5 -> A1, mega:6 -> A2, ... mega:13 -> A9, mega:44 -> A10
const uint8_t BAR_A_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
const uint8_t BAR_C_PINS[10] = {
  // all C pins connected to mega:GND.1 in circuit
  // left here for completeness; not used directly since they are tied to ground in wiring.
  0,0,0,0,0,0,0,0,0,0
};

static_assert(10 == sizeof(BAR_A_PINS) / sizeof(BAR_A_PINS[0]), "Bargraph A pins mismatch");

const unsigned long HOLD_MS = 2000;     // maintained for 2 seconds after init and after each button press
const unsigned long DEBOUNCE_MS = 150;  // debounced for 0.15 seconds
const unsigned long LOCK_MS = 2000;     // transitions locked for 2 seconds after each valid button press

SystemState state = STATE0;

unsigned long stateStartMs = 0;      // when current state's "maintained" period started
unsigned long lockedUntilMs = 0;     // no transitions until this time after a valid press

// Button debouncing
bool btnStableHigh = false;          // with pullup, stable high means released
bool btnLastReadHigh = false;
unsigned long btnDebounceStartMs = 0;

#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setServosForState(SystemState s);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setBargraphForState(SystemState s);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(SystemState s, bool resetTimersForThisState);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool readButtonReleased();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDebounce();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool isRisingEdgeToReleased();
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_74\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setServosForState(SystemState s) {
  switch (s) {
    case STATE0:
      servo1.write(0);
      servo2.write(180);
      break;
    case STATE1:
      servo1.write(90);
      servo2.write(90);
      break;
    case STATE2:
      servo1.write(180);
      servo2.write(0);
      break;
  }
}

void setBargraphForState(SystemState s) {
  // All LEDs off for STATE0, first 5 LEDs lit for STATE1, all 10 lit for STATE2.
  uint8_t litCount = 0;
  switch (s) {
    case STATE0: litCount = 0; break;
    case STATE1: litCount = 5; break;
    case STATE2: litCount = 10; break;
  }

  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_A_PINS[i], (i < litCount) ? HIGH : LOW);
  }
}

void applyState(SystemState s, bool resetTimersForThisState) {
  state = s;
  setServosForState(state);
  setBargraphForState(state);

  if (resetTimersForThisState) {
    unsigned long now = millis();
    stateStartMs = now;       // used only for the "maintained for 2 seconds" requirement
    // lockedUntilMs is set when transitioning due to button; for init we keep it open.
  }
}

bool readButtonReleased() {
  // With wiring to GND and using INPUT_PULLUP:
  // - pressed: BTN_PIN reads LOW
  // - released: BTN_PIN reads HIGH
  return digitalRead(BTN_PIN) == HIGH;
}

void updateDebounce() {
  bool btnReadHigh = readButtonReleased();

  if (btnReadHigh != btnLastReadHigh) {
    btnLastReadHigh = btnReadHigh;
    btnDebounceStartMs = millis();
  }

  // If the input has been stable for DEBOUNCE_MS, update stable value.
  if ((millis() - btnDebounceStartMs) >= DEBOUNCE_MS) {
    btnStableHigh = btnLastReadHigh;
  }
}

bool isRisingEdgeToReleased() {
  // We want a "button press" to advance.
  // With INPUT_PULLUP, a press produces a transition: released(HIGH) -> pressed(LOW).
  // We interpret "valid button press" as the moment we detect the stable transition to LOW.
  static bool prevStableHigh = true;
  bool currentStableHigh = btnStableHigh;

  // Detect falling edge stable HIGH -> stable LOW
  bool pressedEvent = (prevStableHigh == true && currentStableHigh == false);
  prevStableHigh = currentStableHigh;

  return pressedEvent;
}

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW); // start with all off
  }

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization requirements:
  // - Both servos start at 0° position (immediately).
  // - All bargraph segments off.
  servo1.write(0);
  servo2.write(0);
  for (uint8_t i = 0; i < 10; i++) digitalWrite(BAR_A_PINS[i], LOW);

  // Apply initial State 0 configuration and maintain for 2 seconds after initialization.
  applyState(STATE0, true);
  lockedUntilMs = millis(); // allow immediate reads; but we won't transition until lock after a press anyway

  // Ensure initial "maintained for 2 seconds" period exists.
  // (No automatic transitions are specified; this is just the required hold duration.)
  // We'll keep it via stateStartMs; the system only changes upon button presses.
  // Nothing else needed here.
}

void loop() {
  unsigned long now = millis();

  // Keep "maintained for 2 seconds" behavior without auto-advancing:
  // The requirements specify the state is maintained for 2 seconds after init and after each button press.
  // Since transitions occur only on button press, maintaining is naturally satisfied.
  // However, to mirror "locked for 2 seconds after each button press", we enforce that via lockedUntilMs.

  updateDebounce();

  // After we have a valid stable press edge, advance if not locked.
  bool pressedEvent = isRisingEdgeToReleased(); // detects stable transition released->pressed
  if (pressedEvent) {
    if (now >= lockedUntilMs) {
      // Advance state: 0 -> 1 -> 2 -> 0
      SystemState nextState;
      switch (state) {
        case STATE0: nextState = STATE1; break;
        case STATE1: nextState = STATE2; break;
        case STATE2: nextState = STATE0; break;
      }

      // Update immediately after valid transition
      applyState(nextState, true);

      // Lock transitions for 2 seconds after the button press
      lockedUntilMs = now + LOCK_MS;
    }
  }

  // No further behavior; state changes are button-driven only.
  (void)stateStartMs;
}

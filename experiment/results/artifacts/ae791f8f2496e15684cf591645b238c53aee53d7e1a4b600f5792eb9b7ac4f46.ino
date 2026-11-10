#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t PIN_BTN1 = 2;     // io_0, active_high=false (pressed when LOW)
static constexpr uint8_t PIN_SERVO1 = 3;   // io_1, servo1
static constexpr uint8_t PIN_SERVO2 = 4;   // io_2, servo2

// Bargraph segments: io_3_segment_0..9
static constexpr uint8_t PIN_BG0 = 5;   // A1
static constexpr uint8_t PIN_BG1 = 6;   // A2
static constexpr uint8_t PIN_BG2 = 7;   // A3
static constexpr uint8_t PIN_BG3 = 8;   // A4
static constexpr uint8_t PIN_BG4 = 9;   // A5
static constexpr uint8_t PIN_BG5 = 10;  // A6
static constexpr uint8_t PIN_BG6 = 11;  // A7
static constexpr uint8_t PIN_BG7 = 12;  // A8
static constexpr uint8_t PIN_BG8 = 13;  // A9
static constexpr uint8_t PIN_BG9 = 44;  // A10

// Control_period_ms = 10
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Debounce qualification and locks per model
static constexpr int DEBOUNCE_MS_STEP = 10; // tick increment
static constexpr int DEBOUNCE_THRESHOLD_MS = 15; // debounceMs >= 15 -> true
static constexpr int LOCK_MS = 2000;
static constexpr int MAINTAIN_MS_INIT = 2000;

// Model state
static int stateIndex = 0;           // initial 0
static bool prevPressed = false;    // initial false
static bool debounceStable = false; // initial false
static int debounceMs = 0;          // initial 0
static int lockMs = 0;              // initial 0
static int maintainMs = MAINTAIN_MS_INIT; // initial 2000

static Servo servo1;
static Servo servo2;

// Outputs: immediate actuator/LED updates on valid transition
static void applyOutputsForState(int s) {
  // io_1, io_2
  double angle1 = (s == 0) ? 0.0 : (s == 1 ? 90.0 : 180.0);
  double angle2 = (s == 0) ? 180.0 : (s == 1 ? 90.0 : 0.0);
  servo1.write((int)angle1);
  servo2.write((int)angle2);

  // io_3 segments (active_high=true)
  bool seg0 = (s == 1 || s == 2);
  bool seg1 = (s == 1 || s == 2);
  bool seg2 = (s == 1 || s == 2);
  bool seg3 = (s == 1 || s == 2);
  bool seg4 = (s == 1 || s == 2);
  bool seg5 = (s == 2);
  bool seg6 = (s == 2);
  bool seg7 = (s == 2);
  bool seg8 = (s == 2);
  bool seg9 = (s == 2);

  digitalWrite(PIN_BG0, seg0 ? HIGH : LOW);
  digitalWrite(PIN_BG1, seg1 ? HIGH : LOW);
  digitalWrite(PIN_BG2, seg2 ? HIGH : LOW);
  digitalWrite(PIN_BG3, seg3 ? HIGH : LOW);
  digitalWrite(PIN_BG4, seg4 ? HIGH : LOW);
  digitalWrite(PIN_BG5, seg5 ? HIGH : LOW);
  digitalWrite(PIN_BG6, seg6 ? HIGH : LOW);
  digitalWrite(PIN_BG7, seg7 ? HIGH : LOW);
  digitalWrite(PIN_BG8, seg8 ? HIGH : LOW);
  digitalWrite(PIN_BG9, seg9 ? HIGH : LOW);
}

void setup() {
  // Button input with pullup. Pressed = LOW.
  pinMode(PIN_BTN1, INPUT_PULLUP);

  // Bargraph outputs
  pinMode(PIN_BG0, OUTPUT);
  pinMode(PIN_BG1, OUTPUT);
  pinMode(PIN_BG2, OUTPUT);
  pinMode(PIN_BG3, OUTPUT);
  pinMode(PIN_BG4, OUTPUT);
  pinMode(PIN_BG5, OUTPUT);
  pinMode(PIN_BG6, OUTPUT);
  pinMode(PIN_BG7, OUTPUT);
  pinMode(PIN_BG8, OUTPUT);
  pinMode(PIN_BG9, OUTPUT);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);

  // Initial configuration:
  // both servos at 0 deg; all bargraph segments off.
  servo1.write(0);
  servo2.write(0);
  digitalWrite(PIN_BG0, LOW);
  digitalWrite(PIN_BG1, LOW);
  digitalWrite(PIN_BG2, LOW);
  digitalWrite(PIN_BG3, LOW);
  digitalWrite(PIN_BG4, LOW);
  digitalWrite(PIN_BG5, LOW);
  digitalWrite(PIN_BG6, LOW);
  digitalWrite(PIN_BG7, LOW);
  digitalWrite(PIN_BG8, LOW);
  digitalWrite(PIN_BG9, LOW);

  // Model starts with maintainMs=2000 at stateIndex=0
  // But model outputs for stateIndex==0 specify servo1=0, servo2=180 and LEDs off.
  // Apply model outputs immediately so controller reflects frozen model.
  applyOutputsForState(stateIndex);
}

void loop() {
  static uint32_t lastTick = 0;
  uint32_t now = millis();
  if (now - lastTick < CONTROL_PERIOD_MS) return;
  lastTick += CONTROL_PERIOD_MS;

  // Read io_0
  // active_high=false, pullup true: pressed -> D2 reads LOW -> io_0=true
  bool io_0 = (digitalRead(PIN_BTN1) == LOW);

  // ===== Perform model tick in the same order/semantics =====
  if (lockMs > 0) {
    // first start then assign lockMs := 0
    lockMs = 0;
  }

  if (lockMs > 0) {
    // then done
  } else {
    if (debounceStable) {
      // if ((prevPressed) and ((not debounceStable))) then ...
      // This is impossible here (debounceStable && not debounceStable), kept for fidelity.
      if (prevPressed && (!debounceStable)) {
        debounceStable = false;
      }
    }

    if (io_0) {
      if (debounceMs >= DEBOUNCE_THRESHOLD_MS) {
        debounceStable = true;
      } else {
        debounceMs = debounceMs + DEBOUNCE_MS_STEP;
      }
    } else {
      debounceMs = 0;
      debounceStable = false;
    }

    // if (debounceStable and io_0 and not prevPressed) then advance state
    if (debounceStable && io_0 && (!prevPressed)) {
      int nextState;
      if (stateIndex == 0) nextState = 1;
      else if (stateIndex == 1) nextState = 2;
      else nextState = 0;
      stateIndex = nextState;

      lockMs = LOCK_MS;
      maintainMs = MAINTAIN_MS_INIT;

      // Immediate outputs on valid transition
      applyOutputsForState(stateIndex);
    }
  }

  // maintainMs decrement logic:
  if (maintainMs > 0 && !(lockMs == 0)) {
    maintainMs = maintainMs - DEBOUNCE_MS_STEP;
  } else {
    if (maintainMs > 0) {
      maintainMs = maintainMs - DEBOUNCE_MS_STEP;
    }
  }

  // lock countdown
  if (lockMs > 0) {
    lockMs = lockMs - DEBOUNCE_MS_STEP;
    if ((lockMs - DEBOUNCE_MS_STEP <= 0) && (maintainMs > 0)) {
      lockMs = 0;
    }
  }

  // update prevPressed
  prevPressed = io_0;

  // No serial output
}
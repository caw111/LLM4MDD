#include <Servo.h>

Servo servo1;

// Wiring (preserved from circuit)
static const uint8_t LED1_PIN = 2;      // mega:2 -> led1:A (red)
static const uint8_t BTN1_PIN = 3;      // mega:3 -> btn1:1.l (green)
static const uint8_t BTN2_PIN = 4;      // mega:4 -> btn2:1.l (green)
static const uint8_t SERVO1_PIN = 5;    // mega:5 -> servo1:PWM (orange)

static const uint8_t LED_OFF = LOW;
static const uint8_t LED_ON  = HIGH;

static const int MIN_ANGLE = 0;
static const int MAX_ANGLE = 180;
static const int STEP_ANGLE = 30;

static const unsigned long DEBOUNCE_MS = 35;
static const unsigned long MIN_LED_ON_MS = 2000; // at least 2 seconds during movement

int currentAngle = 90;

bool moving = false;
unsigned long moveStartMs = 0;
int targetAngle = 90;
int lastReportedAngle = 90;

// Simple debounced button state tracking
bool readButtonPressed(uint8_t pin, unsigned long nowMs, bool &stableState, unsigned long &lastChangeMs, int &lastRaw) {
  int raw = digitalRead(pin);
  if (raw != lastRaw) {
    lastChangeMs = nowMs;
    lastRaw = raw;
  }
  if ((nowMs - lastChangeMs) >= DEBOUNCE_MS) {
    // With pullups, stableState is "pressed" when raw == LOW
    bool pressed = (raw == LOW);
    stableState = pressed;
  }
  return stableState;
}

void setServoSmooth(int fromAngle, int toAngle) {
  // Move smoothly in small steps; LED remains on at least 2 seconds during movement.
  int step = (toAngle > fromAngle) ? 1 : -1;

  // Movement starts now
  moving = true;
  moveStartMs = millis();
  digitalWrite(LED1_PIN, LED_ON);

  for (int a = fromAngle; a != toAngle; a += step) {
    servo1.write(a);
    delay(15); // smoothness control
  }
  servo1.write(toAngle);

  // Ensure LED remains on for at least 2 seconds after movement start
  unsigned long now = millis();
  unsigned long elapsed = now - moveStartMs;
  if (elapsed < MIN_LED_ON_MS) {
    delay(MIN_LED_ON_MS - elapsed);
  }

  digitalWrite(LED1_PIN, LED_OFF);
  moving = false;
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LED_OFF);

  // Buttons: circuit uses direct wiring; emulate typical button behavior with internal pull-ups.
  // btn1/btn2 are connected to ground through the "r" contact; press makes input LOW.
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  servo1.attach(SERVO1_PIN);
  currentAngle = 90;
  servo1.write(currentAngle);

  // Initialize debouncing vars
  // (No need to persist separately here; handled in loop)
}

void loop() {
  unsigned long nowMs = millis();

  static bool btn1StablePressed = false;
  static bool btn2StablePressed = false;
  static unsigned long btn1LastChange = 0;
  static unsigned long btn2LastChange = 0;
  static int btn1LastRaw = HIGH;
  static int btn2LastRaw = HIGH;

  // Read debounced states
  readButtonPressed(BTN1_PIN, nowMs, btn1StablePressed, btn1LastChange, btn1LastRaw);
  readButtonPressed(BTN2_PIN, nowMs, btn2StablePressed, btn2LastChange, btn2LastRaw);

  // Edge detection: trigger only once per press (pressed transition to true)
  static bool btn1PrevPressed = false;
  static bool btn2PrevPressed = false;

  bool btn1PressedNow = btn1StablePressed;
  bool btn2PressedNow = btn2StablePressed;

  bool btn1Rising = (btn1PressedNow && !btn1PrevPressed);
  bool btn2Rising = (btn2PressedNow && !btn2PrevPressed);

  btn1PrevPressed = btn1PressedNow;
  btn2PrevPressed = btn2PressedNow;

  if (moving) {
    // During movement we keep behavior strict: ignore new commands.
    return;
  }

  if (btn1Rising) {
    int newAngle = currentAngle + STEP_ANGLE;
    if (newAngle > MAX_ANGLE) newAngle = MAX_ANGLE;

    if (newAngle != currentAngle) {
      targetAngle = newAngle;
      // LED should turn on while servo is moving, then off once reached the new angle.
      setServoSmooth(currentAngle, targetAngle);
      currentAngle = targetAngle;
      lastReportedAngle = currentAngle;
    }
    // If already at max, do nothing and LED remains off.
  } else if (btn2Rising) {
    int newAngle = currentAngle - STEP_ANGLE;
    if (newAngle < MIN_ANGLE) newAngle = MIN_ANGLE;

    if (newAngle != currentAngle) {
      targetAngle = newAngle;
      setServoSmooth(currentAngle, targetAngle);
      currentAngle = targetAngle;
      lastReportedAngle = currentAngle;
    }
    // If already at min, do nothing and LED remains off.
  }

  (void)lastReportedAngle;
}
#include <Arduino.h>
#include <TM1637Display.h>

// -------- Pin assignments (per requirements / authoritative mapping) --------
static const uint8_t PIR_LEFT_PIN  = D2; // sensor at one end
static const uint8_t PIR_RIGHT_PIN = D3; // sensor at the other end

static const uint8_t LIGHT_PWM_PIN = D9; // PWM灯光
// given circuit says D6 was earlier in config, but user requirement explicitly says PWM 灯光 D9.
// We preserve the explicit requirement: use D9.
static const uint8_t ENV_LIGHT_A0_PIN = A0; // ambient light sensor analog input

static const uint8_t TM1637_CLK = D4;
static const uint8_t TM1637_DIO = D5;

TM1637Display display(TM1637_CLK, TM1637_DIO);

// -------- System parameters (behavior preserved / common sensible defaults) --------
// Note: The prompt doesn't provide numeric thresholds/timers; we implement a typical
// hallway lighting + energy management system with explicit, modifiable constants.
static const int AMBIENT_DARK_THRESHOLD = 400; // A0 < threshold => dark

// Base lighting timeout after last detected movement (ms)
static const unsigned long BASE_ON_TIME_MS = 8000UL;

// Extend lighting time when movement continues (ms per further trigger)
static const unsigned long EXTEND_ON_TIME_MS = 5000UL;

// Cap maximum continuous on-time extension (prevents indefinite growth from noisy sensors)
static const unsigned long MAX_ON_TIME_MS = 600000UL; // 10 minutes

// Debounce / repeat suppression for PIR edges
static const unsigned long PIR_DEBOUNCE_MS = 80UL;
static const unsigned long MIN_TRIGGER_INTERVAL_MS = 250UL;

// Direction estimation & count logic
// We infer passing direction by the order of sensor activation after dark condition.
// Count increments when a "crossing event" is recognized.
static const unsigned long PASS_EVENT_MAX_GAP_MS = 5000UL; // max time between end activations
static const uint8_t COUNT_CONFIRM_QUICK_STEPS = 1; // require at least one step in direction

// PWM output settings
static const uint8_t LIGHT_PWM_ON_VALUE = 200; // 0..255
static const uint8_t LIGHT_PWM_DIM_VALUE = 60; // optional: could be used for energy saving, but
                                                // we only define it; actual use below depends on logic.
static const bool USE_DIM_WHILE_LIGHT_ON = false; // if true, use dim mode before extension triggers

// -------- State variables --------
volatile unsigned long lastPIRLeftChangeMs = 0;
volatile unsigned long lastPIRRightChangeMs = 0;

bool pirLeftStable = false;
bool pirRightStable = false;

unsigned long lightOnStartMs = 0;
unsigned long lightOnEndDeadlineMs = 0; // when to turn off if no further extension
bool lightIsOn = false;

unsigned long lastTriggerAnyMs = 0;

// Direction / pass estimation
// We record which side was seen first within a time window.
enum Side : uint8_t { SIDE_NONE = 0, SIDE_LEFT = 1, SIDE_RIGHT = 2 };
Side pendingFirstSide = SIDE_NONE;
unsigned long pendingFirstSideMs = 0;

// Passing count (today)
uint16_t todayPassCount = 0;

// Energy/usage tracking
unsigned long accumulatedOnTimeMs = 0;

// For display "today" - no RTC provided; we treat power-on day as "today".
// You can reset by power cycle or via serial command if needed, but prompt doesn't ask.
// We'll just keep a per-boot "today" count.

// Serial
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// -------- Utility functions --------
static bool isDarkEnough() {
  int a = analogRead(ENV_LIGHT_A0_PIN);
  return (a < AMBIENT_DARK_THRESHOLD);
}

static void setLight(bool on) {
  if (on) {
    analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_ON_VALUE);
  } else {
    analogWrite(LIGHT_PWM_PIN, 0);
  }
}

static void resetPending() {
  pendingFirstSide = SIDE_NONE;
  pendingFirstSideMs = 0;
}

// Recognize passing based on order of PIR triggers.
// Strategy:
// - When dark, and a PIR trigger is detected, if we have no pendingFirstSide, set it.
// - If another PIR occurs within PASS_EVENT_MAX_GAP_MS, and it's opposite side, count as a pass.
// - Then reset pending.
static void handlePassingAndExtension(bool leftEvent, bool rightEvent, unsigned long nowMs) {
  if (leftEvent && rightEvent) {
    // Simultaneous: ambiguous; do not count, but extend.
    return;
  }

  Side eventSide = SIDE_NONE;
  if (leftEvent) eventSide = SIDE_LEFT;
  else if (rightEvent) eventSide = SIDE_RIGHT;
  else return;

  // Extend light on-time because movement continues.
  // If light is on: extend deadline; else turn on with base time (but only caller ensures dark).
  if (lightIsOn) {
    unsigned long newDeadline = lightOnEndDeadlineMs + EXTEND_ON_TIME_MS;

    // Clamp so total continuous on time doesn't exceed MAX_ON_TIME_MS from start.
    unsigned long maxDeadline = lightOnStartMs + MAX_ON_TIME_MS;
    if (newDeadline > maxDeadline) newDeadline = maxDeadline;

    lightOnEndDeadlineMs = newDeadline;
  } else {
    lightIsOn = true;
    lightOnStartMs = nowMs;
    lightOnEndDeadlineMs = nowMs + BASE_ON_TIME_MS;
    resetPending();
  }

  // Passing estimation
  if (pendingFirstSide == SIDE_NONE) {
    pendingFirstSide = eventSide;
    pendingFirstSideMs = nowMs;
  } else {
    // If pending expired, restart
    if (nowMs - pendingFirstSideMs > PASS_EVENT_MAX_GAP_MS) {
      pendingFirstSide = eventSide;
      pendingFirstSideMs = nowMs;
    } else {
      // If opposite side arrived, count
      if (pendingFirstSide != eventSide) {
        todayPassCount++;
        pendingFirstSide = SIDE_NONE; // completed one crossing event
      } else {
        // Same side again within window: keep as pending (wait for opposite)
      }
    }
  }

  // Track last trigger interval
  lastTriggerAnyMs = nowMs;
}

// Simple PIR stable read using debounce on edges.
// We read digital level; we update stable state only after debounce interval.
static bool readPIRStable(uint8_t pin, bool &stableState, volatile unsigned long &lastChangeMs, unsigned long nowMs) {
  bool raw = digitalRead(pin);
  if (raw != stableState) {
    // Potential change; wait for debounce
    if (nowMs - lastChangeMs >= PIR_DEBOUNCE_MS) {
      stableState = raw;
      lastChangeMs = nowMs;
      return true; // stable changed
    }
  }
  return false;
}

// -------- Setup / Loop --------
void setup() {
  // Pins
  pinMode(PIR_LEFT_PIN, INPUT);
  pinMode(PIR_RIGHT_PIN, INPUT);

  pinMode(LIGHT_PWM_PIN, OUTPUT);
  analogWrite(LIGHT_PWM_PIN, 0);

  // Initialize ADC
  analogRead(ENV_LIGHT_A0_PIN);

  // TM1637
  display.setBrightness(0x0f, true); // max brightness; library accepts 0..7 or 0..15 depending
  display.showNumberDecEx(todayPassCount, 0b01000000, true); // leading zeros disabled

  // Serial
  Serial.begin(9600);
  // idle framing is handled by our logic; no special config on AVR.

  // Initialize states
  unsigned long nowMs = millis();
  lastPIRLeftChangeMs = nowMs;
  lastPIRRightChangeMs = nowMs;
  pirLeftStable = digitalRead(PIR_LEFT_PIN);
  pirRightStable = digitalRead(PIR_RIGHT_PIN);

  lightIsOn = false;
  resetPending();

  accumulatedOnTimeMs = 0;
  lightOnStartMs = 0;
  lightOnEndDeadlineMs = 0;

  lastTriggerAnyMs = 0;

  // Initial display
  display.showNumberDecEx(todayPassCount, 0b00000000, true);
}

void loop() {
  const unsigned long nowMs = millis();

  // Non-blocking serial (optional: keep silent unless asked)
  // Spec only says "串口输出累计点亮时长" => output when light turns off (or regularly).
  // We'll output a line when turning off.
  // (We ignore incoming data.)

  // Check ambient light condition
  bool dark = isDarkEnough();

  // Read PIR stable states and detect new movement events (rising edges typical).
  // We'll interpret "movement" as PIR being HIGH; count events on stable transitions HIGH.
  static bool lastLeftHigh = false;
  static bool lastRightHigh = false;

  // Update stable state with debounce
  bool leftChanged = readPIRStable(PIR_LEFT_PIN, pirLeftStable, lastPIRLeftChangeMs, nowMs);
  bool rightChanged = readPIRStable(PIR_RIGHT_PIN, pirRightStable, lastPIRRightChangeMs, nowMs);

  // Detect "events": stable went from LOW->HIGH
  bool leftEvent = false;
  bool rightEvent = false;

  if (leftChanged) {
    bool leftHigh = pirLeftStable;
    if (leftHigh && !lastLeftHigh) leftEvent = true;
    lastLeftHigh = leftHigh;
  } else {
    // keep tracking lastHigh in case debounce suppresses transition updates
    lastLeftHigh = pirLeftStable;
  }

  if (rightChanged) {
    bool rightHigh = pirRightStable;
    if (rightHigh && !lastRightHigh) rightEvent = true;
    lastRightHigh = rightHigh;
  } else {
    lastRightHigh = pirRightStable;
  }

  // If it is dark and any end detects person, turn on / extend
  bool anyEvent = leftEvent || rightEvent;
  if (dark) {
    // Only process triggers that are not too frequent
    if (anyEvent && (nowMs - lastTriggerAnyMs >= MIN_TRIGGER_INTERVAL_MS)) {
      if (leftEvent || rightEvent) {
        // Handle passing estimation and lighting extension
        handlePassingAndExtension(leftEvent, rightEvent, nowMs);
      }
    }

    // If light already on, we may optionally dim based on current PIR activity
    if (lightIsOn) {
      if (USE_DIM_WHILE_LIGHT_ON) {
        bool movementStill = (pirLeftStable || pirRightStable);
        if (!movementStill) analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_DIM_VALUE);
        else analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_ON_VALUE);
      } else {
        // ensure ON PWM if not using dim mode
        analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_ON_VALUE);
      }
    }
  } else {
    // Not dark => requirement says only enable when environment dark AND any PIR detects person.
    // So we turn off immediately if not dark, regardless of PIR.
    if (lightIsOn) {
      // accumulate on-time
      if (nowMs > lightOnStartMs) accumulatedOnTimeMs += (nowMs - lightOnStartMs);

      // output accumulated on-time
      Serial.print("CUM_ON_MS=");
      Serial.println(accumulatedOnTimeMs);

      lightIsOn = false;
      setLight(false);
      resetPending();
    }
  }

  // Lighting auto-off if deadline passed (still considering dark condition):
  // If not dark, we already turned it off above.
  if (lightIsOn && nowMs >= lightOnEndDeadlineMs) {
    // accumulate on-time
    if (nowMs > lightOnStartMs) accumulatedOnTimeMs += (nowMs - lightOnStartMs);

    Serial.print("CUM_ON_MS=");
    Serial.println(accumulatedOnTimeMs);

    lightIsOn = false;
    setLight(false);
    resetPending();

    // Update display after possible count changes already handled at triggers
  }

  // Update TM1637 display when count might change.
  // To avoid excessive writes, update periodically or when count changes.
  static uint16_t lastShownCount = 0;
  if (todayPassCount != lastShownCount) {
    lastShownCount = todayPassCount;
    display.showNumberDecEx(todayPassCount, 0b00000000, true);
  }

  // If dark but no events, still keep loop non-blocking.
}
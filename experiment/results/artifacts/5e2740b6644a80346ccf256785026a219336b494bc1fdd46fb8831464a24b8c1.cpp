#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------- Pin assignments (use numeric UNO pin IDs; avoid D2/D3 macros) --------
static const uint8_t PIR_LEFT_PIN  = 2;  // D2
static const uint8_t PIR_RIGHT_PIN = 3;  // D3

static const uint8_t LIGHT_PWM_PIN = 9;  // D9 (PWM light)
static const uint8_t ENV_LIGHT_A0_PIN = A0;

static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

TM1637Display display(TM1637_CLK, TM1637_DIO);

// -------- System parameters (preserve behavior; thresholds/timers implemented) --------
static const int AMBIENT_DARK_THRESHOLD = 400; // A0 < threshold => dark

static const unsigned long BASE_ON_TIME_MS     = 8000UL;
static const unsigned long EXTEND_ON_TIME_MS   = 5000UL;
static const unsigned long MAX_ON_TIME_MS     = 600000UL; // 10 min cap

static const unsigned long PIR_DEBOUNCE_MS = 80UL;
static const unsigned long MIN_TRIGGER_INTERVAL_MS = 250UL;

enum Side : uint8_t { SIDE_NONE = 0, SIDE_LEFT = 1, SIDE_RIGHT = 2 };
static const unsigned long PASS_EVENT_MAX_GAP_MS = 5000UL;

static const uint8_t LIGHT_PWM_ON_VALUE  = 200; // 0..255
static const uint8_t LIGHT_PWM_DIM_VALUE = 60;  // optional
static const bool USE_DIM_WHILE_LIGHT_ON = false;

// -------- State variables --------
unsigned long lastPIRLeftChangeMs  = 0;
unsigned long lastPIRRightChangeMs = 0;

bool pirLeftStable = false;
bool pirRightStable = false;

unsigned long lightOnStartMs = 0;
unsigned long lightOnEndDeadlineMs = 0;
bool lightIsOn = false;

unsigned long lastTriggerAnyMs = 0;

// Passing estimation & count
Side pendingFirstSide = SIDE_NONE;
unsigned long pendingFirstSideMs = 0;

uint16_t todayPassCount = 0;

// Energy/usage tracking
unsigned long accumulatedOnTimeMs = 0;

// -------- Serial framing: 9600, line_or_idle, idle_timeout_ms=20 (handled by our output cadence) --------
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// -------- Utility functions --------
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isDarkEnough();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setLight(bool on);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void resetPending();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handlePassingAndExtension(bool leftEvent, bool rightEvent, unsigned long nowMs);
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readPIRStable(uint8_t pin, bool &stableState, unsigned long &lastChangeMs, unsigned long nowMs);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_026\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline bool isDarkEnough() {
  int a = analogRead(ENV_LIGHT_A0_PIN);
  return (a < AMBIENT_DARK_THRESHOLD);
}

static inline void setLight(bool on) {
  if (on) analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_ON_VALUE);
  else analogWrite(LIGHT_PWM_PIN, 0);
}

static inline void resetPending() {
  pendingFirstSide = SIDE_NONE;
  pendingFirstSideMs = 0;
}

static void handlePassingAndExtension(bool leftEvent, bool rightEvent, unsigned long nowMs) {
  if (leftEvent && rightEvent) {
    // Simultaneous: ambiguous; do not count, but extend light if already on
    if (lightIsOn) {
      unsigned long newDeadline = lightOnEndDeadlineMs + EXTEND_ON_TIME_MS;
      unsigned long maxDeadline = lightOnStartMs + MAX_ON_TIME_MS;
      if (newDeadline > maxDeadline) newDeadline = maxDeadline;
      lightOnEndDeadlineMs = newDeadline;
    } else {
      lightIsOn = true;
      lightOnStartMs = nowMs;
      lightOnEndDeadlineMs = nowMs + BASE_ON_TIME_MS;
      resetPending();
    }
    return;
  }

  Side eventSide = SIDE_NONE;
  if (leftEvent) eventSide = SIDE_LEFT;
  else if (rightEvent) eventSide = SIDE_RIGHT;
  else return;

  // Extend/Start lighting due to continuous movement
  if (lightIsOn) {
    unsigned long newDeadline = lightOnEndDeadlineMs + EXTEND_ON_TIME_MS;
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
    if (nowMs - pendingFirstSideMs > PASS_EVENT_MAX_GAP_MS) {
      pendingFirstSide = eventSide;
      pendingFirstSideMs = nowMs;
    } else {
      if (pendingFirstSide != eventSide) {
        todayPassCount++;
        pendingFirstSide = SIDE_NONE;
      }
    }
  }

  lastTriggerAnyMs = nowMs;
}

static bool readPIRStable(uint8_t pin, bool &stableState, unsigned long &lastChangeMs, unsigned long nowMs) {
  bool raw = digitalRead(pin);
  if (raw != stableState) {
    if (nowMs - lastChangeMs >= PIR_DEBOUNCE_MS) {
      stableState = raw;
      lastChangeMs = nowMs;
      return true;
    }
  }
  return false;
}

// -------- Setup / Loop --------
void setup() {
  pinMode(PIR_LEFT_PIN, INPUT);
  pinMode(PIR_RIGHT_PIN, INPUT);

  pinMode(LIGHT_PWM_PIN, OUTPUT);
  analogWrite(LIGHT_PWM_PIN, 0);

  analogRead(ENV_LIGHT_A0_PIN);

  display.setBrightness(0x0f, true);
  display.showNumberDecEx(todayPassCount, 0b00000000, true);

  Serial.begin(9600);

  unsigned long nowMs = millis();
  lastPIRLeftChangeMs = nowMs;
  lastPIRRightChangeMs = nowMs;

  pirLeftStable = (digitalRead(PIR_LEFT_PIN) != 0);
  pirRightStable = (digitalRead(PIR_RIGHT_PIN) != 0);

  lightIsOn = false;
  resetPending();

  accumulatedOnTimeMs = 0;
  lightOnStartMs = 0;
  lightOnEndDeadlineMs = 0;

  lastTriggerAnyMs = 0;
}

void loop() {
  const unsigned long nowMs = millis();

  bool dark = isDarkEnough();

  static bool lastLeftHigh = false;
  static bool lastRightHigh = false;

  bool leftChanged  = readPIRStable(PIR_LEFT_PIN, pirLeftStable, lastPIRLeftChangeMs, nowMs);
  bool rightChanged = readPIRStable(PIR_RIGHT_PIN, pirRightStable, lastPIRRightChangeMs, nowMs);

  bool leftEvent = false;
  bool rightEvent = false;

  if (leftChanged) {
    bool leftHigh = pirLeftStable;
    if (leftHigh && !lastLeftHigh) leftEvent = true;
    lastLeftHigh = leftHigh;
  } else {
    lastLeftHigh = pirLeftStable;
  }

  if (rightChanged) {
    bool rightHigh = pirRightStable;
    if (rightHigh && !lastRightHigh) rightEvent = true;
    lastRightHigh = rightHigh;
  } else {
    lastRightHigh = pirRightStable;
  }

  // Requirement: Only turn on when environment dark AND any PIR detects person.
  // If not dark: turn off immediately (and output accumulated on-time when turning off).
  if (!dark) {
    if (lightIsOn) {
      if (nowMs > lightOnStartMs) accumulatedOnTimeMs += (nowMs - lightOnStartMs);

      Serial.print("CUM_ON_MS=");
      Serial.println(accumulatedOnTimeMs);

      lightIsOn = false;
      setLight(false);
      resetPending();
    }
  } else {
    // Dark case: handle triggers and extension
    bool anyEvent = leftEvent || rightEvent;
    if (anyEvent && (nowMs - lastTriggerAnyMs >= MIN_TRIGGER_INTERVAL_MS)) {
      handlePassingAndExtension(leftEvent, rightEvent, nowMs);
    }

    // If light already on, manage PWM (optional dim mode)
    if (lightIsOn) {
      if (USE_DIM_WHILE_LIGHT_ON) {
        bool movementStill = (pirLeftStable || pirRightStable);
        analogWrite(LIGHT_PWM_PIN, movementStill ? LIGHT_PWM_ON_VALUE : LIGHT_PWM_DIM_VALUE);
      } else {
        analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_ON_VALUE);
      }
    }
  }

  // Auto-off after deadline (only meaningful when lightIsOn; dark required to turn on, but deadline enforcement runs regardless)
  // To preserve behavior, we'll keep this deadline even if dark remains; if it became not dark, we already turned off above.
  if (lightIsOn && nowMs >= lightOnEndDeadlineMs) {
    if (nowMs > lightOnStartMs) accumulatedOnTimeMs += (nowMs - lightOnStartMs);

    Serial.print("CUM_ON_MS=");
    Serial.println(accumulatedOnTimeMs);

    lightIsOn = false;
    setLight(false);
    resetPending();
  }

  // Update TM1637: display today's pass count
  static uint16_t lastShownCount = 0;
  if (todayPassCount != lastShownCount) {
    lastShownCount = todayPassCount;
    display.showNumberDecEx(todayPassCount, 0b00000000, true);
  }
}

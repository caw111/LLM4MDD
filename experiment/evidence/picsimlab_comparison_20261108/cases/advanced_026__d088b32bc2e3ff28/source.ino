#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Hardware pin mapping (authoritative) --------------------
static const uint8_t PIR_LEFT_PIN  = 2;   // D2
static const uint8_t PIR_RIGHT_PIN = 3;  // D3
static const uint8_t LIGHT_PWM_PIN = 9;   // D9
static const uint8_t ENV_LUX_PIN   = A0;  // A0
static const uint8_t TM_CLK_PIN    = 4;   // D4
static const uint8_t TM_DIO_PIN    = 5;   // D5

// TM1637 display
TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// -------------------- Serial configuration --------------------
static const uint32_t SERIAL_BAUD = 9600;

// -------------------- System behavior configuration --------------------

// Environment-light threshold (ADC). Higher => brighter requirement.
static const uint16_t DEFAULT_DARK_THRESHOLD = 450; // fallback if calibration fails
static const uint16_t CALIBRATION_SAMPLES = 200;

// Light PWM behavior
static const uint8_t  LIGHT_PWM_BRIGHTNESS = 180;   // 0..255
static const uint32_t LIGHT_BASE_ON_MS    = 5000;  // base time after movement (ms)
static const uint32_t LIGHT_EXTEND_ON_MS  = 3000;  // additional extension per new movement while moving (ms)
static const uint32_t LIGHT_MAX_ON_MS     = 60000; // max continuous on time (ms)

// Debounce / motion event qualification
static const uint32_t PIR_EVENT_MIN_GAP_MS = 80; // ignore too-fast toggles
static const uint32_t PIR_EVENT_HOLD_MS    = 80; // treat as event for this window

// Counting logic based on direction estimation:
// infer by which side triggers first, then the other follows within a window.
static const uint32_t DIRECTION_WINDOW_MS = 3000;

// -------------------- State --------------------
static uint16_t envDarkThreshold = DEFAULT_DARK_THRESHOLD;

static uint32_t lastLoopMs = 0;

// Pass counting (today)
static uint32_t passCount = 0;

// Lighting on/off tracking and on-time accumulation
static uint64_t cumulativeOnTimeMs = 0;
static bool lightIsOn = false;
static uint32_t lightOffDeadlineMs = 0;

// PIR event tracking timestamps (ms epoch)
static uint32_t pirLeftEventAt  = 0;
static uint32_t pirRightEventAt = 0;

// -------------------- Utilities --------------------
static inline bool isDark(uint16_t adc) {
  return adc < envDarkThreshold;
}

static inline bool qualifiesEvent(uint8_t /*pin*/, uint32_t &lastEventMs, uint32_t nowMs) {
  if (nowMs - lastEventMs < PIR_EVENT_MIN_GAP_MS) return false;
  lastEventMs = nowMs;
  return true;
}

static void updateDisplay() {
  // TM1637 displays 0-9999; show "today pass count"
  uint16_t shown = (uint16_t)(passCount % 10000UL);
  display.showNumberDecEx(shown, 0b01000000, true); // colon disabled; leading zeros off
}

static void turnLightOn(uint32_t nowMs) {
  if (!lightIsOn) {
    lightIsOn = true;
    lightOffDeadlineMs = nowMs + LIGHT_BASE_ON_MS;
    analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_BRIGHTNESS);
  } else {
    // Extend while movement continues
    uint32_t newDeadline = nowMs + LIGHT_EXTEND_ON_MS;
    if (newDeadline > lightOffDeadlineMs) lightOffDeadlineMs = newDeadline;

    // Enforce max continuous ON window by clamping deadline
    uint32_t maxDeadline = nowMs + LIGHT_MAX_ON_MS;
    if (lightOffDeadlineMs > maxDeadline) lightOffDeadlineMs = maxDeadline;

    analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_BRIGHTNESS);
  }
}

static void turnLightOff() {
  if (lightIsOn) {
    analogWrite(LIGHT_PWM_PIN, 0);
    lightIsOn = false;
  }
}

static void registerPassBySequence(uint32_t nowMs) {
  // Fix for diagnostics: use correct variable names and avoid undeclared identifiers.
  // We count when both sides have recent events and the latest event side is the one that just occurred,
  // then decide order by timestamps.

  bool leftRecent  = (pirLeftEventAt  != 0) && ((nowMs - pirLeftEventAt)  <= DIRECTION_WINDOW_MS);
  bool rightRecent = (pirRightEventAt != 0) && ((nowMs - pirRightEventAt) <= DIRECTION_WINDOW_MS);
  if (!leftRecent || !rightRecent) return;

  // Determine which side is earlier/later.
  bool leftIsLatest  = (pirLeftEventAt >= pirRightEventAt);
  bool rightIsLatest = (pirRightEventAt >= pirLeftEventAt);

  // Ensure the latest event is still "fresh now" to prevent repeated counting.
  bool leftLatestRecentNow  = (pirLeftEventAt  != 0) && ((nowMs - pirLeftEventAt)  <= PIR_EVENT_HOLD_MS);
  bool rightLatestRecentNow = (pirRightEventAt != 0) && ((nowMs - pirRightEventAt) <= PIR_EVENT_HOLD_MS);

  if (leftLatestRecentNow && rightRecent) {
    // right earlier, left latest => sequence right then left (count this passage)
    passCount++;
    pirLeftEventAt = 0;
    pirRightEventAt = 0;
  } else if (rightLatestRecentNow && leftRecent) {
    // left earlier, right latest => sequence left then right (count this passage)
    passCount++;
    pirLeftEventAt = 0;
    pirRightEventAt = 0;
  }
}

// -------------------- Setup/Loop --------------------
void setup() {
  pinMode(PIR_LEFT_PIN, INPUT);
  pinMode(PIR_RIGHT_PIN, INPUT);
  pinMode(LIGHT_PWM_PIN, OUTPUT);
  analogWrite(LIGHT_PWM_PIN, 0);

  Serial.begin(SERIAL_BAUD);

  display.setBrightness(0x0f);
  updateDisplay();

  // Calibration for environment threshold
  uint32_t calibSum = 0;
  uint16_t minVal = 1023;
  uint16_t maxVal = 0;

  for (uint16_t i = 0; i < CALIBRATION_SAMPLES; i++) {
    uint16_t v = analogRead(ENV_LUX_PIN);
    calibSum += v;
    if (v < minVal) minVal = v;
    if (v > maxVal) maxVal = v;
    delay(5);
  }

  uint16_t avg = (uint16_t)(calibSum / CALIBRATION_SAMPLES);

  uint16_t computed;
  if (maxVal > (uint16_t)(minVal + 20)) {
    uint16_t halfDrop = (uint16_t)((avg > minVal) ? ((avg - minVal) / 2) : 0);
    computed = (uint16_t)((avg > halfDrop) ? (avg - halfDrop) : minVal);
  } else {
    computed = avg;
  }

  if (computed < 50) computed = 50;
  if (computed > 900) computed = DEFAULT_DARK_THRESHOLD;

  envDarkThreshold = computed;

  // Initialize timestamps
  pirLeftEventAt = 0;
  pirRightEventAt = 0;
  lightOffDeadlineMs = 0;
  lastLoopMs = millis();
}

void loop() {
  uint32_t nowMs = millis();
  if (lastLoopMs == 0) lastLoopMs = nowMs;
  uint32_t dt = nowMs - lastLoopMs;
  lastLoopMs = nowMs;

  // Read environment light
  uint16_t envAdc = analogRead(ENV_LUX_PIN);
  bool dark = isDark(envAdc);

  // Read PIRs (digital HIGH indicates motion)
  bool leftMotion  = (digitalRead(PIR_LEFT_PIN) == HIGH);
  bool rightMotion = (digitalRead(PIR_RIGHT_PIN) == HIGH);

  // Accumulate on-time precisely while light is on
  if (lightIsOn) {
    cumulativeOnTimeMs += (uint64_t)dt;
  }

  // Motion events with qualification
  if (leftMotion) {
    static uint32_t lastLeftEventMs = 0;
    if (qualifiesEvent(PIR_LEFT_PIN, lastLeftEventMs, nowMs)) {
      pirLeftEventAt = nowMs;
    }
  }
  if (rightMotion) {
    static uint32_t lastRightEventMs = 0;
    if (qualifiesEvent(PIR_RIGHT_PIN, lastRightEventMs, nowMs)) {
      pirRightEventAt = nowMs;
    }
  }

  // Turn light on only if environment is dark and any PIR detects person
  bool anyMotion = leftMotion || rightMotion;
  if (dark && anyMotion) {
    turnLightOn(nowMs);
  }

  // Extend logic while ON is handled inside turnLightOn() via deadline updates

  // Turn off when deadline passed
  if (lightIsOn) {
    if ((int32_t)(nowMs - lightOffDeadlineMs) >= 0) {
      // Output cumulative on time over serial (fix ambiguous print by casting)
      uint64_t totalMs = cumulativeOnTimeMs;
      uint32_t totalSec = (uint32_t)(totalMs / 1000ULL);

      Serial.print(F("ON_TIME_MS="));
      Serial.print((unsigned long)(totalMs & 0xFFFFFFFFULL)); // safe for AVR cast; preserves behavior intent
      Serial.print(F(",ON_TIME_SEC="));
      Serial.println(totalSec);

      turnLightOff();
    }
  }

  // Pass counting based on direction estimation sequence
  if ((pirLeftEventAt != 0 && (nowMs - pirLeftEventAt) <= PIR_EVENT_HOLD_MS) ||
      (pirRightEventAt != 0 && (nowMs - pirRightEventAt) <= PIR_EVENT_HOLD_MS)) {
    if (pirLeftEventAt != 0 && pirRightEventAt != 0) {
      registerPassBySequence(nowMs);
    }
  }

  // Update display when count changes
  static uint32_t lastDisplayed = 0;
  if (passCount != lastDisplayed) {
    lastDisplayed = passCount;
    updateDisplay();
  }

  delay(5);
}
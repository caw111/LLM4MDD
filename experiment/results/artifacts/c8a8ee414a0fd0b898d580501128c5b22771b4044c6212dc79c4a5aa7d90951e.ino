#include <Arduino.h>
#include <TM1637Display.h>

// Pins per requirement
static const uint8_t TRIG_PIN = D8; // HC-SR04 TRIG
static const uint8_t ECHO_PIN = D9; // HC-SR04 ECHO
static const uint8_t TM1637_CLK = D4; // TM1637 CLK
static const uint8_t TM1637_DIO = D5; // TM1637 DIO

// Display: integer centimeters, no fixed requirement on formatting beyond "前导零或空白均可".
// We'll use no leading zeros; when invalid, show blanks.
TM1637Display display(TM1637_CLK, TM1637_DIO);

// Timing/behavior requirements
static const uint32_t ECHO_TIMEOUT_US = 25000UL; // per "每次等待回波必须有25000us超时"
static const uint32_t SAMPLE_INTERVAL_MIN_MS = 60UL;   // "采样开始间隔60..160ms"
static const uint32_t SAMPLE_INTERVAL_MAX_MS = 160UL;
static const uint32_t DISPLAY_UPDATE_WITHIN_MS = 250UL; // "变化250ms内反映"
static const uint32_t INVALID_DISPLAY_BLANK_US = 0; // (implicit)

static inline void setBlankDisplay() {
  // Using 4 digits; no leading zeros.
  // "无回波、卡高或越界时显示空白，恢复后继续显示真实距离。"
  // TM1637 library supports clear/blank.
  display.clear();
}

static inline uint32_t nextSampleDelayMs() {
  // Non-blocking randomness: use micros() LSB to avoid needing RNG.
  uint32_t t = micros();
  uint32_t span = (SAMPLE_INTERVAL_MAX_MS - SAMPLE_INTERVAL_MIN_MS) + 1UL;
  uint32_t r = (t % span);
  return SAMPLE_INTERVAL_MIN_MS + r;
}

static inline uint16_t quantizeCmFromEchoUs(uint32_t echo_us, bool &valid) {
  // Distance in cm: echo microseconds / 58
  // Effective range valid: 2..400 cm.
  // Quantization tolerance 1 cm: integer cm with truncation is fine.
  // IMPORTANT: must distinguish invalid (timeout/high/overflow) from 0.
  valid = false;

  // Avoid overflow and invalid echo ranges
  // For range check: compute cm using truncation.
  uint32_t cm = echo_us / 58UL;

  if (cm >= 2UL && cm <= 400UL) {
    valid = true;
    return (uint16_t)cm;
  }
  return 0;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Ensure trigger low
  digitalWrite(TRIG_PIN, LOW);

  // Initialize display: start with blank as required.
  display.setBrightness(0x0f);
  setBlankDisplay();

  // Prime timing: next measurement should start after an interval within 60..160ms.
  // Use deterministic but varying based on current time.
}

void loop() {
  static bool initialized = false;

  // Scheduling state
  static uint32_t nextStartMs = 0;
  static uint32_t lastUpdateMs = 0;

  // Measurement state machine to ensure:
  // - each echo wait uses 25000us timeout
  // - conversion + display update happen fast enough to be within 250ms
  static bool measuring = false;

  if (!initialized) {
    initialized = true;
    nextStartMs = millis() + nextSampleDelayMs();
    measuring = false;
    lastUpdateMs = millis();
    // Requirement: "初始显示空白" already done in setup().
  }

  uint32_t nowMs = millis();

  if (!measuring) {
    if ((int32_t)(nowMs - nextStartMs) >= 0) {
      measuring = true;

      // Perform one measurement immediately; this block should complete quickly.
      // TRIG pulse: typical HC-SR04 is 10us HIGH.
      // Then wait for ECHO rising and falling. We'll measure pulse width in microseconds
      // with a hard timeout for the echo measurement.
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);

      // Measure echo pulse width. Use timeout 25000us as required.
      // pulseIn(..., HIGH) waits for HIGH pulse and returns duration in microseconds.
      // If no pulse or timeout -> 0.
      // If ECHO is stuck HIGH, pulseIn will time out -> 0.
      uint32_t echo_us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

      bool valid = false;
      uint16_t cm = 0;

      if (echo_us == 0) {
        // No echo or stuck high or timeout -> blank
        valid = false;
      } else {
        // Out-of-bounds (<=1 or >400 cm) should blank.
        cm = quantizeCmFromEchoUs(echo_us, valid);
      }

      // Display update (must reflect within 250ms of changes;
      // this path is fast and we schedule next starts 60..160ms apart)
      if (valid) {
        // Display integer centimeters
        // TM1637Display::showNumberDec can be configured to include/omit leading zeros.
        // We'll omit leading zeros by using leadingZero=false (library: last parameter).
        display.showNumberDec((int)cm, false); // no leading zeros
      } else {
        setBlankDisplay();
      }

      // Next cycle
      lastUpdateMs = nowMs;
      nextStartMs = nowMs + nextSampleDelayMs();
      measuring = false;
    }
  }

  // No delay here: keep loop responsive.
  // Ensure we don't violate 250ms responsiveness: measurement happens only once per 60..160ms window.
}
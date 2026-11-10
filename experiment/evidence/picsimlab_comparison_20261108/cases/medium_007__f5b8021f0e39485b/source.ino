#include <Arduino.h>
#include <TM1637Display.h>

// --- Pin assignments (Uno) ---
// HC-SR04: TRIG=D8, ECHO=D9
static const uint8_t TRIG_PIN = 8;  // D8
static const uint8_t ECHO_PIN = 9;  // D9

// TM1637: CLK=D4, DIO=D5
static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

TM1637Display display(TM1637_CLK, TM1637_DIO);

// Requirements
static const uint32_t ECHO_TIMEOUT_US = 25000UL;    // per measurement: 25000us timeout
static const uint32_t SAMPLE_INTERVAL_MIN_MS = 60UL;
static const uint32_t SAMPLE_INTERVAL_MAX_MS = 160UL;
static const uint32_t DISPLAY_UPDATE_WITHIN_MS = 250UL; // reflected within 250ms after change (satisfied by interval)

// Display behavior
static inline void setBlankDisplay() {
  // Display blank/cleared. Leading zeros not required; for invalid show blank.
  display.clear();
}

// Deterministic jitter in 60..160ms (no RNG dependency)
static inline uint32_t nextSampleDelayMs() {
  uint32_t t = micros();
  uint32_t span = (SAMPLE_INTERVAL_MAX_MS - SAMPLE_INTERVAL_MIN_MS) + 1UL;
  uint32_t r = (t % span);
  return SAMPLE_INTERVAL_MIN_MS + r;
}

// Convert echo microseconds to integer centimeters:
// distance_cm = echo_us / 58
// Valid range: 2..400 cm inclusive.
// Quantization tolerance 1cm: integer truncation is OK.
// Must distinguish invalid from 0: invalid => blank.
static inline uint16_t quantizeCmFromEchoUs(uint32_t echo_us, bool &valid) {
  valid = false;

  // If echo_us == 0, it was timeout/no pulse/high-stuck => invalid (blank)
  if (echo_us == 0) return 0;

  uint32_t cm = echo_us / 58UL; // truncation => quantization

  if (cm >= 2UL && cm <= 400UL) {
    valid = true;
    return (uint16_t)cm;
  }
  return 0;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  display.setBrightness(0x0f);
  setBlankDisplay(); // Initial display blank

  // Scheduler priming
}

void loop() {
  static bool initialized = false;

  static uint32_t nextStartMs = 0;
  static uint32_t lastUpdateMs = 0;

  // Ensure only one measurement per interval
  static bool measuring = false;

  if (!initialized) {
    initialized = true;
    nextStartMs = millis() + nextSampleDelayMs();
    lastUpdateMs = millis();
    measuring = false;
  }

  uint32_t nowMs = millis();

  if (!measuring) {
    if ((int32_t)(nowMs - nextStartMs) >= 0) {
      measuring = true;

      // Trigger HC-SR04: typical 10us HIGH pulse
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);

      // Wait for echo pulse width with strict 25000us timeout per requirement.
      // pulseIn returns 0 on timeout/no pulse (also covers stuck-high timeout).
      uint32_t echo_us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

      bool valid = false;
      uint16_t cm = quantizeCmFromEchoUs(echo_us, valid);

      if (valid) {
        // Display integer centimeters, no leading zeros required.
        display.showNumberDec((int)cm, false);
      } else {
        // No echo, high stuck, timeout, or out-of-bounds => blank
        setBlankDisplay();
      }

      lastUpdateMs = nowMs;
      nextStartMs = nowMs + nextSampleDelayMs();

      measuring = false;
    }
  }

  // Non-blocking loop: all time-consuming behavior is contained in the scheduled measurement.
  // The sampling interval is 60..160ms, with display update well within 250ms.
}
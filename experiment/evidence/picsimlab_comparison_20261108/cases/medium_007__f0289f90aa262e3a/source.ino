#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments for Arduino Uno (use numeric pins since D8/D9/D4/D5 macros may not exist)
static const uint8_t TRIG_PIN = 8; // HC-SR04 TRIG
static const uint8_t ECHO_PIN = 9; // HC-SR04 ECHO

static const uint8_t TM_CLK_PIN = 4; // TM1637 CLK
static const uint8_t TM_DIO_PIN = 5; // TM1637 DIO

TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// Timing requirements
static const uint32_t ECHO_TIMEOUT_US = 25000UL;     // per measurement: must have 25000us timeout
static const uint32_t MIN_START_GAP_MS = 60UL;       // sample start interval 60..160ms
static const uint32_t MAX_START_GAP_MS = 160UL;

// Valid range: 2..400 cm, display integer centimeters
static const int16_t VALID_CM_MIN = 2;
static const int16_t VALID_CM_MAX = 400;

// Scheduling
static uint32_t nextStartMs = 0;

// Display state: must distinguish measurement validity from "0 cm"
static bool hasValid = false;
static int16_t lastCm = 0;

static inline int16_t echoUsToCm(uint32_t echo_us) {
  // Required: distance_cm = echo_us / 58 (integer quantization)
  return (int16_t)(echo_us / 58UL);
}

static void updateDisplay() {
  if (!hasValid) {
    // Blank display for invalid (no echo/timeout/out-of-range)
    display.clear();
    return;
  }

  // Show integer cm, no leading zeros required; behavior allows no leading zeros.
  // 4 digits enough for 400.
  display.showNumberDec((int)lastCm, false, 4, 0);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  pinMode(ECHO_PIN, INPUT);

  display.setBrightness(0x0F);

  // Initial display: blank
  hasValid = false;
  lastCm = 0;
  display.clear();

  // Initial scheduling: first measurement soon after start within 60..160ms envelope
  nextStartMs = millis() + MIN_START_GAP_MS;
}

void loop() {
  uint32_t nowMs = millis();

  // Enforce sample start interval between 60..160ms
  if ((int32_t)(nowMs - nextStartMs) < 0) {
    return;
  }

  // Choose the next interval within allowed range (deterministic-ish, no external RNG)
  // This preserves responsiveness; change must reflect within 250ms (including measurement wait and display transfer).
  uint32_t intervalMs = MIN_START_GAP_MS;
  intervalMs += (uint32_t)(micros() & 0x7F); // 0..127
  if (intervalMs > MAX_START_GAP_MS) intervalMs = MAX_START_GAP_MS;
  nextStartMs = nowMs + intervalMs;

  // Trigger HC-SR04: 10us HIGH pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width with required timeout (25000us)
  // pulseIn() returns microseconds duration of HIGH pulse, or 0 on timeout/no echo.
  unsigned long echo_us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  // Validity:
  // - Must not treat timeout as 0cm: if echo_us == 0 => invalid => blank
  // - Convert to cm by dividing by 58
  // - Only accept 2..400 cm inclusive
  if (echo_us == 0UL) {
    hasValid = false;
  } else {
    int16_t cm = echoUsToCm(echo_us);
    if (cm < VALID_CM_MIN || cm > VALID_CM_MAX) {
      hasValid = false;
    } else {
      hasValid = true;
      lastCm = cm;
    }
  }

  // Update display after each measurement attempt
  updateDisplay();
}
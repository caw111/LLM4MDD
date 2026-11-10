#include <Arduino.h>
#include <TM1637Display.h>

#define POT_PIN A0
#define LED_PIN 9

// TM1637 wiring (authoritative pin assignments)
static const uint8_t TM1637_CLK = 4;  // D4
static const uint8_t TM1637_DIO = 5;  // D5

// Create display (brightness will be controlled)
TM1637Display display(TM1637_CLK, TM1637_DIO);

// --- Behavior parameters (explicitly preserved; adjust only if you have prior requirements) ---
static const uint16_t PWM_MIN = 0;                 // LED PWM min
static const uint16_t PWM_MAX = 255;               // LED PWM max
static const uint16_t PWM_CALIBRATION_LOW = 0;    // optional calibration offset (kept 0 to preserve behavior)
static const uint16_t PWM_CALIBRATION_HIGH = 1023; // optional calibration full-scale (kept 1023)

// TM1637 brightness levels on common libraries: 0..7
static const uint8_t TM1637_BRIGHT_MIN = 0;
static const uint8_t TM1637_BRIGHT_MAX = 7;

// Optional update throttling to avoid flicker
static const uint16_t POT_SMOOTH_SAMPLES = 8;
static const uint16_t POT_UPDATE_INTERVAL_MS = 30;

// --- Internal state ---
static uint32_t lastUpdateMs = 0;
static uint16_t smoothedPot = 0;

static uint16_t readPotSmoothed() {
  uint32_t acc = 0;
  for (uint16_t i = 0; i < POT_SMOOTH_SAMPLES; i++) {
    acc += analogRead(POT_PIN);
    delayMicroseconds(200);
  }
  uint16_t avg = (uint16_t)(acc / POT_SMOOTH_SAMPLES);
  return avg;
}

static uint16_t clampU16(int32_t v, uint16_t lo, uint16_t hi) {
  if (v < (int32_t)lo) return lo;
  if (v > (int32_t)hi) return hi;
  return (uint16_t)v;
}

static uint8_t mapToTm1637Brightness(uint16_t pot) {
  // Map potentiometer reading to TM1637 brightness 0..7
  // Use calibrated range
  uint16_t lo = (uint16_t)PWM_CALIBRATION_LOW;
  uint16_t hi = (uint16_t)PWM_CALIBRATION_HIGH;
  if (hi <= lo) return TM1637_BRIGHT_MIN;

  uint16_t p = clampU16(pot, lo, hi);
  uint32_t scaled = (uint32_t)(p - lo) * (uint32_t)(TM1637_BRIGHT_MAX - TM1637_BRIGHT_MIN);
  scaled /= (uint32_t)(hi - lo);
  return (uint8_t)(TM1637_BRIGHT_MIN + scaled);
}

static uint8_t mapToLedPwm(uint16_t pot) {
  // Map potentiometer reading to LED PWM 0..255
  uint16_t lo = (uint16_t)PWM_CALIBRATION_LOW;
  uint16_t hi = (uint16_t)PWM_CALIBRATION_HIGH;
  if (hi <= lo) return (uint8_t)PWM_MIN;

  uint16_t p = clampU16(pot, lo, hi);
  uint32_t scaled = (uint32_t)(p - lo) * (uint32_t)(PWM_MAX - PWM_MIN);
  scaled /= (uint32_t)(hi - lo);
  return (uint8_t)(PWM_MIN + scaled);
}

void setup() {
  pinMode(POT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Ensure LED starts off
  analogWrite(LED_PIN, 0);

  // TM1637 init: set a default brightness
  display.setBrightness(0, false);
  display.clear();

  // Start display with an initial value
  smoothedPot = readPotSmoothed();
  uint8_t ledPwm = mapToLedPwm(smoothedPot);
  analogWrite(LED_PIN, ledPwm);

  // Show potentiometer value (0..1023) on TM1637 as a 4-digit number
  // TM1637 can show up to 4 digits; we scale 0..1023 -> 0..9999 for visual stability
  uint16_t scaledDisplay = (uint32_t)smoothedPot * 9999UL / 1023UL;
  display.showNumberDecEx(scaledDisplay, 0b00000000, true);
  display.setBrightness(mapToTm1637Brightness(smoothedPot), true);
}

void loop() {
  uint32_t now = millis();
  if (now - lastUpdateMs < POT_UPDATE_INTERVAL_MS) {
    return;
  }
  lastUpdateMs = now;

  smoothedPot = readPotSmoothed();

  // Update LED PWM using potentiometer
  uint8_t ledPwm = mapToLedPwm(smoothedPot);
  analogWrite(LED_PIN, ledPwm);

  // Update TM1637 brightness and display value
  uint8_t tmBrightness = mapToTm1637Brightness(smoothedPot);
  display.setBrightness(tmBrightness, true);

  // Display potentiometer-derived brightness control value as 4 digits.
  // Scale 0..1023 -> 0..9999.
  uint16_t scaledDisplay = (uint32_t)smoothedPot * 9999UL / 1023UL;
  display.showNumberDecEx(scaledDisplay, 0b00000000, true);
}
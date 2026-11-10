#include <Arduino.h>

static const uint8_t PWM_PIN = 5; // D5

// Requirements:
// - Initial brightness: 0
// - Linear triangular wave
// - 1000ms fade in 0->255, then 1000ms fade out 255->0
// - Full cycle: 2000ms, continuous
// - Brightness tolerance: 5 PWM counts
// - Update interval <= 20ms
//
// Using 20ms update interval and exact 0..1000ms mapping with integer math.
static const uint32_t CYCLE_MS = 2000;
static const uint32_t HALF_MS  = 1000;
static const uint32_t UPDATE_MS = 20;

static uint32_t lastUpdateMs = 0;

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  analogWrite(PWM_PIN, 0);  // Initial brightness 0
  lastUpdateMs = millis();
}

void loop() {
  uint32_t now = millis();

  // Ensure update interval is not more than 20ms
  if ((uint32_t)(now - lastUpdateMs) < UPDATE_MS) {
    return;
  }
  lastUpdateMs = now;

  uint32_t t = now % CYCLE_MS; // 0..1999
  uint8_t brightness = 0;

  if (t < HALF_MS) {
    // Fade in: 0..1000ms -> 0..255 (linear)
    // Map t in [0,999] to [0,255] with integer math.
    // For t=0 => 0, for t=999 => ~255 within tolerance.
    brightness = (uint8_t)((t * 255UL) / (HALF_MS - 1)); // 1000-1=999
  } else {
    // Fade out: 1000..1999ms -> 255..0 (linear)
    uint32_t td = t - HALF_MS; // 0..999
    brightness = (uint8_t)((255UL * (HALF_MS - 1 - td)) / (HALF_MS - 1));
    // t=1000 => ~255, t=1999 => 0 within tolerance.
  }

  analogWrite(PWM_PIN, brightness);
}
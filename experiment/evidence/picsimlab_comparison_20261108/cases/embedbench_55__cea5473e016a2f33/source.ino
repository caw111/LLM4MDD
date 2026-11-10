#include <Arduino.h>

// Pins per frozen hardware contract / circuit
// RGB1 PWM: D2, D3, D4 (red, green, blue)
static const uint8_t RGB1_RED_PIN   = 2;
static const uint8_t RGB1_GREEN_PIN = 3;
static const uint8_t RGB1_BLUE_PIN  = 4;

// RGB2 PWM: D5, D6, D7 (red, green, blue)
static const uint8_t RGB2_RED_PIN   = 5;
static const uint8_t RGB2_GREEN_PIN = 6;
static const uint8_t RGB2_BLUE_PIN  = 7;

// Bargraph segments (active HIGH): D8..D13, D44, D45 (A1..A8)
static const uint8_t BAR_SEG_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45};

// Potentiometers analog inputs
static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;

// Control period chosen as 10 ms
static const uint32_t CONTROL_PERIOD_MS = 10;

static inline float readPotF(uint8_t pin) {
  // Model expects 0..1023 as f64 equivalent.
  // Use integer ADC value, cast to float.
  return (float)analogRead(pin);
}

static inline float clamp01ToPWM(float x) {
  // Model outputs are exactly 0 or 255 for PWM channels.
  // This guard keeps PWM sane if any numerical drift occurs.
  if (x <= 0.0f) return 0.0f;
  if (x >= 255.0f) return 255.0f;
  return x;
}

static void applyRGB1FromPot(float pot) {
  // io_0..io_2 represent rgb1 red/green/blue PWM counts
  float pwmRed = 0.0f;
  float pwmGreen = 0.0f;
  float pwmBlue = 0.0f;

  if (pot <= 340.0f) {
    pwmRed = 255.0f;
  } else if (pot <= 681.0f) { // (pot > 340) && (pot <= 681)
    pwmGreen = 255.0f;
  } else { // pot >= 682
    pwmBlue = 255.0f;
  }

  analogWrite(RGB1_RED_PIN, (int)clamp01ToPWM(pwmRed));
  analogWrite(RGB1_GREEN_PIN, (int)clamp01ToPWM(pwmGreen));
  analogWrite(RGB1_BLUE_PIN, (int)clamp01ToPWM(pwmBlue));
}

static void applyRGB2FromPot(float pot) {
  // io_3..io_5 represent rgb2 red/green/blue PWM counts
  float pwmRed = 0.0f;
  float pwmGreen = 0.0f;
  float pwmBlue = 0.0f;

  if (pot <= 340.0f) {
    pwmRed = 255.0f;
  } else if (pot <= 681.0f) { // (pot > 340) && (pot <= 681)
    pwmGreen = 255.0f;
  } else { // pot >= 682
    pwmBlue = 255.0f;
  }

  analogWrite(RGB2_RED_PIN, (int)clamp01ToPWM(pwmRed));
  analogWrite(RGB2_GREEN_PIN, (int)clamp01ToPWM(pwmGreen));
  analogWrite(RGB2_BLUE_PIN, (int)clamp01ToPWM(pwmBlue));
}

static void applyBargraphFromPots(float pot1, float pot2) {
  // Average mapped to 0..8 using the exact frozen boolean thresholds:
  // segment_0 true if avg>=127.875, segment_1 avg>=255.75, ..., segment_7 avg>=1023.0
  float avg = (pot1 + pot2) / 2.0f;

  const float thresholds[8] = {
    127.875f, 255.75f, 383.625f, 511.5f,
    639.375f, 767.25f, 895.125f, 1023.0f
  };

  for (int i = 0; i < 8; i++) {
    bool on = (avg >= thresholds[i]);
    digitalWrite(BAR_SEG_PINS[i], on ? HIGH : LOW);
  }
}

void setup() {
  // Ensure all components start OFF (model semantics initial=false for all boolean segments;
  // contract initial 0 for all PWM outputs).
  pinMode(BAR_SEG_PINS[0], OUTPUT);
  pinMode(BAR_SEG_PINS[1], OUTPUT);
  pinMode(BAR_SEG_PINS[2], OUTPUT);
  pinMode(BAR_SEG_PINS[3], OUTPUT);
  pinMode(BAR_SEG_PINS[4], OUTPUT);
  pinMode(BAR_SEG_PINS[5], OUTPUT);
  pinMode(BAR_SEG_PINS[6], OUTPUT);
  pinMode(BAR_SEG_PINS[7], OUTPUT);

  for (int i = 0; i < 8; i++) digitalWrite(BAR_SEG_PINS[i], LOW);

  // RGB pins for PWM
  pinMode(RGB1_RED_PIN, OUTPUT);
  pinMode(RGB1_GREEN_PIN, OUTPUT);
  pinMode(RGB1_BLUE_PIN, OUTPUT);

  pinMode(RGB2_RED_PIN, OUTPUT);
  pinMode(RGB2_GREEN_PIN, OUTPUT);
  pinMode(RGB2_BLUE_PIN, OUTPUT);

  analogWrite(RGB1_RED_PIN, 0);
  analogWrite(RGB1_GREEN_PIN, 0);
  analogWrite(RGB1_BLUE_PIN, 0);

  analogWrite(RGB2_RED_PIN, 0);
  analogWrite(RGB2_GREEN_PIN, 0);
  analogWrite(RGB2_BLUE_PIN, 0);

  // ADC pins
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  // Start control loop immediately; no additional delay to preserve "immediate" updates.
}

void loop() {
  static uint32_t lastTick = 0;

  uint32_t now = millis();
  if ((uint32_t)(now - lastTick) < CONTROL_PERIOD_MS) {
    return;
  }
  lastTick = now;

  // Read inputs as f64-equivalent floats in 0..1023
  float pot1 = readPotF(POT1_PIN);
  float pot2 = readPotF(POT2_PIN);

  // Apply controller outputs exactly as per frozen model logic.
  applyRGB1FromPot(pot1);
  applyRGB2FromPot(pot2);
  applyBargraphFromPots(pot1, pot2);
}
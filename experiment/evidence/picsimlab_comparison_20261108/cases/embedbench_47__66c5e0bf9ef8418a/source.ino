#include <Arduino.h>

static float clamp01(float x) {
  if (x < 0.0f) return 0.0f;
  if (x > 1.0f) return 1.0f;
  return x;
}

// Convert HSV (H in degrees [0..360), S [0..1], V [0..1]) to RGB in [0..1]
static void hsvToRgb(float hDeg, float s, float v, float &r, float &g, float &b) {
  hDeg = fmodf(hDeg, 360.0f);
  if (hDeg < 0.0f) hDeg += 360.0f;

  s = clamp01(s);
  v = clamp01(v);

  float c = v * s;                 // chroma
  float hPrime = hDeg / 60.0f;     // sector 0..6
  float x = c * (1.0f - fabsf(fmodf(hPrime, 2.0f) - 1.0f));

  float r1 = 0, g1 = 0, b1 = 0;
  if (hPrime >= 0 && hPrime < 1)      { r1 = c; g1 = x; b1 = 0; }
  else if (hPrime >= 1 && hPrime < 2) { r1 = x; g1 = c; b1 = 0; }
  else if (hPrime >= 2 && hPrime < 3) { r1 = 0; g1 = c; b1 = x; }
  else if (hPrime >= 3 && hPrime < 4) { r1 = 0; g1 = x; b1 = c; }
  else if (hPrime >= 4 && hPrime < 5) { r1 = x; g1 = 0; b1 = c; }
  else                                 { r1 = c; g1 = 0; b1 = x; }

  float m = v - c;
  r = r1 + m;
  g = g1 + m;
  b = b1 + m;
}

// Linear map from [inMin..inMax] to [outMin..outMax]
static float mapFloat(long x, long inMin, long inMax, float outMin, float outMax) {
  return outMin + (float)(x - inMin) * (outMax - outMin) / (float)(inMax - inMin);
}

// RGB pins (common cathode => higher PWM value means brighter LED)
constexpr uint8_t RGB1_R = 2;
constexpr uint8_t RGB1_G = 3;
constexpr uint8_t RGB1_B = 4;

constexpr uint8_t RGB2_R = 5;
constexpr uint8_t RGB2_G = 6;
constexpr uint8_t RGB2_B = 7;

constexpr uint8_t POT1_PIN = A3;
constexpr uint8_t POT2_PIN = A4;

static int smoothTo(int current, int target, float alpha) {
  // alpha in (0..1); higher = faster response
  float next = current + alpha * (target - current);
  if (next < 0) next = 0;
  if (next > 255) next = 255;
  return (int)(next + 0.5f);
}

void setup() {
  // Initialize to OFF on power/reset
  pinMode(RGB1_R, OUTPUT); pinMode(RGB1_G, OUTPUT); pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT); pinMode(RGB2_G, OUTPUT); pinMode(RGB2_B, OUTPUT);

  analogWrite(RGB1_R, 0);
  analogWrite(RGB1_G, 0);
  analogWrite(RGB1_B, 0);

  analogWrite(RGB2_R, 0);
  analogWrite(RGB2_G, 0);
  analogWrite(RGB2_B, 0);

  // Optionally set analog reference to default (5V) as per wiring
  analogReference(DEFAULT);

  // Start Serial only if desired; not required by spec
  // Serial.begin(115200);
}

void loop() {
  // Read potentiometers (0..1023)
  int pot1 = analogRead(POT1_PIN);
  int pot2 = analogRead(POT2_PIN);

  // Map to hue degrees 0..360
  float hue1 = mapFloat(pot1, 0, 1023, 0.0f, 360.0f);
  float hue2 = mapFloat(pot2, 0, 1023, 0.0f, 360.0f);

  // Saturation and Value are maximum (1)
  const float s = 1.0f;
  const float v = 1.0f;

  float r1f, g1f, b1f;
  float r2f, g2f, b2f;

  hsvToRgb(hue1, s, v, r1f, g1f, b1f);
  hsvToRgb(hue2, s, v, r2f, g2f, b2f);

  // Convert to PWM brightness (0..255)
  int targetR1 = (int)(clamp01(r1f) * 255.0f + 0.5f);
  int targetG1 = (int)(clamp01(g1f) * 255.0f + 0.5f);
  int targetB1 = (int)(clamp01(b1f) * 255.0f + 0.5f);

  int targetR2 = (int)(clamp01(r2f) * 255.0f + 0.5f);
  int targetG2 = (int)(clamp01(g2f) * 255.0f + 0.5f);
  int targetB2 = (int)(clamp01(b2f) * 255.0f + 0.5f);

  // Smooth transition: low-pass filter on PWM values
  static int currR1 = 0, currG1 = 0, currB1 = 0;
  static int currR2 = 0, currG2 = 0, currB2 = 0;

  // Alpha chosen for continuous smooth updates
  const float alpha = 0.15f;

  currR1 = smoothTo(currR1, targetR1, alpha);
  currG1 = smoothTo(currG1, targetG1, alpha);
  currB1 = smoothTo(currB1, targetB1, alpha);

  currR2 = smoothTo(currR2, targetR2, alpha);
  currG2 = smoothTo(currG2, targetG2, alpha);
  currB2 = smoothTo(currB2, targetB2, alpha);

  // Update LEDs continuously
  analogWrite(RGB1_R, currR1);
  analogWrite(RGB1_G, currG1);
  analogWrite(RGB1_B, currB1);

  analogWrite(RGB2_R, currR2);
  analogWrite(RGB2_G, currG2);
  analogWrite(RGB2_B, currB2);

  // Small delay to stabilize readings and keep smoothness
  delay(10);
}
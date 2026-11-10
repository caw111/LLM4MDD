#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

static const uint8_t POT1_SIG = A3;
static const uint8_t POT2_SIG = A4;

static uint8_t hsvHueToRgbByte(float hueDeg, bool isRedChannel, uint8_t r, uint8_t g, uint8_t b);

struct RGBu8 {
  uint8_t r, g, b;
};

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBu8 hsvToRgb(float hDeg, float s, float v);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t smoothStep(float target, float current, float alpha);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static RGBu8 hsvToRgb(float hDeg, float s, float v) {
  // h in [0,360), s and v in [0,1]
  // Output in [0,255]
  if (hDeg < 0) hDeg = 0;
  while (hDeg >= 360.0f) hDeg -= 360.0f;

  float c = v * s;                 // Chroma
  float hh = hDeg / 60.0f;        // sector 0..6
  float x = c * (1.0f - fabsf(fmodf(hh, 2.0f) - 1.0f));

  float r1 = 0, g1 = 0, b1 = 0;
  if (0.0f <= hh && hh < 1.0f)      { r1 = c; g1 = x; b1 = 0; }
  else if (1.0f <= hh && hh < 2.0f) { r1 = x; g1 = c; b1 = 0; }
  else if (2.0f <= hh && hh < 3.0f) { r1 = 0; g1 = c; b1 = x; }
  else if (3.0f <= hh && hh < 4.0f) { r1 = 0; g1 = x; b1 = c; }
  else if (4.0f <= hh && hh < 5.0f) { r1 = x; g1 = 0; b1 = c; }
  else if (5.0f <= hh && hh < 6.0f) { r1 = c; g1 = 0; b1 = x; }
  else                              { r1 = 0; g1 = 0; b1 = 0; }

  float m = v - c;
  float rf = (r1 + m) * 255.0f;
  float gf = (g1 + m) * 255.0f;
  float bf = (b1 + m) * 255.0f;

  RGBu8 out;
  out.r = (uint8_t)constrain((int)lroundf(rf), 0, 255);
  out.g = (uint8_t)constrain((int)lroundf(gf), 0, 255);
  out.b = (uint8_t)constrain((int)lroundf(bf), 0, 255);
  return out;
}

static uint8_t smoothStep(float target, float current, float alpha) {
  float out = current + alpha * (target - current);
  if (out < 0) out = 0;
  if (out > 255) out = 255;
  return (uint8_t)out;
}

void setup() {
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Common cathode: HIGH means off for anode-driven LED pins.
  analogWrite(RGB1_R, 255);
  analogWrite(RGB1_G, 255);
  analogWrite(RGB1_B, 255);

  analogWrite(RGB2_R, 255);
  analogWrite(RGB2_G, 255);
  analogWrite(RGB2_B, 255);

  pinMode(POT1_SIG, INPUT);
  pinMode(POT2_SIG, INPUT);
}

void loop() {
  int raw1 = analogRead(POT1_SIG); // 0..1023
  int raw2 = analogRead(POT2_SIG); // 0..1023

  float hue1 = (raw1 * 360.0f) / 1023.0f;
  float hue2 = (raw2 * 360.0f) / 1023.0f;

  // Saturation and value set to 1
  const float s = 1.0f;
  const float v = 1.0f;

  RGBu8 target1 = hsvToRgb(hue1, s, v);
  RGBu8 target2 = hsvToRgb(hue2, s, v);

  // Smooth transition: update gradually toward the new RGB values.
  // Use a fixed alpha per loop; this yields smooth changes as pots move.
  static float cur1r = 0, cur1g = 0, cur1b = 0;
  static float cur2r = 0, cur2g = 0, cur2b = 0;

  // Initialize cur values to "off" (LEDs off => PWM duty 255 for common cathode).
  // Map internal cur values in brightness domain [0..255] where 0=off, 255=max.
  static bool initialized = false;
  if (!initialized) {
    cur1r = cur1g = cur1b = 0;
    cur2r = cur2g = cur2b = 0;
    initialized = true;
  }

  const float alpha = 0.12f; // smoothing factor

  cur1r = cur1r + alpha * ((float)target1.r - cur1r);
  cur1g = cur1g + alpha * ((float)target1.g - cur1g);
  cur1b = cur1b + alpha * ((float)target1.b - cur1b);

  cur2r = cur2r + alpha * ((float)target2.r - cur2r);
  cur2g = cur2g + alpha * ((float)target2.g - cur2g);
  cur2b = cur2b + alpha * ((float)target2.b - cur2b);

  // Convert brightness [0..255] to PWM for common cathode:
  // brightness 0 => LED off => write 255
  // brightness 255 => LED fully on => write 0
  auto writeCommonCathode = [](uint8_t pin, float brightness) {
    int b = (int)lroundf(brightness);
    b = constrain(b, 0, 255);
    int pwm = 255 - b;
    analogWrite(pin, pwm);
  };

  writeCommonCathode(RGB1_R, cur1r);
  writeCommonCathode(RGB1_G, cur1g);
  writeCommonCathode(RGB1_B, cur1b);

  writeCommonCathode(RGB2_R, cur2r);
  writeCommonCathode(RGB2_G, cur2g);
  writeCommonCathode(RGB2_B, cur2b);

  // Small delay to stabilize ADC readings and help smoothness.
  delay(10);
}

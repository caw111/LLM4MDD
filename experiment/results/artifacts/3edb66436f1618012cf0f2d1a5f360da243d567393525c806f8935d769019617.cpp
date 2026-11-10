#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;

const uint8_t SERVO1_PWM_PIN = 8;
const uint8_t SERVO2_PWM_PIN = 9;

// RGB1 pins
const uint8_t RGB1_R_PIN = 2;
const uint8_t RGB1_G_PIN = 3;
const uint8_t RGB1_B_PIN = 4;

// RGB2 pins
const uint8_t RGB2_R_PIN = 5;
const uint8_t RGB2_G_PIN = 6;
const uint8_t RGB2_B_PIN = 7;

// 6-segment bargraph pins (A1..A6)
const uint8_t BAR_A_PINS[6] = {10, 11, 12, 13, 44, 45};
// C1..C6 are tied to GND (bargraph uses common cathode), no need to control.

// Timing: every 2 seconds increment by 30 degrees.
const unsigned long STEP_INTERVAL_MS = 2000;

int angle1 = 0;
int angle2 = 0;

unsigned long lastStepMs = 0;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBOffCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin);
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBColorCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool r, bool g, bool b);
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBargraphFromAverageAngle(int a1, int a2);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateRGBFromAngles(int a1, int a2);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setRGBOffCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin) {
  // Common cathode: ON is HIGH, OFF is LOW.
  digitalWrite(rPin, LOW);
  digitalWrite(gPin, LOW);
  digitalWrite(bPin, LOW);
}

static inline void setRGBColorCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool r, bool g, bool b) {
  digitalWrite(rPin, r ? HIGH : LOW);
  digitalWrite(gPin, g ? HIGH : LOW);
  digitalWrite(bPin, b ? HIGH : LOW);
}

static void updateBargraphFromAverageAngle(int a1, int a2) {
  // Requirement:
  // - [0°, 90°] => first RGB LED is lit, bargraph behavior: "number of lit LEDs corresponds
  //   to average angle of the servos"
  // - When servos at [0°, 90°], the first RGB LED is lit.
  // - When at (90°, 180°], both RGB LEDs are lit.
  // - "All LEDs reset when the servos return to 0°."
  //
  // Mapping bargraph LEDs to average angle:
  //  - average angle 0 => none lit
  //  - average angle in (0, 180] => light N LEDs proportional to avg angle.
  // We'll implement N = floor(avg/180 * 6), but with N=0 at avg==0.
  int avg = (a1 + a2) / 2;
  if (avg <= 0) {
    for (int i = 0; i < 6; i++) digitalWrite(BAR_A_PINS[i], LOW);
    return;
  }

  // For avg == 180, N becomes 6.
  // Use integer math with 180-based scaling.
  int n = (avg * 6) / 180; // avg=1..179 => 0..5, avg=180 => 6
  if (n < 0) n = 0;
  if (n > 6) n = 6;

  for (int i = 0; i < 6; i++) {
    digitalWrite(BAR_A_PINS[i], (i < n) ? HIGH : LOW);
  }
}

static void updateRGBFromAngles(int a1, int a2) {
  // RGB behavior requirement:
  // rgb1 show red, rgb2 show blue.
  // With threshold based on average angle:
  // - When servos at [0°, 90°], first RGB LED is lit.
  // - When at (90°, 180°], both RGB LEDs are lit.
  // - All LEDs reset when servos return to 0° (bargraph and both RGBs off).
  int avg = (a1 + a2) / 2;

  if (a1 == 0 && a2 == 0) {
    setRGBOffCommonCathode(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN);
    setRGBOffCommonCathode(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN);
    return;
  }

  // [0°, 90°] => rgb1 red on, rgb2 off.
  if (avg <= 90) {
    // rgb1 red
    setRGBColorCommonCathode(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN, true, false, false);
    // rgb2 off
    setRGBOffCommonCathode(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN);
  } else {
    // (90°, 180°] => rgb1 red on, rgb2 blue on.
    setRGBColorCommonCathode(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN, true, false, false);
    setRGBColorCommonCathode(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN, false, false, true);
  }
}

void setup() {
  // Bargraph pins
  for (int i = 0; i < 6; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }

  // RGB pins (common cathode)
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  setRGBOffCommonCathode(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN);
  setRGBOffCommonCathode(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN);

  // Servos
  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);

  // Both servos start at 0°
  angle1 = 0;
  angle2 = 0;
  servo1.write(angle1);
  servo2.write(angle2);

  // Initial LEDs: RGB off; bargraph none lit.
  updateRGBFromAngles(angle1, angle2);
  updateBargraphFromAverageAngle(angle1, angle2);

  lastStepMs = millis();
}

void loop() {
  unsigned long now = millis();

  if (now - lastStepMs >= STEP_INTERVAL_MS) {
    lastStepMs = now;

    // Increment by 30° simultaneously; reset after reaching 180°.
    angle1 += 30;
    angle2 += 30;

    if (angle1 >= 180) angle1 = 0;
    if (angle2 >= 180) angle2 = 0;

    // Apply to servos
    servo1.write(angle1);
    servo2.write(angle2);

    // Update RGB LEDs and bargraph dynamically based on angles.
    updateRGBFromAngles(angle1, angle2);
    updateBargraphFromAverageAngle(angle1, angle2);
  }

  // No other behavior required.
}

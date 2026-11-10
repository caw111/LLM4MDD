#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_53\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// Wiring (from provided circuit)
static const uint8_t SERVO1_PIN = 5;   // mega:5 -> servo1:PWM
static const uint8_t SERVO2_PIN = 6;   // mega:6 -> servo2:PWM
static const uint8_t SERVO3_PIN = 7;   // mega:7 -> servo3:PWM

// RGB LED PWM pins (from provided circuit)
static const uint8_t RGB_R_PIN = 2;     // mega:2 -> rgb1:R
static const uint8_t RGB_G_PIN = 3;     // mega:3 -> rgb1:G
static const uint8_t RGB_B_PIN = 4;     // mega:4 -> rgb1:B

// Bargraph pins (0-8 LEDs mapped to A1..A8)
static const uint8_t BAR_A_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45}; // A1..A8 on mega

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_53\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t angleToBrightness(uint8_t angleDeg);
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_53\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t brightnessToBarIndex(uint8_t r, uint8_t g, uint8_t b);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_53\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateRgb(uint8_t r, uint8_t g, uint8_t b);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_53\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setServoAnglesAndUpdate(uint8_t a1, uint8_t a2, uint8_t a3);
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_53\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_53\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_53\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t angleToBrightness(uint8_t angleDeg) {
  // Map 0-180 linearly to 0-255
  // brightness = angleDeg * 255 / 180
  return (uint32_t)angleDeg * 255u / 180u;
}

static uint8_t brightnessToBarIndex(uint8_t r, uint8_t g, uint8_t b) {
  // Average brightness mapped to 0-8 LEDs
  // avg = (R+G+B)/3 in [0..255]
  // index (0..8) = avg * 8 / 255
  uint16_t avg = (uint16_t(r) + uint16_t(g) + uint16_t(b)) / 3u; // 0..255
  uint8_t idx = (uint32_t)avg * 8u / 255u; // 0..8
  return idx;
}

static void setBargraph(uint8_t onLeds /*0..8*/) {
  // Turn on first N LEDs among A1..A8 (connected to pins BAR_A_PINS)
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(BAR_A_PINS[i], (i < onLeds) ? HIGH : LOW);
  }
}

static void updateRgb(uint8_t r, uint8_t g, uint8_t b) {
  // Must preserve behavior: RGB LED brightness determined by servo angles.
  analogWrite(RGB_R_PIN, r);
  analogWrite(RGB_G_PIN, g);
  analogWrite(RGB_B_PIN, b);
}

static void setServoAnglesAndUpdate(uint8_t a1, uint8_t a2, uint8_t a3) {
  servo1.write(a1);
  servo2.write(a2);
  servo3.write(a3);

  uint8_t R = angleToBrightness(a1);
  uint8_t G = angleToBrightness(a2);
  uint8_t B = angleToBrightness(a3);

  updateRgb(R, G, B);

  uint8_t barOn = brightnessToBarIndex(R, G, B); // 0..8
  setBargraph(barOn);
}

void setup() {
  // Initialize servos to 0°
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // RGB LED initially off
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);
  analogWrite(RGB_R_PIN, 0);
  analogWrite(RGB_G_PIN, 0);
  analogWrite(RGB_B_PIN, 0);

  // Bar graph initially off
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }

  // Ensure outputs reflect starting state (servos at 0 => R,G,B = 0 => bargraph 0 LEDs)
  setServoAnglesAndUpdate(0, 0, 0);
}

void loop() {
  static uint32_t lastMoveMs = 0;
  static uint8_t step = 0; // 0->servo1, 1->servo2, 2->servo3 repeating

  // Maintain current angles (only 0 or 180 per requirement)
  static uint8_t a1 = 0;
  static uint8_t a2 = 0;
  static uint8_t a3 = 0;

  uint32_t now = millis();
  if (now - lastMoveMs >= 2000UL) { // Every 2 seconds
    lastMoveMs = now;

    // Move one servo 180° in order: servo1 -> servo2 -> servo3 -> servo1 ...
    if (step == 0) {
      a1 = (a1 == 0) ? 180 : 0;
    } else if (step == 1) {
      a2 = (a2 == 0) ? 180 : 0;
    } else { // step == 2
      a3 = (a3 == 0) ? 180 : 0;
    }

    setServoAnglesAndUpdate(a1, a2, a3);

    step = (step + 1) % 3;
  }
}

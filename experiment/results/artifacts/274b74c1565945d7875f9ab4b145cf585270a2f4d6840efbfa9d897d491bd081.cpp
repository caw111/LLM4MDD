#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_52\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

constexpr uint8_t SERVO1_PIN = 8;
constexpr uint8_t SERVO2_PIN = 9;

constexpr uint8_t RGB1_RED_PIN   = 2;
constexpr uint8_t RGB1_GREEN_PIN = 3;
constexpr uint8_t RGB1_BLUE_PIN  = 4;
constexpr uint8_t RGB2_RED_PIN   = 5;
constexpr uint8_t RGB2_GREEN_PIN = 6;
constexpr uint8_t RGB2_BLUE_PIN  = 7;

constexpr uint8_t BAR_PINS[] = {10, 11, 12, 13, 44, 45};
constexpr uint8_t BAR_COUNT = sizeof(BAR_PINS) / sizeof(BAR_PINS[0]);

constexpr unsigned long STEP_INTERVAL_MS = 2000UL;
constexpr uint8_t ANGLE_STEP = 30;

Servo servo1;
Servo servo2;

uint8_t servo1Angle = 0;
uint8_t servo2Angle = 0;
unsigned long lastStepTime = 0;

#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_52\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_52\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_52\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_52\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays() {
  const uint16_t averageAngle =
      (static_cast<uint16_t>(servo1Angle) + servo2Angle) / 2;

  // Zero degrees overrides the RGB range rule: all LEDs are off.
  // At 30..90 degrees, only rgb1 is red.
  // Above 90 degrees, rgb1 remains red and rgb2 is blue.
  digitalWrite(RGB1_RED_PIN, averageAngle > 0 ? HIGH : LOW);
  digitalWrite(RGB1_GREEN_PIN, LOW);
  digitalWrite(RGB1_BLUE_PIN, LOW);

  digitalWrite(RGB2_RED_PIN, LOW);
  digitalWrite(RGB2_GREEN_PIN, LOW);
  digitalWrite(RGB2_BLUE_PIN, averageAngle > 90 ? HIGH : LOW);

  // Each wired bar segment represents 30 degrees.
  const uint8_t litSegments = (averageAngle * BAR_COUNT) / 180;
  for (uint8_t i = 0; i < BAR_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], i < litSegments ? HIGH : LOW);
  }
}

void setup() {
  const uint8_t rgbPins[] = {
    RGB1_RED_PIN, RGB1_GREEN_PIN, RGB1_BLUE_PIN,
    RGB2_RED_PIN, RGB2_GREEN_PIN, RGB2_BLUE_PIN
  };

  for (uint8_t i = 0; i < sizeof(rgbPins) / sizeof(rgbPins[0]); ++i) {
    digitalWrite(rgbPins[i], LOW);
    pinMode(rgbPins[i], OUTPUT);
  }

  for (uint8_t i = 0; i < BAR_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  // Preload zero-degree commands before enabling the servo outputs.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  updateDisplays();
  lastStepTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastStepTime >= STEP_INTERVAL_MS) {
    lastStepTime += STEP_INTERVAL_MS;

    servo1Angle = (servo1Angle >= 180) ? 0 : servo1Angle + ANGLE_STEP;
    servo2Angle = (servo2Angle >= 180) ? 0 : servo2Angle + ANGLE_STEP;

    servo1.write(servo1Angle);
    servo2.write(servo2Angle);

    updateDisplays();
  }
}

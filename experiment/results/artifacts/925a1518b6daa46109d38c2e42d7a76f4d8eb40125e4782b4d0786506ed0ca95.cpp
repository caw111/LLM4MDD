#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_53\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

const uint8_t SERVO_COUNT = 3;
const uint8_t BAR_LED_COUNT = 8;

const uint8_t SERVO_PINS[SERVO_COUNT] = {5, 6, 7};
const uint8_t RGB_PINS[SERVO_COUNT] = {2, 3, 4};  // Red, green, blue
const uint8_t BAR_PINS[BAR_LED_COUNT] = {
  8, 9, 10, 11, 12, 13, 44, 45  // Bar graph A1 through A8
};

const unsigned long MOVE_INTERVAL_MS = 2000UL;

Servo servos[SERVO_COUNT];
uint8_t servoAngles[SERVO_COUNT] = {0, 0, 0};
uint8_t nextServo = 0;
unsigned long lastMoveTime = 0;

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_53\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLighting();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_53\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_53\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_53\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLighting() {
  uint16_t brightnessSum = 0;

  for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
    const uint8_t brightness =
        static_cast<uint8_t>(map(servoAngles[i], 0, 180, 0, 255));

    // Common-cathode RGB LED: higher PWM means greater brightness.
    analogWrite(RGB_PINS[i], brightness);
    brightnessSum += brightness;
  }

  const uint16_t averageBrightness = brightnessSum / SERVO_COUNT;
  const uint8_t illuminatedLEDs =
      static_cast<uint8_t>(map(averageBrightness, 0, 255, 0, BAR_LED_COUNT));

  for (uint8_t i = 0; i < BAR_LED_COUNT; ++i) {
    // Digital output remains usable on Mega pins 44/45 when Servo uses Timer5.
    digitalWrite(BAR_PINS[i], i < illuminatedLEDs ? HIGH : LOW);
  }
}

void setup() {
  for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
    digitalWrite(RGB_PINS[i], LOW);
    pinMode(RGB_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < BAR_LED_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
    // Set the initial pulse width before enabling servo pulses.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
  }

  updateLighting();
  lastMoveTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastMoveTime >= MOVE_INTERVAL_MS) {
    lastMoveTime += MOVE_INTERVAL_MS;

    servoAngles[nextServo] = (servoAngles[nextServo] == 0) ? 180 : 0;
    servos[nextServo].write(servoAngles[nextServo]);

    updateLighting();
    nextServo = (nextServo + 1) % SERVO_COUNT;
  }
}

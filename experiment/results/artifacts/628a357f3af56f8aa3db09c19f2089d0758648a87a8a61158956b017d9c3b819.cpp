#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_109\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

const uint8_t LED_PINS[] = {2, 3, 4};
const uint8_t RGB_PINS[] = {5, 6, 7};  // Common cathode: R, G, B
const uint8_t SERVO1_PIN = 8;
const uint8_t SERVO2_PIN = 9;

const unsigned long SWEEP_MS = 2000;
const unsigned long PAUSE_MS = 2000;

Servo servo1;
Servo servo2;

enum MotionState {
  PAUSED_AT_ZERO,
  SWEEPING_TO_180,
  PAUSED_AT_180,
  SWEEPING_TO_ZERO
};

MotionState motionState = PAUSED_AT_ZERO;
unsigned long stateStartedAt = 0;

int lastAngle = 0;
uint8_t nextLed = 0;
uint8_t rgbColor = 0;  // 0 = red, 1 = green, 2 = blue

#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_109\\direct\\attempt_0\\sketch\\sketch.ino"
void setServoAngle(int angle);
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_109\\direct\\attempt_0\\sketch\\sketch.ino"
void setRgbColor(uint8_t color);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_109\\direct\\attempt_0\\sketch\\sketch.ino"
void onEndpointReached();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_109\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_109\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_109\\direct\\attempt_0\\sketch\\sketch.ino"
void setServoAngle(int angle) {
  if (angle != lastAngle) {
    servo1.write(angle);
    servo2.write(angle);
    lastAngle = angle;
  }
}

void setRgbColor(uint8_t color) {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(RGB_PINS[i], i == color ? HIGH : LOW);
  }
}

void onEndpointReached() {
  // Advance both light sequences at the same endpoint event.
  // Each selected LED remains on until the next endpoint is reached.
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], i == nextLed ? HIGH : LOW);
  }
  nextLed = (nextLed + 1) % 3;

  rgbColor = (rgbColor + 1) % 3;
  setRgbColor(rgbColor);
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);

    pinMode(RGB_PINS[i], OUTPUT);
    digitalWrite(RGB_PINS[i], LOW);
  }

  setRgbColor(0);

  // Preload zero degrees before attaching to avoid a default 90-degree command.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);

  lastAngle = 0;
  motionState = PAUSED_AT_ZERO;
  stateStartedAt = millis();
}

void loop() {
  const unsigned long now = millis();
  const unsigned long elapsed = now - stateStartedAt;

  switch (motionState) {
    case PAUSED_AT_ZERO:
      if (elapsed >= PAUSE_MS) {
        motionState = SWEEPING_TO_180;
        stateStartedAt = now;
      }
      break;

    case SWEEPING_TO_180:
      if (elapsed >= SWEEP_MS) {
        setServoAngle(180);
        onEndpointReached();
        motionState = PAUSED_AT_180;
        stateStartedAt = now;
      } else {
        setServoAngle((elapsed * 180UL) / SWEEP_MS);
      }
      break;

    case PAUSED_AT_180:
      if (elapsed >= PAUSE_MS) {
        motionState = SWEEPING_TO_ZERO;
        stateStartedAt = now;
      }
      break;

    case SWEEPING_TO_ZERO:
      if (elapsed >= SWEEP_MS) {
        setServoAngle(0);
        onEndpointReached();
        motionState = PAUSED_AT_ZERO;
        stateStartedAt = now;
      } else {
        setServoAngle(180 - (elapsed * 180UL) / SWEEP_MS);
      }
      break;
  }
}

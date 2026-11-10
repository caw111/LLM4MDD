#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_46\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t RED_PIN = 2;
constexpr uint8_t GREEN_PIN = 3;
constexpr uint8_t BLUE_PIN = 4;

constexpr uint8_t SERVO1_PIN = 5;
constexpr uint8_t SERVO2_PIN = 6;
constexpr uint8_t SERVO3_PIN = 7;

constexpr unsigned long STATE_DURATION_MS = 2000;

// The supplied standard servos support 0–180 degrees.
constexpr uint16_t SERVO_MAX_ANGLE = 180;

Servo servo1;
Servo servo2;
Servo servo3;

struct ColorState {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint16_t angle1;
  uint16_t angle2;
  uint16_t angle3;
};

const ColorState states[] = {
  {HIGH, LOW,  LOW,    0,  90, 180},  // Red
  {LOW,  HIGH, LOW,   90, 180,   0},  // Green
  {LOW,  LOW,  HIGH, 180,   0,  90},  // Blue
  {HIGH, HIGH, LOW,   45, 135, 225},  // Yellow
  {LOW,  HIGH, HIGH, 135, 225,  45},  // Cyan
  {HIGH, LOW,  HIGH, 225,  45, 135},  // Magenta
  {HIGH, HIGH, HIGH,   0,  90, 180}   // White
};

constexpr uint8_t STATE_COUNT = sizeof(states) / sizeof(states[0]);

uint8_t currentState = 0;
unsigned long stateStartedAt = 0;

#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_46\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t supportedAngle(uint16_t requestedAngle);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_46\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(uint8_t index);
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_46\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_46\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_46\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t supportedAngle(uint16_t requestedAngle) {
  return requestedAngle > SERVO_MAX_ANGLE
             ? SERVO_MAX_ANGLE
             : requestedAngle;
}

void applyState(uint8_t index) {
  const ColorState &state = states[index];

  // Common-cathode RGB LED: HIGH turns a channel on.
  digitalWrite(RED_PIN, state.red);
  digitalWrite(GREEN_PIN, state.green);
  digitalWrite(BLUE_PIN, state.blue);

  servo1.write(supportedAngle(state.angle1));
  servo2.write(supportedAngle(state.angle2));
  servo3.write(supportedAngle(state.angle3));

  stateStartedAt = millis();
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  applyState(currentState);
}

void loop() {
  if (millis() - stateStartedAt >= STATE_DURATION_MS) {
    currentState = (currentState + 1) % STATE_COUNT;
    applyState(currentState);
  }
}

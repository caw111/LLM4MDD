#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_27\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const uint8_t SERVO1_PIN = 2;
static const uint8_t SERVO2_PIN = 3;
static const uint8_t SERVO3_PIN = 4;

static const uint8_t BARGRAPH_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

int angle1 = 0;
int angle2 = 0;
int angle3 = 0;

unsigned long lastUpdateMs = 0;
const unsigned long intervalMs = 2000;

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_27\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBargraph();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_27\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void stepServo(int &angle);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_27\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_27\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_27\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBargraph() {
  // Total in range 0..540
  int total = angle1 + angle2 + angle3;

  // Divide into 10 equal ranges (54 units per LED).
  // LED count = floor(total / 54), but clamp to [0,10]
  int lit = total / 54;
  if (lit < 0) lit = 0;
  if (lit > 10) lit = 10;

  for (int i = 0; i < 10; i++) {
    digitalWrite(BARGRAPH_PINS[i], (i < lit) ? HIGH : LOW);
  }
}

static void stepServo(int &angle) {
  if (angle < 180) {
    angle += 18;
    if (angle > 180) angle = 180;
  } else if (angle == 180) {
    angle -= 18;
    if (angle < 0) angle = 0;
  } else {
    // Safety clamp (should never happen with the given stepping logic)
    if (angle > 180) angle = 180;
    if (angle < 0) angle = 0;
  }
}

void setup() {
  // LED bar graph pins
  for (int i = 0; i < 10; i++) {
    pinMode(BARGRAPH_PINS[i], OUTPUT);
    digitalWrite(BARGRAPH_PINS[i], LOW); // remains off initially
  }

  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialize at 0 degrees
  angle1 = 0;
  angle2 = 0;
  angle3 = 0;

  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);

  updateBargraph(); // should keep all LEDs off at total=0

  lastUpdateMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdateMs >= intervalMs) {
    lastUpdateMs = now;

    // Update all servos simultaneously each 2 seconds
    stepServo(angle1);
    stepServo(angle2);
    stepServo(angle3);

    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);

    // Update bargraph to represent combined position
    updateBargraph();
  }
}

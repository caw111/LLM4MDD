#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_91\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;

static const uint8_t SERVO1_PIN = 4; // mega:4 -> servo1:PWM
static const uint8_t SERVO2_PIN = 5; // mega:5 -> servo2:PWM
static const uint8_t SERVO3_PIN = 6; // mega:6 -> servo3:PWM

static const unsigned long LED_BLINK_INTERVAL_MS = 2000;
static const unsigned long SERVO_STATE_DURATION_MS = 2000;

unsigned long lastLedToggleTime = 0;
bool led1On = false;

unsigned long servoStateStartTime = 0;
uint8_t servoState = 0; // 0: (0,90,180) ; 1: (180,0,90)

#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_91\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyServoState(uint8_t state);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_91\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_91\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_91\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyServoState(uint8_t state) {
  if (state == 0) {
    servo1.write(0);
    servo2.write(90);
    servo3.write(180);
  } else {
    servo1.write(180);
    servo2.write(0);
    servo3.write(90);
  }
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Initial condition: both LEDs off
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  led1On = false;

  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initial condition: all servos at 0°
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Start synchronized timeline
  lastLedToggleTime = millis();
  servoStateStartTime = millis();
  servoState = 0;

  // Maintain servo state at (0°,90°,180°) for 2 seconds as required
  applyServoState(servoState);
}

void loop() {
  unsigned long now = millis();

  // LED blinking alternately every 2000 ms
  if (now - lastLedToggleTime >= LED_BLINK_INTERVAL_MS) {
    lastLedToggleTime = now;
    led1On = !led1On;

    if (led1On) {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
    } else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
    }
  }

  // Servo sequence: two states, each held for 2 seconds
  if (now - servoStateStartTime >= SERVO_STATE_DURATION_MS) {
    servoStateStartTime = now;
    servoState = (servoState == 0) ? 1 : 0;
    applyServoState(servoState);
  }
}

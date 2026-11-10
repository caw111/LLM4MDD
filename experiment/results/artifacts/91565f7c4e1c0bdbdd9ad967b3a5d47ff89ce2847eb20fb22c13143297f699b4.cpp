#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;

static const uint8_t SERVO_PIN = 3;
static const uint8_t LED_PWM_PIN = 2; // connected to mega:2
static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;
static const uint8_t POT3_PIN = A5;

static const int SERVO_MIN_ANGLE = 0;
static const int SERVO_MAX_ANGLE = 180;

static const uint8_t LED_MIN_BRIGHTNESS = 0;
static const uint8_t LED_MAX_BRIGHTNESS = 255;

// Servo stepping
static int currentServoAngle = 0;

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_94\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PWM_PIN, OUTPUT);

  servo1.attach(SERVO_PIN);
  currentServoAngle = SERVO_MIN_ANGLE;
  servo1.write(currentServoAngle);

  analogReference(DEFAULT);
}

void loop() {
  // Read potentiometers (0..1023)
  int pot1Val = analogRead(POT1_PIN);
  int pot2Val = analogRead(POT2_PIN);
  int pot3Val = analogRead(POT3_PIN);

  // Map pot1 -> servo angle (0..180)
  int targetAngle = map(pot1Val, 0, 1023, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  targetAngle = constrain(targetAngle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);

  // Map pot2 -> LED brightness (0..255)
  uint8_t brightness = (uint8_t)map(pot2Val, 0, 1023, LED_MIN_BRIGHTNESS, LED_MAX_BRIGHTNESS);
  analogWrite(LED_PWM_PIN, brightness);

  // Map pot3 -> delay between each servo step (10..100 ms)
  unsigned long stepDelayMs = (unsigned long)map(pot3Val, 0, 1023, 10, 100);
  stepDelayMs = constrain(stepDelayMs, 10UL, 100UL);

  // Smooth move servo towards target, stepping by 1 degree with delay between steps
  if (currentServoAngle < targetAngle) {
    currentServoAngle++;
    servo1.write(currentServoAngle);
    delay(stepDelayMs);
  } else if (currentServoAngle > targetAngle) {
    currentServoAngle--;
    servo1.write(currentServoAngle);
    delay(stepDelayMs);
  } else {
    // Already at target; keep LED updated continuously.
    // Small pause to avoid overly tight loop when servo doesn't move.
    delay(5);
  }
}

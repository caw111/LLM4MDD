#include <Servo.h>

Servo servo1;

static const uint8_t LED_PIN = 2;     // led1:A -> mega:2
static const uint8_t SERVO_PIN = 3;   // mega:3 -> servo1:PWM

static const uint8_t POT1_PIN = A3;   // pot1:SIG -> mega:A3
static const uint8_t POT2_PIN = A4;   // pot2:SIG -> mega:A4
static const uint8_t POT3_PIN = A5;   // pot3:SIG -> mega:A5

// Servo motion state
int currentAngle = 0;
int targetAngle = 0;

// Timing for smooth step movement
unsigned long lastStepMs = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  servo1.attach(SERVO_PIN);
  currentAngle = servo1.read();
  targetAngle = currentAngle;
  servo1.write(currentAngle);

  // Initialize timing
  lastStepMs = millis();
}

void loop() {
  // Read potentiometers (0..1023)
  int pot1Val = analogRead(POT1_PIN);
  int pot2Val = analogRead(POT2_PIN);
  int pot3Val = analogRead(POT3_PIN);

  // 1) Servo target angle: map pot1 to 0..180
  targetAngle = map(pot1Val, 0, 1023, 0, 180);

  // 2) LED brightness: map pot2 to 0..255 (analogWrite expects 0..255)
  int ledBrightness = map(pot2Val, 0, 1023, 0, 255);
  analogWrite(LED_PIN, ledBrightness);

  // 3) Servo speed: map pot3 to delay between steps (10..100 ms)
  int stepDelayMs = map(pot3Val, 0, 1023, 10, 100);

  // Smooth movement: step toward target angle
  unsigned long now = millis();
  if (now - lastStepMs >= (unsigned long)stepDelayMs) {
    lastStepMs = now;

    if (currentAngle < targetAngle) {
      currentAngle++;
      servo1.write(currentAngle);
    } else if (currentAngle > targetAngle) {
      currentAngle--;
      servo1.write(currentAngle);
    }
  }

  // Continuous update is handled by the loop reading pots each iteration
}
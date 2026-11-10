#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_48\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const uint8_t RGB1_R_PIN = 2;
static const uint8_t RGB1_G_PIN = 3;
static const uint8_t RGB1_B_PIN = 4;

static const uint8_t RGB2_R_PIN = 5;
static const uint8_t RGB2_G_PIN = 6;
static const uint8_t RGB2_B_PIN = 7;

static const uint8_t RGB3_R_PIN = 8;
static const uint8_t RGB3_G_PIN = 9;
static const uint8_t RGB3_B_PIN = 10;

static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;

static const uint8_t SERVO1_PIN = 11;
static const uint8_t SERVO2_PIN = 12;
static const uint8_t SERVO3_PIN = 13;

static const unsigned long UPDATE_INTERVAL_MS = 100;
static const unsigned long SERVO3_TOGGLE_INTERVAL_MS = 2000;

unsigned long lastUpdateMs = 0;
unsigned long lastServo3ToggleMs = 0;

int servo3Angle = 0;

#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_48\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapPotToAngle0_180(int potValue);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_48\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapPotToByte0_255(int potValue);
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_48\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_48\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_48\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapPotToAngle0_180(int potValue) {
  // potValue expected 0..1023
  return (potValue * 180L) / 1023L;
}

static int mapPotToByte0_255(int potValue) {
  // potValue expected 0..1023
  return (potValue * 255L) / 1023L;
}

void setup() {
  // RGB LED pins
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  pinMode(RGB3_R_PIN, OUTPUT);
  pinMode(RGB3_G_PIN, OUTPUT);
  pinMode(RGB3_B_PIN, OUTPUT);

  // Potentiometer inputs
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  // Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialize outputs to match requirements (blue fixed at 0)
  analogWrite(RGB1_B_PIN, 0);
  analogWrite(RGB2_B_PIN, 0);
  analogWrite(RGB3_B_PIN, 0);

  // Initialize servo3 to 0 and set initial time references
  servo3Angle = 0;
  servo3.write(servo3Angle);

  unsigned long now = millis();
  lastUpdateMs = now;
  lastServo3ToggleMs = now;
}

void loop() {
  unsigned long now = millis();

  if (now - lastServo3ToggleMs >= SERVO3_TOGGLE_INTERVAL_MS) {
    lastServo3ToggleMs += SERVO3_TOGGLE_INTERVAL_MS; // keep stable timing
    servo3Angle = (servo3Angle == 0) ? 180 : 0;
    servo3.write(servo3Angle);
  }

  if (now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    lastUpdateMs += UPDATE_INTERVAL_MS;

    int pot1 = analogRead(POT1_PIN);
    int pot2 = analogRead(POT2_PIN);

    int red = mapPotToByte0_255(pot1);   // 0..255 from pot1
    int green = mapPotToByte0_255(pot2); // 0..255 from pot2

    // Blue fixed at 0
    analogWrite(RGB1_B_PIN, 0);
    analogWrite(RGB2_B_PIN, 0);
    analogWrite(RGB3_B_PIN, 0);

    // Apply red/green to all RGB LEDs
    analogWrite(RGB1_R_PIN, red);
    analogWrite(RGB1_G_PIN, green);

    analogWrite(RGB2_R_PIN, red);
    analogWrite(RGB2_G_PIN, green);

    analogWrite(RGB3_R_PIN, red);
    analogWrite(RGB3_G_PIN, green);

    // Servos 1 and 2 follow potentiometers
    int servo1Angle = mapPotToAngle0_180(pot1); // 0..180 from pot1
    int servo2Angle = mapPotToAngle0_180(pot2); // 0..180 from pot2

    servo1.write(servo1Angle);
    servo2.write(servo2Angle);
    // servo3 handled independently above
  }
}

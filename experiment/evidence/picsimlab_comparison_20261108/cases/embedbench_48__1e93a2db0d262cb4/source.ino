#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const uint8_t POT1_PIN = A3; // pot1: SIG -> A3
static const uint8_t POT2_PIN = A4; // pot2: SIG -> A4

// RGB LED wiring (common cathode): COM -> GND, channels -> digital pins
// rgb1: R=2, G=3, B=4
static const uint8_t RGB1_R_PIN = 2;
static const uint8_t RGB1_G_PIN = 3;
static const uint8_t RGB1_B_PIN = 4;

// rgb2: R=5, G=6, B=7
static const uint8_t RGB2_R_PIN = 5;
static const uint8_t RGB2_G_PIN = 6;
static const uint8_t RGB2_B_PIN = 7;

// rgb3: R=8, G=9, B=10
static const uint8_t RGB3_R_PIN = 8;
static const uint8_t RGB3_G_PIN = 9;
static const uint8_t RGB3_B_PIN = 10;

static const uint32_t UPDATE_INTERVAL_MS = 100;

int servo3Target = 0;
bool servo3Toggle = false;
unsigned long lastUpdateMs = 0;

static uint8_t mapTo8bit(int raw10bit) {
  // Map 0-1023 -> 0-255
  if (raw10bit < 0) raw10bit = 0;
  if (raw10bit > 1023) raw10bit = 1023;
  return (uint8_t)((raw10bit * 255L) / 1023L);
}

static void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  // Because B is fixed at 0 per requirements, we keep b at 0.
  analogWrite(RGB1_R_PIN, r);
  analogWrite(RGB1_G_PIN, g);
  analogWrite(RGB1_B_PIN, b);

  analogWrite(RGB2_R_PIN, r);
  analogWrite(RGB2_G_PIN, g);
  analogWrite(RGB2_B_PIN, b);

  analogWrite(RGB3_R_PIN, r);
  analogWrite(RGB3_G_PIN, g);
  analogWrite(RGB3_B_PIN, b);
}

static int mapToServoAngle(int raw10bit) {
  // Map 0-1023 -> 0-180 degrees
  if (raw10bit < 0) raw10bit = 0;
  if (raw10bit > 1023) raw10bit = 1023;
  return (int)((raw10bit * 180L) / 1023L);
}

void setup() {
  // RGB pins: use PWM capable pins for analogWrite
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  pinMode(RGB3_R_PIN, OUTPUT);
  pinMode(RGB3_G_PIN, OUTPUT);
  pinMode(RGB3_B_PIN, OUTPUT);

  // Initialize to off blue (b=0) and initial unknown r/g (will be updated immediately)
  setRGB(0, 0, 0);

  // Servos:
  // servo1:PWM -> pin 11
  // servo2:PWM -> pin 12
  // servo3:PWM -> pin 13
  servo1.attach(11);
  servo2.attach(12);
  servo3.attach(13);

  servo3Target = 0;
  servo3Toggle = false;
  servo1.write(0);
  servo2.write(0);
  servo3.write(servo3Target);

  lastUpdateMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdateMs < UPDATE_INTERVAL_MS) return;
  lastUpdateMs = now;

  int pot1Raw = analogRead(POT1_PIN);
  int pot2Raw = analogRead(POT2_PIN);

  // RGB: pot1 controls R (0-255), pot2 controls G (0-255), B fixed at 0.
  uint8_t rVal = mapTo8bit(pot1Raw);
  uint8_t gVal = mapTo8bit(pot2Raw);
  uint8_t bVal = 0;
  setRGB(rVal, gVal, bVal);

  // Servos 1 & 2: pot1 and pot2 map to 0-180 degrees.
  int servo1Angle = mapToServoAngle(pot1Raw);
  int servo2Angle = mapToServoAngle(pot2Raw);

  servo1.write(servo1Angle);
  servo2.write(servo2Angle);

  // Servo 3: alternate between 0 and 180 every 2 seconds, independent of pots.
  // Ensure behavior aligns with real time by using elapsed time since boot.
  // Toggle state when crossing each 2-second boundary.
  unsigned long elapsedMs = now;
  // Number of completed 2-second intervals:
  unsigned long intervals = elapsedMs / 2000UL;
  bool desiredHigh = (intervals % 2UL) == 1UL;

  int desiredAngle = desiredHigh ? 180 : 0;
  // Update write each 100ms with desiredAngle to keep consistent timing.
  if (desiredAngle != servo3Target) {
    servo3Target = desiredAngle;
  }
  servo3.write(servo3Target);
}
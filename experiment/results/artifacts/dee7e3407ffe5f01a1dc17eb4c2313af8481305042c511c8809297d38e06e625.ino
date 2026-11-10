#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// Pin mapping (preserve wiring from provided circuit)
static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

static const uint8_t POT_PIN = A3;

static const uint8_t SERVO1_PWM = 8;
static const uint8_t SERVO2_PWM = 9;
static const uint8_t SERVO3_PWM = 10;

static int lastRGB = -1;

unsigned long lastServoUpdateMs = 0;
const unsigned long SERVO_MIN_HOLD_MS = 2000; // maintain position at least 2 seconds

int mapToServoAngle(int potValue) {
  // Map 0..1023 to 0..180
  if (potValue < 0) potValue = 0;
  if (potValue > 1023) potValue = 1023;
  return map(potValue, 0, 1023, 0, 180);
}

void setRGBComponents(int r, int g, int b) {
  // Clamp
  if (r < 0) r = 0; if (r > 255) r = 255;
  if (g < 0) g = 0; if (g > 255) g = 255;
  if (b < 0) b = 0; if (b > 255) b = 255;

  // Both RGB LEDs display same color at all times
  analogWrite(RGB1_R, r);
  analogWrite(RGB1_G, g);
  analogWrite(RGB1_B, b);

  analogWrite(RGB2_R, r);
  analogWrite(RGB2_G, g);
  analogWrite(RGB2_B, b);
}

void updateRGBFromPot(int potValue) {
  // Requirements:
  // Red increases 0..255 as pot 0..341
  // Green increases 0..255 as pot 342..682
  // Blue increases 0..255 as pot 683..1023
  // (others are 0 outside their ranges)

  int r = 0, g = 0, b = 0;

  if (potValue <= 341) {
    r = (potValue * 255) / 341; // 0..341 -> 0..255
  } else if (potValue <= 682) {
    // red stays at 255
    r = 255;
    g = ((potValue - 342) * 255) / (682 - 342); // 342..682 -> 0..255
  } else {
    // potValue >= 683
    r = 255;
    g = 255;
    b = ((potValue - 683) * 255) / (1023 - 683); // 683..1023 -> 0..255
  }

  // Ensure 0..255 constraints precisely at boundaries
  if (potValue <= 341) {
    g = 0;
    b = 0;
  } else if (potValue <= 682) {
    // b stays 0
    b = 0;
  }

  setRGBComponents(r, g, b);
}

void moveServosToPot(int potValue) {
  int angle = mapToServoAngle(potValue);

  // Requirements: potentiometer determines angle of each servo (same mapping for all)
  servo1.write(angle);
  servo2.write(angle);
  servo3.write(angle);
}

void setup() {
  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Pot input
  pinMode(POT_PIN, INPUT);

  // Servos
  servo1.attach(SERVO1_PWM);
  servo2.attach(SERVO2_PWM);
  servo3.attach(SERVO3_PWM);

  // Initialization:
  // - RGB should display a color corresponding to potentiometer initial value
  // - servos should move to initial positions
  int potInit = analogRead(POT_PIN);

  updateRGBFromPot(potInit);
  moveServosToPot(potInit);

  lastServoUpdateMs = millis();
}

void loop() {
  int potValue = analogRead(POT_PIN);

  // RGB should respond immediately
  updateRGBFromPot(potValue);

  // Servos: maintain position for at least 2 seconds before updating
  unsigned long now = millis();
  if (now - lastServoUpdateMs >= SERVO_MIN_HOLD_MS) {
    moveServosToPot(potValue);
    lastServoUpdateMs = now;
  }
}
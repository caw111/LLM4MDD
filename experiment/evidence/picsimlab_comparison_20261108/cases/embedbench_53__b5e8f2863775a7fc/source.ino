#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int SERVO1_PIN = 5;  // mega pin 5
const int SERVO2_PIN = 6;  // mega pin 6
const int SERVO3_PIN = 7;  // mega pin 7

// RGB LED pins (common cathode)
const int RGB_R_PIN = 2; // mega pin 2
const int RGB_G_PIN = 3; // mega pin 3
const int RGB_B_PIN = 4; // mega pin 4

// LED bar graph pins A1..A8 mapped to mega analog pins A1..A8
const int BAR_PINS[8] = {A1, A2, A3, A4, A5, A6, A7, A8};

int angle1 = 0;
int angle2 = 0;
int angle3 = 0;

// Keep track for 2-second stepping order
unsigned long lastStepMs = 0;
int currentServoIndex = 0; // 0->servo1, 1->servo2, 2->servo3

static uint8_t mapAngleToBrightness(int angleDeg) {
  if (angleDeg < 0) angleDeg = 0;
  if (angleDeg > 180) angleDeg = 180;
  // Linear mapping 0..180 -> 0..255
  return (uint8_t)((long)angleDeg * 255L / 180L);
}

static void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RGB_R_PIN, r);
  analogWrite(RGB_G_PIN, g);
  analogWrite(RGB_B_PIN, b);
}

static void updateBarGraphFromRGB(uint8_t r, uint8_t g, uint8_t b) {
  // Average brightness
  float avg = (r + g + b) / 3.0f; // 0..255
  // Map to 0..8 LEDs on bar graph
  // Determine count of LEDs to light: 0..8
  int ledsToLight = (int)((avg * 8.0f) / 255.0f + 0.5f); // nearest
  if (ledsToLight < 0) ledsToLight = 0;
  if (ledsToLight > 8) ledsToLight = 8;

  for (int i = 0; i < 8; i++) {
    // bargraph has common connected to GND. For "green" outputs:
    // likely active-high; we'll drive HIGH to illuminate an LED.
    digitalWrite(BAR_PINS[i], (i < ledsToLight) ? HIGH : LOW);
  }
}

static void applyFromServoAngles() {
  uint8_t r = mapAngleToBrightness(angle1);
  uint8_t g = mapAngleToBrightness(angle2);
  uint8_t b = mapAngleToBrightness(angle3);
  setRGB(r, g, b);
  updateBarGraphFromRGB(r, g, b);
}

void setup() {
  // Servo initialization: all start at 0°
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  angle1 = 0;
  angle2 = 0;
  angle3 = 0;

  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);

  // RGB LED initially off
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);
  analogWrite(RGB_R_PIN, 0);
  analogWrite(RGB_G_PIN, 0);
  analogWrite(RGB_B_PIN, 0);

  // Bar graph initially off
  for (int i = 0; i < 8; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  applyFromServoAngles();

  lastStepMs = millis();
  currentServoIndex = 0; // servo1 first
}

void loop() {
  unsigned long now = millis();
  if (now - lastStepMs >= 2000UL) {
    lastStepMs = now;

    // Move exactly one servo 180° in order: servo1 -> servo2 -> servo3 -> servo1 ...
    if (currentServoIndex == 0) {
      angle1 = (angle1 == 0) ? 180 : 0;
      servo1.write(angle1);
    } else if (currentServoIndex == 1) {
      angle2 = (angle2 == 0) ? 180 : 0;
      servo2.write(angle2);
    } else {
      angle3 = (angle3 == 0) ? 180 : 0;
      servo3.write(angle3);
    }

    currentServoIndex = (currentServoIndex + 1) % 3;

    // Update RGB and bar graph based on servo angles
    applyFromServoAngles();
  }
}
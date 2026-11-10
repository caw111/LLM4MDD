#include <Servo.h>

const int RGB1_R = 2;
const int RGB1_G = 3;
const int RGB1_B = 4;

const int RGB2_R = 5;
const int RGB2_G = 6;
const int RGB2_B = 7;

const int POT1_SIG = A3;

const int SERVO1_PWM = 8;
const int SERVO2_PWM = 9;
const int SERVO3_PWM = 10;

Servo servo1;
Servo servo2;
Servo servo3;

static uint8_t potToRed(int pot) {
  // Red: 0..341 -> 0..255 (clamp)
  if (pot <= 0) return 0;
  if (pot >= 341) return 255;
  return (uint32_t)pot * 255UL / 341UL;
}

static uint8_t potToGreen(int pot) {
  // Green: 342..682 -> 0..255 (clamp)
  if (pot <= 342) return 0;
  if (pot >= 682) return 255;
  return (uint32_t)(pot - 342) * 255UL / 340UL;
}

static uint8_t potToBlue(int pot) {
  // Blue: 683..1023 -> 0..255 (clamp)
  if (pot <= 683) return 0;
  if (pot >= 1023) return 255;
  return (uint32_t)(pot - 683) * 255UL / 340UL;
}

static int potToAngle(int pot) {
  // Map 0..1023 to 0..180 degrees
  if (pot <= 0) return 0;
  if (pot >= 1023) return 180;
  return (int)((uint32_t)pot * 180UL / 1023UL);
}

static void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RGB1_R, r);
  analogWrite(RGB1_G, g);
  analogWrite(RGB1_B, b);

  analogWrite(RGB2_R, r);
  analogWrite(RGB2_G, g);
  analogWrite(RGB2_B, b);
}

unsigned long lastServoUpdateMs = 0;
int lastServPot1 = -1;
int lastServPot2 = -1;
int lastServPot3 = -1;

void setup() {
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  servo1.attach(SERVO1_PWM);
  servo2.attach(SERVO2_PWM);
  servo3.attach(SERVO3_PWM);

  int pot = analogRead(POT1_SIG);

  uint8_t r = potToRed(pot);
  uint8_t g = potToGreen(pot);
  uint8_t b = potToBlue(pot);
  setRGB(r, g, b);

  int angle = potToAngle(pot);
  servo1.write(angle);
  servo2.write(angle);
  servo3.write(angle);

  lastServoUpdateMs = millis();
  lastServPot1 = pot;
  lastServPot2 = pot;
  lastServPot3 = pot;
}

void loop() {
  int pot = analogRead(POT1_SIG);

  // RGB LEDs respond immediately to changes in pot value
  uint8_t r = potToRed(pot);
  uint8_t g = potToGreen(pot);
  uint8_t b = potToBlue(pot);
  setRGB(r, g, b);

  // Servos: maintain position for at least 2 seconds before updating
  unsigned long now = millis();
  if (now - lastServoUpdateMs >= 2000UL) {
    int angle = potToAngle(pot);
    servo1.write(angle);
    servo2.write(angle);
    servo3.write(angle);

    lastServoUpdateMs = now;
    lastServPot1 = pot;
    lastServPot2 = pot;
    lastServPot3 = pot;
  }
}
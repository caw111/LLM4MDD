#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// RGB LED (common cathode)
// rgb1: R=2, G=3, B=4
const uint8_t rgb1R = 2;
const uint8_t rgb1G = 3;
const uint8_t rgb1B = 4;

// rgb2: R=5, G=6, B=7
const uint8_t rgb2R = 5;
const uint8_t rgb2G = 6;
const uint8_t rgb2B = 7;

// rgb3: R=8, G=9, B=10
const uint8_t rgb3R = 8;
const uint8_t rgb3G = 9;
const uint8_t rgb3B = 10;

struct ColorStep {
  const char *name;
  uint8_t r; // 0/1
  uint8_t g; // 0/1
  uint8_t b; // 0/1
  uint16_t s1;
  uint16_t s2;
  uint16_t s3;
};

const ColorStep sequence[] = {
  // Red: Servo1 0°, Servo2 45°, Servo3 90°
  {"Red",     1,0,0,  0,  45,  90},
  // Green: Servo1 45°, Servo2 90°, Servo3 135°
  {"Green",   0,1,0, 45,  90, 135},
  // Blue: Servo1 90°, Servo2 135°, Servo3 180°
  {"Blue",    0,0,1, 90, 135, 180},
  // Yellow: Servo1 135°, Servo2 180°, Servo3 0°
  {"Yellow",  1,1,0,135, 180,   0},
  // Cyan: Servo1 180°, Servo2 0°, Servo3 45°
  {"Cyan",    0,1,1,180,   0,  45},
  // Magenta: Servo1 0°, Servo2 45°, Servo3 90°
  {"Magenta", 1,0,1,  0,  45,  90},
  // White: Servo1 90°, Servo2 135°, Servo3 180°
  {"White",   1,1,1, 90, 135, 180}
};

static const size_t sequenceLen = sizeof(sequence) / sizeof(sequence[0]);

void setAllRGB(uint8_t r, uint8_t g, uint8_t b) {
  // Common cathode: drive HIGH to turn ON each color channel.
  digitalWrite(rgb1R, r ? HIGH : LOW);
  digitalWrite(rgb1G, g ? HIGH : LOW);
  digitalWrite(rgb1B, b ? HIGH : LOW);

  digitalWrite(rgb2R, r ? HIGH : LOW);
  digitalWrite(rgb2G, g ? HIGH : LOW);
  digitalWrite(rgb2B, b ? HIGH : LOW);

  digitalWrite(rgb3R, r ? HIGH : LOW);
  digitalWrite(rgb3G, g ? HIGH : LOW);
  digitalWrite(rgb3B, b ? HIGH : LOW);
}

void setServosTo(uint16_t a1, uint16_t a2, uint16_t a3) {
  servo1.write(a1);
  servo2.write(a2);
  servo3.write(a3);
}

void setup() {
  // RGB LED pins
  pinMode(rgb1R, OUTPUT); pinMode(rgb1G, OUTPUT); pinMode(rgb1B, OUTPUT);
  pinMode(rgb2R, OUTPUT); pinMode(rgb2G, OUTPUT); pinMode(rgb2B, OUTPUT);
  pinMode(rgb3R, OUTPUT); pinMode(rgb3G, OUTPUT); pinMode(rgb3B, OUTPUT);

  // All RGB LEDs off upon power on/reset
  setAllRGB(0, 0, 0);

  // Servos: PWM pins per circuit
  // servo1:PWM -> mega:11
  // servo2:PWM -> mega:12
  // servo3:PWM -> mega:13
  servo1.attach(11);
  servo2.attach(12);
  servo3.attach(13);

  // All servos to 0° upon power on/reset
  setServosTo(0, 0, 0);
  delay(50);
}

void loop() {
  for (size_t i = 0; i < sequenceLen; ++i) {
    const ColorStep &step = sequence[i];

    // Set color immediately; servos move to corresponding angles when the color changes
    setAllRGB(step.r, step.g, step.b);
    setServosTo(step.s1, step.s2, step.s3);

    delay(2000); // each color displayed for 2 seconds
  }
}
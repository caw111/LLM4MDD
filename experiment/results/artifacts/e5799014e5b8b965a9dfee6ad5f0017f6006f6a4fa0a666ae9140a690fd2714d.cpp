#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// RGB LED pins (common cathode)
const uint8_t RGB1_R = 2;
const uint8_t RGB1_G = 3;
const uint8_t RGB1_B = 4;

const uint8_t RGB2_R = 5;
const uint8_t RGB2_G = 6;
const uint8_t RGB2_B = 7;

const uint8_t RGB3_R = 8;
const uint8_t RGB3_G = 9;
const uint8_t RGB3_B = 10;

const uint16_t STEP_MS = 2000;

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint8_t r, uint8_t g, uint8_t b);
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void allRGBOff();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setServosForColor(int s1, int s2, int s3);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint8_t r, uint8_t g, uint8_t b) {
  // Common cathode: HIGH turns a channel off? Actually with common cathode, segments are driven HIGH to light.
  // In Wokwi RGB LED, typical behavior: with common=cathode, each color is active HIGH.
  digitalWrite(rPin, r ? HIGH : LOW);
  digitalWrite(gPin, g ? HIGH : LOW);
  digitalWrite(bPin, b ? HIGH : LOW);
}

void allRGBOff() {
  // Ensure all channels are off (LOW for common cathode in Wokwi)
  digitalWrite(RGB1_R, LOW); digitalWrite(RGB1_G, LOW); digitalWrite(RGB1_B, LOW);
  digitalWrite(RGB2_R, LOW); digitalWrite(RGB2_G, LOW); digitalWrite(RGB2_B, LOW);
  digitalWrite(RGB3_R, LOW); digitalWrite(RGB3_G, LOW); digitalWrite(RGB3_B, LOW);
}

void setServosForColor(int s1, int s2, int s3) {
  servo1.write(s1);
  servo2.write(s2);
  servo3.write(s3);
}

struct ColorStep {
  uint8_t r, g, b;
  int s1, s2, s3;
};

const ColorStep sequence[] = {
  // red
  {1, 0, 0,   0,  45,  90},
  // green
  {0, 1, 0,   45, 90, 135},
  // blue
  {0, 0, 1,   90, 135, 180},
  // yellow (R+G)
  {1, 1, 0,   135, 180, 0},
  // cyan (G+B)
  {0, 1, 1,   180, 0,   45},
  // magenta (R+B)
  {1, 0, 1,   0,   45,  90},
  // white (R+G+B)
  {1, 1, 1,   90,  135, 180}
};

void setup() {
  // RGB LED pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);

  allRGBOff();

  // Servos (wiring from circuit)
  // servo1:PWM -> mega:11
  // servo2:PWM -> mega:12
  // servo3:PWM -> mega:13
  servo1.attach(11);
  servo2.attach(12);
  servo3.attach(13);

  // Initialization: servos at 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  delay(50); // brief settle
}

void loop() {
  const uint8_t steps = sizeof(sequence) / sizeof(sequence[0]);

  for (uint8_t i = 0; i < steps; i++) {
    // Set all RGB LEDs to the current color
    setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, sequence[i].r, sequence[i].g, sequence[i].b);
    setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, sequence[i].r, sequence[i].g, sequence[i].b);
    setRGBCommonCathode(RGB3_R, RGB3_G, RGB3_B, sequence[i].r, sequence[i].g, sequence[i].b);

    // When the color changes, move servos to the corresponding angles
    setServosForColor(sequence[i].s1, sequence[i].s2, sequence[i].s3);

    delay(STEP_MS);
  }
}

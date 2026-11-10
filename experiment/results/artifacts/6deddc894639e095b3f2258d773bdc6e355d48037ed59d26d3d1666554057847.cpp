#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LED1_PIN = 2;   // led1:A
const uint8_t LED2_PIN = 3;   // led2:A

// rgb1: COM -> GND, outputs on R/G/B
const uint8_t RGB1_R_PIN = 4; // rgb1:R
const uint8_t RGB1_G_PIN = 5; // rgb1:G
const uint8_t RGB1_B_PIN = 6; // rgb1:B

// rgb2: COM -> GND, outputs on R/G/B
const uint8_t RGB2_R_PIN = 7; // rgb2:R
const uint8_t RGB2_G_PIN = 8; // rgb2:G
const uint8_t RGB2_B_PIN = 9; // rgb2:B

// bargraph1 segments (all referenced as A1..A6)
const uint8_t BAR_A1_PIN = 10; // bargraph1:A1
const uint8_t BAR_A2_PIN = 11; // bargraph1:A2
const uint8_t BAR_A3_PIN = 12; // bargraph1:A3
const uint8_t BAR_A4_PIN = 13; // bargraph1:A4
const uint8_t BAR_A5_PIN = 44; // bargraph1:A5
const uint8_t BAR_A6_PIN = 45; // bargraph1:A6

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllOff();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB1(uint8_t r, uint8_t g, uint8_t b);
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB2(uint8_t r, uint8_t g, uint8_t b);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarSegments(bool s1, bool s2, bool s3, bool s4, bool s5, bool s6);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setAllOff() {
  // Mono LEDs
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // RGB LEDs
  digitalWrite(RGB1_R_PIN, LOW);
  digitalWrite(RGB1_G_PIN, LOW);
  digitalWrite(RGB1_B_PIN, LOW);

  digitalWrite(RGB2_R_PIN, LOW);
  digitalWrite(RGB2_G_PIN, LOW);
  digitalWrite(RGB2_B_PIN, LOW);

  // Bar graph segments
  digitalWrite(BAR_A1_PIN, LOW);
  digitalWrite(BAR_A2_PIN, LOW);
  digitalWrite(BAR_A3_PIN, LOW);
  digitalWrite(BAR_A4_PIN, LOW);
  digitalWrite(BAR_A5_PIN, LOW);
  digitalWrite(BAR_A6_PIN, LOW);
}

static inline void setRGB1(uint8_t r, uint8_t g, uint8_t b) {
  digitalWrite(RGB1_R_PIN, r ? HIGH : LOW);
  digitalWrite(RGB1_G_PIN, g ? HIGH : LOW);
  digitalWrite(RGB1_B_PIN, b ? HIGH : LOW);
}

static inline void setRGB2(uint8_t r, uint8_t g, uint8_t b) {
  digitalWrite(RGB2_R_PIN, r ? HIGH : LOW);
  digitalWrite(RGB2_G_PIN, g ? HIGH : LOW);
  digitalWrite(RGB2_B_PIN, b ? HIGH : LOW);
}

static inline void setBarSegments(bool s1, bool s2, bool s3, bool s4, bool s5, bool s6) {
  digitalWrite(BAR_A1_PIN, s1 ? HIGH : LOW);
  digitalWrite(BAR_A2_PIN, s2 ? HIGH : LOW);
  digitalWrite(BAR_A3_PIN, s3 ? HIGH : LOW);
  digitalWrite(BAR_A4_PIN, s4 ? HIGH : LOW);
  digitalWrite(BAR_A5_PIN, s5 ? HIGH : LOW);
  digitalWrite(BAR_A6_PIN, s6 ? HIGH : LOW);
}

void setup() {
  // Mono LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // RGB LEDs
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  // Bar graph segments
  pinMode(BAR_A1_PIN, OUTPUT);
  pinMode(BAR_A2_PIN, OUTPUT);
  pinMode(BAR_A3_PIN, OUTPUT);
  pinMode(BAR_A4_PIN, OUTPUT);
  pinMode(BAR_A5_PIN, OUTPUT);
  pinMode(BAR_A6_PIN, OUTPUT);

  // Initialization: all off
  setAllOff();
}

void loop() {
  // State 1 (2 seconds)
  // led1 on, led2 off; rgb1 red; rgb2 off; bargraph A1-A3 on
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);

  setRGB1(1, 0, 0); // red
  setRGB2(0, 0, 0); // ensure off per "goes out" for rgb2 in state 1 (not specified to light)

  setBarSegments(true, true, true, false, false, false); // A1-A3 on
  delay(2000);

  // State 2 (2 seconds)
  // led2 on, led1 off; rgb2 green; rgb1 off; bargraph A4-A6 on
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);

  setRGB1(0, 0, 0); // ensure off
  setRGB2(0, 1, 0); // green

  setBarSegments(false, false, false, true, true, true); // A4-A6 on
  delay(2000);

  // State 3 (2 seconds)
  // both mono LEDs on; both rgb LEDs cyan (blue+green); all bar segments on
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);

  setRGB1(0, 1, 1); // cyan
  setRGB2(0, 1, 1); // cyan

  setBarSegments(true, true, true, true, true, true); // A1-A6 on
  delay(2000);
}

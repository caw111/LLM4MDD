#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_51\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

static const uint8_t RGB3_R = 8;
static const uint8_t RGB3_G = 9;
static const uint8_t RGB3_B = 10;

static const uint8_t BAR_A1 = A1;
static const uint8_t BAR_A2 = A2;
static const uint8_t BAR_A3 = A3;
static const uint8_t BAR_A4 = A4;
static const uint8_t BAR_A5 = A5;

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_51\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint8_t r, uint8_t g, uint8_t b);
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_51\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarSegments(uint8_t segmentsOn);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_51\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_51\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_51\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint8_t r, uint8_t g, uint8_t b) {
  // common cathode => HIGH turns OFF, LOW turns ON.
  digitalWrite(rPin, r ? LOW : HIGH);
  digitalWrite(gPin, g ? LOW : HIGH);
  digitalWrite(bPin, b ? LOW : HIGH);
}

static void setBarSegments(uint8_t segmentsOn) {
  // bar graph lights up from pin:A1 to pin:A5
  digitalWrite(BAR_A1, (segmentsOn >= 1) ? HIGH : LOW);
  digitalWrite(BAR_A2, (segmentsOn >= 2) ? HIGH : LOW);
  digitalWrite(BAR_A3, (segmentsOn >= 3) ? HIGH : LOW);
  digitalWrite(BAR_A4, (segmentsOn >= 4) ? HIGH : LOW);
  digitalWrite(BAR_A5, (segmentsOn >= 5) ? HIGH : LOW);
}

void setup() {
  // RGB LEDs (common cathode)
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);

  // Initialize RGB outputs to OFF (HIGH for common cathode)
  setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, 0, 0, 0);
  setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, 0, 0, 0);
  setRgbCommonCathode(RGB3_R, RGB3_G, RGB3_B, 0, 0, 0);

  // Bar graph segments (A1..A5)
  pinMode(BAR_A1, OUTPUT);
  pinMode(BAR_A2, OUTPUT);
  pinMode(BAR_A3, OUTPUT);
  pinMode(BAR_A4, OUTPUT);
  pinMode(BAR_A5, OUTPUT);

  // Initialize bar segments to OFF
  setBarSegments(0);
}

void loop() {
  // Phase 1: 2 seconds - RGB1 red, bargraph 1 segment
  setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, 1, 0, 0);
  setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, 0, 0, 0);
  setRgbCommonCathode(RGB3_R, RGB3_G, RGB3_B, 0, 0, 0);
  setBarSegments(1);
  delay(2000);

  // Phase 2: 2 seconds - RGB2 green, bargraph 2 segments
  setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, 0, 0, 0);
  setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, 0, 1, 0);
  setRgbCommonCathode(RGB3_R, RGB3_G, RGB3_B, 0, 0, 0);
  setBarSegments(2);
  delay(2000);

  // Phase 3: 2 seconds - RGB3 blue, bargraph 3 segments
  setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, 0, 0, 0);
  setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, 0, 0, 0);
  setRgbCommonCathode(RGB3_R, RGB3_G, RGB3_B, 0, 0, 1);
  setBarSegments(3);
  delay(2000);

  // Phase 4: 2 seconds - RGB1 red + RGB2 green, bargraph 4 segments
  setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, 1, 0, 0);
  setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, 0, 1, 0);
  setRgbCommonCathode(RGB3_R, RGB3_G, RGB3_B, 0, 0, 0);
  setBarSegments(4);
  delay(2000);

  // Phase 5: 2 seconds - RGB1+RGB2+RGB3 white, bargraph 5 segments
  setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, 1, 1, 1);
  setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, 1, 1, 1);
  setRgbCommonCathode(RGB3_R, RGB3_G, RGB3_B, 1, 1, 1);
  setBarSegments(5);
  delay(2000);
}

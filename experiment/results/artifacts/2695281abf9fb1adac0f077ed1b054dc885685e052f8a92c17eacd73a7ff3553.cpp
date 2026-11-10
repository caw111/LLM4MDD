#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// RGB LED control for Arduino Mega 2560 (common cathode LEDs)
// Wiring preserved:
// rgb1: COM->GND.1, R->2, G->3, B->4
// rgb2: COM->GND.1, R->5, G->6, B->7
// rgb3: COM->GND.1, R->8, G->9, B->10

// rgb1 pins
const uint8_t RGB1_R = 2;
const uint8_t RGB1_G = 3;
const uint8_t RGB1_B = 4;

// rgb2 pins
const uint8_t RGB2_R = 5;
const uint8_t RGB2_G = 6;
const uint8_t RGB2_B = 7;

// rgb3 pins
const uint8_t RGB3_R = 8;
const uint8_t RGB3_G = 9;
const uint8_t RGB3_B = 10;

const unsigned long COLOR_DURATION_MS = 2000;

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool rOn, bool gOn, bool bOn);
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setRGBCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin,
                                        bool rOn, bool gOn, bool bOn) {
  // Common cathode: HIGH = OFF, LOW = ON
  digitalWrite(rPin, rOn ? LOW : HIGH);
  digitalWrite(gPin, gOn ? LOW : HIGH);
  digitalWrite(bPin, bOn ? LOW : HIGH);
}

void setup() {
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);

  // Initialization: all LEDs off
  setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, false, false);
  setRGBCommonCathode(RGB3_R, RGB3_G, RGB3_B, false, false, false);
}

void loop() {
  // 1st Pattern:
  // LED1 red, LED2 blue, LED3 green
  setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, true, false, false);
  setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, false, true);
  setRGBCommonCathode(RGB3_R, RGB3_G, RGB3_B, false, true, false);
  delay(COLOR_DURATION_MS);

  // 2nd Pattern:
  // LED1 green, LED2 red, LED3 blue
  setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, true, false);
  setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, true, false, false);
  setRGBCommonCathode(RGB3_R, RGB3_G, RGB3_B, false, false, true);
  delay(COLOR_DURATION_MS);

  // 3rd Pattern:
  // LED1 blue, LED2 green, LED3 red
  setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, false, true);
  setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, true, false);
  setRGBCommonCathode(RGB3_R, RGB3_G, RGB3_B, true, false, false);
  delay(COLOR_DURATION_MS);
}

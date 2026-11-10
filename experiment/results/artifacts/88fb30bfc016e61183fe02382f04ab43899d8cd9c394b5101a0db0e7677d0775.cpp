#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const int POT_PIN = A3;

const int LED_R_PIN = 2;
const int LED_G_PIN = 3;
const int LED_B_PIN = 4;

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbOff();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbRed();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbGreen();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbBlue();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_6\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setRgbOff() {
  // Common cathode RGB LED:
  // HIGH = OFF, LOW = ON (because anodes are driven; COM is at GND)
  digitalWrite(LED_R_PIN, HIGH);
  digitalWrite(LED_G_PIN, HIGH);
  digitalWrite(LED_B_PIN, HIGH);
}

static inline void setRgbRed() {
  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, HIGH);
  digitalWrite(LED_B_PIN, HIGH);
}

static inline void setRgbGreen() {
  digitalWrite(LED_R_PIN, HIGH);
  digitalWrite(LED_G_PIN, LOW);
  digitalWrite(LED_B_PIN, HIGH);
}

static inline void setRgbBlue() {
  digitalWrite(LED_R_PIN, HIGH);
  digitalWrite(LED_G_PIN, HIGH);
  digitalWrite(LED_B_PIN, LOW);
}

void setup() {
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);

  setRgbOff();
}

void loop() {
  int val = analogRead(POT_PIN); // 0..1023
  int third = 1024 / 3;          // 341

  if (val < third) {
    setRgbRed();
  } else if (val < 2 * third) {
    setRgbGreen();
  } else {
    setRgbBlue();
  }
}

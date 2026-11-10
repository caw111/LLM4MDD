#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const int LED_PIN = 2;      // led1:A -> mega:2 (PWM capable on Mega)
const int POT1_PIN = A3;    // pot1:SIG -> mega:A3
const int POT2_PIN = A4;    // pot2:SIG -> mega:A4
const int POT3_PIN = A5;    // pot3:SIG -> mega:A5

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readPot01(int pin);
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readPot01(int pin) {
  int raw = analogRead(pin); // 0..1023
  return (float)raw / 1023.0f;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0); // LED off on startup/reset
}

void loop() {
  float pot1 = readPot01(POT1_PIN); // upper bound [0,1]
  float pot2 = readPot01(POT2_PIN); // lower bound [0,1]
  float pot3 = readPot01(POT3_PIN); // actual level mapped into [pot2, pot1]

  if (pot1 >= pot2) {
    float level = pot2 + pot3 * (pot1 - pot2); // mapped [pot2, pot1]
    level = constrain(level, 0.0f, 1.0f);

    int brightness = (int)lroundf(level * 255.0f); // PWM 0..255
    brightness = constrain(brightness, 0, 255);

    analogWrite(LED_PIN, brightness);
  } else {
    analogWrite(LED_PIN, 0); // upper < lower => off
  }
}

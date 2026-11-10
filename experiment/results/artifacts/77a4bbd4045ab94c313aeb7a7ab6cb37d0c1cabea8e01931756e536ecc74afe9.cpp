#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LED_PIN  = 2;   // mega:2 -> led1:A (red)
const uint8_t POT1_PIN = A3;  // pot1:SIG -> mega:A3
const uint8_t POT2_PIN = A4;  // pot2:SIG -> mega:A4
const uint8_t POT3_PIN = A5;  // pot3:SIG -> mega:A5

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readPot01(uint8_t pin);
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_7\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readPot01(uint8_t pin) {
  int raw = analogRead(pin); // 0..1023 on Mega
  return (float)raw / 1023.0f; // map uniformly to 0..1
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0); // LED off on power-up/reset
}

void loop() {
  float pot1 = readPot01(POT1_PIN); // upper bound
  float pot2 = readPot01(POT2_PIN); // lower bound
  float pot3 = readPot01(POT3_PIN); // actual brightness control

  float brightness01 = 0.0f;

  if (pot1 >= pot2) {
    // Map pot3 uniformly from pot2..pot1
    brightness01 = pot2 + (pot1 - pot2) * pot3;
  } else {
    // upper bound < lower bound => LED remains off
    brightness01 = 0.0f;
  }

  // Convert 0..1 to PWM duty (Mega uses 0..255)
  if (brightness01 < 0.0f) brightness01 = 0.0f;
  if (brightness01 > 1.0f) brightness01 = 1.0f;

  uint8_t duty = (uint8_t)lrintf(brightness01 * 255.0f);
  analogWrite(LED_PIN, duty);

  // Update continuously
  delay(5);
}

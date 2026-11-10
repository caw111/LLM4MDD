#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;

const uint8_t BAR_A_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // A1..A10
// C1..C10 are all connected to GND in the provided circuit.

const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;
const uint8_t POT3_PIN = A5;

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t mapTo255(int v10bit);
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline uint8_t mapTo255(int v10bit) {
  // v10bit is 0..1023
  // Map uniformly to 0..255
  return (uint8_t)((v10bit * 255L) / 1023L);
}

void setup() {
  // Ensure all outputs start OFF
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }

  // Potentiometers are analog inputs; no need to set pinMode explicitly
}

void loop() {
  // Read potentiometers
  int pot1 = analogRead(POT1_PIN); // 0..1023
  int pot2 = analogRead(POT2_PIN); // 0..1023
  int pot3 = analogRead(POT3_PIN); // 0..1023

  // Brightness control for individual LEDs
  uint8_t led1Brightness = mapTo255(pot1);
  uint8_t led2Brightness = mapTo255(pot2);

  // Bargraph segment count: map uniformly from 0..10
  // pot3 (0..1023) -> [0..10]
  uint8_t segments = (uint8_t)((pot3 * 10L) / 1023L); // 0..10

  // Update LEDs continuously using PWM
  // Note: pins 2 and 3 on Mega2560 support PWM.
  analogWrite(LED1_PIN, led1Brightness);
  analogWrite(LED2_PIN, led2Brightness);

  // Update bargraph:
  // segments == 0 => all off
  // segments == 10 => all on
  // Turn on A1..Asegments.
  for (uint8_t i = 0; i < 10; i++) {
    if (i < segments) {
      digitalWrite(BAR_A_PINS[i], HIGH);
    } else {
      digitalWrite(BAR_A_PINS[i], LOW);
    }
  }
}

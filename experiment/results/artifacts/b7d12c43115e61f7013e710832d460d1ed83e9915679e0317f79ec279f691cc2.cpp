#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_112\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring; RGB LEDs are common-cathode.
const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;

const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;
const uint8_t LED3_PIN = 4;

const uint8_t RGB1_R_PIN = 5;
const uint8_t RGB1_G_PIN = 6;
const uint8_t RGB1_B_PIN = 7;

const uint8_t RGB2_R_PIN = 8;
const uint8_t RGB2_G_PIN = 9;
const uint8_t RGB2_B_PIN = 10;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_112\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_112\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_112\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  const uint8_t outputPins[] = {
    LED1_PIN, LED2_PIN, LED3_PIN,
    RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN,
    RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN
  };

  for (uint8_t i = 0; i < sizeof(outputPins) / sizeof(outputPins[0]); ++i) {
    pinMode(outputPins[i], OUTPUT);
    analogWrite(outputPins[i], 0);
  }
}

void loop() {
  const int pot1 = analogRead(POT1_PIN);
  const int pot2 = analogRead(POT2_PIN);
  const bool bothAtMaximum = (pot1 == 1023 && pot2 == 1023);

  const uint8_t red = static_cast<uint8_t>(map(pot1, 0, 1023, 0, 255));
  const uint8_t green = static_cast<uint8_t>(map(pot2, 0, 1023, 0, 255));

  // Blue follows the lower component for a smooth combined gradient.
  // At both maxima, red, green, and blue are all 255 (white).
  const uint8_t blue = (red < green) ? red : green;

  analogWrite(LED1_PIN, red);
  analogWrite(LED2_PIN, green);
  digitalWrite(LED3_PIN, bothAtMaximum ? HIGH : LOW);

  analogWrite(RGB1_R_PIN, red);
  analogWrite(RGB1_G_PIN, green);
  analogWrite(RGB1_B_PIN, blue);

  analogWrite(RGB2_R_PIN, red);
  analogWrite(RGB2_G_PIN, green);
  analogWrite(RGB2_B_PIN, blue);
}

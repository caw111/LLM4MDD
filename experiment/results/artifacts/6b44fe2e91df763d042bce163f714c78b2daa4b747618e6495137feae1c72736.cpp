#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_23\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t POT_PIN = A3;
constexpr uint8_t RED_PIN = 2;
constexpr uint8_t GREEN_PIN = 3;
constexpr uint8_t BLUE_PIN = 4;

// Common-cathode RGB LED: HIGH turns a color channel on.
const uint8_t COLORS[8][3] = {
  {LOW,  LOW,  LOW },  //    0-127:  Off
  {HIGH, LOW,  LOW },  //  128-255:  Red
  {LOW,  HIGH, LOW },  //  256-383:  Green
  {LOW,  LOW,  HIGH},  //  384-511:  Blue
  {HIGH, HIGH, LOW },  //  512-639:  Yellow
  {HIGH, LOW,  HIGH},  //  640-767:  Magenta
  {LOW,  HIGH, HIGH},  //  768-895:  Cyan
  {HIGH, HIGH, HIGH}   // 896-1023:  White
};

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_23\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_23\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_23\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(POT_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void loop() {
  const uint8_t colorIndex = analogRead(POT_PIN) / 128;

  digitalWrite(RED_PIN, COLORS[colorIndex][0]);
  digitalWrite(GREEN_PIN, COLORS[colorIndex][1]);
  digitalWrite(BLUE_PIN, COLORS[colorIndex][2]);
}

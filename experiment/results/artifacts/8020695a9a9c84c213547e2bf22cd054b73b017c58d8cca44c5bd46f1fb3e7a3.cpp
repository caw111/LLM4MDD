#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_7\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t LED_PIN = 2;  // PWM-capable on Arduino Mega 2560
constexpr uint8_t UPPER_BOUND_PIN = A3;
constexpr uint8_t LOWER_BOUND_PIN = A4;
constexpr uint8_t BRIGHTNESS_PIN = A5;

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_7\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_7\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_7\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  pinMode(UPPER_BOUND_PIN, INPUT);
  pinMode(LOWER_BOUND_PIN, INPUT);
  pinMode(BRIGHTNESS_PIN, INPUT);
}

void loop() {
  const int upperRaw = analogRead(UPPER_BOUND_PIN);
  const int lowerRaw = analogRead(LOWER_BOUND_PIN);
  const int brightnessRaw = analogRead(BRIGHTNESS_PIN);

  if (upperRaw < lowerRaw) {
    analogWrite(LED_PIN, 0);
    return;
  }

  const float upperBound = upperRaw / 1023.0f;
  const float lowerBound = lowerRaw / 1023.0f;
  const float position = brightnessRaw / 1023.0f;

  const float brightness =
      lowerBound + position * (upperBound - lowerBound);

  const int pwmValue = constrain(
      static_cast<int>(brightness * 255.0f + 0.5f), 0, 255);

  analogWrite(LED_PIN, pwmValue);
}

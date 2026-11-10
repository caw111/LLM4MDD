#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_111\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring; all LEDs are active HIGH.
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr uint8_t LED3_PIN = 4;
constexpr uint8_t RGB_RED_PIN = 5;
constexpr uint8_t RGB_GREEN_PIN = 6;
constexpr uint8_t RGB_BLUE_PIN = 7;
constexpr uint8_t POT_PIN = A3;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_111\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_111\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_111\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  const uint8_t outputPins[] = {
    LED1_PIN, LED2_PIN, LED3_PIN,
    RGB_RED_PIN, RGB_GREEN_PIN, RGB_BLUE_PIN
  };

  // Initialize every LED off before enabling its output.
  for (uint8_t pin : outputPins) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
  }

  analogWrite(RGB_RED_PIN, 0);
  analogWrite(RGB_GREEN_PIN, 0);
  analogWrite(RGB_BLUE_PIN, 0);

  pinMode(POT_PIN, INPUT);
}

void loop() {
  const int potValue = analogRead(POT_PIN);
  const uint8_t brightness =
      static_cast<uint8_t>(map(potValue, 0, 1023, 0, 255));

  const bool lowerThird = potValue <= 341;
  const bool middleThird = potValue >= 342 && potValue <= 682;
  const bool upperThird = potValue >= 683;

  digitalWrite(LED1_PIN, lowerThird ? HIGH : LOW);
  digitalWrite(LED2_PIN, middleThird ? HIGH : LOW);
  digitalWrite(LED3_PIN, upperThird ? HIGH : LOW);

  // Apply the mapped brightness to each channel, with the channel
  // designated by the current range overridden to maximum brightness.
  analogWrite(RGB_RED_PIN, lowerThird ? 255 : brightness);
  analogWrite(RGB_GREEN_PIN, middleThird ? 255 : brightness);
  analogWrite(RGB_BLUE_PIN, upperThird ? 255 : brightness);
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_92\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t LED_PIN = 2;  // PWM-capable on Arduino Mega 2560
constexpr uint8_t POT1_PIN = A3;
constexpr uint8_t POT2_PIN = A4;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_92\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_92\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_92\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
}

void loop() {
  const int brightnessReading = analogRead(POT1_PIN);
  const int thresholdReading = analogRead(POT2_PIN);
  const int brightness = map(brightnessReading, 0, 1023, 0, 255);

  // Compare potentiometers in their shared 0–1023 range.
  analogWrite(LED_PIN,
              brightnessReading > thresholdReading ? brightness : 0);
}

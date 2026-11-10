#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_55\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring; both RGB LEDs are common cathode.
const uint8_t RGB1_PINS[3] = {2, 3, 4};  // Red, green, blue
const uint8_t RGB2_PINS[3] = {5, 6, 7};
const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;

// Only these eight bar-graph segments are wired.
const uint8_t BAR_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45};
const uint8_t BAR_COUNT = sizeof(BAR_PINS) / sizeof(BAR_PINS[0]);

int8_t previousColor1 = -1;
int8_t previousColor2 = -1;
int8_t previousBarCount = -1;

#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_55\\direct\\attempt_0\\sketch\\sketch.ino"
void initializeOutputOff(uint8_t pin);
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_55\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t selectColor(int reading);
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_55\\direct\\attempt_0\\sketch\\sketch.ino"
void setRgbColor(const uint8_t pins[3], uint8_t color);
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_55\\direct\\attempt_0\\sketch\\sketch.ino"
void setBarGraph(uint8_t count);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_55\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_55\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_55\\direct\\attempt_0\\sketch\\sketch.ino"
void initializeOutputOff(uint8_t pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

uint8_t selectColor(int reading) {
  if (reading <= 340) {
    return 0;  // Red
  }
  if (reading <= 681) {
    return 1;  // Green
  }
  return 2;    // Blue
}

void setRgbColor(const uint8_t pins[3], uint8_t color) {
  // Turn every channel off before enabling the selected primary.
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(pins[i], LOW);
  }
  digitalWrite(pins[color], HIGH);
}

void setBarGraph(uint8_t count) {
  for (uint8_t i = 0; i < BAR_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], i < count ? HIGH : LOW);
  }
}

void setup() {
  // Initialize all connected LED channels to off.
  for (uint8_t i = 0; i < 3; ++i) {
    initializeOutputOff(RGB1_PINS[i]);
    initializeOutputOff(RGB2_PINS[i]);
  }

  for (uint8_t i = 0; i < BAR_COUNT; ++i) {
    initializeOutputOff(BAR_PINS[i]);
  }

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
}

void loop() {
  const int pot1 = analogRead(POT1_PIN);
  const int pot2 = analogRead(POT2_PIN);

  const uint8_t color1 = selectColor(pot1);
  const uint8_t color2 = selectColor(pot2);

  // Equivalent to floor(average * 8 / 1023), without losing a
  // half-step when computing the average. An average of 512 lights 4.
  const uint8_t litSegments =
      (static_cast<uint32_t>(pot1 + pot2) * BAR_COUNT) / 2046UL;

  if (color1 != previousColor1) {
    setRgbColor(RGB1_PINS, color1);
    previousColor1 = color1;
  }

  if (color2 != previousColor2) {
    setRgbColor(RGB2_PINS, color2);
    previousColor2 = color2;
  }

  if (litSegments != previousBarCount) {
    setBarGraph(litSegments);
    previousBarCount = litSegments;
  }

  // No delay: adjustments are reflected immediately. With stationary
  // potentiometers, outputs remain latched indefinitely (including
  // for at least two seconds), without interrupting input monitoring.
}

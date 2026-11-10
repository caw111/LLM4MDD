#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_18\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: common-cathode RGB LEDs (HIGH = on).
const uint8_t LED_PINS[3][3] = {
  {2, 3, 4},   // rgb1: red, green, blue
  {5, 6, 7},   // rgb2: red, green, blue
  {8, 9, 10}   // rgb3: red, green, blue
};

// Channel indices: 0 = red, 1 = green, 2 = blue.
const uint8_t PATTERNS[3][3] = {
  {0, 2, 1},  // rgb1 red,   rgb2 blue,  rgb3 green
  {1, 0, 2},  // rgb1 green, rgb2 red,   rgb3 blue
  {2, 1, 0}   // rgb1 blue,  rgb2 green, rgb3 red
};

const unsigned long PATTERN_DURATION_MS = 2000UL;
uint8_t currentPattern = 0;

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_18\\direct\\attempt_0\\sketch\\sketch.ino"
void allOff();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_18\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_18\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_18\\direct\\attempt_0\\sketch\\sketch.ino"
void allOff() {
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(LED_PINS[led][channel], LOW);
    }
  }
}

void setup() {
  // Preload LOW before enabling outputs to avoid startup flashes.
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(LED_PINS[led][channel], LOW);
      pinMode(LED_PINS[led][channel], OUTPUT);
    }
  }
  allOff();
}

void loop() {
  // Clear the previous colors before applying the next pattern.
  allOff();

  for (uint8_t led = 0; led < 3; ++led) {
    digitalWrite(LED_PINS[led][PATTERNS[currentPattern][led]], HIGH);
  }

  delay(PATTERN_DURATION_MS);
  currentPattern = (currentPattern + 1) % 3;
}

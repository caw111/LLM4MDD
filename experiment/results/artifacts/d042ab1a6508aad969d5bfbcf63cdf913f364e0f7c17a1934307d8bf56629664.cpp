#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_114\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring; all LEDs are active HIGH.
const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;

const uint8_t RGB1_R_PIN = 4;
const uint8_t RGB1_G_PIN = 5;
const uint8_t RGB1_B_PIN = 6;

const uint8_t RGB2_R_PIN = 7;
const uint8_t RGB2_G_PIN = 8;
const uint8_t RGB2_B_PIN = 9;

// Bar graph terminals A1-A6, not Arduino analog pins.
const uint8_t BAR_PINS[6] = {10, 11, 12, 13, 44, 45};

const uint8_t ALL_PINS[] = {
  LED1_PIN, LED2_PIN,
  RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN,
  RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN,
  10, 11, 12, 13, 44, 45
};

const unsigned long STATE_DURATION_MS = 2000UL;

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_114\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, bool red, bool green, bool blue);
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_114\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(uint8_t state);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_114\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_114\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_114\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(uint8_t redPin, uint8_t greenPin, uint8_t bluePin,
            bool red, bool green, bool blue) {
  digitalWrite(redPin, red ? HIGH : LOW);
  digitalWrite(greenPin, green ? HIGH : LOW);
  digitalWrite(bluePin, blue ? HIGH : LOW);
}

void applyState(uint8_t state) {
  digitalWrite(LED1_PIN, state != 1 ? HIGH : LOW);
  digitalWrite(LED2_PIN, state != 0 ? HIGH : LOW);

  switch (state) {
    case 0:  // State 1: LED1, RGB1 red, first three segments.
      setRGB(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN, true, false, false);
      setRGB(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN, false, false, false);
      break;

    case 1:  // State 2: LED2, RGB2 green, last three segments.
      setRGB(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN, false, false, false);
      setRGB(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN, false, true, false);
      break;

    case 2:  // State 3: both LEDs, both RGB LEDs cyan, all segments.
      setRGB(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN, false, true, true);
      setRGB(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN, false, true, true);
      break;
  }

  for (uint8_t i = 0; i < 6; ++i) {
    bool on = (state == 2) ||
              (state == 0 && i < 3) ||
              (state == 1 && i >= 3);
    digitalWrite(BAR_PINS[i], on ? HIGH : LOW);
  }
}

void setup() {
  // Clear each output latch before enabling the output.
  for (uint8_t i = 0; i < sizeof(ALL_PINS) / sizeof(ALL_PINS[0]); ++i) {
    digitalWrite(ALL_PINS[i], LOW);
    pinMode(ALL_PINS[i], OUTPUT);
  }
}

void loop() {
  for (uint8_t state = 0; state < 3; ++state) {
    applyState(state);
    delay(STATE_DURATION_MS);
  }
}

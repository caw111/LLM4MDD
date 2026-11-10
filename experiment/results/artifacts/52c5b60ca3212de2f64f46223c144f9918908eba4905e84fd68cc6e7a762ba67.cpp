#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_115\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
const uint8_t LED_PINS[3] = {2, 3, 4};
const uint8_t RGB_PINS[2][3] = {
  {5, 6, 7},   // rgb1: red, green, blue
  {8, 9, 10}   // rgb2: red, green, blue
};

const uint8_t DATA_PIN  = A0;
const uint8_t LATCH_PIN = A1;
const uint8_t CLOCK_PIN = A2;

const unsigned long STEP_INTERVAL_MS = 2000UL;

// Bits 0..6 correspond to segments A..G.
// The display commons are connected to +5V, so segments are active LOW.
const uint8_t DIGIT_PATTERNS[10] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F  // 9
};

uint8_t countdownValue = 9;
unsigned long lastStepTime = 0;
bool countdownStarted = false;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_115\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDigitOnBoth(uint8_t digit);
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_115\\direct\\attempt_0\\sketch\\sketch.ino"
void turnAllLightsOff();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_115\\direct\\attempt_0\\sketch\\sketch.ino"
void applyCountdownState();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_115\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_115\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_115\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDigitOnBoth(uint8_t digit) {
  const uint8_t pattern = static_cast<uint8_t>(~DIGIT_PATTERNS[digit]);

  digitalWrite(LATCH_PIN, LOW);
  // The first byte travels through sr1 into sr2.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
  digitalWrite(LATCH_PIN, HIGH);
}

void turnAllLightsOff() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], LOW);
  }

  for (uint8_t rgb = 0; rgb < 2; ++rgb) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[rgb][channel], LOW);
    }
  }
}

void applyCountdownState() {
  displayDigitOnBoth(countdownValue);
  turnAllLightsOff();

  if (countdownValue == 0) {
    // Hold "00" and steady white for one full two-second interval.
    for (uint8_t rgb = 0; rgb < 2; ++rgb) {
      for (uint8_t channel = 0; channel < 3; ++channel) {
        digitalWrite(RGB_PINS[rgb][channel], HIGH);
      }
    }
    return;
  }

  // At 9: led1/red; at 8: led2/green; at 7: led3/blue; repeat.
  const uint8_t sequenceIndex = (9 - countdownValue) % 3;
  digitalWrite(LED_PINS[sequenceIndex], HIGH);

  for (uint8_t rgb = 0; rgb < 2; ++rgb) {
    digitalWrite(RGB_PINS[rgb][sequenceIndex], HIGH);
  }
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (uint8_t rgb = 0; rgb < 2; ++rgb) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[rgb][channel], LOW);
      pinMode(RGB_PINS[rgb][channel], OUTPUT);
    }
  }

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  // Initial state: "99", with all single-color and RGB LEDs off.
  turnAllLightsOff();
  displayDigitOnBoth(9);
}

void loop() {
  if (!countdownStarted) {
    // Begin the first full countdown interval after initialization.
    countdownStarted = true;
    applyCountdownState();
    lastStepTime = millis();
    return;
  }

  const unsigned long now = millis();
  if (now - lastStepTime >= STEP_INTERVAL_MS) {
    lastStepTime = now;

    if (countdownValue == 0) {
      countdownValue = 9;
    } else {
      --countdownValue;
    }

    applyCountdownState();
  }
}

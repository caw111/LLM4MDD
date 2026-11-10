#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_43\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: preserve all supplied wiring.
const uint8_t SHIFT_DATA_PIN  = A0;  // 74HC595 DS
const uint8_t SHIFT_LATCH_PIN = A1;  // 74HC595 STCP
const uint8_t SHIFT_CLOCK_PIN = A2;  // 74HC595 SHCP

const uint8_t BRIGHTNESS_POT_PIN = A3;
const uint8_t SPEED_POT2_PIN     = A4;
const uint8_t SPEED_POT3_PIN     = A5;

// All ten pins support hardware PWM on the Mega 2560.
const uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// Bits 0-6 correspond to segments A-G; bit 7 is unused.
// These are active-high patterns, inverted when sent because COM is +5V.
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

uint8_t currentDigit = 0;
uint8_t previousBarDigit = 255;
int previousBrightness = -1;
unsigned long lastDigitChangeMs = 0;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_43\\direct\\attempt_0\\sketch\\sketch.ino"
void showDigit(uint8_t digit);
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_43\\direct\\attempt_0\\sketch\\sketch.ino"
void updateBarGraph(uint8_t digit, uint8_t brightness);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_43\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_43\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_43\\direct\\attempt_0\\sketch\\sketch.ino"
void showDigit(uint8_t digit) {
  const uint8_t commonAnodePattern =
      static_cast<uint8_t>(~DIGIT_PATTERNS[digit]);

  digitalWrite(SHIFT_LATCH_PIN, LOW);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, commonAnodePattern);
  digitalWrite(SHIFT_LATCH_PIN, HIGH);
}

void updateBarGraph(uint8_t digit, uint8_t brightness) {
  for (uint8_t i = 0; i < 10; ++i) {
    analogWrite(BAR_PINS[i], (i < digit) ? brightness : 0);
  }
}

void setup() {
  pinMode(SHIFT_DATA_PIN, OUTPUT);
  pinMode(SHIFT_LATCH_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);

  digitalWrite(SHIFT_DATA_PIN, LOW);
  digitalWrite(SHIFT_CLOCK_PIN, LOW);
  digitalWrite(SHIFT_LATCH_PIN, LOW);

  pinMode(BRIGHTNESS_POT_PIN, INPUT);
  pinMode(SPEED_POT2_PIN, INPUT);
  pinMode(SPEED_POT3_PIN, INPUT);

  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(BAR_PINS[i], OUTPUT);
    analogWrite(BAR_PINS[i], 0);
  }

  showDigit(currentDigit);
  lastDigitChangeMs = millis();
}

void loop() {
  const uint8_t brightness = static_cast<uint8_t>(
      map(analogRead(BRIGHTNESS_POT_PIN), 0, 1023, 0, 255));

  const unsigned long pot2IntervalMs = static_cast<unsigned long>(
      map(analogRead(SPEED_POT2_PIN), 0, 1023, 1000, 2000));

  const unsigned long pot3IntervalMs = static_cast<unsigned long>(
      map(analogRead(SPEED_POT3_PIN), 0, 1023, 1000, 2000));

  // Each speed control contributes 1-2 seconds. Their required sum
  // gives a final interval of 2-4 seconds, including the 2-second baseline.
  const unsigned long digitIntervalMs = pot2IntervalMs + pot3IntervalMs;
  const unsigned long now = millis();

  // Unsigned subtraction remains valid across millis() rollover.
  if (now - lastDigitChangeMs >= digitIntervalMs) {
    lastDigitChangeMs = now;
    currentDigit = (currentDigit + 1) % 10;
    showDigit(currentDigit);
  }

  if (currentDigit != previousBarDigit ||
      brightness != previousBrightness) {
    updateBarGraph(currentDigit, brightness);
    previousBarDigit = currentDigit;
    previousBrightness = brightness;
  }
}

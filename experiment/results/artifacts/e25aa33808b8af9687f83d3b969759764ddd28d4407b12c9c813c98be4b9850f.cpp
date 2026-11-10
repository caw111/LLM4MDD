#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_11\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring to the 74HC595.
constexpr uint8_t DATA_PIN  = A0; // DS
constexpr uint8_t LATCH_PIN = A1; // STCP
constexpr uint8_t CLOCK_PIN = A2; // SHCP

constexpr unsigned long DISPLAY_TIME_MS = 2000;

// Q0..Q6 connect to segments A..G.
// The display common is connected to +5V, so segments are active LOW.
// Q7 is unused and is kept HIGH.
constexpr uint8_t DISPLAY_OFF = 0xFF;
constexpr uint8_t CHARACTER_0 = 0xC0;
constexpr uint8_t CHARACTER_A = 0x88;
constexpr uint8_t CHARACTER_P = 0x8C;

#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_11\\direct\\attempt_0\\sketch\\sketch.ino"
void writeDisplay(uint8_t pattern);
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_11\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_11\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_11\\direct\\attempt_0\\sketch\\sketch.ino"
void writeDisplay(uint8_t pattern) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  digitalWrite(DATA_PIN, HIGH);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  // Initialize the display to OFF before starting the sequence.
  writeDisplay(DISPLAY_OFF);
}

void loop() {
  writeDisplay(CHARACTER_0);
  delay(DISPLAY_TIME_MS);

  writeDisplay(CHARACTER_A);
  delay(DISPLAY_TIME_MS);

  writeDisplay(CHARACTER_P);
  delay(DISPLAY_TIME_MS);
}

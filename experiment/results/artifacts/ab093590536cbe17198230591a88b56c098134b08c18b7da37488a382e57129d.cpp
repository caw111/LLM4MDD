#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_60\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560, preserving all supplied wiring.
constexpr uint8_t RGB1_RED_PIN   = 2;
constexpr uint8_t RGB1_GREEN_PIN = 3;
constexpr uint8_t RGB1_BLUE_PIN  = 4;
constexpr uint8_t RGB2_RED_PIN   = 5;
constexpr uint8_t RGB2_GREEN_PIN = 6;
constexpr uint8_t RGB2_BLUE_PIN  = 7;

constexpr uint8_t DATA_PIN  = A0;  // sr1 DS
constexpr uint8_t LATCH_PIN = A1;  // Both STCP inputs
constexpr uint8_t CLOCK_PIN = A2;  // Both SHCP inputs

constexpr uint8_t POT_RED_PIN   = A3;
constexpr uint8_t POT_GREEN_PIN = A4;
constexpr uint8_t POT_BLUE_PIN  = A5;

// Segment bits: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G.
// Both display commons are wired to +5V, so segments are active LOW.
constexpr uint8_t DIGIT_SEGMENTS[10] = {
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

// The supplied hardware has two single-digit displays, not two-digit
// displays. Follow the explicit detail rules: sevseg1 shows red (0-9),
// sevseg2 shows blue (0-9). Green controls both LEDs continuously.
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_60\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays(uint8_t redDigit, uint8_t blueDigit);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_60\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_60\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_60\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays(uint8_t redDigit, uint8_t blueDigit) {
  const uint8_t redPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[redDigit]);
  const uint8_t bluePattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[blueDigit]);

  digitalWrite(LATCH_PIN, LOW);

  // sr1 Q7S feeds sr2 DS: send the downstream display's byte first.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, bluePattern);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, redPattern);

  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  pinMode(RGB1_RED_PIN, OUTPUT);
  pinMode(RGB1_GREEN_PIN, OUTPUT);
  pinMode(RGB1_BLUE_PIN, OUTPUT);
  pinMode(RGB2_RED_PIN, OUTPUT);
  pinMode(RGB2_GREEN_PIN, OUTPUT);
  pinMode(RGB2_BLUE_PIN, OUTPUT);

  analogWrite(RGB1_RED_PIN, 0);
  analogWrite(RGB1_GREEN_PIN, 0);
  analogWrite(RGB1_BLUE_PIN, 0);
  analogWrite(RGB2_RED_PIN, 0);
  analogWrite(RGB2_GREEN_PIN, 0);
  analogWrite(RGB2_BLUE_PIN, 0);

  pinMode(POT_RED_PIN, INPUT);
  pinMode(POT_GREEN_PIN, INPUT);
  pinMode(POT_BLUE_PIN, INPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  updateDisplays(0, 0);
}

void loop() {
  const uint8_t red =
      static_cast<uint8_t>(map(analogRead(POT_RED_PIN), 0, 1023, 0, 255));
  const uint8_t green =
      static_cast<uint8_t>(map(analogRead(POT_GREEN_PIN), 0, 1023, 0, 255));
  const uint8_t blue =
      static_cast<uint8_t>(map(analogRead(POT_BLUE_PIN), 0, 1023, 0, 255));

  // Common-cathode RGB LEDs: larger PWM values mean brighter channels.
  analogWrite(RGB1_RED_PIN, red);
  analogWrite(RGB1_GREEN_PIN, green);
  analogWrite(RGB1_BLUE_PIN, blue);

  analogWrite(RGB2_RED_PIN, red);
  analogWrite(RGB2_GREEN_PIN, green);
  analogWrite(RGB2_BLUE_PIN, blue);

  const uint8_t redDigit =
      static_cast<uint8_t>(map(red, 0, 255, 0, 9));
  const uint8_t blueDigit =
      static_cast<uint8_t>(map(blue, 0, 255, 0, 9));

  updateDisplays(redDigit, blueDigit);
  delay(5);
}

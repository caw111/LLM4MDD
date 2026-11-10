#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_61\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t RED_PIN   = 2;
constexpr uint8_t GREEN_PIN = 3;
constexpr uint8_t BLUE_PIN  = 4;

constexpr uint8_t DATA_PIN  = A0;  // 74HC595 DS
constexpr uint8_t LATCH_PIN = A1;  // 74HC595 STCP
constexpr uint8_t CLOCK_PIN = A2;  // 74HC595 SHCP

constexpr uint8_t POT1_PIN = A3;
constexpr uint8_t POT2_PIN = A4;

// Bits 0..6 correspond to segments A..G on Q0..Q6.
// These masks identify illuminated segments; output is inverted because
// the display's common anode is wired to +5 V.
constexpr uint8_t DIGIT_MASKS[10] = {
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

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_61\\direct\\attempt_0\\sketch\\sketch.ino"
void showDigit(uint8_t digit);
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_61\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_61\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_61\\direct\\attempt_0\\sketch\\sketch.ino"
void showDigit(uint8_t digit) {
  if (digit > 9) {
    digit = 9;
  }

  const uint8_t outputs = static_cast<uint8_t>(~DIGIT_MASKS[digit]);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, outputs);
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  // The RGB LED is common cathode: LOW means off.
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
  showDigit(0);
}

void loop() {
  const int pot1Value = analogRead(POT1_PIN);
  const int pot2Value = analogRead(POT2_PIN);

  const uint8_t red =
      static_cast<uint8_t>(map(pot1Value, 0, 1023, 0, 255));
  const uint8_t green = red;
  const uint8_t blue =
      static_cast<uint8_t>(map(pot2Value, 0, 1023, 0, 255));

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);

  const float averageBrightness =
      (static_cast<float>(red) + green + blue) / 3.0f;
  const int brightnessDigit =
      constrain(static_cast<int>(averageBrightness / 28.33f), 0, 9);

  showDigit(static_cast<uint8_t>(brightnessDigit));
}

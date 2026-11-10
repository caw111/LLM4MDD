#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_104\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: preserve all supplied wiring.
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;
constexpr uint8_t POT1_PIN = A3;
constexpr uint8_t POT2_PIN = A4;
constexpr uint8_t POT3_PIN = A5;

// Q0..Q6 connect to segments A..G. Common-anode segments are active LOW.
// Q7 is unused and remains HIGH.
const uint8_t DIGIT_PATTERNS[10] = {
  0xC0, // 0
  0xF9, // 1
  0xA4, // 2
  0xB0, // 3
  0x99, // 4
  0x92, // 5
  0x82, // 6
  0xF8, // 7
  0x80, // 8
  0x90  // 9
};

#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_104\\direct\\attempt_0\\sketch\\sketch.ino"
void displayNumber(uint8_t number);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_104\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_104\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_104\\direct\\attempt_0\\sketch\\sketch.ino"
void displayNumber(uint8_t number) {
  const uint8_t tens = number / 10;
  const uint8_t units = number % 10;

  digitalWrite(LATCH_PIN, LOW);
  // The first byte travels through sr1 to sr2 (units).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[units]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);
  analogReference(DEFAULT);

  displayNumber(0);
}

void loop() {
  const uint16_t pot1 = analogRead(POT1_PIN);
  const uint16_t pot2 = analogRead(POT2_PIN);
  const uint16_t pot3 = analogRead(POT3_PIN);

  const uint8_t brightness1 =
      static_cast<uint32_t>(pot1) * 255UL / 1023UL;
  const uint8_t brightness2 =
      static_cast<uint32_t>(pot2) * 255UL / 1023UL;
  const uint8_t number =
      static_cast<uint32_t>(pot3) * 99UL / 1023UL;

  // Turn off the inactive LED before enabling the active one.
  if ((number & 1U) == 0) {
    analogWrite(LED2_PIN, 0);
    analogWrite(LED1_PIN, brightness1);
  } else {
    analogWrite(LED1_PIN, 0);
    analogWrite(LED2_PIN, brightness2);
  }

  displayNumber(number);
}

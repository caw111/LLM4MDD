#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_40\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: 74HC595 wiring.
constexpr uint8_t DATA_PIN  = A0;  // DS
constexpr uint8_t LATCH_PIN = A1;  // STCP
constexpr uint8_t CLOCK_PIN = A2;  // SHCP

// Bar graph anodes A1 through A10; all cathodes are grounded.
constexpr uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// Q0..Q6 connect to segments A..G.
// Common anode is tied to +5 V, so segment outputs are active LOW.
constexpr uint8_t DIGIT_SEGMENTS[10] = {
  0x3F, // 0: A B C D E F
  0x06, // 1: B C
  0x5B, // 2: A B D E G
  0x4F, // 3: A B C D G
  0x66, // 4: B C F G
  0x6D, // 5: A C D F G
  0x7D, // 6: A C D E F G
  0x07, // 7: A B C
  0x7F, // 8: A B C D E F G
  0x6F  // 9: A B C D F G
};

constexpr unsigned long STEP_INTERVAL_MS = 2000UL;

uint8_t currentValue = 0;
unsigned long lastStepTime = 0;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_40\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays(uint8_t value);
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_40\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_40\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_40\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays(uint8_t value) {
  const uint8_t outputPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[value]);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, outputPattern);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < value ? HIGH : LOW);
  }

  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  currentValue = 0;
  updateDisplays(currentValue);
  lastStepTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastStepTime >= STEP_INTERVAL_MS) {
    lastStepTime += STEP_INTERVAL_MS;
    currentValue = (currentValue + 1) % 10;
    updateDisplays(currentValue);
  }
}

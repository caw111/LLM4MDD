#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_39\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: preserve the supplied wiring.
constexpr uint8_t DATA_PIN  = A0;  // sr1 DS
constexpr uint8_t LATCH_PIN = A1;  // sr1 STCP
constexpr uint8_t CLOCK_PIN = A2;  // sr1 SHCP

// Bar graph anodes A1 through A10 (not Arduino analog pin names).
constexpr uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

constexpr unsigned long UPDATE_INTERVAL_MS = 2000UL;

// Bits 0..6 correspond to segments A..G; bit 7 is unused.
// The display common is connected to +5 V, so segments are active LOW.
constexpr uint8_t SEGMENT_PATTERNS[10] = {
  0x3F,  // 0
  0x06,  // 1
  0x5B,  // 2
  0x4F,  // 3
  0x66,  // 4
  0x6D,  // 5
  0x7D,  // 6
  0x07,  // 7
  0x7F,  // 8
  0x6F   // 9
};

uint8_t currentNumber = 0;
unsigned long lastUpdateMs = 0;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_39\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays(uint8_t number);
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_39\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_39\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_39\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays(uint8_t number) {
  const uint8_t outputs = static_cast<uint8_t>(~SEGMENT_PATTERNS[number]);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, outputs);

  // Light the first "number" LEDs, in A1-to-A10 order.
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < number ? HIGH : LOW);
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

  currentNumber = 0;
  updateDisplays(currentNumber);
  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const unsigned long elapsed = now - lastUpdateMs;

  if (elapsed >= UPDATE_INTERVAL_MS) {
    const unsigned long steps = elapsed / UPDATE_INTERVAL_MS;
    lastUpdateMs += steps * UPDATE_INTERVAL_MS;

    // Display 0 through 9, then wrap to 0 with all bar LEDs off.
    currentNumber = static_cast<uint8_t>(
      (currentNumber + (steps % 10UL)) % 10UL
    );
    updateDisplays(currentNumber);
  }
}

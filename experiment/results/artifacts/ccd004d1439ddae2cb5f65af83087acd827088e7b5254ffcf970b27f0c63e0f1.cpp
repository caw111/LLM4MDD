#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_13\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
const uint8_t LED_PINS[3] = {2, 3, 4};
const uint8_t DATA_PIN = A0;   // 74HC595 DS
const uint8_t LATCH_PIN = A1;  // 74HC595 STCP
const uint8_t CLOCK_PIN = A2;  // 74HC595 SHCP

const unsigned long UPDATE_INTERVAL_MS = 2000UL;
unsigned long lastUpdateMs = 0;

// Q0..Q6 connect to segments A..G.
// The display's common terminal is connected to +5V (common anode),
// so LOW turns a segment on. Q7 is unused and kept HIGH.
const uint8_t DIGIT_PATTERNS[4] = {
  0xC0,  // 0: A B C D E F
  0xF9,  // 1: B C
  0xA4,  // 2: A B D E G
  0xB0   // 3: A B C D G
};

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_13\\direct\\attempt_0\\sketch\\sketch.ino"
void displayCount(uint8_t count);
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_13\\direct\\attempt_0\\sketch\\sketch.ino"
void randomizeLEDs();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_13\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_13\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_13\\direct\\attempt_0\\sketch\\sketch.ino"
void displayCount(uint8_t count) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[count]);
  digitalWrite(LATCH_PIN, HIGH);
}

void randomizeLEDs() {
  // Choose each count (0 through 3) with equal probability.
  const uint8_t requestedCount = static_cast<uint8_t>(random(4));

  // Shuffle LED indices to select a random subset of the chosen size.
  uint8_t order[3] = {0, 1, 2};
  for (uint8_t i = 2; i > 0; --i) {
    const uint8_t j = static_cast<uint8_t>(random(i + 1));
    const uint8_t temporary = order[i];
    order[i] = order[j];
    order[j] = temporary;
  }

  bool enabled[3] = {false, false, false};
  for (uint8_t i = 0; i < requestedCount; ++i) {
    enabled[order[i]] = true;
  }

  uint8_t litCount = 0;
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], enabled[i] ? HIGH : LOW);
    if (enabled[i]) {
      ++litCount;
    }
  }

  displayCount(litCount);
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  displayCount(0);

  // A3 is unconnected in the supplied circuit.
  unsigned long seed = micros();
  for (uint8_t i = 0; i < 16; ++i) {
    seed = (seed << 1) ^ static_cast<unsigned long>(analogRead(A3));
    seed ^= micros();
  }
  randomSeed(seed ? seed : 1UL);

  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();
  if (now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    lastUpdateMs = now;
    randomizeLEDs();
  }
}

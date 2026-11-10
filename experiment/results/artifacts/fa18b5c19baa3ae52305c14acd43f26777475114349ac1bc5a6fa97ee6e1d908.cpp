#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_106\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: preserve the supplied circuit wiring.
constexpr uint8_t DATA_PIN  = A0;  // sr1 DS
constexpr uint8_t LATCH_PIN = A1;  // sr1/sr2 STCP
constexpr uint8_t CLOCK_PIN = A2;  // sr1/sr2 SHCP

constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;

// Bar graph anodes A1-A10 connect to Mega digital pins 4-13.
constexpr uint8_t BAR_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

constexpr unsigned long INTERVAL_MS = 2000UL;

// Bits 0-6 correspond to segments A-G.
// Both displays have their common anodes connected to +5 V,
// so these patterns are inverted before being shifted out.
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

uint8_t displayedNumber = 0;
bool led1Turn = false;
unsigned long lastUpdateMs = 0;

#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_106\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplaysAndBar();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_106\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_106\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_106\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplaysAndBar() {
  const uint8_t tens = displayedNumber / 10;
  const uint8_t units = displayedNumber % 10;

  digitalWrite(LATCH_PIN, LOW);

  // The first byte travels through sr1 into sr2 (units).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_SEGMENTS[units]));
  // The second byte remains in sr1 (tens).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_SEGMENTS[tens]));

  digitalWrite(LATCH_PIN, HIGH);

  const uint8_t litSegments = tens + 1;
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < litSegments ? HIGH : LOW);
  }
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  // Initial state: "00", first bar segment on, both LEDs off.
  updateDisplaysAndBar();
  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();

  // Unsigned subtraction remains valid across millis() rollover.
  while (now - lastUpdateMs >= INTERVAL_MS) {
    lastUpdateMs += INTERVAL_MS;

    displayedNumber = (displayedNumber + 1) % 100;
    updateDisplaysAndBar();

    // First interval starts with LED1 on; subsequent intervals alternate.
    led1Turn = !led1Turn;
    digitalWrite(LED1_PIN, led1Turn ? HIGH : LOW);
    digitalWrite(LED2_PIN, led1Turn ? LOW : HIGH);
  }
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_44\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t DATA_PIN  = A0;  // sr1 DS
constexpr uint8_t LATCH_PIN = A1;  // sr1 and sr2 STCP
constexpr uint8_t CLOCK_PIN = A2;  // sr1 and sr2 SHCP
constexpr uint8_t POT_PIN   = A3;

constexpr uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// Bits 0..6 correspond to segments A..G; bit 7 is unused.
// Both displays are common-anode, so these patterns are inverted
// before being sent to the shift registers.
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

int16_t lastValue = -1;

#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_44\\direct\\attempt_0\\sketch\\sketch.ino"
void displayValue(uint8_t value);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_44\\direct\\attempt_0\\sketch\\sketch.ino"
void updateBarGraph(uint8_t value);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_44\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_44\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_44\\direct\\attempt_0\\sketch\\sketch.ino"
void displayValue(uint8_t value) {
  const uint8_t tens = value / 10;
  const uint8_t units = value % 10;

  const uint8_t tensPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[tens]);
  const uint8_t unitsPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[units]);

  digitalWrite(LATCH_PIN, LOW);

  // sr1 Q7S feeds sr2 DS: the first byte reaches sr2 (units),
  // and the second byte remains in sr1 (tens).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, unitsPattern);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, tensPattern);

  // Update both digits simultaneously.
  digitalWrite(LATCH_PIN, HIGH);
}

void updateBarGraph(uint8_t value) {
  const uint8_t litCount = value / 10;

  for (uint8_t index = 0; index < 10; ++index) {
    // Only indices 0..8 can light; index 9 always remains off.
    digitalWrite(BAR_PINS[index], index < litCount ? HIGH : LOW);
  }
}

void setup() {
  // Initialize every bar-graph segment off.
  for (uint8_t index = 0; index < 10; ++index) {
    digitalWrite(BAR_PINS[index], LOW);
    pinMode(BAR_PINS[index], OUTPUT);
  }

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  // Startup state: "00" with all bar-graph segments off.
  displayValue(0);
  updateBarGraph(0);
}

void loop() {
  const int raw = analogRead(POT_PIN);
  const uint8_t value =
      static_cast<uint8_t>(map(raw, 0, 1023, 0, 99));

  if (value != lastValue) {
    displayValue(value);
    updateBarGraph(value);
    lastValue = value;
  }

  // No delays: continuously sample the potentiometer.
}

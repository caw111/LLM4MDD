#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_81\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr uint8_t BUTTON_PINS[] = {2, 3, 4};
constexpr uint8_t BAR_PINS[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
constexpr unsigned long DEBOUNCE_MS = 25;

// Bits 0..6 map to segments A..G. Common-anode displays are active LOW.
// Bit 7 remains HIGH (unused output / decimal point off).
constexpr uint8_t DIGIT_PATTERNS[] = {
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

struct DebouncedButton {
  uint8_t pin;
  uint8_t rawState;
  uint8_t stableState;
  unsigned long lastChange;

  void begin(uint8_t assignedPin) {
    pin = assignedPin;
    pinMode(pin, INPUT_PULLUP);
    rawState = digitalRead(pin);
    stableState = HIGH;
    lastChange = millis();
  }

  bool pressed(unsigned long now) {
    const uint8_t sample = digitalRead(pin);

    if (sample != rawState) {
      rawState = sample;
      lastChange = now;
    }

    if (rawState != stableState &&
        static_cast<unsigned long>(now - lastChange) >= DEBOUNCE_MS) {
      stableState = rawState;
      return stableState == LOW;
    }

    return false;
  }

  bool held() const {
    return stableState == LOW;
  }
};

DebouncedButton buttons[3];
uint8_t currentNumber = 0;

#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_81\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs();
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_81\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_81\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_81\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs() {
  const uint8_t tens = currentNumber / 10;
  const uint8_t ones = currentNumber % 10;

  digitalWrite(LATCH_PIN, LOW);

  // sr2 is downstream, so send its ones digit first.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[ones]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);

  digitalWrite(LATCH_PIN, HIGH);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < tens ? HIGH : LOW);
  }
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

  for (uint8_t i = 0; i < 3; ++i) {
    buttons[i].begin(BUTTON_PINS[i]);
  }

  currentNumber = 0;
  updateOutputs();
}

void loop() {
  const unsigned long now = millis();
  const bool incrementOne = buttons[0].pressed(now);
  const bool incrementFive = buttons[1].pressed(now);
  const bool resetPressed = buttons[2].pressed(now);
  const uint8_t previousNumber = currentNumber;

  // Reset takes priority over increment presses, including while held.
  if (resetPressed || buttons[2].held()) {
    currentNumber = 0;
  } else {
    if (incrementOne) {
      const uint16_t next = currentNumber + 1U;
      currentNumber = next > 99 ? 0 : next;
    }

    if (incrementFive) {
      const uint16_t next = currentNumber + 5U;
      currentNumber = next > 99 ? 0 : next;
    }
  }

  if (currentNumber != previousNumber || resetPressed) {
    updateOutputs();
  }
}

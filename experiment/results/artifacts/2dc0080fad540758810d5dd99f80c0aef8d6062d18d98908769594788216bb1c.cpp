#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_124\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t LED_PIN = 2;
constexpr uint8_t INCREMENT_PIN = 3;
constexpr uint8_t DECREMENT_PIN = 4;
constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr unsigned long DEBOUNCE_MS = 25;

// Bits 0..6 correspond to segments A..G on Q0..Q6.
// The display common is connected to +5V, so segments are active LOW.
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

struct DebouncedButton {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeMs;

  void begin(uint8_t buttonPin) {
    pin = buttonPin;
    pinMode(pin, INPUT_PULLUP);
    lastReading = digitalRead(pin);
    stableState = HIGH;
    lastChangeMs = millis();
  }

  bool pressed(unsigned long now) {
    const uint8_t reading = digitalRead(pin);

    if (reading != lastReading) {
      lastReading = reading;
      lastChangeMs = now;
    }

    if (reading != stableState &&
        (unsigned long)(now - lastChangeMs) >= DEBOUNCE_MS) {
      stableState = reading;
      return stableState == LOW;
    }

    return false;
  }
};

DebouncedButton incrementButton;
DebouncedButton decrementButton;
uint8_t displayedNumber = 0;

#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_124\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_124\\direct\\attempt_0\\sketch\\sketch.ino"
void applyButtonStep(int8_t step);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_124\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_124\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_124\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay() {
  const uint8_t outputBits =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[displayedNumber]);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, outputBits);
  digitalWrite(LATCH_PIN, HIGH);
}

void applyButtonStep(int8_t step) {
  displayedNumber =
      static_cast<uint8_t>((displayedNumber + step + 10) % 10);

  updateDisplay();
  digitalWrite(LED_PIN, (displayedNumber % 2 == 0) ? HIGH : LOW);
}

void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  incrementButton.begin(INCREMENT_PIN);
  decrementButton.begin(DECREMENT_PIN);

  displayedNumber = 0;
  updateDisplay();

  // Startup exception: show 0 with LED off until a button press.
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  const unsigned long now = millis();

  if (incrementButton.pressed(now)) {
    applyButtonStep(1);
  }

  if (decrementButton.pressed(now)) {
    applyButtonStep(-1);
  }
}

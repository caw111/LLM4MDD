#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_42\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560; pin assignments match the supplied circuit.
constexpr uint8_t DATA_PIN  = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
constexpr uint8_t SERVO_PINS[3] = {12, 13, 44};
constexpr unsigned long UPDATE_INTERVAL_MS = 2000UL;

Servo servos[3];
uint8_t currentNumber = 0;
unsigned long lastUpdateMs = 0;

// Active-low patterns for common-anode displays.
// Q0..Q6 drive A..G; unused Q7 remains HIGH.
constexpr uint8_t DIGIT_PATTERNS[10] = {
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

#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_42\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays(uint8_t number);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_42\\direct\\attempt_0\\sketch\\sketch.ino"
void updateBarGraph(uint8_t number);
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_42\\direct\\attempt_0\\sketch\\sketch.ino"
void resetServos();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_42\\direct\\attempt_0\\sketch\\sketch.ino"
void updateServos(uint8_t units);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_42\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_42\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_42\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplays(uint8_t number) {
  const uint8_t tens = number / 10;
  const uint8_t units = number % 10;

  digitalWrite(LATCH_PIN, LOW);

  // First byte reaches downstream sr2 (right/units display).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[units]);
  // Second byte stays in sr1 (left/tens display).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);

  digitalWrite(LATCH_PIN, HIGH);
}

void updateBarGraph(uint8_t number) {
  const uint8_t litCount = number / 10;

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < litCount ? HIGH : LOW);
  }
}

void resetServos() {
  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(0);
  }
}

void updateServos(uint8_t units) {
  for (uint8_t i = 0; i < 3; ++i) {
    const int requestedAngle = units * 18 + i * 60;

    // The supplied positional servos support only 0..180 degrees.
    // Saturate larger calculated angles at their physical limit.
    const int supportedAngle = constrain(requestedAngle, 0, 180);
    servos[i].write(supportedAngle);
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

  updateDisplays(0);
  updateBarGraph(0);

  for (uint8_t i = 0; i < 3; ++i) {
    // Preload zero before enabling servo pulses.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
    servos[i].write(0);
  }

  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastUpdateMs < UPDATE_INTERVAL_MS) {
    return;
  }
  lastUpdateMs += UPDATE_INTERVAL_MS;

  // A units digit of 9 has now been held for the full two seconds.
  const bool resetAfterNine = (currentNumber % 10 == 9);

  currentNumber = (currentNumber + 1) % 100;

  updateDisplays(currentNumber);
  updateBarGraph(currentNumber);

  if (resetAfterNine) {
    // The explicit post-9 reset takes precedence over the offset-angle
    // formula for this rollover update, including the 99 -> 00 rollover.
    resetServos();
  } else {
    updateServos(currentNumber % 10);
  }
}

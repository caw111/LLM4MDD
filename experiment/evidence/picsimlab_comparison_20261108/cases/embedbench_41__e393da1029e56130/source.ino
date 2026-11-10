#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t DATA_PIN  = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
constexpr uint8_t SERVO1_PIN = 12;
constexpr uint8_t SERVO2_PIN = 13;

constexpr unsigned long INCREMENT_INTERVAL_MS = 2000UL;
constexpr int SERVO_MIN_US = 544;
constexpr int SERVO_MAX_US = 2400;

// Bits 0 through 6 correspond to segments A through G.
// These are active-high patterns; invert for the wired common-anode displays.
constexpr uint8_t DIGIT_PATTERNS[10] = {
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

Servo servo1;
Servo servo2;

uint8_t number = 0;
unsigned long lastIncrementMs = 0;

void updateDisplays(uint8_t value) {
  const uint8_t tens = value / 10;
  const uint8_t units = value % 10;

  digitalWrite(LATCH_PIN, LOW);

  // The first byte travels through sr1 into sr2 (units).
  // The second byte remains in sr1 (tens).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_PATTERNS[units]));
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_PATTERNS[tens]));

  digitalWrite(LATCH_PIN, HIGH);
}

void updateBarGraph(uint8_t value) {
  // Each fully reached 10% lights one LED: 0 -> 0, 50 -> 5, 99 -> 9.
  const uint8_t litCount = value / 10;

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < litCount ? HIGH : LOW);
  }
}

void updateServos(uint8_t value) {
  // Angle = value * 1.8 degrees.
  // Since angle / 180 = value / 100, calculate the pulse directly
  // to retain fractional-degree positioning instead of truncating angles.
  const int pulseUs = SERVO_MIN_US +
      ((static_cast<long>(SERVO_MAX_US - SERVO_MIN_US) * value + 50L) / 100L);

  servo1.writeMicroseconds(pulseUs);
  servo2.writeMicroseconds(pulseUs);
}

void updateOutputs() {
  updateDisplays(number);
  updateBarGraph(number);
  updateServos(number);
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

  // Preload zero-degree commands before enabling servo pulse generation.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN, SERVO_MIN_US, SERVO_MAX_US);
  servo2.attach(SERVO2_PIN, SERVO_MIN_US, SERVO_MAX_US);

  number = 0;
  updateOutputs();
  lastIncrementMs = millis();
}

void loop() {
  const unsigned long now = millis();

  // Unsigned subtraction remains correct across millis() rollover.
  if (now - lastIncrementMs >= INCREMENT_INTERVAL_MS) {
    lastIncrementMs += INCREMENT_INTERVAL_MS;

    ++number;
    if (number > 99) {
      number = 0;
    }

    updateOutputs();
  }
}
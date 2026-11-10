#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t DATA_PIN  = A0; // 74HC595 DS
constexpr uint8_t LATCH_PIN = A1; // 74HC595 STCP
constexpr uint8_t CLOCK_PIN = A2; // 74HC595 SHCP
constexpr uint8_t SERVO_PINS[3] = {2, 3, 4};

constexpr unsigned long DISPLAY_INTERVAL_MS = 2000;
constexpr unsigned long STARTUP_SETTLE_MS = 500;

// Standard servos provide no position feedback. Allow travel time before
// commanding the next servo. Adjust this value for the actual servo speed.
constexpr unsigned long SERVO_MS_PER_DEGREE = 3;
constexpr unsigned long SERVO_SETTLE_MS = 40;

// Bits 0..6 correspond to A..G. The display common is connected to +5 V,
// so segments are active LOW. Q7 is unused and is held HIGH.
const uint8_t DIGIT_SEGMENTS[10] = {
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

Servo servos[3];
uint8_t currentAngles[3] = {0, 0, 0};
uint8_t targetAngles[3] = {0, 0, 0};

uint8_t displayedNumber = 0;
uint8_t sequenceIndex = 0;
bool sequenceActive = false;
bool servoMoving = false;
bool startupPending = true;

unsigned long startupStartedAt = 0;
unsigned long digitStartedAt = 0;
unsigned long movementStartedAt = 0;
unsigned long movementDuration = 0;

void showDigit(uint8_t digit) {
  const uint8_t outputs = static_cast<uint8_t>(~DIGIT_SEGMENTS[digit]);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, outputs);
  digitalWrite(LATCH_PIN, HIGH);
}

void beginServoSequence(uint8_t digit) {
  if (digit <= 2) {
    targetAngles[0] = 0;
    targetAngles[1] = 45;
    targetAngles[2] = 90;
  } else if (digit <= 5) {
    targetAngles[0] = 45;
    targetAngles[1] = 90;
    targetAngles[2] = 135;
  } else if (digit <= 8) {
    targetAngles[0] = 90;
    targetAngles[1] = 135;
    targetAngles[2] = 180;
  } else {
    targetAngles[0] = 0;
    targetAngles[1] = 0;
    targetAngles[2] = 0;
  }

  sequenceIndex = 0;
  servoMoving = false;
  sequenceActive = true;
}

void serviceServoSequence(unsigned long now) {
  if (!sequenceActive) {
    return;
  }

  if (servoMoving) {
    if (now - movementStartedAt < movementDuration) {
      return;
    }

    currentAngles[sequenceIndex] = targetAngles[sequenceIndex];
    servoMoving = false;
    ++sequenceIndex;
  }

  while (sequenceIndex < 3) {
    const uint8_t from = currentAngles[sequenceIndex];
    const uint8_t to = targetAngles[sequenceIndex];

    if (from == to) {
      ++sequenceIndex;
      continue;
    }

    const uint8_t distance = (to > from) ? (to - from) : (from - to);

    // Issue the final target immediately; do not ramp through intermediate
    // angles. Wait for this servo before starting the next one.
    servos[sequenceIndex].write(to);
    movementStartedAt = now;
    movementDuration =
        static_cast<unsigned long>(distance) * SERVO_MS_PER_DEGREE
        + SERVO_SETTLE_MS;
    servoMoving = true;
    return;
  }

  sequenceActive = false;
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  showDigit(0);

  for (uint8_t i = 0; i < 3; ++i) {
    // Preload zero before attaching to avoid an initial 90-degree command.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
    servos[i].write(0);
  }

  startupStartedAt = millis();
  digitStartedAt = startupStartedAt;
}

void loop() {
  const unsigned long now = millis();

  if (startupPending) {
    // First establish the required all-zero startup position, then apply
    // digit 0's targets while retaining its original two-second display slot.
    if (now - startupStartedAt < STARTUP_SETTLE_MS) {
      return;
    }

    startupPending = false;
    beginServoSequence(0);
  }

  serviceServoSequence(now);

  if (!sequenceActive && now - digitStartedAt >= DISPLAY_INTERVAL_MS) {
    digitStartedAt += DISPLAY_INTERVAL_MS;
    displayedNumber = (displayedNumber + 1) % 10;

    showDigit(displayedNumber);
    beginServoSequence(displayedNumber);
    serviceServoSequence(now);
  }
}
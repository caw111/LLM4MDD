#include <Arduino.h>
#include <Servo.h>

const uint8_t LED_PINS[] = {2, 3, 4};
const uint8_t SERVO_PINS[] = {5, 6, 7};

const uint8_t DATA_PIN = A0;
const uint8_t LATCH_PIN = A1;
const uint8_t CLOCK_PIN = A2;

const unsigned long INCREMENT_INTERVAL_MS = 2000UL;

// Q0-Q6 connect to segments A-G. The displays are common-anode,
// so LOW illuminates a segment. Q7 stays HIGH (unused).
const uint8_t DIGIT_PATTERNS[10] = {
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

Servo servos[3];
uint8_t displayedNumber = 0;
unsigned long lastIncrementMs = 0;

void updateDisplay(uint8_t number) {
  const uint8_t tens = number / 10;
  const uint8_t units = number % 10;

  digitalWrite(LATCH_PIN, LOW);

  // The first byte travels through sr1 into sr2 (right/units).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[units]);
  // The second byte remains in sr1 (left/tens).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);

  digitalWrite(LATCH_PIN, HIGH);
}

void updateIndicatorsAndServos(uint8_t number) {
  const uint8_t units = number % 10;
  const uint8_t tens = number / 10;

  const uint8_t selectedLED = (units <= 2) ? 0 : ((units <= 5) ? 1 : 2);

  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], i == selectedLED ? HIGH : LOW);
  }

  const uint8_t firstAngle = (tens <= 2) ? 30 : ((tens <= 5) ? 60 : 90);

  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(firstAngle + 30 * i);
  }
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

  updateDisplay(0);

  for (uint8_t i = 0; i < 3; ++i) {
    // Preload zero before attaching to avoid an initial 90-degree command.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
    servos[i].write(0);
  }

  // Preserve the special startup state until the first increment.
  lastIncrementMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastIncrementMs >= INCREMENT_INTERVAL_MS) {
    lastIncrementMs += INCREMENT_INTERVAL_MS;

    displayedNumber = (displayedNumber + 1) % 100;
    updateDisplay(displayedNumber);
    updateIndicatorsAndServos(displayedNumber);
  }
}
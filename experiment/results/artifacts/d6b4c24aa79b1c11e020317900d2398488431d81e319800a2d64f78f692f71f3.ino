#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring.
const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;
const uint8_t SERVO1_PIN = 4;
const uint8_t SERVO2_PIN = 5;

const uint8_t SHIFT_DATA_PIN  = A0;
const uint8_t SHIFT_LATCH_PIN = A1;
const uint8_t SHIFT_CLOCK_PIN = A2;

const unsigned long LED_INTERVAL_MS = 2000UL;
const unsigned long DIGIT_INTERVAL_MS = 2000UL;
const unsigned long SWEEP_MS = 4000UL;  // Each one-way sweep.
const unsigned long HOLD_MS = 2000UL;
const unsigned long SERVO_CYCLE_MS = 2UL * (SWEEP_MS + HOLD_MS);

// Active-high segment patterns: bit 0 = A, ..., bit 6 = G.
// The wired common-anode display requires these patterns inverted.
const uint8_t DIGIT_SEGMENTS[10] = {
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

Servo servo1;
Servo servo2;

bool led1On = false;
uint8_t digit = 0;
int lastServoAngle = 0;

unsigned long ledTimestamp = 0;
unsigned long digitTimestamp = 0;
unsigned long servoTimestamp = 0;
unsigned long servoPhase = 0;

void displayDigit(uint8_t value) {
  const uint8_t outputs = static_cast<uint8_t>(~DIGIT_SEGMENTS[value]);

  digitalWrite(SHIFT_LATCH_PIN, LOW);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, outputs);
  digitalWrite(SHIFT_LATCH_PIN, HIGH);
}

void setup() {
  // Initial LED state: led1 off, led2 on.
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(SHIFT_DATA_PIN, LOW);
  digitalWrite(SHIFT_CLOCK_PIN, LOW);
  digitalWrite(SHIFT_LATCH_PIN, LOW);
  pinMode(SHIFT_DATA_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_LATCH_PIN, OUTPUT);
  displayDigit(0);

  // Preload starting positions before enabling servo pulses.
  servo1.write(0);
  servo2.write(180);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  const unsigned long now = millis();
  ledTimestamp = now;
  digitTimestamp = now;
  servoTimestamp = now;
}

void loop() {
  const unsigned long now = millis();

  // Alternate LEDs independently, preserving timing if a loop runs late.
  const unsigned long ledElapsed = now - ledTimestamp;
  if (ledElapsed >= LED_INTERVAL_MS) {
    const unsigned long intervals = ledElapsed / LED_INTERVAL_MS;
    ledTimestamp += intervals * LED_INTERVAL_MS;

    if (intervals & 1UL) {
      led1On = !led1On;
    }

    digitalWrite(LED1_PIN, led1On ? HIGH : LOW);
    digitalWrite(LED2_PIN, led1On ? LOW : HIGH);
  }

  // Count from 0 through 9, holding each digit for two seconds.
  const unsigned long digitElapsed = now - digitTimestamp;
  if (digitElapsed >= DIGIT_INTERVAL_MS) {
    const unsigned long intervals = digitElapsed / DIGIT_INTERVAL_MS;
    digitTimestamp += intervals * DIGIT_INTERVAL_MS;
    digit = static_cast<uint8_t>((digit + intervals % 10UL) % 10UL);
    displayDigit(digit);
  }

  // Repeating servo cycle:
  // 0-4 s: outward sweep; 4-6 s: hold;
  // 6-10 s: return sweep; 10-12 s: hold.
  // Phase starts at zero, so there is no initial endpoint pause.
  const unsigned long servoElapsed = now - servoTimestamp;
  servoTimestamp = now;
  servoPhase =
      (servoPhase + servoElapsed % SERVO_CYCLE_MS) % SERVO_CYCLE_MS;

  int angle;
  if (servoPhase < SWEEP_MS) {
    angle = static_cast<int>((180UL * servoPhase) / SWEEP_MS);
  } else if (servoPhase < SWEEP_MS + HOLD_MS) {
    angle = 180;
  } else if (servoPhase < 2UL * SWEEP_MS + HOLD_MS) {
    const unsigned long returnElapsed = servoPhase - SWEEP_MS - HOLD_MS;
    angle = 180 - static_cast<int>((180UL * returnElapsed) / SWEEP_MS);
  } else {
    angle = 0;
  }

  if (angle != lastServoAngle) {
    servo1.write(angle);
    servo2.write(180 - angle);
    lastServoAngle = angle;
  }
}
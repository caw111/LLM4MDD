#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t DATA_PIN  = A0;  // sr1 DS
constexpr uint8_t LATCH_PIN = A1;  // sr1 and sr2 STCP
constexpr uint8_t CLOCK_PIN = A2;  // sr1 and sr2 SHCP
constexpr uint8_t POT1_PIN  = A3;
constexpr uint8_t POT2_PIN  = A4;
constexpr uint8_t SERVO1_PIN = 2;
constexpr uint8_t SERVO2_PIN = 3;

constexpr unsigned long POT_INTERVAL_MS = 10;
constexpr unsigned long SERVO_INTERVAL_MS = 20;
constexpr unsigned long STARTUP_SETTLE_MS = 500;

// Bits 0..6 correspond to segments A..G. The displays are common-anode,
// so a LOW output illuminates a segment. Bit 7 stays HIGH (unused).
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

Servo servo1;
Servo servo2;

int currentAngle1 = 0;
int currentAngle2 = 0;
int targetAngle1 = 0;
int targetAngle2 = 0;

unsigned long startupTime = 0;
unsigned long lastPotTime = 0;
unsigned long lastServoTime = 0;
bool startupComplete = false;

void displayAngle(uint8_t angle) {
  const uint8_t tens = angle / 10;
  const uint8_t units = angle % 10;

  digitalWrite(LATCH_PIN, LOW);

  // sr1 Q7S feeds sr2 DS: send the farthest register's byte first.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[units]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);

  // Both displays change simultaneously.
  digitalWrite(LATCH_PIN, HIGH);
}

int readTargetAngle(uint8_t pin) {
  // Discard the first conversion after switching ADC channels.
  analogRead(pin);
  const int reading = analogRead(pin);
  return static_cast<int>(map(reading, 0, 1023, 0, 99));
}

bool stepServo(Servo &servo, int &currentAngle, int targetAngle) {
  if (currentAngle == targetAngle) {
    return false;
  }

  currentAngle += (targetAngle > currentAngle) ? 1 : -1;
  servo.write(currentAngle);
  return true;
}

void setup() {
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  displayAngle(0);

  servo1.attach(SERVO1_PIN);
  servo1.write(0);
  servo2.attach(SERVO2_PIN);
  servo2.write(0);

  // Keep the initial "00" and zero-degree commands while the servos settle.
  startupTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (!startupComplete) {
    if (now - startupTime < STARTUP_SETTLE_MS) {
      return;
    }

    startupComplete = true;
    targetAngle1 = readTargetAngle(POT1_PIN);
    targetAngle2 = readTargetAngle(POT2_PIN);
    lastPotTime = now;
    lastServoTime = now;
  }

  if (now - lastPotTime >= POT_INTERVAL_MS) {
    lastPotTime = now;
    targetAngle1 = readTargetAngle(POT1_PIN);
    targetAngle2 = readTargetAngle(POT2_PIN);
  }

  if (now - lastServoTime >= SERVO_INTERVAL_MS) {
    lastServoTime = now;

    const bool servo1Moved = stepServo(servo1, currentAngle1, targetAngle1);
    stepServo(servo2, currentAngle2, targetAngle2);

    // Display servo1's current commanded angle, not its final target.
    // These servos provide no physical position feedback.
    if (servo1Moved) {
      displayAngle(static_cast<uint8_t>(currentAngle1));
    }
  }
}
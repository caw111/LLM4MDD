#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring.
const uint8_t SR_DATA_PIN  = A0;
const uint8_t SR_LATCH_PIN = A1;
const uint8_t SR_CLOCK_PIN = A2;
const uint8_t POT1_PIN     = A3;
const uint8_t POT2_PIN     = A4;
const uint8_t SERVO_PINS[3] = {2, 3, 4};

const unsigned long STATE_HOLD_MS = 2000;
const unsigned long SERVO_STEP_MS = 20;

// The supplied Wokwi servos support 0..180 degrees.
// Change to 270 only when servo3 is actually a calibrated 270-degree servo.
const int SERVO3_MAX_DEGREES = 180;
const int SERVO3_MIN_PULSE_US = 544;
const int SERVO3_MAX_PULSE_US = 2400;

// Bits 0..6 correspond to segments A..G; bit 7 is unused.
// COM.1 is connected to +5 V, so the display is common-anode.
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

Servo servos[3];

int currentAngles[3] = {0, 90, 180};
int targetAngles[3]  = {0, 90, 180};
int pendingAngles[3] = {0, 90, 180};

uint8_t displayedDigit = 0;
uint8_t pendingDigit = 0;

bool holdingState = true;
unsigned long holdStartedMs = 0;
unsigned long lastServoStepMs = 0;

void showDigit(uint8_t digit) {
  digitalWrite(SR_LATCH_PIN, LOW);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_SEGMENTS[digit]));
  digitalWrite(SR_LATCH_PIN, HIGH);
}

void writeServoAngle(uint8_t index, int angle) {
  if (index == 2 && SERVO3_MAX_DEGREES > 180) {
    angle = constrain(angle, 0, SERVO3_MAX_DEGREES);
    const int pulseUs = static_cast<int>(
      map(angle, 0, SERVO3_MAX_DEGREES,
          SERVO3_MIN_PULSE_US, SERVO3_MAX_PULSE_US)
    );
    servos[index].writeMicroseconds(pulseUs);
  } else {
    servos[index].write(constrain(angle, 0, 180));
  }
}

void sampleInputs() {
  const int pot1Value = analogRead(POT1_PIN);
  const int pot2Value = analogRead(POT2_PIN);

  pendingDigit = static_cast<uint8_t>(map(pot1Value, 0, 1023, 0, 9));
  pendingAngles[0] = static_cast<int>(map(pot2Value, 0, 1023, 0, 90));
  pendingAngles[1] = static_cast<int>(map(pot2Value, 0, 1023, 90, 180));

  const int requestedServo3 =
    static_cast<int>(map(pot2Value, 0, 1023, 180, 270));
  pendingAngles[2] = constrain(requestedServo3, 0, SERVO3_MAX_DEGREES);
}

bool pendingStateChanged() {
  if (pendingDigit != displayedDigit) {
    return true;
  }

  for (uint8_t i = 0; i < 3; ++i) {
    if (pendingAngles[i] != targetAngles[i]) {
      return true;
    }
  }
  return false;
}

void setup() {
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);

  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  sampleInputs();
  displayedDigit = pendingDigit;
  showDigit(displayedDigit);

  for (uint8_t i = 0; i < 3; ++i) {
    currentAngles[i] = pendingAngles[i];
    targetAngles[i] = pendingAngles[i];
    servos[i].attach(SERVO_PINS[i]);
    writeServoAngle(i, currentAngles[i]);
  }

  // Allow initial positioning before beginning the verified-state hold.
  holdingState = false;
  lastServoStepMs = millis();
}

void loop() {
  const unsigned long now = millis();

  // Continuously track both potentiometers. To satisfy the minimum state
  // duration, commit their latest values only after the current state has
  // reached its servo targets and remained unchanged for two seconds.
  sampleInputs();

  if (holdingState) {
    if (now - holdStartedMs < STATE_HOLD_MS || !pendingStateChanged()) {
      return;
    }

    displayedDigit = pendingDigit;
    showDigit(displayedDigit);

    for (uint8_t i = 0; i < 3; ++i) {
      targetAngles[i] = pendingAngles[i];
    }

    holdingState = false;
    lastServoStepMs = now;
  }

  if (now - lastServoStepMs < SERVO_STEP_MS) {
    return;
  }
  lastServoStepMs = now;

  bool allAtTarget = true;

  // Slew by one degree per step rather than jumping directly to new angles.
  for (uint8_t i = 0; i < 3; ++i) {
    if (currentAngles[i] < targetAngles[i]) {
      ++currentAngles[i];
      writeServoAngle(i, currentAngles[i]);
    } else if (currentAngles[i] > targetAngles[i]) {
      --currentAngles[i];
      writeServoAngle(i, currentAngles[i]);
    }

    if (currentAngles[i] != targetAngles[i]) {
      allAtTarget = false;
    }
  }

  if (allAtTarget) {
    holdingState = true;
    holdStartedMs = now;
  }
}
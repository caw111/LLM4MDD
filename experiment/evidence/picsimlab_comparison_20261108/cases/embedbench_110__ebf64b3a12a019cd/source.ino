#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t LED_PIN = 2;
constexpr uint8_t RGB_PINS[2][3] = {
  {3, 4, 5},
  {6, 7, 8}
};
constexpr uint8_t SERVO_PINS[3] = {9, 10, 11};

constexpr unsigned long INTERVAL_MS = 2000;
constexpr unsigned long SERVO_UPDATE_MS = 20;

// Common-cathode RGB LEDs: HIGH turns a channel on.
constexpr uint8_t COLORS[][3] = {
  {255,   0,   0},  // Red
  {  0, 255,   0},  // Green
  {  0,   0, 255},  // Blue
  {255, 255,   0},  // Yellow
  {  0, 255, 255},  // Cyan
  {255,   0, 255},  // Magenta
  {255, 255, 255}   // White
};
constexpr uint8_t COLOR_COUNT = sizeof(COLORS) / sizeof(COLORS[0]);

Servo servos[3];

enum MotionPhase : uint8_t {
  STARTUP_HOLD,
  POSITION_OPERATIONAL_START,
  SCAN_FORWARD,
  SCAN_REVERSE
};

MotionPhase motionPhase = STARTUP_HOLD;
int segmentStart[3] = {0, 0, 0};
int segmentTarget[3] = {0, 0, 0};

uint8_t colorIndex = 0;
bool ledOn = false;
unsigned long segmentStartedAt = 0;
unsigned long lastServoUpdateAt = 0;

void showColor(uint8_t index) {
  for (uint8_t rgb = 0; rgb < 2; ++rgb) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      analogWrite(RGB_PINS[rgb][channel], COLORS[index][channel]);
    }
  }
}

void setTargets(int angle1, int angle2, int angle3) {
  segmentTarget[0] = angle1;
  segmentTarget[1] = angle2;
  segmentTarget[2] = angle3;
}

void advanceSynchronizedInterval() {
  // Finish the previous scan exactly at its endpoints.
  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(segmentTarget[i]);
    segmentStart[i] = segmentTarget[i];
  }

  ledOn = !ledOn;
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);

  colorIndex = (colorIndex + 1) % COLOR_COUNT;
  showColor(colorIndex);

  switch (motionPhase) {
    case STARTUP_HOLD:
      // Reconcile the two initialization requirements:
      // first hold ALL servos at 0 degrees for the startup interval,
      // then smoothly establish the specified operational pose.
      motionPhase = POSITION_OPERATIONAL_START;
      setTargets(0, 180, 90);
      break;

    case POSITION_OPERATIONAL_START:
    case SCAN_REVERSE:
      motionPhase = SCAN_FORWARD;
      setTargets(180, 0, 135);
      break;

    case SCAN_FORWARD:
      motionPhase = SCAN_REVERSE;
      setTargets(0, 180, 90);
      break;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  for (uint8_t rgb = 0; rgb < 2; ++rgb) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      pinMode(RGB_PINS[rgb][channel], OUTPUT);
    }
  }
  showColor(0);

  // On the Mega 2560 these three servos use Timer5, leaving
  // PWM on the wired LED/RGB pins 2 through 8 available.
  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
    servos[i].write(0);
  }

  segmentStartedAt = millis();
  lastServoUpdateAt = segmentStartedAt;
}

void loop() {
  const unsigned long now = millis();

  // Use one shared clock for blinking, color changes, and scan endpoints.
  // Advancing by the fixed interval prevents accumulated timing drift.
  bool intervalAdvanced = false;
  while (now - segmentStartedAt >= INTERVAL_MS) {
    segmentStartedAt += INTERVAL_MS;
    advanceSynchronizedInterval();
    intervalAdvanced = true;
  }

  if (intervalAdvanced || now - lastServoUpdateAt >= SERVO_UPDATE_MS) {
    lastServoUpdateAt = now;
    const unsigned long elapsed = now - segmentStartedAt;

    // Linear interpolation makes each synchronized scan take two seconds,
    // including servo3's shorter 45-degree travel.
    for (uint8_t i = 0; i < 3; ++i) {
      const long travel = segmentTarget[i] - segmentStart[i];
      const int angle = segmentStart[i]
                      + (travel * static_cast<long>(elapsed))
                      / static_cast<long>(INTERVAL_MS);
      servos[i].write(angle);
    }
  }
}
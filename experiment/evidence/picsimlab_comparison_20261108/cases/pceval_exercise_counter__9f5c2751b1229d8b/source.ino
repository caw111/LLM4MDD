#include <Wire.h>
#include <MPU6050.h>

// Common-cathode display: A, B, C, D, E, F, G.
const uint8_t SEGMENT_PINS[7] = {2, 3, 4, 5, 6, 7, 8};

// Bit 0 = A, bit 1 = B, ..., bit 6 = G.
const uint8_t DIGIT_PATTERNS[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

const int32_t TRIGGER_THRESHOLD = 15000;
const int32_t RESET_THRESHOLD = 2000;
const unsigned long RETRY_INTERVAL_MS = 1000;

MPU6050 mpu;
uint32_t exerciseCount = 0;
bool sensorConnected = false;
bool armed = false;
unsigned long lastConnectionAttempt = 0;

void displayCount() {
  // A single digit displays the count modulo 10: 9 is followed by 0.
  const uint8_t pattern = DIGIT_PATTERNS[exerciseCount % 10];

  for (uint8_t segment = 0; segment < 7; ++segment) {
    digitalWrite(SEGMENT_PINS[segment],
                 (pattern & (1U << segment)) ? HIGH : LOW);
  }
}

void connectSensor() {
  lastConnectionAttempt = millis();
  mpu.initialize();
  sensorConnected = mpu.testConnection();
  armed = false;

  if (sensorConnected) {
    // Keep readings in raw gyro units at the default +/-250 degrees/s range.
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    Serial.println(F("MPU6050 connected."));
  } else {
    Serial.println(F("MPU6050 connection failed; retrying."));
  }
}

void setup() {
  for (uint8_t segment = 0; segment < 7; ++segment) {
    pinMode(SEGMENT_PINS[segment], OUTPUT);
    digitalWrite(SEGMENT_PINS[segment], LOW);
  }

  Serial.begin(115200);
  Wire.begin(); // Uno: SDA = A4, SCL = A5.
  connectSensor();
  displayCount();
}

void loop() {
  displayCount();

  if (!sensorConnected) {
    if (millis() - lastConnectionAttempt >= RETRY_INTERVAL_MS) {
      connectSensor();
    }
    return;
  }

  // Promote before negation so that -32768 is handled correctly on the Uno.
  const int32_t rotationY = mpu.getRotationY();
  const int32_t magnitudeY = (rotationY < 0) ? -rotationY : rotationY;

  if (magnitudeY < RESET_THRESHOLD) {
    armed = true;
  } else if (armed && magnitudeY > TRIGGER_THRESHOLD) {
    ++exerciseCount;
    armed = false;
  }

  displayCount();
  delay(5);
}
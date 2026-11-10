#include <Arduino.h>

const int TRIG_PIN = 3;   // connected to Arduino pin3
const int ECHO_PIN = 2;   // connected to Arduino pin2
const long BAUD_RATE = 9600;

static long readDistanceCmInteger() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse duration in microseconds
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  // Distance calculation: distance_cm = duration / 58
  // (Integer centimeters as required)
  return (duration == 0) ? 0 : (long)(duration / 58);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(BAUD_RATE);
  // Wait briefly for Serial to be ready (harmless if not supported)
  unsigned long start = millis();
  while (!Serial && (millis() - start < 2000)) {
    // no-op
  }

  Serial.setTimeout(1000);
}

void loop() {
  // 1. Continuously trigger and read the sensor
  long measuredCm = readDistanceCmInteger();

  // 2. Print prompt and read serial input
  Serial.print("Enter a distance in cm to compare: ");
  long targetCm = 0;

  // Read integer distance from serial; block until available or timeout
  if (Serial.available() <= 0) {
    // Give time for user to type; avoids tight printing
    delay(10);
  }

  if (Serial.available() > 0) {
    targetCm = Serial.parseInt();
    // If parseInt fails, it returns 0; behavior still allows comparison as-is.
  } else {
    // If nothing provided, keep targetCm as 0
  }

  // 3. Compare and print result
  if (measuredCm > targetCm) {
    Serial.println("Measured distance is greater");
  } else if (measuredCm == targetCm) {
    Serial.println("Measured distance is equal to the target");
  } else {
    Serial.println("Measured distance is less");
  }

  // Small delay to prevent overwhelming serial output
  delay(50);
}
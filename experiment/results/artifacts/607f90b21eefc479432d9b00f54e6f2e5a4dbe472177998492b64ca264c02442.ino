#include <Arduino.h>

const int trigPin = 3;  // TRIG -> pin3
const int echoPin = 2;  // ECHO -> pin2

static bool targetPrompted = false;
static int targetCm = 0;

long readUltrasonicDistanceCm() {
  // Ensure a clean reading
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trigger pulse (at least 10us)
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse duration
  unsigned long duration = pulseIn(echoPin, HIGH);

  // Distance calculation: duration / 58 (integer cm)
  // If pulseIn times out, duration may be 0; keep behavior deterministic.
  return (long)(duration / 58UL);
}

static bool readTargetFromSerial(int &outValue) {
  // Wait until we receive at least one complete integer line.
  // The environment specifies "framing=line_or_idle" and idle_timeout_ms=20,
  // so we can use a small timeout.
  unsigned long start = millis();
  while (millis() - start < 200) { // bounded wait
    if (Serial.available() > 0) {
      // read first integer token (handles newline/space)
      long v = Serial.parseInt();
      if (v != 0 || Serial.peek() == EOF) {
        // parseInt returns 0 if no valid number found; but if target can be 0
        // that's unlikely. Still, accept if digits were parsed.
      }
      // parseInt sets it even if 0 is read; we need to know if something was parsed.
      // We'll check by ensuring there is still no non-digit buffered for a moment.
      // Simpler: attempt to parse again if no digits were present.
      // parseInt does skip non-digit by default, so we accept parseInt result when any data existed.
      outValue = (int)v;
      return true;
    }
  }
  return false;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(trigPin, LOW);

  Serial.begin(9600);
  // Wait briefly for Serial Monitor to connect (harmless if not connected)
  unsigned long start = millis();
  while (!Serial && (millis() - start) < 2000) { }
}

void loop() {
  if (!targetPrompted) {
    Serial.print("Enter a distance in cm to compare: ");
    // Read target from serial input
    int v;
    if (readTargetFromSerial(v)) {
      targetCm = v;
      targetPrompted = true;
      // Consume remaining characters on the line if any
      while (Serial.available() > 0) Serial.read();
    }
    // Even before target is read, continue attempting to read without blocking too long.
    return;
  }

  // Continuously trigger and read sensor, compare with input, and print result.
  long measured = readUltrasonicDistanceCm(); // already integer cm

  if (measured > targetCm) {
    Serial.println("Measured distance is greater");
  } else if (measured == targetCm) {
    Serial.println("Measured distance is equal to the target");
  } else {
    Serial.println("Measured distance is less");
  }

  delay(100); // modest pacing to avoid flooding
}
#include <Arduino.h>

const uint8_t TRIG_PIN = 8;
const uint8_t ECHO_PIN = 9;
const unsigned long ECHO_TIMEOUT_US = 30000UL;
const unsigned long MEASUREMENT_INTERVAL_MS = 100UL;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
  Serial.setTimeout(20);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long durationUs =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (durationUs == 0) {
    Serial.println(F("Distance: no echo (timeout)"));
  } else {
    const float distanceCm = durationUs * 0.0343f / 2.0f;
    Serial.print(F("Distance: "));
    Serial.print(distanceCm, 2);
    Serial.println(F(" cm"));
  }

  delay(MEASUREMENT_INTERVAL_MS);
}
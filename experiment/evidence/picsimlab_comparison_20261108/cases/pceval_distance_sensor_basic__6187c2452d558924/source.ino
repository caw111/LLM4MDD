#include <Arduino.h>

const uint8_t ECHO_PIN = 2;
const uint8_t TRIG_PIN = 3;
// Sensor VCC connects to 5V; sensor GND connects to GND.3.
const unsigned long ECHO_TIMEOUT_US = 30000UL;
float distanceCm = 400.0f;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  delay(60);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long durationUs =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (durationUs > 0) {
    distanceCm = durationUs / 58.0f;
    Serial.print(F("Distance in CM: "));
    Serial.println(distanceCm, 2);
  } else {
    Serial.println(F("Distance in CM: Out of range"));
  }

  delay(60);
}
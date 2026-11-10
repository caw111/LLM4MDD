#include <Arduino.h>

const uint8_t TRIG_PIN = 8;
const uint8_t ECHO_PIN = 9;
const uint8_t LED_PIN = 13;

const float DISTANCE_THRESHOLD_CM = 20.0f;
const unsigned long ECHO_TIMEOUT_US = 30000UL;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  const float distanceCm = duration * 0.0343f / 2.0f;

  // A timeout is not a valid distance measurement.
  digitalWrite(LED_PIN,
               (duration > 0 && distanceCm < DISTANCE_THRESHOLD_CM)
                   ? HIGH
                   : LOW);

  delay(60);
}
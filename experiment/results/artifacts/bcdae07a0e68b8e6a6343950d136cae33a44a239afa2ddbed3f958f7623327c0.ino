#include <Arduino.h>

const uint8_t TRIG_PIN = 8;
const uint8_t ECHO_PIN = 9;
const uint8_t LED_PIN = 13;
const uint8_t BUZZER_PIN = 6;

const float ALARM_DISTANCE_CM = 20.0f;
const unsigned int ALARM_FREQUENCY_HZ = 2000;
const unsigned long ECHO_TIMEOUT_US = 30000UL;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoDuration =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  const float distanceCm = echoDuration * 0.0343f / 2.0f;

  // A timeout is not a valid distance measurement.
  const bool alarm =
      echoDuration > 0 && distanceCm < ALARM_DISTANCE_CM;

  digitalWrite(LED_PIN, alarm ? HIGH : LOW);

  if (alarm) {
    tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Allow sufficient time between ultrasonic measurements.
  delay(60);
}
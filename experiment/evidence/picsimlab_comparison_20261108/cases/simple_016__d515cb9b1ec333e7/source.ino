#include <Arduino.h>

const uint8_t TRIG_PIN = 8;
const uint8_t ECHO_PIN = 9;
const uint8_t LED_PIN  = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  // Ensure a clean low period before triggering
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send a 10us trigger pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  // Convert time to distance (cm)
  // distance = (duration * speed_of_sound) / 2
  // With duration in microseconds and speed of sound ~ 0.0343 cm/us:
  // distance_cm = duration * 0.0343 / 2
  float distance_cm = (duration * 0.0343f) / 2.0f;

  if (distance_cm < 20.0f) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
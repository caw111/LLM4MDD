#include <Arduino.h>

const uint8_t DIST_TRIG_PIN = 3;
const uint8_t DIST_ECHO_PIN = 2;

static unsigned long readDistanceCm() {
  // Trigger a 10us pulse
  digitalWrite(DIST_TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(DIST_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(DIST_TRIG_PIN, LOW);

  // Read echo pulse width (in microseconds)
  unsigned long duration = pulseIn(DIST_ECHO_PIN, HIGH);

  // Convert to distance in centimeters
  // distance_cm = duration / 58
  if (duration == 0) return 0;
  return (unsigned long)(duration / 58UL);
}

void setup() {
  pinMode(DIST_TRIG_PIN, OUTPUT);
  pinMode(DIST_ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  unsigned long distanceCm = readDistanceCm();
  Serial.print("Distance in CM: ");
  Serial.println(distanceCm);
  delay(50);
}
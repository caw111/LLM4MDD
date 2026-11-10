#include <Arduino.h>

const uint8_t TRIG_PIN = 8; // D8
const uint8_t ECHO_PIN = 9; // D9

static long readDistanceCm() {
  // Ensure a clean trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width
  // Timeout prevents hanging if no echo is received.
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, 30000UL); // up to ~3.4m round-trip

  if (durationUs == 0) {
    return -1; // signal timeout/no echo
  }

  // Speed of sound: 343 m/s -> 0.0343 cm/us
  // Distance (cm) = duration(us) * 0.0343 / 2
  // Using typical Arduino-friendly constant: 58 us per cm (round trip)
  long distanceCm = (long)(durationUs / 58UL);

  return distanceCm;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(9600);
}

void loop() {
  long distanceCm = readDistanceCm();

  Serial.println(distanceCm);

  // Small pacing to match HC-SR04 limitations / prevent flooding serial
  delay(60);
}
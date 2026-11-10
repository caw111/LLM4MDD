#include <Arduino.h>

const uint8_t TRIG_PIN = 8;   // HC-SR04 Trig = D8
const uint8_t ECHO_PIN = 9;   // HC-SR04 Echo = D9

const uint8_t LED_PIN  = 13;  // LED = D13
const uint8_t BUZZ_PIN = 6;   // Buzzer = D6

// Distance threshold
const float DIST_THRESHOLD_CM = 20.0f;

// Measurement parameters
// HC-SR04 typical: measure pulse width, convert to distance.
// speed of sound ~ 343 m/s -> 0.0343 cm/us
// distance (cm) = duration_us * 0.0343 / 2
static float readDistanceCm() {
  // Ensure clean trig
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Trigger pulse: at least 10us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width
  // Timeout to avoid blocking forever if no echo
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, 30000UL); // up to ~5m (30ms)
  if (durationUs == 0) {
    // No echo received within timeout
    return 9999.0f;
  }

  float distanceCm = (durationUs * 0.0343f) / 2.0f;
  return distanceCm;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZ_PIN, LOW);
}

void loop() {
  float distance = readDistanceCm();

  if (distance < DIST_THRESHOLD_CM) {
    digitalWrite(LED_PIN, HIGH);

    // Alarm behavior on D6 using tone
    // (No specified frequency/duration; keep simple continuous alert while in range.)
    tone(BUZZ_PIN, 2000); // 2kHz
  } else {
    digitalWrite(LED_PIN, LOW);

    // Stop alarm
    noTone(BUZZ_PIN);
    digitalWrite(BUZZ_PIN, LOW);
  }

  delay(50);
}
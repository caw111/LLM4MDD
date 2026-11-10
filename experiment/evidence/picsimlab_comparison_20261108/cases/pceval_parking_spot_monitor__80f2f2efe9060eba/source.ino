#include <Arduino.h>

const int TRIG_PIN = 8;
const int ECHO_PIN = 9;

const int RGB_R_PIN = 4;
const int RGB_G_PIN = 3;
const int RGB_B_PIN = 2;

static long readDistanceCm() {
  // Trigger ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width in microseconds
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH);

  // Convert to distance (cm). HC-SR04: distance = duration / 58
  // If pulseIn times out, durationUs can be 0.
  long cm = (durationUs == 0) ? -1 : (long)(durationUs / 58UL);
  return cm;
}

static void setRGB(bool redOn, bool greenOn, bool blueOn) {
  // Common cathode LED: setting pin HIGH turns that color ON.
  digitalWrite(RGB_R_PIN, redOn ? HIGH : LOW);
  digitalWrite(RGB_G_PIN, greenOn ? HIGH : LOW);
  digitalWrite(RGB_B_PIN, blueOn ? HIGH : LOW);
}

void setup() {
  // Expected: set pin8, pin2 ~ pin4 as OUTPUT, and pin9 as INPUT
  pinMode(TRIG_PIN, OUTPUT);   // pin8
  pinMode(RGB_B_PIN, OUTPUT);  // pin2
  pinMode(RGB_G_PIN, OUTPUT);  // pin3
  pinMode(RGB_R_PIN, OUTPUT);  // pin4
  pinMode(ECHO_PIN, INPUT);    // pin9

  // Initialize RGB to off
  setRGB(false, false, false);
}

void loop() {
  long distanceCm = readDistanceCm();

  if (distanceCm < 50) {
    // distance smaller than 50 => red on
    setRGB(true, false, false);
  } else {
    // distance >= 50 => green on
    setRGB(false, true, false);
  }

  // Continue continuously triggering/reading
  delay(50);
}
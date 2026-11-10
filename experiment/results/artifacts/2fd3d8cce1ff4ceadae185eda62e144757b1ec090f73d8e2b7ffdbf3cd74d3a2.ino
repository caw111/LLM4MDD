#include <Arduino.h>

constexpr uint8_t R_PIN = 4;
constexpr uint8_t G_PIN = 3;
constexpr uint8_t B_PIN = 2;

void setup() {
  // Common-anode RGB LED: HIGH is off, LOW is fully on.
  // Preload output levels to prevent red or green startup flashes.
  digitalWrite(R_PIN, HIGH);
  digitalWrite(G_PIN, HIGH);
  digitalWrite(B_PIN, LOW);

  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
}

void loop() {
  // Blue remains fully on; no further actions are required.
}
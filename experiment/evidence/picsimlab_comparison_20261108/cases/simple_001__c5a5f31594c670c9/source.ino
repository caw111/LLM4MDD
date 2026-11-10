#include <Arduino.h>

void setup() {
  pinMode(13, OUTPUT);          // D13 LED output
  delay(20);                    // Ensure LED turns on within 20ms after initialization
  digitalWrite(13, HIGH);      // Keep constant ON
}

void loop() {
  // Keep LED constantly ON
  digitalWrite(13, HIGH);
}
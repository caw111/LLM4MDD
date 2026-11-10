#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  // No additional setup required
}

void loop() {
  if (Serial.available() > 0) {
    String value = Serial.readStringUntil('\n');
    value.trim(); // Remove \r/newline and surrounding whitespace

    if (value.length() > 0) {
      Serial.print("Received: ");
      Serial.println(value);
    } else {
      // If only whitespace/newline was received, ignore it.
    }
  }
}
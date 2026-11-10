#include <Arduino.h>

void setup() {
  Serial.begin(9600);

  // Send startup message
  Serial.println("LED Blink test starts");

  // Configure LED pin
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  // ON for 1 second
  digitalWrite(13, HIGH);
  delay(1000);

  // OFF for 1 second
  digitalWrite(13, LOW);
  delay(1000);
}
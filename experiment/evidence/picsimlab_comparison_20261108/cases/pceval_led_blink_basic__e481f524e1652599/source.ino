#include <Arduino.h>

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  while (!Serial) { } // ensure serial is ready (safe on boards that support it)

  Serial.println("LED Blink test starts");
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}
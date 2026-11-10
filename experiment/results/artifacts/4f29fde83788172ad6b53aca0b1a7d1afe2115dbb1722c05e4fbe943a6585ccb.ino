#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(A2);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  delay(500);
}
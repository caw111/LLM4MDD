/**
  Read photoresistor value and write to serial monitor
  2 APR 2025 by Blind Author
*/

#define LDR_PIN A2

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  delay(500);
}

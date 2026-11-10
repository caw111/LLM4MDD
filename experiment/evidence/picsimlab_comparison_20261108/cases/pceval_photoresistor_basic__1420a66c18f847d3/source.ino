void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(A2);
  Serial.print("LDR Value: ");
  Serial.println(value);
  delay(500);
}
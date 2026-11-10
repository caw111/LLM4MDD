void setup() {
  pinMode(4, OUTPUT); // R pin
  pinMode(3, OUTPUT); // G pin
  pinMode(2, OUTPUT); // B pin

  // Common pin is connected to 5V, so active segment is driven low.
  digitalWrite(4, HIGH); // R off
  digitalWrite(3, HIGH); // G off
  digitalWrite(2, LOW);  // B on (full brightness)
}

void loop() {
  // No actions required
}
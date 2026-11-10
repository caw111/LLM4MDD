void setup() {
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);

  // RGB LED common (COM) is connected to 5V.
  // Turn on only blue segment at full brightness (active LOW).
  digitalWrite(4, HIGH); // R off
  digitalWrite(3, HIGH); // G off
  digitalWrite(2, LOW);  // B on
}

void loop() {
  // No actions required
}
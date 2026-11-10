void setup() {
  pinMode(13, OUTPUT);     // D13 LED
  delay(20);               // Ensure it lights within 20ms after initialization
  digitalWrite(13, HIGH); // Constant ON
}

void loop() {
  digitalWrite(13, HIGH); // Keep it constantly on
}
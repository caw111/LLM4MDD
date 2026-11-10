void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  static bool state = false;

  state = !state;

  // Switch LED states every 1 second
  digitalWrite(12, state ? HIGH : LOW);
  digitalWrite(13, state ? LOW : HIGH);

  delay(1000);
}
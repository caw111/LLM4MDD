const uint8_t LED_PIN = 2;

void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);

  digitalWrite(LED_PIN, LOW);
  delay(2000);

  digitalWrite(LED_PIN, HIGH);
  delay(3000);

  digitalWrite(LED_PIN, LOW);
  delay(3000);
}
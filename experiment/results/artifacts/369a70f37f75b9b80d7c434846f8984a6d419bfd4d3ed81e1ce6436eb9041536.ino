// Arduino Uno sketch
// Requirement: Make the buzzer on D6 beep HIGH for 200ms every 2 seconds.

const uint8_t BUZZER_PIN = 6;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(2000 - 200); // remaining time to make the period 2 seconds total
}
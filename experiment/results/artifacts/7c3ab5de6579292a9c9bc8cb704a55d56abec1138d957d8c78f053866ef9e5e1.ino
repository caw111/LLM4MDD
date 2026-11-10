// Arduino Uno sketch
// Requirement: When button on D3 is pressed, D6 buzzer beeps continuously.

const uint8_t BUTTON_PIN = 3;   // D3
const uint8_t BUZZER_PIN  = 6;   // D6

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // assume button connects to GND when pressed
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // Active-low button: pressed => LOW (due to INPUT_PULLUP)
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Continuous beeping while pressed
    tone(BUZZER_PIN, 2000); // 2 kHz audible tone
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}
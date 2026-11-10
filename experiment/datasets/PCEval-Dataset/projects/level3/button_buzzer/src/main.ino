/*
    Use a pushbutton to control piezoelectric buzzer.
    9 APR 2025 by Blind Author
*/

const int BUTTON_PIN = 3;
const int BUZZER_PIN = 9;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    tone(BUZZER_PIN, 262, 250);
  } else noTone(BUZZER_PIN);
}

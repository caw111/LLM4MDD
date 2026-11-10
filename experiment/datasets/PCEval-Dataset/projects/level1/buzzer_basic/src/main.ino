/*
    Use a piezoelectric buzzer to play 262Hz tone.
    4 APR 2025 by Blind Author
*/

const int BUZZER_PIN = 2;

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
    tone(BUZZER_PIN, 262, 250);
}


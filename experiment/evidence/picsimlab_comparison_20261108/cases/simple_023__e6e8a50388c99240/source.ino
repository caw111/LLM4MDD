/*
  Requirements:
  - Read A0 potentiometer value.
  - Map it to change the frequency of the D6 buzzer (tone).
  - Arduino Uno: A0 analog input, D6 digital output for tone.
*/

const uint8_t BUZZER_PIN = 6;  // D6

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  int adc = analogRead(A0); // 0..1023

  // Map potentiometer to frequency range.
  // (Preserves the behavior: A0 controls buzzer frequency.)
  // Choose a practical audible range for UNO tone():
  //  - min 100 Hz
  //  - max 2000 Hz
  long freq = map(adc, 0, 1023, 100, 2000);

  // Use tone on D6. Duration is set to 50ms and loop repeats to update freq.
  tone(BUZZER_PIN, (unsigned int)freq, 50);

  // Small delay to keep updates responsive while not blocking too long.
  delay(10);
}
/**
  Turn on the corresponding LED when the brightness detected by the photoresistor is lower
  07 MAY 2025 by Blind Author
*/

const int LDR_pins[] = {A0, A1, A2, A3, A4};
const int led_pins[] = {2, 3, 4, 5, 6};

int threshold = 500;

void setup() {
  for (int i=0; i<5; i++) {
    pinMode(LDR_pins[i], INPUT);
    pinMode(led_pins[i], OUTPUT);
  }
}

void loop() {
  for (int i=0; i<5; i++) {
    int analogValue = analogRead(LDR_pins[i]);
    if (analogValue > threshold) {
      digitalWrite(led_pins[i], HIGH);
    } else {
      digitalWrite(led_pins[i], LOW);
    }
  }

  delay(1000);
}

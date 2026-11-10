/**
  Control bar LED with serial monitor
  10 APR 2025 by Blind Author
*/

int pinList[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

int threshold = 0;

void setup() {
  for (int i=0; i<10; i++) {
    pinMode(pinList[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    threshold = Serial.parseInt();
  }

  for (int i=0; i<10; i++) {
    if (i < threshold) {
      digitalWrite(pinList[i], HIGH);
    } else {
      digitalWrite(pinList[i], LOW);
    }
  }
  delay(100);
}
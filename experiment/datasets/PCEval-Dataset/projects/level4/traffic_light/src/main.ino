/**
  Creating a traffic light with leds and button.
  02 MAY 2025 by Blind Author
*/

const int BUTTON_PIN = 2;
const int RED_LED = 8;
const int YELLOW_LED = 9;
const int GREEN_LED = 10;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
}

void loop() {

  if (digitalRead(BUTTON_PIN)==HIGH) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    delay(1000);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(2000);
    digitalWrite(RED_LED, LOW);
  }

  digitalWrite(GREEN_LED, HIGH);
  delay(500);
}

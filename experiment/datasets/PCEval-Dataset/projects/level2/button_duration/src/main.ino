/**
  Determine how long a switch is pressed
  6 APR 2025 by Blind Author
*/

const int inPin = 4;

bool prevButtonState = HIGH;
unsigned long pressStartTime = 0;
unsigned long pressDuration = 0;

void setup() {
  pinMode(inPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  bool buttonState = digitalRead(inPin);

  if (prevButtonState == HIGH && buttonState == LOW) {
    pressStartTime = millis();
  }

  if (prevButtonState == LOW && buttonState == HIGH) {
    pressDuration = (millis() - pressStartTime) / 1000;
    Serial.print("Button pressed duration: ");
    Serial.print(pressDuration);
    Serial.println("s");
  }

  prevButtonState = buttonState;

  delay(10);
}

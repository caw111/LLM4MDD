/**
  Press the button to turn on the LED
  31 Mar 2025 by Blind Author
*/

const int BUTTON_PIN = 2;
const int LED_PIN = 13;

void setup() {
  Serial.begin(9600);
  Serial.println("Button LED test starts");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == LOW) {
    digitalWrite(LED_PIN, HIGH);
  } 

  else {
    digitalWrite(LED_PIN, LOW);
  }
  
}
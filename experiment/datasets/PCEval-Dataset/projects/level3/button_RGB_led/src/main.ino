/**
  Press the button to turn on the LED
  8 APR 2025 by Blind Author
*/

const int BUTTON_R = 4;
const int BUTTON_G = 3;
const int BUTTON_B = 2;

const int LED_R = 11;
const int LED_G = 10;
const int LED_B = 9;

void setup() {
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_G, INPUT);
  pinMode(BUTTON_R, INPUT);

  pinMode(LED_B, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
}

void loop() {
  if (digitalRead(BUTTON_R) == LOW) analogWrite(LED_R, 255);
  else analogWrite(LED_R, 0);
  if (digitalRead(BUTTON_G) == LOW) analogWrite(LED_G, 255);
  else analogWrite(LED_G, 0);
  if (digitalRead(BUTTON_B) == LOW) analogWrite(LED_B, 255);
  else analogWrite(LED_B, 0);
}
/**
  Control RGB LED with serial monitor
  10 APR 2025 by Blind Author
*/
const int LED_R = 4;
const int LED_G = 3;
const int LED_B = 2;

void setup() {
  pinMode(LED_B, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.indexOf("Red") != -1) {
      analogWrite(LED_R, 0);
    } else analogWrite(LED_R, 255);

    if (input.indexOf("Green") != -1) {
        analogWrite(LED_G, 0);
    } else analogWrite(LED_G, 255);

    if (input.indexOf("Blue") != -1) {
        analogWrite(LED_B, 0);
    } else analogWrite(LED_B, 255);
  }
}
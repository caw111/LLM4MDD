/* 
    Press the button to control servo motor
    8 APR 2025 by Blind Author
*/

#include <Servo.h>

Servo myservo;

const int BUTTON_PIN = 3;
const int SERVO_PIN = 9;

void setup() {
  Serial.begin(9600);
  myservo.attach(SERVO_PIN);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    myservo.write(180);
    delay(10); 
  } else {
    myservo.write(90);
    delay(10); 
  }

  Serial.println(myservo.read());
  delay(100);
}


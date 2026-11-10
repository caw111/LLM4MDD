/* 
    Control servo motor
    31 Mar 2025 by Blind Author
*/

#include <Servo.h>

Servo myservo;

int pos = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Servo Motor Control test starts");
  myservo.attach(9);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(10);
  }

  Serial.println(myservo.read());

  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(10);
  }

  Serial.println(myservo.read());
}


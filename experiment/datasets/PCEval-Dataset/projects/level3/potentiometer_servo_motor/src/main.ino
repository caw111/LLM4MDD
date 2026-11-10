/* 
    Control servo motor with potentiometer
    11 APR 2025 by Blind Author
*/

#include <Servo.h>

Servo myservo;

const int POT_PIN = A0;
const int SERVO_PIN = 9;

void setup() {
    Serial.begin(9600);
    myservo.attach(SERVO_PIN);
    pinMode(POT_PIN, INPUT);
}

void loop() {
    int pot = analogRead(POT_PIN);
    myservo.write(map(pot, 0, 1023, 0, 180));

    Serial.println(myservo.read());
    delay(100);
}


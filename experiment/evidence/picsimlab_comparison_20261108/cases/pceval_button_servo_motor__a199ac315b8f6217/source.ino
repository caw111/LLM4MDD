#include <Servo.h>

static const uint8_t servoPin = 9;
static const uint8_t buttonPin = 3;

static const int servoAngle1 = 180; // when pressed
static const int servoAngle2 = 90;  // when not pressed

Servo servoObj;

void setup() {
  Serial.begin(9600);

  servoObj.attach(servoPin);

  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttonValue = digitalRead(buttonPin);

  // Expected behavior: print the angle of the servo motor continuously
  // Set servo angle based on button pressed/not pressed
  int angle = (buttonValue == HIGH) ? servoAngle2 : servoAngle1;

  servoObj.write(angle);
  Serial.println(angle);

  delay(20);
}
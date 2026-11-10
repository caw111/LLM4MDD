#include <Arduino.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
int ledPins[] = {2, 3, 4};  
int rgbPins[] = {5, 6, 7};  
int servoPins[] = {8, 9};   
int currentLed = 0;
int currentColor = 0;
int servoPos = 0;
int sweepDirection = -1;

void setup();
void loop();
void setColor(int color);
void sweepServos();
void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(rgbPins[i], OUTPUT);
  }
  servo1.attach(servoPins[0]);
  servo2.attach(servoPins[1]);
  servo1.write(0);
  servo2.write(0);
  setColor(0); 
}
void loop() {
  digitalWrite(ledPins[currentLed], HIGH);
  setColor(currentColor);
  sweepServos();
  delay(200);
  digitalWrite(ledPins[currentLed], LOW);
  currentLed = (currentLed + 1) % 3;
  currentColor = (currentColor + 1) % 3;
}
void setColor(int color) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(rgbPins[i], LOW);
  }
  digitalWrite(rgbPins[color], HIGH);
}
void sweepServos() {
  if (servoPos == 0 || servoPos == 180) {
    sweepDirection *= -1;
    delay(2000);
  }
  if (sweepDirection == 1 )
  {    while(servoPos < 180)
    {servoPos += sweepDirection * 10;
    servo1.write(servoPos);
    servo2.write(servoPos);
    delay(100);}}
  if (sweepDirection == -1 )
  {  while(servoPos > 0 )
    {servoPos += sweepDirection * 10;
    servo1.write(servoPos);
    servo2.write(servoPos);
    delay(100);}}
}
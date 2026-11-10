#include <Arduino.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
const int btn1Pin = 2;  
const int btn2Pin = 3;  
const int btn3Pin = 4;  
const int redPin = 5;   
const int greenPin = 6; 
const int bluePin = 7;  
const int servo1Pin = 8; 
const int servo2Pin = 9; 

void setup();
void loop();
void setColor(int red, int green, int blue);
void setup() {
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(btn3Pin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(0);
  servo2.write(0);
  setColor(0, 0, 0); 
}
void loop() {
  if (digitalRead(btn1Pin) == LOW) {
    delay(20); 
    if (digitalRead(btn1Pin) == LOW) {
      servo1.write(90);
      setColor(255, 0, 0); 
      delay(2000); 
      while (digitalRead(btn1Pin) == LOW); 
    }
  }
  if (digitalRead(btn2Pin) == LOW) {
    delay(20); 
    if (digitalRead(btn2Pin) == LOW) {
      servo2.write(90);
      setColor(0, 255, 0); 
      delay(2000); 
      while (digitalRead(btn2Pin) == LOW); 
    }
  }
  if (digitalRead(btn3Pin) == LOW) {
    delay(20); 
    if (digitalRead(btn3Pin) == LOW) {
      servo1.write(0);
      servo2.write(0);
      setColor(0, 0, 255); 
      delay(2000); 
      while (digitalRead(btn3Pin) == LOW); 
    }
  }
}
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
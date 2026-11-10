#include <Arduino.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
const int btn1Pin = 2;  
const int btn2Pin = 3;  
const int redPin = 4;   
const int greenPin = 5; 
const int bluePin = 6;  
bool btn1State = false; 
bool btn2State = false; 

void setup();
void loop();
void resetState();
void setRGB(int red, int green, int blue);
void setup() {
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  servo1.attach(7);  
  servo2.attach(8);  
  servo1.write(0);
  servo2.write(0);
  setRGB(0, 0, 0); 
}
void loop() {
  bool btn1Pressed = digitalRead(btn1Pin) == LOW;
  bool btn2Pressed = digitalRead(btn2Pin) == LOW;
  if (btn1Pressed && !btn1State) {
    if (btn2State) {
      resetState();
    } else {
      servo1.write(90);
      setRGB(255, 0, 0); 
      delay(2000); 
      btn1State = true;
    }
  } else if (btn2Pressed && !btn2State) {
    if (btn1State) {
      resetState();
    } else {
      servo2.write(180);
      setRGB(0, 0, 255); 
      delay(2000); 
      btn2State = true;
    }
  } 
}
void resetState() {
  servo1.write(0);
  servo2.write(0);
  setRGB(0, 255, 0); 
  delay(2000); 
  btn1State = false;
  btn2State = false;
}
void setRGB(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
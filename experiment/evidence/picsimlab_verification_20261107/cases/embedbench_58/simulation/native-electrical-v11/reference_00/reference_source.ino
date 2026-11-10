#include <Arduino.h>
#include <Servo.h>
const int RGB_RED_PIN = 2;       
const int RGB_GREEN_PIN = 3;     
const int RGB_BLUE_PIN = 4;      
const int SHCP_PIN = A2;         
const int STCP_PIN = A1;         
const int DS_PIN = A0;           
const int SERVO_PIN = 5;         
Servo servo1;
const byte segmentCodes[] = {
  0b11000000, 
  0b11111001, 
  0b10100100, 
  0b10110000, 
  0b10011001, 
  0b10010010, 
  0b10000010, 
  0b11111000, 
  0b10000000, 
  0b10010000  
};
const int colors[7][3] = {
  {255, 0, 0},   
  {0, 255, 0},   
  {0, 0, 255},   
  {255, 255, 0}, 
  {0, 255, 255}, 
  {255, 0, 255}, 
  {255, 255, 255} 
};
int countdown = 9;
int colorIndex = 0;

void setup();
void loop();
void displayNumber(int number);
void setup() {
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(SHCP_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(DS_PIN, OUTPUT);
  servo1.attach(SERVO_PIN);
  servo1.write(0);
  displayNumber(countdown);
}
void loop() {
  analogWrite(RGB_RED_PIN, colors[colorIndex][0]);
  analogWrite(RGB_GREEN_PIN, colors[colorIndex][1]);
  analogWrite(RGB_BLUE_PIN, colors[colorIndex][2]);
  displayNumber(countdown);
  int angle = map(countdown, 9, 0, 0, 180);
  servo1.write(angle);
  delay(2000);
  countdown--;
  if (countdown < 0) {
    countdown = 9;
  }
  colorIndex = (colorIndex + 1) % 7;
}
void displayNumber(int number) {
  byte code = segmentCodes[number];
  shiftOut(DS_PIN, SHCP_PIN, MSBFIRST, code);
  digitalWrite(STCP_PIN, HIGH);
  digitalWrite(STCP_PIN, LOW);
}
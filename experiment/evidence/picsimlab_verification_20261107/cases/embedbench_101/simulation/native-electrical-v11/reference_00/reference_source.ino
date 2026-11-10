#include <Arduino.h>
const int SHCP_PIN = A2;  
const int STCP_PIN = A1;  
const int DS_PIN = A0;    
const int LED1_PIN = 2;   
const int LED2_PIN = 3;   
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
  0b10010000, 
};
int currentNumber = 9;
unsigned long previousMillis = 0;
const long interval = 2000; 
bool led1State = HIGH;
bool led2State = LOW;

void setup();
void loop();
void displayNumber(int number);
void setup() {
  pinMode(SHCP_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(DS_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  displayNumber(currentNumber);
  digitalWrite(LED1_PIN, led1State);
  digitalWrite(LED2_PIN, led2State);
}
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    led1State = !led1State;
    led2State = !led2State;
    digitalWrite(LED1_PIN, led1State);
    digitalWrite(LED2_PIN, led2State);
    if (led1State == HIGH) {
      currentNumber--;
      if (currentNumber < 0) {
        currentNumber = 9;
      }
      displayNumber(currentNumber);
    }
  }
}
void displayNumber(int number) {
  byte code = segmentCodes[number];
  shiftOut(DS_PIN, SHCP_PIN, MSBFIRST, code);
  digitalWrite(STCP_PIN, HIGH);
  digitalWrite(STCP_PIN, LOW);
}
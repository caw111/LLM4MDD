#include <Arduino.h>
const int dataPin = A0;   
const int clockPin = A2;  
const int latchPin = A1;  
const byte digit[10] = {
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

void sendScore(uint8_t high, uint8_t low);
void setup();
void loop();
void sendScore(uint8_t high, uint8_t low) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, low);
  shiftOut(dataPin, clockPin, MSBFIRST, high);
  digitalWrite(latchPin, HIGH);
}
unsigned long previousMillis = -1000;
const unsigned long interval = 1000; 
int seconds = -6;
void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
}
void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    seconds += 6;
    int high = seconds % 100 / 10;
    int low = seconds % 10;
    sendScore(high ? digit[high] : 0xff, digit[low]);
  }
}
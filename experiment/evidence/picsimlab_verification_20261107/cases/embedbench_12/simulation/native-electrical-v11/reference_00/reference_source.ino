#include <Arduino.h>
#include <Arduino.h>
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
int count = 0;

void setup();
void loop();
void displayNumber(int num);
void setup() {
  pinMode(A0, OUTPUT); 
  pinMode(A1, OUTPUT); 
  pinMode(A2, OUTPUT); 
  pinMode(2, INPUT_PULLUP);  
  displayNumber(count);
}
void loop() {
  if (digitalRead(2) == LOW) {
    delay(20); 
    if (digitalRead(2) == LOW) {
      count = (count + 3) % 10;
      displayNumber(count);
      while (digitalRead(2) == LOW); 
    }
  }
}
void displayNumber(int num) {
  byte segCode = digit[num];
  shiftOut(A0, A2, MSBFIRST, segCode);
  digitalWrite(A1, HIGH); 
  digitalWrite(A1, LOW);
}
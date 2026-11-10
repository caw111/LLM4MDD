#include <Arduino.h>
const int BTN1_PIN = 2;
const int BTN2_PIN = 3;
const int R_PIN = 4;
const int G_PIN = 5;
const int B_PIN = 6;
byte colors[][3] = {
  {LOW,  LOW,  LOW },  
  {HIGH, LOW,  LOW },  
  {LOW,  HIGH, LOW },  
  {LOW,  LOW,  HIGH},  
  {HIGH, HIGH, LOW },  
  {LOW,  HIGH, HIGH},  
  {HIGH, LOW,  HIGH},  
  {HIGH, HIGH, HIGH}   
};
int currentColor = 0;

void setup();
void loop();
void handleButton(int btnPin, int direction);
void updateLED();
void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  updateLED();
}
void loop() {
  handleButton(BTN1_PIN, 1);  
  handleButton(BTN2_PIN, -1); 
}
void handleButton(int btnPin, int direction) {
  if (digitalRead(btnPin) == LOW) {
    delay(100); 
    if (digitalRead(btnPin) == LOW) {
      currentColor = (currentColor + direction + 8) % 8;
      updateLED();
      while (digitalRead(btnPin) == LOW); 
    }
  }
}
void updateLED() {
  digitalWrite(R_PIN, colors[currentColor][0]);
  digitalWrite(G_PIN, colors[currentColor][1]);
  digitalWrite(B_PIN, colors[currentColor][2]);
}
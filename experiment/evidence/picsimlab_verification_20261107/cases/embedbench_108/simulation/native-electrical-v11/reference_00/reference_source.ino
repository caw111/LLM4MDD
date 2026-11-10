#include <Arduino.h>
const int LED_PIN = 2;  
const int RGB1_R = 3;   
const int RGB1_G = 4;   
const int RGB1_B = 5;   
const int RGB2_R = 6;   
const int RGB2_G = 7;   
const int RGB2_B = 8;   
const int RGB3_R = 9;   
const int RGB3_G = 10;   
const int RGB3_B = 11;   
const int COLOR_DURATION = 2000;  
const int BLINK_INTERVAL = 2000;  
unsigned long lastBlinkTime = 0;
unsigned long lastColorChangeTime = 0;
bool ledState = LOW;
int currentColorIndex = 0;
const int colors[7][3] = {
  {255, 0, 0},    
  {0, 255, 0},    
  {0, 0, 255},    
  {255, 255, 0},  
  {0, 255, 255},  
  {255, 0, 255},  
  {255, 255, 255} 
};

void setup();
void loop();
void setRGBColor(int red, int green, int blue);
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);
  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);
  setRGBColor(colors[currentColorIndex][0], colors[currentColorIndex][1], colors[currentColorIndex][2]);
}
void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastBlinkTime = currentTime;
  }
  if (currentTime - lastColorChangeTime >= COLOR_DURATION) {
    currentColorIndex = (currentColorIndex + 1) % 7;
    setRGBColor(colors[currentColorIndex][0], colors[currentColorIndex][1], colors[currentColorIndex][2]);
    lastColorChangeTime = currentTime;
  }
}
void setRGBColor(int red, int green, int blue) {
  analogWrite(RGB1_R, red);
  analogWrite(RGB1_G, green);
  analogWrite(RGB1_B, blue);
  analogWrite(RGB2_R, red);
  analogWrite(RGB2_G, green);
  analogWrite(RGB2_B, blue);
  analogWrite(RGB3_R, red);
  analogWrite(RGB3_G, green);
  analogWrite(RGB3_B, blue);
}
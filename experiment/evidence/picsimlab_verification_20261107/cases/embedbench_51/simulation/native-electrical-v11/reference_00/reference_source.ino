#include <Arduino.h>
const int PHASE_DURATION = 2000;  
unsigned long phaseStartTime = 0;
int currentPhase = -1;

void setup();
void loop();
void updateHardware();
void setup() {
  int pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45};
  for (int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }
  phaseStartTime = millis() - 2000;
}
void loop() {
  if (millis() - phaseStartTime >= PHASE_DURATION) {
    currentPhase = (currentPhase + 1) % 5;
    phaseStartTime = millis();
    updateHardware();
  }
}
void updateHardware() {
  int pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45};
  for (int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
    digitalWrite(pins[i], LOW);
  }
  switch (currentPhase) {
    case 0:  
      digitalWrite(2, HIGH);  
      digitalWrite(11, HIGH);  
      break;
    case 1:  
      digitalWrite(6, HIGH);  
      digitalWrite(11, HIGH);  
      digitalWrite(12, HIGH); 
      break;
    case 2:  
      digitalWrite(10, HIGH);  
      digitalWrite(11, HIGH);  
      digitalWrite(12, HIGH); 
      digitalWrite(13, HIGH); 
      break;
    case 3:  
      digitalWrite(2, HIGH);  
      digitalWrite(6, HIGH);  
      digitalWrite(11, HIGH);  
      digitalWrite(12, HIGH); 
      digitalWrite(13, HIGH); 
      digitalWrite(44, HIGH); 
      break;
    case 4:  
      for (int pin = 2; pin <= 10; pin++) digitalWrite(pin, HIGH);  
      for (int pin = 11; pin <= 13; pin++) digitalWrite(pin, HIGH); 
      digitalWrite(44, HIGH); 
      digitalWrite(45, HIGH); 
      break;
  }
}
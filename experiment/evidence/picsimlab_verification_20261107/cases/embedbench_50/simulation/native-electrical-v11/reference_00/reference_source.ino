#include <Arduino.h>
const int BAR_PINS[] = {8, 9, 10, 11, 12, 13, 44, 45};  

void setup();
void loop();
void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
  }
  for (int pin = 2; pin <= 7; pin++) {  
    pinMode(pin, OUTPUT);
  }
}
void loop() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(BAR_PINS[i], HIGH);
    if (i < 4) {  
      digitalWrite(2, HIGH);  
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
    } else {      
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
    }
    delay(2000);  
    digitalWrite(BAR_PINS[i], LOW);
  }
}
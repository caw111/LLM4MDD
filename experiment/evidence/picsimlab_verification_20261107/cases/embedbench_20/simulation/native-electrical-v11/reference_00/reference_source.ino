#include <Arduino.h>
#include <Servo.h>
Servo servo1;
int currentPosition = 0;
int increment = 10;

void setup();
void loop();
void setup() {
  servo1.attach(2);  
  servo1.write(currentPosition);  
}
void loop() {
  for (currentPosition = 0; currentPosition <= 180; currentPosition += increment) {
    servo1.write(currentPosition);  
    delay(2000);  
  }
  for (currentPosition = 180; currentPosition >= 0; currentPosition -= increment) {
    servo1.write(currentPosition);  
    delay(2000);  
  }
}
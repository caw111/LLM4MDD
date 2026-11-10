#include <Arduino.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;

void setup();
void loop();
void setup() {
  servo1.attach(2);  
  servo2.attach(3);  
  servo3.attach(4);  
  servo1.write(0);   
  servo2.write(0);   
  servo3.write(0);   
  delay(2000);       
}
void loop() {
  servo1.write(0);
  servo2.write(45);
  servo3.write(90);
  delay(2000);
  servo1.write(45);
  servo2.write(90);
  servo3.write(135);
  delay(2000);
  servo1.write(90);
  servo2.write(135);
  servo3.write(180);
  delay(2000);
  servo1.write(135);
  servo2.write(180);
  servo3.write(0);
  delay(2000);
  servo1.write(180);
  servo2.write(0);
  servo3.write(45);
  delay(2000);
}
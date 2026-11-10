#include <Arduino.h>
void setup(){Serial.begin(9600);pinMode(2,INPUT_PULLUP);pinMode(5,OUTPUT);pinMode(6,OUTPUT);pinMode(7,OUTPUT);pinMode(8,OUTPUT);Serial.print("ADC:");Serial.println(analogRead(A0));Serial.print("BUTTON:");Serial.println(digitalRead(2));}void loop(){analogWrite(5,32);analogWrite(6,0);analogWrite(7,0);digitalWrite(8,HIGH);delay(5);}

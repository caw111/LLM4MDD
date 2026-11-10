#include <Arduino.h>
int pins[]={2,3,4,5,6,7,8,9,10,11,12,13,44,45,46};
void setup(){for(int p:pins)pinMode(p,OUTPUT);}
void loop(){int values[]={64,128,192,255};for(int value:values){unsigned long started=millis();while(millis()-started<1000){for(int p:pins)analogWrite(p,value);}}}

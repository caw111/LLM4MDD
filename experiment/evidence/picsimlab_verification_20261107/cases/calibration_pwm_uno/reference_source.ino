#include <Arduino.h>
int pins[]={3,5,6,9,10,11};
void setup(){for(int p:pins)pinMode(p,OUTPUT);}
void loop(){int values[]={64,128,192,255};for(int value:values){unsigned long started=millis();while(millis()-started<1000){for(int p:pins)analogWrite(p,value);}}}

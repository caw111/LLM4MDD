#include <Arduino.h>
const uint8_t pins[]={8,9,10,11,12,13,44,45};
void setup(){for(uint8_t p:pins)pinMode(p,OUTPUT);pinMode(5,OUTPUT);pinMode(6,OUTPUT);pinMode(7,OUTPUT);}
void loop(){unsigned phase=millis()/2000;uint8_t level=phase==0?32:phase==1?64:phase==2?224:phase==3?255:0;analogWrite(5,level);analogWrite(6,0);analogWrite(7,0);unsigned count=level==255?8:level/32;for(unsigned i=0;i<8;i++)digitalWrite(pins[i],i<count?HIGH:LOW);delay(5);}

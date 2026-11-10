#include <Arduino.h>
#include <SPI.h>
void setup(){SPI.begin();SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));pinMode(10,OUTPUT);digitalWrite(10,LOW);}
void loop(){SPI.transfer(0xA5);delayMicroseconds(10);}

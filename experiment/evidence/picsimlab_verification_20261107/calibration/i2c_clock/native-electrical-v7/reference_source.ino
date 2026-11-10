#include <Arduino.h>
#include <Wire.h>
void setup(){Wire.begin();Wire.setClock(100000);}
void loop(){Wire.beginTransmission(0x68);Wire.write(0);Wire.endTransmission();delayMicroseconds(100);}

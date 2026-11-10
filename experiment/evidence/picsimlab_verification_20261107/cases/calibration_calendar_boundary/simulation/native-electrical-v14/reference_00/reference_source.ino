#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
RTC_DS3231 rtc;
void setup(){Serial.begin(9600);Wire.begin();rtc.begin();}
void loop(){DateTime now=rtc.now();Serial.print("DATE:");Serial.print(now.year());Serial.print('-');if(now.month()<10)Serial.print('0');Serial.print(now.month());Serial.print('-');if(now.day()<10)Serial.print('0');Serial.println(now.day());Serial.print("RAW:");Serial.println(now.unixtime());DateTime f((uint32_t)(double)now.unixtime());Serial.print("FLOAT_DAY:");Serial.println(f.day());delay(250);}

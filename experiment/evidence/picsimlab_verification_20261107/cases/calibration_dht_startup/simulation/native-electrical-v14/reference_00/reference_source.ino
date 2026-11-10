#include <Arduino.h>
#include <DHT.h>
DHT dht(7,DHT11);void setup(){Serial.begin(9600);dht.begin();Serial.print("INITIAL:");Serial.println(dht.readTemperature(),2);}void loop(){}

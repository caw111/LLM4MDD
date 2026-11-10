#include <Arduino.h>
#include <Servo.h>
#include <DHT.h>
#include <TM1637Display.h>
Servo actuator;DHT sensor(2,DHT22);TM1637Display display(4,5);
unsigned long last=0,lastDht=0;
void setup(){Serial.begin(9600);pinMode(8,OUTPUT);pinMode(7,INPUT);pinMode(13,OUTPUT);actuator.attach(9);sensor.begin();display.setBrightness(7);tone(6,1000);}
void loop(){
 if(millis()-last<500)return;last=millis();
 int raw=analogRead(A0);actuator.write(map(raw,0,1023,0,180));display.showNumberDec(raw);
 digitalWrite(8,LOW);delayMicroseconds(2);digitalWrite(8,HIGH);delayMicroseconds(10);digitalWrite(8,LOW);
 unsigned long width=pulseIn(7,HIGH,30000);
 Serial.print("ADC:");Serial.println(raw);Serial.print("Distance:");Serial.println(width/58.0,2);
 if(millis()-lastDht>=2000){lastDht=millis();Serial.print("Temperature:");Serial.println(sensor.readTemperature(),2);Serial.print("Humidity:");Serial.println(sensor.readHumidity(),2);}
 digitalWrite(13,(millis()/1000)%2);
}

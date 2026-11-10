#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <stdio.h>

RTC_DS1307 rtc;
bool rtcAvailable = false;

void printTimestamp(const DateTime &value) {
  char text[24];
  snprintf(text, sizeof(text), "%04u-%02u-%02uT%02u:%02u:%02u",
           (unsigned int)value.year(),
           (unsigned int)value.month(),
           (unsigned int)value.day(),
           (unsigned int)value.hour(),
           (unsigned int)value.minute(),
           (unsigned int)value.second());
  Serial.println(text);
}

void setup() {
  pinMode(2, INPUT);
  Serial.begin(9600);
  Wire.begin();
  rtcAvailable = rtc.begin();
}

void loop() {
  if (!rtcAvailable) {
    delay(50);
    return;
  }
  DateTime current = rtc.now();
  if (digitalRead(2) == HIGH) {
    current = current + TimeSpan(0, 9, 0, 0);
  }
  printTimestamp(current);
  delay(50);
}

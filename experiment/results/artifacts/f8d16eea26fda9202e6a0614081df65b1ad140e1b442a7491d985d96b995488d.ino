#include <Wire.h>
#include <RTClib.h>

// DS1307 wiring: 5V -> 5V, GND -> GND.2, SDA -> A4, SCL -> A5.
RTC_DS1307 rtc;

const char* const daysOfTheWeek[] = {
  "Sunday", "Monday", "Tuesday", "Wednesday",
  "Thursday", "Friday", "Saturday"
};

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    return;
  }

  const DateTime now = rtc.now();

  Serial.print(F("Current time: "));
  Serial.print(now.year());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.day());
  Serial.print(F(" ("));
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(F(") "));
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.println(now.second());
}

void loop() {
  // No recurring actions required.
}
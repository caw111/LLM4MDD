#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);

  // Check if RTC module can begin
  if (!rtc.begin()) {
    // If begin fails, nothing else is specified to do.
    // Still keep behavior deterministic by printing nothing further.
    return;
  }

  // Read current time with RTC module
  DateTime now = rtc.now();

  // Day of week names (RTClib: 0=Sunday ... 6=Saturday)
  const char* dayNames[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
  };

  Serial.print("Current time: ");
  Serial.print(now.year());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.day());
  Serial.print(' ');
  Serial.print(dayNames[now.dayOfTheWeek()]);
  Serial.print(' ');
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.print(now.second());
}

void loop() {
  // No actions are required in the loop function.
}
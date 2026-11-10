#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  while (!Serial) { /* wait for serial (some boards) */ }

  Wire.begin();

  if (!rtc.begin()) {
    // If RTC cannot begin, halt after printing error for visibility.
    Serial.println("RTC module not found!");
    while (true) {
      delay(1000);
    }
  }

  DateTime now = rtc.now();

  const char* days[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
  };

  Serial.print("Current time: ");
  Serial.print(now.year());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.day());
  Serial.print(" (");
  Serial.print(days[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.print(now.second());
}

void loop() {
  // No actions are required in the loop function.
}
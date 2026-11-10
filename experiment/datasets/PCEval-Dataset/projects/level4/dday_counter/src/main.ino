/**
  Compare RTC sensor time and serial input
  11 APR 2025 by Blind Author
*/

#include "RTClib.h"

RTC_DS1307 rtc;
DateTime now;

void setup () {
    Serial.begin(115200);

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        abort();
    }

    now = rtc.now();

    Serial.println("Enter target datetime in format YYYY-MM-DDTHH:MM:SS");
}

void loop () {

    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');

        int year   = input.substring(0, 4).toInt();
        int month  = input.substring(5, 7).toInt();
        int day    = input.substring(8, 10).toInt();
        int hour   = input.substring(11, 13).toInt();
        int minute = input.substring(14, 16).toInt();
        int second = input.substring(17, 19).toInt();

        DateTime target(year, month, day, hour, minute, second);

        TimeSpan diff = target - now;
        Serial.print("D-");
        Serial.println(abs(diff.days()));
    }

    delay(500);
}

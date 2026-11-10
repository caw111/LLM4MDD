/**
  Press the button to turn on the LED
  31 Mar 2025 by Blind Author
*/

#include "RTClib.h"

RTC_DS1307 rtc;

const int BUTTON_PIN = 2;

void setup () {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
}

void loop () {
  DateTime now = rtc.now();

  if (digitalRead(BUTTON_PIN) == HIGH) {
    now = now + TimeSpan(0, 9, 0, 0);
  }

  Serial.println(now.timestamp());
  
  delay(500);
}

/**
  Trigger the buzzer when the specified time is reached
  06 MAY 2025 by Blind Author
*/

#define BUZZER_PIN 8
#include "RTClib.h"

RTC_DS1307 rtc;

DateTime targetTime(2024, 11, 24, 04, 49, 0);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      abort();
    }
}

void loop() {
  DateTime now = rtc.now();

  if (now >= targetTime) {
    tone(BUZZER_PIN, 262);
  } else {
    noTone(BUZZER_PIN);
  }
}

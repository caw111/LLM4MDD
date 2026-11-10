#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

const int BUZZER_PIN = 8;

// Trigger when current time is on or after this moment.
const char *TRIGGER_ISO = "2024-11-24T04:49:00";

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  Wire.begin();

  if (!rtc.begin()) {
    // If RTC cannot be initialized, do nothing (no buzzer behavior defined for this case).
    return;
  }
}

void loop() {
  // Continuously read the current time
  DateTime now = rtc.now();

  DateTime triggerTime(2024, 11, 24, 4, 49, 0);

  // Tone the buzzer if the current time is on or after the specified time
  if (now >= triggerTime) {
    // Use a consistent audible tone
    tone(BUZZER_PIN, 1000);
  } else {
    noTone(BUZZER_PIN);
  }

  // Avoid excessive I2C reads / tone restarts
  delay(200);
}
#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_alarm\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

const int BUZZER_PIN = 8;

// Trigger when current time is on or after this moment.
const char *TRIGGER_ISO = "2024-11-24T04:49:00";

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_alarm\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_alarm\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_alarm\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
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

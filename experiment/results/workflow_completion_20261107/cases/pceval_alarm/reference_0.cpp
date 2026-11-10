#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

const uint8_t BUZZER_PIN = 8;
RTC_DS1307 rtc;
bool rtcReady = false;
bool alarmSounding = false;
uint32_t alarmEpoch = 0;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN);
  Wire.begin();
  rtcReady = rtc.begin();
  alarmEpoch = DateTime(2024, 11, 24, 4, 49, 0).unixtime();
}

void loop() {
  if (!rtcReady) {
    noTone(BUZZER_PIN);
    delay(10);
    return;
  }

  const DateTime current = rtc.now();
  const bool alarmDue = current.unixtime() >= alarmEpoch;

  if (alarmDue && !alarmSounding) {
    tone(BUZZER_PIN, 1000);
    alarmSounding = true;
  } else if (!alarmDue && alarmSounding) {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
    alarmSounding = false;
  }

  delay(10);
}

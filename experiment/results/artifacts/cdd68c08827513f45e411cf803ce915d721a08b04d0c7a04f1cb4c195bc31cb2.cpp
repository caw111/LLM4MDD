#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_alarm\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

static const int BUZZER_PIN = 8;

// Trigger time: 2024-11-24T04:49:00
static const uint16_t TRIGGER_YEAR = 2024;
static const uint8_t  TRIGGER_MONTH = 11;
static const uint8_t  TRIGGER_DAY = 24;
static const uint8_t  TRIGGER_HOUR = 4;
static const uint8_t  TRIGGER_MINUTE = 49;
static const uint8_t  TRIGGER_SECOND = 0;

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_alarm\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_alarm\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_alarm\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  Wire.begin();

  if (!rtc.begin()) {
    // If RTC cannot be initialized, keep behavior deterministic:
    // no buzzer until we can read time.
    // (No blocking loops to preserve continuous loop behavior.)
  }
}

void loop() {
  DateTime now = rtc.now();

  DateTime triggerTime(
    TRIGGER_YEAR,
    TRIGGER_MONTH,
    TRIGGER_DAY,
    TRIGGER_HOUR,
    TRIGGER_MINUTE,
    TRIGGER_SECOND
  );

  // When current time is on or after specified time, tone the buzzer.
  if (now >= triggerTime) {
    tone(BUZZER_PIN, 1000);
  } else {
    noTone(BUZZER_PIN);
  }

  delay(200);
}

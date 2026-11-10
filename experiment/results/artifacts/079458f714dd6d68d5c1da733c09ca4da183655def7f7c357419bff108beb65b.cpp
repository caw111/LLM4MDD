#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_RTC_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include "RTClib.h"

static const uint8_t BUTTON_PIN = 2;

RTC_DS1307 rtc;

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_RTC_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_RTC_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_RTC_timezone\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // 1. Configure the button pin as an input
  pinMode(BUTTON_PIN, INPUT);

  // 2. Initialize the serial monitor
  Serial.begin(9600);

  // 3. Check if the RTC module is available and initialize it
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("RTC module not found");
    // Keep behavior deterministic: do not proceed.
    while (true) {
      delay(1000);
    }
  }

  // If the RTC lost power, it may need setting.
  // Do not set a compile-time time; preserve behavior as "retrieve current time".
  if (rtc.lostPower()) {
    // Attempt a graceful message; do not change RTC time automatically.
    Serial.println("RTC lost power");
  }
}

void loop() {
  // 4. Continuously retrieve the current time from the RTC
  DateTime nowUtc = rtc.now();

  // 5. Check the button state, and if it is pressed, add 9 hours to adjust the timezone
  bool pressed = (digitalRead(BUTTON_PIN) == HIGH);
  DateTime adjusted = pressed ? nowUtc + TimeSpan(9, 0, 0) : nowUtc;

  // 6. Print the resulting timestamp in YYYY-MM-DDTHH:MM:SS
  char buf[20];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
           adjusted.year(), adjusted.month(), adjusted.day(),
           adjusted.hour(), adjusted.minute(), adjusted.second());
  Serial.println(buf);

  // Small delay to avoid flooding output
  delay(500);
}

#include <Wire.h>
#include "RTClib.h"

static const uint8_t BUTTON_PIN = 2;
RTC_DS1307 rtc;

void setup() {
  // 1. Configure the button pin as an input
  pinMode(BUTTON_PIN, INPUT);

  // 2. Initialize the serial monitor
  Serial.begin(9600);

  // 3. Check if the RTC module is available and initialize it
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("RTC module not found");
    while (true) {
      delay(1000);
    }
  }

  // Do not set or change RTC time automatically.
  // Keep behavior: just report availability and continue.
  if (rtc.isrunning()) {
    // Nothing required by spec.
  } else {
    Serial.println("RTC is not running");
  }
}

void loop() {
  // 4. Continuously retrieve the current time from the RTC
  DateTime nowUtc = rtc.now();

  // 5. Check the button state, and if it is pressed, add 9 hours to adjust the timezone
  bool pressed = (digitalRead(BUTTON_PIN) == HIGH);
  DateTime adjusted = pressed ? (nowUtc + TimeSpan(0, 9, 0, 0)) : nowUtc;

  // 6. Print the resulting timestamp in YYYY-MM-DDTHH:MM:SS
  char buf[20];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
           adjusted.year(), adjusted.month(), adjusted.day(),
           adjusted.hour(), adjusted.minute(), adjusted.second());
  Serial.println(buf);

  // Small delay to avoid flooding output
  delay(500);
}
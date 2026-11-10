#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <RTClib.h>
#include <stdio.h>
#include <string.h>

Adafruit_ILI9341 lcd(10, 9, 8);
RTC_DS1307 rtc;
bool rtcReady = false;
int32_t dayOffset = 0;
uint32_t lastDateKey = 0;
const char *weekdays[7] = {
  "Sunday", "Monday", "Tuesday", "Wednesday",
  "Thursday", "Friday", "Saturday"
};

struct Button {
  uint8_t pin;
  bool raw;
  bool stable;
  uint32_t changedAt;
};
Button previousButton = {3, false, false, 0};
Button nextButton = {2, false, false, 0};

bool pressedEdge(Button &b, uint32_t now) {
  bool value = digitalRead(b.pin) == HIGH;
  if (value != b.raw) {
    b.raw = value;
    b.changedAt = now;
  }
  if (b.stable != b.raw && (uint32_t)(now - b.changedAt) >= 20) {
    b.stable = b.raw;
    return b.stable;
  }
  return false;
}

void updateCalendar() {
  if (!rtcReady) return;
  DateTime current = rtc.now();
  int64_t shifted = (int64_t)current.unixtime() + (int64_t)dayOffset * 86400;
  if (shifted < 0 || shifted > 4294967295LL) return;
  DateTime shown((uint32_t)shifted);
  uint32_t key = (uint32_t)shown.year() * 10000UL + (uint32_t)shown.month() * 100UL + shown.day();
  if (key == lastDateKey) return;
  lastDateKey = key;

  char dateText[11];
  snprintf(dateText, sizeof(dateText), "%04u-%02u-%02u", (unsigned)shown.year(), (unsigned)shown.month(), (unsigned)shown.day());
  lcd.setCursor(10, 30);
  lcd.print(dateText);

  // Opaque text and trailing spaces overwrite a longer previous weekday.
  char weekdayText[10];
  memset(weekdayText, ' ', 9);
  weekdayText[9] = '\0';
  const char *name = weekdays[shown.dayOfTheWeek()];
  memcpy(weekdayText, name, strlen(name));
  lcd.setCursor(10, 80);
  lcd.print(weekdayText);
}

void setup() {
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  Wire.begin();
  rtcReady = rtc.begin();
  lcd.begin();
  lcd.setTextSize(3);
  lcd.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  lcd.setTextWrap(false);
  // Initial display is rendered in loop, so its observation is setup-relative.
}

void loop() {
  uint32_t now = millis();
  bool previousPressed = pressedEdge(previousButton, now);
  bool nextPressed = pressedEdge(nextButton, now);
  if (previousPressed) --dayOffset;
  if (nextPressed) ++dayOffset;
  updateCalendar();
  delay(5);
}

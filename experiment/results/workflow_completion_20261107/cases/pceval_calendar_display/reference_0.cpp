#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <RTClib.h>
#include <stdio.h>

Adafruit_ILI9341 lcd(10, 9, 8);
RTC_DS1307 rtc;
bool rtcReady = false;
int32_t dayOffset = 0;
uint32_t lastDisplayedDay = 0xFFFFFFFFUL;
const char *weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

struct Button {
  uint8_t pin;
  bool raw;
  bool stable;
  uint32_t changedAt;
};
Button previousButton = {3, false, false, 0};
Button nextButton = {2, false, false, 0};

bool pressed(Button &button) {
  bool level = digitalRead(button.pin) == HIGH;
  uint32_t now = millis();
  if (level != button.raw) {
    button.raw = level;
    button.changedAt = now;
  }
  if (button.stable != button.raw && (uint32_t)(now - button.changedAt) >= 20) {
    button.stable = button.raw;
    return button.stable;
  }
  return false;
}

void updateDisplay() {
  if (!rtcReady) return;
  DateTime now = rtc.now();
  int64_t adjusted = (int64_t)now.unixtime() + (int64_t)dayOffset * 86400LL;
  if (adjusted < 0 || adjusted > 0xFFFFFFFFLL) return;
  DateTime selected((uint32_t)adjusted);
  uint32_t day = selected.unixtime() / 86400UL;
  if (day == lastDisplayedDay) return;
  lastDisplayedDay = day;
  char dateText[11];
  snprintf(dateText, sizeof(dateText), "%04u-%02u-%02u", (unsigned)selected.year(), (unsigned)selected.month(), (unsigned)selected.day());
  lcd.fillRect(10, 30, 220, 24, ILI9341_BLACK);
  lcd.setCursor(10, 30);
  lcd.print(dateText);
  lcd.fillRect(10, 80, 220, 24, ILI9341_BLACK);
  lcd.setCursor(10, 80);
  lcd.print(weekdays[selected.dayOfTheWeek()]);
}

void setup() {
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  Wire.begin();
  rtcReady = rtc.begin();
  lcd.begin();
  lcd.fillScreen(ILI9341_BLACK);
  lcd.setTextSize(3);
  lcd.setTextColor(ILI9341_WHITE);
  lcd.setTextWrap(false);
  updateDisplay();
}

void loop() {
  if (pressed(previousButton)) --dayOffset;
  if (pressed(nextButton)) ++dayOffset;
  updateDisplay();
  delay(2);
}

/**
  Using an RTC, display the current date and day of the week on ili9341 LCD, and navigate days with buttons
  03 MAY 2025 by Blind Author
*/

#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
RTC_DS3231 rtc;

#define LEFT_BUTTON_PIN 3
#define RIGHT_BUTTON_PIN 2

DateTime currentDate;
int dayOffset = 0;

const char* daysOfWeek[] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  displayDate();
}

void loop() {
  if (digitalRead(LEFT_BUTTON_PIN) == HIGH) {
    dayOffset--;
    displayDate();
    delay(300);
  }

  if (digitalRead(RIGHT_BUTTON_PIN) == HIGH) {
    dayOffset++;
    displayDate();
    delay(300);
  }
}

void displayDate() {
  DateTime now = rtc.now();
  currentDate = now + TimeSpan(dayOffset, 0, 0, 0);

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(10, 30);

  char buffer[20];
  sprintf(buffer, "%04d-%02d-%02d", currentDate.year(), currentDate.month(), currentDate.day());
  tft.println(buffer);

  tft.setCursor(10, 80);
  tft.println(daysOfWeek[currentDate.dayOfTheWeek()]);
}

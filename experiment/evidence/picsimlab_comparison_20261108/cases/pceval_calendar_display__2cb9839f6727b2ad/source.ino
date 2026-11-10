#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "RTClib.h"

constexpr uint8_t TFT_RST = 8;
constexpr uint8_t TFT_DC = 9;
constexpr uint8_t TFT_CS = 10;
constexpr uint8_t PREVIOUS_BUTTON_PIN = 3;
constexpr uint8_t NEXT_BUTTON_PIN = 2;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long RTC_POLL_MS = 250;
constexpr int64_t SECONDS_PER_DAY = 86400LL;

// DS1307 calendar range: January 1, 2000 through December 31, 2099.
constexpr int64_t FIRST_TIMESTAMP = 946684800LL;
constexpr int64_t LAST_TIMESTAMP = 4102444799LL;

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);
RTC_DS1307 rtc;

struct Button {
  uint8_t pin;
  bool lastReading;
  bool stableState;
  unsigned long changedAt;
};

Button previousButton = {PREVIOUS_BUTTON_PIN, LOW, LOW, 0};
Button nextButton = {NEXT_BUTTON_PIN, LOW, LOW, 0};

int32_t dayOffset = 0;
uint32_t displayedDate = 0;
unsigned long lastRtcPoll = 0;
bool refreshRequested = true;

bool buttonPressed(Button &button, unsigned long now) {
  const bool reading = digitalRead(button.pin) == HIGH;

  if (reading != button.lastReading) {
    button.lastReading = reading;
    button.changedAt = now;
  }

  if (reading != button.stableState &&
      now - button.changedAt >= DEBOUNCE_MS) {
    button.stableState = reading;
    return button.stableState == HIGH;
  }

  return false;
}

const __FlashStringHelper *weekdayName(uint8_t weekday) {
  switch (weekday) {
    case 0: return F("Sunday");
    case 1: return F("Monday");
    case 2: return F("Tuesday");
    case 3: return F("Wednesday");
    case 4: return F("Thursday");
    case 5: return F("Friday");
    default: return F("Saturday");
  }
}

void drawDate(const DateTime &date) {
  const uint32_t dateKey =
      static_cast<uint32_t>(date.year()) * 10000UL +
      static_cast<uint32_t>(date.month()) * 100UL + date.day();

  if (dateKey == displayedDate) {
    return;
  }
  displayedDate = dateKey;

  char dateText[11];
  snprintf(dateText, sizeof(dateText), "%04u-%02u-%02u",
           static_cast<unsigned int>(date.year()),
           static_cast<unsigned int>(date.month()),
           static_cast<unsigned int>(date.day()));

  // Clear both lines so shorter weekday names leave no old characters.
  tft.fillRect(10, 30, tft.width() - 10, 24, ILI9341_BLACK);
  tft.fillRect(10, 80, tft.width() - 10, 24, ILI9341_BLACK);

  tft.setCursor(10, 30);
  tft.print(dateText);
  tft.setCursor(10, 80);
  tft.print(weekdayName(date.dayOfTheWeek()));
}

void setup() {
  Serial.begin(9600);

  // External 10k-ohm pull-down resistors: buttons are active HIGH.
  pinMode(PREVIOUS_BUTTON_PIN, INPUT);
  pinMode(NEXT_BUTTON_PIN, INPUT);

  Wire.begin();

  tft.begin();  // Hardware SPI: MOSI=11, MISO=12, SCK=13.
  tft.setRotation(0);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextWrap(false);

  if (!rtc.begin()) {
    Serial.println(F("DS1307 RTC not found."));
    tft.setCursor(10, 30);
    tft.print(F("RTC error"));
    while (true) {
      delay(1000);
    }
  }

  // Preserve the existing RTC time unless its oscillator is stopped.
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  const unsigned long nowMillis = millis();
  const bool previousPressed = buttonPressed(previousButton, nowMillis);
  const bool nextPressed = buttonPressed(nextButton, nowMillis);
  const int8_t direction =
      static_cast<int8_t>(nextPressed) - static_cast<int8_t>(previousPressed);

  if (refreshRequested || direction != 0 ||
      nowMillis - lastRtcPoll >= RTC_POLL_MS) {
    lastRtcPoll = nowMillis;
    refreshRequested = false;

    const DateTime current = rtc.now();
    const int64_t currentTimestamp = current.unixtime();

    if (direction != 0) {
      const int32_t candidateOffset = dayOffset + direction;
      const int64_t candidateTimestamp =
          currentTimestamp + static_cast<int64_t>(candidateOffset) * SECONDS_PER_DAY;

      if (candidateTimestamp >= FIRST_TIMESTAMP &&
          candidateTimestamp <= LAST_TIMESTAMP) {
        dayOffset = candidateOffset;
      }
    }

    // Navigation changes only the displayed date, never the RTC.
    int64_t selectedTimestamp =
        currentTimestamp + static_cast<int64_t>(dayOffset) * SECONDS_PER_DAY;

    if (selectedTimestamp < FIRST_TIMESTAMP) {
      selectedTimestamp = FIRST_TIMESTAMP;
    } else if (selectedTimestamp > LAST_TIMESTAMP) {
      selectedTimestamp = LAST_TIMESTAMP;
    }

    drawDate(DateTime(static_cast<uint32_t>(selectedTimestamp)));
  }
}
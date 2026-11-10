#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <RTClib.h>

#define LCD_CS   10
#define LCD_DC    9
#define LCD_RST   8

// SPI pins: 11=MOSI, 12=MISO, 13=SCK (wired in circuit)
Adafruit_ILI9341 tft(LCD_CS, LCD_DC, LCD_RST);
RTC_DS1307 rtc;

// Buttons (as wired):
// button1 -> Arduino pin 3, connected to 5V when pressed; otherwise pulled down to GND via 10k
// button2 -> Arduino pin 2, connected to 5V when pressed; otherwise pulled down to GND via 10k
const uint8_t BTN_PREV_PIN = 3; // button1
const uint8_t BTN_NEXT_PIN = 2; // button2

static const char* daysOfWeek[] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

bool readButtonPressed(uint8_t pin) {
  // Button is HIGH when pressed (connected to 5V), otherwise LOW (via 10k to GND).
  return digitalRead(pin) == HIGH;
}

void drawStaticLayout() {
  // Requirements specify text size=3 and color=white.
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
}

void drawDateAndDOW(const DateTime& now) {
  // Show YYYY-MM-DD to (10, 30)
  // Show days of the week to (10, 80)
  char dateStr[11]; // "YYYY-MM-DD" + null
  snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d", now.year(), now.month(), now.day());

  // Clear top area to avoid leftover characters when values shrink/shift.
  // (Keep within required positions.)
  tft.fillRect(0, 0, 240, 120, ILI9341_BLACK);

  tft.setCursor(10, 30);
  tft.print(dateStr);

  tft.setCursor(10, 80);
  tft.print(daysOfWeek[now.dayOfTheWeek()]);
}

void adjustDate(int dayDelta) {
  // Read current time, adjust by dayDelta, and set RTC accordingly.
  DateTime now = rtc.now();
  DateTime adjusted = now + TimeSpan((int64_t)dayDelta * 24LL * 60LL * 60LL);

  rtc.adjust(adjusted);

  // Immediate refresh display after adjustment
  drawDateAndDOW(rtc.now());
}

void setup() {
  pinMode(BTN_PREV_PIN, INPUT);
  pinMode(BTN_NEXT_PIN, INPUT);

  tft.begin();
  drawStaticLayout();

  Wire.begin();

  // 1. Initialize the RTC module and check if it can begin
  if (!rtc.begin()) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(10, 30);
    tft.setTextSize(3);
    tft.setTextColor(ILI9341_WHITE);
    tft.print("RTC not found");
    while (true) {
      delay(1000);
    }
  }

  // 2. Set text size as 3, and text color to white.
  drawStaticLayout();

  // 3. Initial draw
  drawDateAndDOW(rtc.now());
}

void loop() {
  // 4. Continuously read the current time, and show date and day of week.
  // Also handle button presses to navigate days.
  static uint32_t lastDrawMs = 0;
  const uint32_t drawIntervalMs = 200; // frequent updates, avoid flicker

  // Simple edge-detection to prevent repeated increments while held.
  static bool prevBtnPrev = false;
  static bool prevBtnNext = false;

  bool btnPrev = readButtonPressed(BTN_PREV_PIN); // button1 = previous date
  bool btnNext = readButtonPressed(BTN_NEXT_PIN); // button2 = next date

  if (btnPrev && !prevBtnPrev) {
    adjustDate(-1);
  } else if (btnNext && !prevBtnNext) {
    adjustDate(1);
  }

  prevBtnPrev = btnPrev;
  prevBtnNext = btnNext;

  uint32_t nowMs = millis();
  if (nowMs - lastDrawMs >= drawIntervalMs) {
    lastDrawMs = nowMs;
    DateTime current = rtc.now();
    drawDateAndDOW(current);
  }
}
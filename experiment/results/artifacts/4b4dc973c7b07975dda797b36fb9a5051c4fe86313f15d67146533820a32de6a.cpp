#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_calendar_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <RTClib.h>

// LCD wiring (per circuit)
static const uint8_t LCD_CS  = 10;
static const uint8_t LCD_RST = 8;
static const uint8_t LCD_DC  = 9;

// Buttons (per circuit)
static const uint8_t BTN_PREV = 3; // Button 1
static const uint8_t BTN_NEXT = 2; // Button 2

// Day names
const char* const daysOfWeek[] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

Adafruit_ILI9341 tft(LCD_CS, LCD_DC, LCD_RST);
RTC_DS1307 rtc;

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_calendar_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonStablePressed(uint8_t pin);
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_calendar_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_calendar_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_calendar_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonStablePressed(uint8_t pin) {
  // Buttons are connected to 5V when pressed, otherwise pulled down to GND via 10k resistor.
  // So pressed => HIGH.
  if (digitalRead(pin) != HIGH) return false;

  // Simple debounce: require stable HIGH across short interval
  delay(20);
  if (digitalRead(pin) != HIGH) return false;

  // Wait for release to avoid repeated stepping while held
  while (digitalRead(pin) == HIGH) {
    delay(5);
  }
  return true;
}

void setup() {
  // Buttons
  pinMode(BTN_PREV, INPUT);
  pinMode(BTN_NEXT, INPUT);

  // Serial not required, but harmless
  // Serial.begin(9600);

  // LCD
  tft.begin();
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);

  // RTC
  Wire.begin();
  if (!rtc.begin()) {
    // If RTC cannot start, keep screen visible with message and halt.
    tft.clear();
    tft.setCursor(10, 30);
    tft.print("RTC not found");
    while (true) {
      delay(1000);
    }
  }

  // If RTC lost power, clock will run but might be wrong; we do not overwrite per requirements.
  // Optionally display something to indicate readiness.
  tft.clear();
}

void loop() {
  DateTime now = rtc.now();

  // Navigate days with buttons
  // Button 1 = previous date, Button 2 = next date
  // (As per expected behavior: displayed date changes correctly when pressed.)
  if (readButtonStablePressed(BTN_PREV)) {
    // Adjust displayed date by changing RTC time by -1 day
    // Preserve time-of-day as-is.
    now = now - TimeSpan(1, 0, 0, 0);
    rtc.adjust(now);
  } else if (readButtonStablePressed(BTN_NEXT)) {
    now = now + TimeSpan(1, 0, 0, 0);
    rtc.adjust(now);
  }

  // Read again after possible adjustments
  now = rtc.now();

  // Display date: YYYY-MM-DD at (10, 30)
  tft.fillRect(0, 0, 240, 120, ILI9341_BLACK); // Clear top area (safe for positions used)

  char buf[11]; // "YYYY-MM-DD" + null
  snprintf(buf, sizeof(buf), "%04u-%02u-%02u",
           (unsigned)now.year(),
           (unsigned)now.month(),
           (unsigned)now.day());

  tft.setCursor(10, 30);
  tft.print(buf);

  // Display day of week at (10, 80)
  const uint8_t dow = (uint8_t)now.dayOfTheWeek(); // 0=Sunday
  tft.setCursor(10, 80);
  tft.print(daysOfWeek[dow]);

  // Continuously read current time
  delay(250);
}

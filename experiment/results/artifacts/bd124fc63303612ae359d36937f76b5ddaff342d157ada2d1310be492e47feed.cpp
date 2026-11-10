#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiple_timezone\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

// Button pins (per circuit)
static const uint8_t BTN1_PIN = 2;
static const uint8_t BTN2_PIN = 3;
static const uint8_t BTN3_PIN = 4;
static const uint8_t BTN4_PIN = 5;

// Timezone offsets relative to UTC (hours)
static const int TZ_UTC   = 0;
static const int TZ_UTC_P9  = +9;
static const int TZ_UTC_P4  = +4;
static const int TZ_UTC_M10 = -10;
static const int TZ_UTC_M5  = -5;

static unsigned long lastPrintMs = 0;

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiple_timezone\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readButton(uint8_t pin);
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiple_timezone\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void printAdjustedTimestamp(const DateTime& utcNow, int offsetHours);
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiple_timezone\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiple_timezone\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiple_timezone\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readButton(uint8_t pin) {
  // Requirement: button input will read HIGH when pressed
  return digitalRead(pin) == HIGH;
}

static void printAdjustedTimestamp(const DateTime& utcNow, int offsetHours) {
  // Apply timezone by adding/subtracting whole hours
  DateTime adjusted = utcNow + TimeSpan((int32_t)offsetHours * 3600L);

  // Format: YYYY-MM-DDTHH:MM:SS
  char buf[20];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
           adjusted.year(), adjusted.month(), adjusted.day(),
           adjusted.hour(), adjusted.minute(), adjusted.second());
  Serial.println(buf);
}

void setup() {
  // 1. Configure the button pin as an input
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(BTN3_PIN, INPUT);
  pinMode(BTN4_PIN, INPUT);

  // 2. Initialize the serial monitor
  Serial.begin(9600);

  // 3. Check if the RTC module is available and initialize it
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("RTC module not found");
    // Keep running; still follow expected behavior format when possible
  } else {
    // If the RTC module lost power, try to set it to compile time (best-effort)
    // (Some RTClib versions do not provide lostPower() for DS1307, so avoid calling it.)
    // We won't block; we will still attempt to read time in loop().
  }
}

void loop() {
  // 4. Continuously retrieve the current time from the RTC
  // Simple rate limiting to avoid flooding Serial
  const unsigned long nowMs = millis();
  if (nowMs - lastPrintMs < 200) return;
  lastPrintMs = nowMs;

  DateTime utcNow = rtc.now(); // best-effort regardless of isrunning()

  // 5. Check the state of buttons, and apply the corresponding timezone
  // If multiple are pressed, priority order follows Button1..Button4 checks.
  int offset = TZ_UTC; // 1) none pressed => UTC
  if (readButton(BTN1_PIN)) offset = TZ_UTC_P9;       // Button 1: UTC+9
  else if (readButton(BTN2_PIN)) offset = TZ_UTC_P4;  // Button 2: UTC+4
  else if (readButton(BTN3_PIN)) offset = TZ_UTC_M10; // Button 3: UTC-10
  else if (readButton(BTN4_PIN)) offset = TZ_UTC_M5;  // Button 4: UTC-5

  // 6. Print the resulting timestamp
  printAdjustedTimestamp(utcNow, offset);
}

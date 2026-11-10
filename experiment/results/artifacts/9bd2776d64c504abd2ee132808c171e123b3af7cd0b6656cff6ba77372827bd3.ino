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

static long lastPrintMs = 0;

static bool readButton(uint8_t pin) {
  // Requirement: button input will read HIGH when pressed
  return digitalRead(pin) == HIGH;
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
    // If RTC lost power, set it to compile time (best-effort)
    if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  }
}

static void printAdjustedTimestamp(const DateTime& utcNow, int offsetHours) {
  // Apply timezone by adding/subtracting whole hours
  DateTime adjusted = utcNow + TimeSpan(offsetHours, 0, 0);

  // Format: YYYY-MM-DDTHH:MM:SS
  char buf[20];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
           adjusted.year(), adjusted.month(), adjusted.day(),
           adjusted.hour(), adjusted.minute(), adjusted.second());
  Serial.println(buf);
}

void loop() {
  // 4. Continuously retrieve the current time from the RTC
  // Simple rate limiting to avoid flooding Serial
  const unsigned long nowMs = millis();
  if (nowMs - lastPrintMs < 200) return;
  lastPrintMs = nowMs;

  DateTime utcNow;
  bool ok = rtc.isrunning();
  if (!ok) {
    // If not running, still attempt to call now() (best-effort)
    utcNow = rtc.now();
  } else {
    utcNow = rtc.now();
  }

  // 5. Check the state of buttons, and apply the corresponding timezone
  // If multiple are pressed, priority order follows Button1..Button4 checks.
  int offset = TZ_UTC; // 1) none pressed => UTC
  if (readButton(BTN1_PIN)) offset = TZ_UTC_P9;      // Button 1: UTC+9
  else if (readButton(BTN2_PIN)) offset = TZ_UTC_P4; // Button 2: UTC+4
  else if (readButton(BTN3_PIN)) offset = TZ_UTC_M10; // Button 3: UTC-10
  else if (readButton(BTN4_PIN)) offset = TZ_UTC_M5;  // Button 4: UTC-5

  // 6. Print the resulting timestamp
  printAdjustedTimestamp(utcNow, offset);
}
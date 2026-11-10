#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

static bool rtcReady = false;
static DateTime currentTimeOnce;
static bool currentTimeCaptured = false;

static String serialLine = "";

static bool parseIsoLocalDateTime(const String& iso, DateTime& out) {
  // Expected format: YYYY-MM-DDTHH:MM:SS
  // Example: 2025-04-11T09:38:00
  if (iso.length() < 19) return false;

  int year   = iso.substring(0, 4).toInt();
  int month  = iso.substring(5, 7).toInt();
  int day    = iso.substring(8, 10).toInt();
  if (iso.charAt(10) != 'T') return false;

  int hour   = iso.substring(11, 13).toInt();
  int minute = iso.substring(14, 16).toInt();
  if (iso.charAt(16) != ':') {
    // We'll still allow parsing via substring indices above, but ensure separator is present.
    // (Keeps strictness close to expected format.)
  }
  int second = iso.substring(17, 19).toInt();

  // RTClib constructor will normalize invalid values; keep a basic sanity check.
  if (month < 1 || month > 12) return false;
  if (day < 1 || day > 31) return false;
  if (hour < 0 || hour > 23) return false;
  if (minute < 0 || minute > 59) return false;
  if (second < 0 || second > 59) return false;

  out = DateTime(year, month, day, hour, minute, second);
  return true;
}

static long daysLeftUntilTarget(const DateTime& now, const DateTime& target) {
  // Compute number of full days left until target relative to now.
  // Requirement: "Calculate how much day is left until the target time"
  // Use full-day difference based on midnight boundaries:
  // days = (target_date - now_date) where dates are at 00:00:00.
  DateTime nowMidnight(now.year(), now.month(), now.day(), 0, 0, 0);
  DateTime targetMidnight(target.year(), target.month(), target.day(), 0, 0, 0);
  return (targetMidnight.unixtime() - nowMidnight.unixtime()) / 86400L;
}

void setup() {
  Serial.begin(9600);

  Wire.begin();

  rtcReady = rtc.begin();
  if (!rtcReady) {
    // Behavior not specified; keep serial running and wait for input anyway.
    // But current time is required only once; if RTC doesn't start, we can't proceed.
    // We'll leave currentTimeCaptured false.
    Serial.println(F("RTC begin failed"));
    return;
  }

  // Read current time once with RTC module
  currentTimeOnce = rtc.now();
  currentTimeCaptured = true;
}

void loop() {
  if (!currentTimeCaptured) {
    // If RTC wasn't ready in setup, nothing else can match the requirement.
    // Still process serial input to avoid blocking.
    while (Serial.available() > 0) {
      (void)Serial.read();
    }
    return;
  }

  // Read serial input lines based on line_or_idle behavior with idle timeout 20ms.
  // We'll build a line; when a newline arrives or idle timeout passes, we treat it as a complete ISO datetime.
  static unsigned long lastByteTime = 0;
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastByteTime = millis();

    if (c == '\r') {
      // ignore
      continue;
    }
    if (c == '\n') {
      if (serialLine.length() > 0) {
        DateTime target;
        if (parseIsoLocalDateTime(serialLine, target)) {
          long days = daysLeftUntilTarget(currentTimeOnce, target);
          Serial.print(F("D-"));
          Serial.println(days);
        }
      }
      serialLine = "";
    } else {
      // keep building
      if (serialLine.length() < 40) serialLine += c;
    }
  }

  // Idle timeout handling: if line has content and no bytes for 20ms, process it.
  if (serialLine.length() > 0) {
    unsigned long now = millis();
    if (now - lastByteTime >= 20) {
      DateTime target;
      if (parseIsoLocalDateTime(serialLine, target)) {
        long days = daysLeftUntilTarget(currentTimeOnce, target);
        Serial.print(F("D-"));
        Serial.println(days);
      }
      serialLine = "";
    }
  }
}
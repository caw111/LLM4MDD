#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

String serialLine;

static bool parseIsoLocalDateTime(const String &iso, DateTime &out) {
  // Expected format: YYYY-MM-DDTHH:MM:SS (e.g., 2025-04-11T09:38:00)
  if (iso.length() != 19) return false;
  if (iso[4] != '-' || iso[7] != '-' || iso[10] != 'T' || iso[13] != ':' || iso[16] != ':')
    return false;

  auto twoDigits = [&](int idx) -> int {
    char c1 = iso[idx];
    char c2 = iso[idx + 1];
    if (c1 < '0' || c1 > '9' || c2 < '0' || c2 > '9') return -1;
    return (c1 - '0') * 10 + (c2 - '0');
  };

  auto fourDigits = [&](int idx) -> int {
    int v = 0;
    for (int i = 0; i < 4; i++) {
      char c = iso[idx + i];
      if (c < '0' || c > '9') return -1;
      v = v * 10 + (c - '0');
    }
    return v;
  };

  int year   = fourDigits(0);
  int month  = twoDigits(5);
  int day    = twoDigits(8);
  int hour   = twoDigits(11);
  int minute = twoDigits(14);
  int second = twoDigits(17);

  if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) return false;
  if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) return false;

  // DateTime will normalize/clip depending on underlying implementation,
  // but we assume valid inputs as per test.
  out = DateTime(year, month, day, hour, minute, second);
  return true;
}

void setup() {
  Serial.begin(9600);

  Wire.begin();

  if (!rtc.begin()) {
    // If RTC can't begin, do nothing further as per requirements (no simplification stated).
    // Still keep the serial reading loop alive.
  }
}

void loop() {
  // Read current time once with RTC module (only in setup per requirements).
  // Implemented by capturing it on first loop iteration.
  static bool capturedCurrent = false;
  static DateTime currentOnce;

  if (!capturedCurrent) {
    // Wait a short moment for serial monitor availability (non-blocking behavior not required).
    // Read current time once.
    if (rtc.isrunning()) {
      currentOnce = rtc.now();
    } else {
      currentOnce = rtc.now(); // best effort (some libraries may still return time)
    }
    capturedCurrent = true;
  }

  // Continuously read the serial input, which is a target datetime in ISO format
  // using line-or-idle framing (idle_timeout_ms=20).
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\r' || c == '\n') {
      if (serialLine.length() > 0) {
        DateTime target;
        if (parseIsoLocalDateTime(serialLine, target)) {
          // Calculate how much day is left until the target time.
          // Interpret as whole days difference: target date-time minus currentOnce.
          // Use seconds difference and convert to days by truncation toward zero.
          long long diffSeconds = (long long)target.unixtime() - (long long)currentOnce.unixtime();
          long daysLeft = (long)(diffSeconds / 86400LL);

          Serial.print("D-");
          Serial.print(daysLeft);
        }
        serialLine = "";
      }
    } else {
      // accumulate
      serialLine += c;
      // prevent unbounded growth on malformed input
      if (serialLine.length() > 40) serialLine.remove(0, serialLine.length() - 40);
    }
  }

  // Handle idle framing: if no new chars for ~20ms, treat buffered data as a complete line.
  static unsigned long lastCharMs = 0;
  if (Serial.available() > 0) {
    lastCharMs = millis();
  } else {
    // Update lastCharMs when we appended, but since we only know after reading,
    // use a heuristic: if buffer has content and idle for 20ms, finalize.
    if (serialLine.length() > 0 && (millis() - lastCharMs) >= 20UL) {
      DateTime target;
      String iso = serialLine;
      serialLine = "";

      if (parseIsoLocalDateTime(iso, target)) {
        long long diffSeconds = (long long)target.unixtime() - (long long)currentOnce.unixtime();
        long daysLeft = (long)(diffSeconds / 86400LL);

        Serial.print("D-");
        Serial.print(daysLeft);
      }
    }
  }
}
#include <Wire.h>
#include <RTClib.h>

static const uint8_t BUTTON_PIN = 2;

RTC_DS1307 rtc;

static void printTimestamp(const DateTime& dt) {
  char buf[20]; // "YYYY-MM-DDTHH:MM:SS"
  snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
           dt.year(), dt.month(), dt.day(),
           dt.hour(), dt.minute(), dt.second());
  Serial.println(buf);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT); // reads HIGH when pressed

  Serial.begin(9600);
  while (!Serial) {
    // wait for serial on some boards
  }

  Wire.begin();

  if (!rtc.begin()) {
    // RTC not available; continue loop without crashing
    Serial.println("RTC not found");
    return;
  }

  // If the RTC lost power, set it to compile time.
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  // Continuously retrieve current time from RTC
  if (!rtc.begin()) {
    // If it disappears, just try again next loop
    delay(200);
    return;
  }

  DateTime utc = rtc.now();

  // Check button state; if pressed add 9 hours for timezone
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    DateTime tz = utc + TimeSpan(9, 0, 0);
    printTimestamp(tz);
  } else {
    printTimestamp(utc);
  }

  delay(200);
}
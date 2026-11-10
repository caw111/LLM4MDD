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
  while (!Serial) { }

  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("RTC not found");
    // Continue without crashing; loop will keep attempting.
    return;
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  if (!rtc.begin()) {
    delay(200);
    return;
  }

  DateTime utc = rtc.now();

  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    // Add 9 hours for timezone (UTC+9)
    DateTime tz = utc + TimeSpan(0, 9, 0, 0);
    printTimestamp(tz);
  } else {
    printTimestamp(utc);
  }

  delay(200);
}
#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_RTC_timezone\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <RTClib.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr unsigned long PRINT_INTERVAL_MS = 1000;

RTC_DS1307 rtc;

unsigned long lastPrintMs = 0;
bool firstPrint = true;
bool previousButtonPressed = false;

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_RTC_timezone\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_RTC_timezone\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_RTC_timezone\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // External 10k ohm pull-down resistor; HIGH means pressed.
  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(9600);
  Wire.begin();  // Uno: SDA = A4, SCL = A5.

  if (!rtc.begin()) {
    Serial.println(F("Error: DS1307 RTC not found."));
    while (true) {
      delay(1000);
    }
  }

  // Preserve the RTC's existing UTC time when it is already running.
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(2024, 11, 24, 4, 48, 0));
  }
}

void loop() {
  const DateTime utcNow = rtc.now();
  const bool buttonPressed = (digitalRead(BUTTON_PIN) == HIGH);
  const unsigned long currentMs = millis();

  // Print periodically and immediately when the button state changes.
  if (firstPrint ||
      buttonPressed != previousButtonPressed ||
      currentMs - lastPrintMs >= PRINT_INTERVAL_MS) {
    DateTime displayedTime = utcNow;

    if (buttonPressed) {
      // DateTime arithmetic handles day, month, and year rollover.
      displayedTime = utcNow + TimeSpan(0, 9, 0, 0);
    }

    char timestamp[20];
    snprintf(timestamp, sizeof(timestamp),
             "%04u-%02u-%02uT%02u:%02u:%02u",
             static_cast<unsigned int>(displayedTime.year()),
             static_cast<unsigned int>(displayedTime.month()),
             static_cast<unsigned int>(displayedTime.day()),
             static_cast<unsigned int>(displayedTime.hour()),
             static_cast<unsigned int>(displayedTime.minute()),
             static_cast<unsigned int>(displayedTime.second()));

    Serial.println(timestamp);

    lastPrintMs = currentMs;
    previousButtonPressed = buttonPressed;
    firstPrint = false;
  }

  delay(10);
}

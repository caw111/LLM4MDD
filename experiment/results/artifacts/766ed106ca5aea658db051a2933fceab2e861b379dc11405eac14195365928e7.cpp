#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_multiple_timezone\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <RTClib.h>
#include <stdio.h>

RTC_DS1307 rtc;

// Existing external 10k-ohm resistors pull these inputs LOW.
const uint8_t BUTTON_PINS[4] = {2, 3, 4, 5};
const int8_t UTC_OFFSETS[4] = {9, 4, -10, -5};

const unsigned long DEBOUNCE_MS = 25;
bool rawState[4];
bool stableState[4];
unsigned long lastChangeMs[4];

bool hasPrinted = false;
uint32_t lastPrintedUtcSecond = 0;
int8_t lastPrintedOffset = 0;

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_multiple_timezone\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_multiple_timezone\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_multiple_timezone\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  for (uint8_t i = 0; i < 4; ++i) {
    pinMode(BUTTON_PINS[i], INPUT);
    rawState[i] = digitalRead(BUTTON_PINS[i]) == HIGH;
    stableState[i] = rawState[i];
    lastChangeMs[i] = millis();
  }

  Serial.begin(9600);
  Wire.begin();  // Uno: SDA = A4, SCL = A5.

  if (!rtc.begin()) {
    Serial.println(F("ERROR: DS1307 RTC not found."));
    while (true) {
      delay(1000);
    }
  }

  // Preserve the circuit's initialized RTC time when it is running.
  // If stopped, initialize from build time; build time must represent UTC.
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  const unsigned long nowMs = millis();

  for (uint8_t i = 0; i < 4; ++i) {
    const bool pressed = digitalRead(BUTTON_PINS[i]) == HIGH;

    if (pressed != rawState[i]) {
      rawState[i] = pressed;
      lastChangeMs[i] = nowMs;
    }

    if (nowMs - lastChangeMs[i] >= DEBOUNCE_MS) {
      stableState[i] = rawState[i];
    }
  }

  // No pressed button means UTC. For simultaneous presses,
  // the lowest-numbered button takes priority.
  int8_t offsetHours = 0;
  for (uint8_t i = 0; i < 4; ++i) {
    if (stableState[i]) {
      offsetHours = UTC_OFFSETS[i];
      break;
    }
  }

  // Keep the RTC in UTC; apply offsets only to the displayed timestamp.
  const DateTime utcNow = rtc.now();
  const uint32_t utcSecond = utcNow.unixtime();

  if (!hasPrinted ||
      utcSecond != lastPrintedUtcSecond ||
      offsetHours != lastPrintedOffset) {
    const DateTime localNow =
        utcNow + TimeSpan((int32_t)offsetHours * 3600L);

    char timestamp[20];
    snprintf(timestamp, sizeof(timestamp),
             "%04u-%02u-%02uT%02u:%02u:%02u",
             (unsigned int)localNow.year(),
             (unsigned int)localNow.month(),
             (unsigned int)localNow.day(),
             (unsigned int)localNow.hour(),
             (unsigned int)localNow.minute(),
             (unsigned int)localNow.second());

    Serial.println(timestamp);

    lastPrintedUtcSecond = utcSecond;
    lastPrintedOffset = offsetHours;
    hasPrinted = true;
  }

  delay(5);
}

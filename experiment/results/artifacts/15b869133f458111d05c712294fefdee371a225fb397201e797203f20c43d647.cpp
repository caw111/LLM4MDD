#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_dday_counter\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

// Wiring: RTC 5V -> Uno 5V, GND -> GND.2, SDA -> A4, SCL -> A5.
const unsigned long IDLE_TIMEOUT_MS = 20;
const uint8_t ISO_LENGTH = 19;

uint32_t referenceTime;
char inputBuffer[ISO_LENGTH + 1];
uint8_t inputLength = 0;
bool inputOverflow = false;
unsigned long lastInputMillis = 0;

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_dday_counter\\direct\\attempt_0\\sketch\\sketch.ino"
bool parseISODateTime(const char *text, DateTime &result);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_dday_counter\\direct\\attempt_0\\sketch\\sketch.ino"
void processInput();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_dday_counter\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_dday_counter\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_dday_counter\\direct\\attempt_0\\sketch\\sketch.ino"
bool parseISODateTime(const char *text, DateTime &result) {
  for (uint8_t i = 0; i < ISO_LENGTH; ++i) {
    if (i == 4 || i == 7) {
      if (text[i] != '-') return false;
    } else if (i == 10) {
      if (text[i] != 'T') return false;
    } else if (i == 13 || i == 16) {
      if (text[i] != ':') return false;
    } else if (text[i] < '0' || text[i] > '9') {
      return false;
    }
  }

  const uint16_t year =
      (text[0] - '0') * 1000U + (text[1] - '0') * 100U +
      (text[2] - '0') * 10U + (text[3] - '0');
  const uint8_t month = (text[5] - '0') * 10 + text[6] - '0';
  const uint8_t day = (text[8] - '0') * 10 + text[9] - '0';
  const uint8_t hour = (text[11] - '0') * 10 + text[12] - '0';
  const uint8_t minute = (text[14] - '0') * 10 + text[15] - '0';
  const uint8_t second = (text[17] - '0') * 10 + text[18] - '0';

  // RTClib's supported DateTime range is 2000 through 2099.
  if (year < 2000 || year > 2099 ||
      month < 1 || month > 12 ||
      hour > 23 || minute > 59 || second > 59) {
    return false;
  }

  const uint8_t monthDays[] = {
      31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };
  uint8_t maximumDay = monthDays[month - 1];
  if (month == 2 && year % 4 == 0) {
    maximumDay = 29;
  }
  if (day < 1 || day > maximumDay) return false;

  result = DateTime(year, month, day, hour, minute, second);
  return true;
}

void processInput() {
  if (inputLength == 0 && !inputOverflow) return;

  inputBuffer[inputLength] = '\0';
  DateTime target;

  if (inputOverflow || inputLength != ISO_LENGTH ||
      !parseISODateTime(inputBuffer, target)) {
    Serial.println(F("Error: expected YYYY-MM-DDTHH:MM:SS"));
  } else {
    const int64_t secondsLeft =
        static_cast<int64_t>(target.unixtime()) -
        static_cast<int64_t>(referenceTime);

    // Report complete 24-hour days relative to the single startup reading.
    // Negative values indicate targets in the past.
    const long daysLeft = static_cast<long>(secondsLeft / 86400LL);

    Serial.print(F("D-"));
    Serial.println(daysLeft);
  }

  inputLength = 0;
  inputOverflow = false;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println(F("Error: RTC not found"));
    while (true) {
      delay(1000);
    }
  }

  // Read the RTC exactly once; all targets use this fixed reference.
  const DateTime currentTime = rtc.now();
  referenceTime = currentTime.unixtime();
}

void loop() {
  while (Serial.available() > 0) {
    const char character = static_cast<char>(Serial.read());
    lastInputMillis = millis();

    if (character == '\r' || character == '\n') {
      processInput();
    } else if (inputLength < ISO_LENGTH) {
      inputBuffer[inputLength++] = character;
    } else {
      inputOverflow = true;
    }
  }

  // Also accept input sent without a line ending.
  if ((inputLength > 0 || inputOverflow) &&
      millis() - lastInputMillis >= IDLE_TIMEOUT_MS) {
    processInput();
  }
}

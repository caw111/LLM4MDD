#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_alarm\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <RTClib.h>

constexpr uint8_t BUZZER_PIN = 8;
constexpr unsigned int BUZZER_FREQUENCY_HZ = 1000;

RTC_DS1307 rtc;
const DateTime alarmTime(2024, 11, 24, 4, 49, 0);

bool buzzerActive = false;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_alarm\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_alarm\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_alarm\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Arduino Uno I2C: SDA = A4, SCL = A5.
  Wire.begin();

  if (!rtc.begin()) {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
    while (true) {
      delay(1000);
    }
  }

  // Preserve the RTC's configured time; do not reset it on startup.
}

void loop() {
  const DateTime now = rtc.now();
  const bool alarmDue = now.unixtime() >= alarmTime.unixtime();

  if (alarmDue && !buzzerActive) {
    tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
    buzzerActive = true;
  } else if (!alarmDue && buzzerActive) {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
    buzzerActive = false;
  }

  delay(100);
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_021\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t SENSOR_PIN = A0;
const uint8_t LED_PIN = 9;
const uint8_t BUZZER_PIN = 6;
const int ALARM_THRESHOLD = 700;
const unsigned int ALARM_FREQUENCY_HZ = 1000;

bool alarmActive = false;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_021\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_021\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_021\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  analogWrite(LED_PIN, 0);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  const int sensorValue = analogRead(SENSOR_PIN);
  const bool shouldAlarm = sensorValue > ALARM_THRESHOLD;

  if (shouldAlarm != alarmActive) {
    alarmActive = shouldAlarm;

    if (alarmActive) {
      analogWrite(LED_PIN, 255);
      tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
    } else {
      analogWrite(LED_PIN, 0);
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

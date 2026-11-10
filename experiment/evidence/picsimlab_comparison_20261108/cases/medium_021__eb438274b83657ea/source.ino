#include <Arduino.h>

const uint8_t PIN_SENSOR_A0 = A0;
const uint8_t PIN_LED_D9     = 9;
const uint8_t PIN_BUZZ_D6    = 6;

const int THRESHOLD = 700;     // A0 > 700 => LED full + buzzer alarm
const unsigned int BUZZ_FREQ_HZ = 2000; // alarm tone frequency (typical audible buzzer)

void setup() {
  pinMode(PIN_LED_D9, OUTPUT);
  pinMode(PIN_BUZZ_D6, OUTPUT); // tone() will drive the pin, but set mode anyway.

  digitalWrite(PIN_LED_D9, LOW);
  noTone(PIN_BUZZ_D6);
}

void loop() {
  int sensorValue = analogRead(PIN_SENSOR_A0);

  if (sensorValue > THRESHOLD) {
    // LED 全亮
    digitalWrite(PIN_LED_D9, HIGH);

    // 蜂鸣器报警
    tone(PIN_BUZZ_D6, BUZZ_FREQ_HZ);
  } else {
    digitalWrite(PIN_LED_D9, LOW);
    noTone(PIN_BUZZ_D6);
  }
}
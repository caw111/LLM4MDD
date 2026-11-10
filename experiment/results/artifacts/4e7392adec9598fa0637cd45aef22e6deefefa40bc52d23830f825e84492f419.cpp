#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_013\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t POTENTIOMETER_PIN = A0;
const uint8_t BUTTON_PIN = 2;
const unsigned long REPORT_INTERVAL_MS = 1000UL;

unsigned long lastReportMs = 0;

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_013\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_013\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_013\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(9600);
  Serial.setTimeout(20);

  lastReportMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastReportMs >= REPORT_INTERVAL_MS) {
    lastReportMs = now;

    const int potentiometerValue = analogRead(POTENTIOMETER_PIN);
    const int buttonState = digitalRead(BUTTON_PIN);

    Serial.print(F("A0="));
    Serial.print(potentiometerValue);
    Serial.print(F(", D2="));
    Serial.println(buttonState);
  }
}

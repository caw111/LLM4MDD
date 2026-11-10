#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_parking_spot_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t TRIG_PIN = 8;
const uint8_t ECHO_PIN = 9;
const uint8_t RED_PIN = 4;
const uint8_t GREEN_PIN = 3;
const uint8_t BLUE_PIN = 2;

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_parking_spot_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_parking_spot_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_parking_spot_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  // A timeout is not a valid distance; retain the last indication.
  if (duration > 0) {
    const float distanceCm = duration / 58.0f;

    // Common-cathode RGB LED: HIGH turns a color on.
    if (distanceCm < 50.0f) {
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(RED_PIN, HIGH);
    } else {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
    }
    digitalWrite(BLUE_PIN, LOW);
  }

  // Allow the sensor to settle between measurements.
  delay(60);
}

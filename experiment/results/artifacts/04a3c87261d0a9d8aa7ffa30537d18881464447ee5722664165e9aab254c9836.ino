#include <Arduino.h>

const uint8_t SELECT_PINS[4] = {2, 3, 4, 5};  // S0, S1, S2, S3
const uint8_t SIGNAL_PIN = A0;
const uint8_t SENSOR_COUNT = 5;

void setup() {
  for (uint8_t bit = 0; bit < 4; ++bit) {
    pinMode(SELECT_PINS[bit], OUTPUT);
    digitalWrite(SELECT_PINS[bit], LOW);
  }

  pinMode(SIGNAL_PIN, INPUT);
  analogReference(DEFAULT);
  Serial.begin(9600);

  // Multiplexer EN is wired to GND, so it is always enabled.
}

void loop() {
  for (uint8_t channel = 0; channel < SENSOR_COUNT; ++channel) {
    for (uint8_t bit = 0; bit < 4; ++bit) {
      digitalWrite(SELECT_PINS[bit], (channel >> bit) & 0x01);
    }

    // Allow the multiplexer signal to settle after changing channels.
    delayMicroseconds(10);
    analogRead(SIGNAL_PIN);  // Discard the first ADC sample after switching.
    const int value = analogRead(SIGNAL_PIN);

    Serial.print(F("Sensor "));
    Serial.print(channel);
    Serial.print(F(": "));
    Serial.println(value);
  }

  delay(100);
}
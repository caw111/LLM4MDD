#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_multiplexer_photoresistor\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t SELECT_PINS[4] = {2, 3, 4, 5}; // S0, S1, S2, S3
const uint8_t SIGNAL_PIN = A0;
const uint8_t SENSOR_COUNT = 6;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_multiplexer_photoresistor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_multiplexer_photoresistor\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_multiplexer_photoresistor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  for (uint8_t i = 0; i < 4; ++i) {
    pinMode(SELECT_PINS[i], OUTPUT);
    digitalWrite(SELECT_PINS[i], LOW);
  }

  pinMode(SIGNAL_PIN, INPUT);
  analogReference(DEFAULT);
  Serial.begin(9600);

  // Multiplexer EN is wired to GND, so it is always enabled.
}

void loop() {
  for (uint8_t channel = 0; channel < SENSOR_COUNT; ++channel) {
    for (uint8_t bit = 0; bit < 4; ++bit) {
      digitalWrite(SELECT_PINS[bit],
                   (channel & (1U << bit)) ? HIGH : LOW);
    }

    // Allow the selected analog signal to settle.
    delayMicroseconds(100);
    analogRead(SIGNAL_PIN); // Discard the first conversion after switching.
    const int lightValue = analogRead(SIGNAL_PIN);

    Serial.print(F("Sensor "));
    Serial.print(channel);
    Serial.print(F(": "));
    Serial.println(lightValue);
  }

  delay(250);
}

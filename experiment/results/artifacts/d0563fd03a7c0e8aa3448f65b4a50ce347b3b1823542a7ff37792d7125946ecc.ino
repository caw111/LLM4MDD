#include <Arduino.h>

const int LED_PINS[5] = {2, 3, 4, 5, 6};
const int PHOTO_PINS[5] = {A0, A1, A2, A3, A4};

const int THRESHOLD = 500;

void setup() {
  // 1. Set pin2 to pin6 as OUTPUT, and A0 to A4 as INPUT
  for (int i = 0; i < 5; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
    pinMode(PHOTO_PINS[i], INPUT);
  }
}

void loop() {
  // 2. Continuously read the photoresistor 1 to 5 values
  // 3. Check if the sensor value is greater than the threshold, which is 500
  // 4. If the sensor value is greater than the threshold, turn the corresponding LED on.
  for (int i = 0; i < 5; i++) {
    int sensorValue = analogRead(PHOTO_PINS[i]);

    if (sensorValue > THRESHOLD) {
      digitalWrite(LED_PINS[i], HIGH);
    } else {
      digitalWrite(LED_PINS[i], LOW);
    }
  }
}
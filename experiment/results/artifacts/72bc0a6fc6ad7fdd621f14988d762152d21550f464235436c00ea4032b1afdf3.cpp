#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_potentiometer_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Potentiometer Bar LED
// Arduino Uno + bar graph LED + 220 ohm resistors + potentiometer

const uint8_t ledPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // Anode pins 1~10
const uint8_t potSigPin = A0;                                      // Potentiometer SIG

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_potentiometer_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_potentiometer_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_potentiometer_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(potSigPin, INPUT);
}

void loop() {
  int potVal = analogRead(potSigPin); // 0..1023
  int mappedVal = map(potVal, 0, 1023, 0, 10); // 0..10

  // Turn on only LEDs up to mapped value; others off.
  // Success criterion: anode pins with number smaller than mapped value get HIGH.
  // With pins numbered 1..10: HIGH for 1..mappedVal, so index (0-based) < mappedVal.
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], (i < (uint8_t)mappedVal) ? HIGH : LOW);
  }
}

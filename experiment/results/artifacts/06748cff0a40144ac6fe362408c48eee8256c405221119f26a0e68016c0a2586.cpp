#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_5\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LED_PIN = 3;   // led1:A -> mega:3 (red)
const uint8_t BTN_PIN = 2;   // mega:2 -> k1:1.l (green)
const uint8_t LED_INIT_STATE = HIGH; // LED should be ON after reset

bool ledState = true;
bool lastBtnReading = HIGH;
bool buttonStableState = HIGH;
unsigned long lastDebounceTimeMs = 0;
const unsigned long debounceDelayMs = 30;

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_5\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_5\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_5\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP); // k1 provides switch to GND (wired to mega:GND.1)

  ledState = true;              // initialize LED ON
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);

  lastBtnReading = digitalRead(BTN_PIN);
  buttonStableState = lastBtnReading;
}

void loop() {
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    lastDebounceTimeMs = millis();
    lastBtnReading = reading;
  }

  if ((millis() - lastDebounceTimeMs) >= debounceDelayMs) {
    if (reading != buttonStableState) {
      buttonStableState = reading;

      // Button press: using INPUT_PULLUP, press connects to GND => LOW
      if (buttonStableState == LOW) {
        ledState = !ledState; // toggle on each press
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      }
    }
  }
}

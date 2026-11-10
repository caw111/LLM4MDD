#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 2; // D2

volatile unsigned long counterValue = 0;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  // No extra serial framing settings needed on Arduino; baud=9600.
}

void loop() {
  static bool lastStable = HIGH;
  static unsigned long lastChangeMs = 0;
  static bool latchedPressed = false;

  bool reading = digitalRead(BUTTON_PIN);

  // Simple debounce
  if (reading != lastStable) {
    lastStable = reading;
    lastChangeMs = millis();
  }

  // When stable for a short time, accept state
  if (millis() - lastChangeMs >= 25) {
    if (reading == LOW) { // pressed (INPUT_PULLUP)
      if (!latchedPressed) {
        latchedPressed = true;
        counterValue++;
        Serial.println(counterValue);
      }
    } else {
      latchedPressed = false;
    }
  }
}

#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino UNO
// D2: Button (system switch)
// D9: LED controlled by button state
// A0: Referenced as analog control input when system is ON (per requirement)

const uint8_t PIN_BUTTON = 2;   // D2
const uint8_t PIN_LED     = 9;   // D9

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP); // assume button to GND when pressed
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  // "开启后 A0 控制 D9 LED，否则关闭"
  // Interpret button pressed == system ON (D2 goes LOW with INPUT_PULLUP).
  bool systemOn = (digitalRead(PIN_BUTTON) == LOW);

  if (!systemOn) {
    digitalWrite(PIN_LED, LOW);
    return;
  }

  // System is ON: A0 controls D9 LED brightness/state.
  // Use full-range analog value to drive PWM on D9.
  int a0 = analogRead(A0);                 // 0..1023
  uint8_t pwm = (uint32_t)a0 * 255 / 1023; // 0..255
  analogWrite(PIN_LED, pwm);
}

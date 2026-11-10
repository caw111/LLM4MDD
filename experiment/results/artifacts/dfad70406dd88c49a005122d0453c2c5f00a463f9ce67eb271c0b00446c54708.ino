#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;   // D2
const uint8_t LED_PIN     = 9;   // D9

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // 按钮（D2）控制系统开关：开启后 A0 控制 D9 LED，否则关闭
  bool systemOn = (digitalRead(BUTTON_PIN) == LOW); // pressed -> LOW (INPUT_PULLUP)

  if (!systemOn) {
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // A0 控制 D9 LED
  // Use analog value on A0 to drive brightness via PWM on D9
  int a0 = analogRead(A0);               // 0..1023
  int pwm = map(a0, 0, 1023, 0, 255);   // 0..255
  analogWrite(LED_PIN, pwm);
}
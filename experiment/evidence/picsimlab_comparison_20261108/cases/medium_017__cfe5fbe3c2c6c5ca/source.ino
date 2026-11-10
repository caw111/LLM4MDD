#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;
const uint8_t LED_PIN = 9;
const uint8_t CONTROL_PIN = A0;
const unsigned long DEBOUNCE_MS = 30;

bool systemEnabled = false;
bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastButtonChangeTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // 按钮连接 D2 与 GND
  pinMode(LED_PIN, OUTPUT);
  pinMode(CONTROL_PIN, INPUT);

  analogWrite(LED_PIN, 0);
}

void loop() {
  const unsigned long now = millis();
  const bool buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonReading) {
    lastButtonChangeTime = now;
    lastButtonReading = buttonReading;
  }

  if (now - lastButtonChangeTime >= DEBOUNCE_MS &&
      buttonReading != stableButtonState) {
    stableButtonState = buttonReading;

    // 每次按下切换系统开关，长按不重复触发。
    if (stableButtonState == LOW) {
      systemEnabled = !systemEnabled;
    }
  }

  if (systemEnabled) {
    const int controlValue = analogRead(CONTROL_PIN);
    const int brightness = map(controlValue, 0, 1023, 0, 255);
    analogWrite(LED_PIN, brightness);
  } else {
    analogWrite(LED_PIN, 0);
  }
}
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t LED_PIN = 13;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr unsigned long DEBOUNCE_MS = 30;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

uint16_t count = 0;
uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeTime = 0;

void updateOutputs() {
  display.showNumberDec(count, false);
  // 奇数亮灯，偶数灭灯。
  digitalWrite(LED_PIN, (count % 2 != 0) ? HIGH : LOW);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // 按钮连接 D2 与 GND。
  pinMode(LED_PIN, OUTPUT);

  display.setBrightness(7, true);
  updateOutputs();

  lastRawButtonState = digitalRead(BUTTON_PIN);
  lastButtonChangeTime = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeTime = now;
  }

  if (now - lastButtonChangeTime >= DEBOUNCE_MS &&
      rawButtonState != stableButtonState) {
    stableButtonState = rawButtonState;

    // 仅在按下时计数；长按不会重复计数。
    if (stableButtonState == LOW) {
      count = (count + 1) % 10000;  // 四位显示器：9999 后回到 0。
      updateOutputs();
    }
  }
}
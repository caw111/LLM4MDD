#include <Arduino.h>

// 按钮接法：D2/D3 -> 按钮 -> GND，使用内部上拉。
// 电位器：两端接 5V/GND，中间滑动端接 A0。
// PWM 输出：D9；若连接 LED，必须串联限流电阻。
// D2：自动渐变模式；D3：手动电位器调光模式。
constexpr uint8_t AUTO_BUTTON_PIN = 2;
constexpr uint8_t MANUAL_BUTTON_PIN = 3;
constexpr uint8_t POT_PIN = A0;
constexpr uint8_t PWM_OUTPUT_PIN = 9;

constexpr unsigned long DEBOUNCE_MS = 25;
constexpr unsigned long FADE_STEP_MS = 10;

enum Mode : uint8_t {
  AUTO_MODE,
  MANUAL_MODE
};

struct Button {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeMs;
};

Button autoButton = {AUTO_BUTTON_PIN, HIGH, HIGH, 0};
Button manualButton = {MANUAL_BUTTON_PIN, HIGH, HIGH, 0};

Mode currentMode = AUTO_MODE;
int brightness = 0;
int fadeDirection = 1;
unsigned long lastFadeMs = 0;

// 非阻塞消抖：仅在确认按下时返回一次 true。
bool wasPressed(Button &button, unsigned long now) {
  const uint8_t reading = digitalRead(button.pin);

  if (reading != button.lastReading) {
    button.lastReading = reading;
    button.lastChangeMs = now;
  }

  if (reading != button.stableState &&
      now - button.lastChangeMs >= DEBOUNCE_MS) {
    button.stableState = reading;
    return reading == LOW;
  }

  return false;
}

void setup() {
  pinMode(AUTO_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MANUAL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(PWM_OUTPUT_PIN, OUTPUT);

  analogWrite(PWM_OUTPUT_PIN, brightness);
  lastFadeMs = millis();
}

void loop() {
  const unsigned long now = millis();

  const bool autoPressed = wasPressed(autoButton, now);
  const bool manualPressed = wasPressed(manualButton, now);

  if (autoPressed) {
    currentMode = AUTO_MODE;
    lastFadeMs = now;

    // 从当前亮度继续渐变，避免切换模式时亮度突变。
    if (brightness <= 0) {
      fadeDirection = 1;
    } else if (brightness >= 255) {
      fadeDirection = -1;
    }
  }

  // 两个按钮同时确认按下时，手动模式优先。
  if (manualPressed) {
    currentMode = MANUAL_MODE;
  }

  if (currentMode == MANUAL_MODE) {
    const int potValue = analogRead(POT_PIN);
    brightness = (static_cast<long>(potValue) * 255L + 511L) / 1023L;
  } else {
    // 无 delay()，保证渐变过程中按钮仍能及时响应。
    while (now - lastFadeMs >= FADE_STEP_MS) {
      lastFadeMs += FADE_STEP_MS;
      brightness += fadeDirection;

      if (brightness >= 255) {
        brightness = 255;
        fadeDirection = -1;
      } else if (brightness <= 0) {
        brightness = 0;
        fadeDirection = 1;
      }
    }
  }

  analogWrite(PWM_OUTPUT_PIN, brightness);
}
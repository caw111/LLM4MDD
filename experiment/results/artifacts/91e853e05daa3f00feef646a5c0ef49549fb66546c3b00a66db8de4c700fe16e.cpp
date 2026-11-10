#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;   // D2 (INPUT_PULLUP), low = pressed
const uint8_t LED_PIN     = 13;  // D13

static bool running = false;
static uint8_t ledState = LOW;
static unsigned long lastFlipMs = 0;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 初始停止且D13熄灭
}

void loop() {
  // --- Edge detection: only toggle on falling edge (button pressed) ---
  static bool prevButtonLow = false;

  bool buttonLow = (digitalRead(BUTTON_PIN) == LOW); // low means pressed
  if (buttonLow && !prevButtonLow) {
    // falling edge (press event)
    running = !running;

    if (!running) {
      // 停止立即熄灭，再启动重新计时
      ledState = LOW;
      digitalWrite(LED_PIN, ledState);
    } else {
      // 启动时从熄灭开始，每500ms翻转
      ledState = LOW;
      digitalWrite(LED_PIN, ledState);
      lastFlipMs = millis(); // 重新计时
    }
  }
  prevButtonLow = buttonLow;

  // --- Blinking behavior when running ---
  if (running) {
    unsigned long now = millis();
    if (now - lastFlipMs >= 500UL) { // 每500ms翻转（容差由计时方式允许20ms）
      lastFlipMs = now;
      ledState = (ledState == LOW) ? HIGH : LOW;
      digitalWrite(LED_PIN, ledState);
    }
  }
}

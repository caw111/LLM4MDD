#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;   // D2 按钮
const uint8_t LED_PIN     = 13;  // D13 LED

// 模式控制：按一次开始闪烁，再按一次停止闪烁并熄灭
volatile bool flashing = false;

// 简单消抖参数
const unsigned long DEBOUNCE_MS = 50;
unsigned long lastDebounceTime = 0;

bool lastButtonState = HIGH; // 默认未按下为高电平（内部上拉）
bool currentButtonState = HIGH;

unsigned long lastToggleMs = 0;
bool ledOn = false;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 按钮按下接地
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  currentButtonState = digitalRead(BUTTON_PIN);

  // 检测按钮边沿 + 消抖
  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis();
    lastButtonState = currentButtonState;
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    // 有效按压：从释放(HIGH)到按下(LOW)
    static bool stableState = HIGH;
    if (stableState != currentButtonState) {
      stableState = currentButtonState;

      if (stableState == LOW) { // 按下
        flashing = !flashing;

        if (!flashing) {
          // 停止闪烁并熄灭
          ledOn = false;
          digitalWrite(LED_PIN, LOW);
        } else {
          // 开始闪烁：LED 状态从当前开始
          lastToggleMs = millis();
        }
      }
    }
  }

  // 闪烁逻辑（仅当 flashing=true）
  if (flashing) {
    // 闪烁周期：此处采用常见 LED 闪烁 500ms（1Hz）
    const unsigned long INTERVAL_MS = 500;

    if (millis() - lastToggleMs >= INTERVAL_MS) {
      lastToggleMs = millis();
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }
  }
}

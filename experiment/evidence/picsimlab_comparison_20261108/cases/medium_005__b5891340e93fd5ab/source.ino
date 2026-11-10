#include <Arduino.h>

const uint8_t BUTTON_PIN = 3; // D3 按钮
const uint8_t BUZZER_PIN  = 6; // D6 蜂鸣器（按住发声，松开后延迟 1 秒停止）

// 按需求：按住发声；松开后延迟 1 秒停止
static bool buzzerActive = false;
static unsigned long stopRequestedAt = 0;
static bool stopScheduled = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 使用上拉：未按下为高电平，按下为低电平
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // 读按钮：按下为 LOW
  bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if (pressed) {
    // 按住：立刻发声
    buzzerActive = true;

    // 清除“松开后延迟停止”的调度
    stopScheduled = false;

    // 这里用 tone 产生蜂鸣声（保持“按住发声”的行为）
    tone(BUZZER_PIN, 2000); // 频率可按需求保持固定
  } else {
    // 松开：若当前未安排停止，则安排 1 秒后停止
    if (!buzzerActive) {
      // 已经停止状态，不需要进一步处理
    } else if (!stopScheduled) {
      stopScheduled = true;
      stopRequestedAt = millis();
    }

    // 若已安排停止，检查是否到时间
    if (stopScheduled) {
      if (millis() - stopRequestedAt >= 1000UL) {
        noTone(BUZZER_PIN);
        digitalWrite(BUZZER_PIN, LOW);
        buzzerActive = false;
        stopScheduled = false;
      }
    }
  }

  // 小延时避免过度占用 CPU（不影响时序逻辑）
  delay(1);
}
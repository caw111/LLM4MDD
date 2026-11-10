#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 3;   // D3
const uint8_t BUZZER_PIN  = 6;   // D6

const uint16_t BUZZER_STOP_DELAY_MS = 1000; // 松开后延迟 1 秒停止
const uint16_t TONE_FREQ_HZ = 2000;          // 发声频率（按住发声）

bool buzzerActive = false;
unsigned long stopPendingAt = 0; // 记录松开后的停止计划时间

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 使用上拉，按下为低电平
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  buzzerActive = false;
  stopPendingAt = 0;
}

void loop() {
  const bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);

  if (buttonPressed) {
    // 按住发声：立即开始/保持发声，并取消停止计划
    buzzerActive = true;
    stopPendingAt = 0;
    tone(BUZZER_PIN, TONE_FREQ_HZ);
  } else {
    // 松开：若之前在发声，则延迟 1 秒后停止
    if (buzzerActive && stopPendingAt == 0) {
      stopPendingAt = millis();
    }

    if (stopPendingAt != 0) {
      if (millis() - stopPendingAt >= BUZZER_STOP_DELAY_MS) {
        noTone(BUZZER_PIN);
        buzzerActive = false;
        stopPendingAt = 0;
      }
    }
  }
}

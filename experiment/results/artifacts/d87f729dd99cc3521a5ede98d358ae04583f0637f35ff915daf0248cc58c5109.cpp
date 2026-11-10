#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t LED_PWM_PIN = 9;   // D9 PWM LED
static const uint8_t LDR_PIN     = A0;  // A0 光敏电阻
static const uint8_t STAT_PIN    = 13;  // D13（按配置保留，不参与逻辑除非需要）

// 光控亮度：环境光越强，LED 越暗
// 基本策略：读入 A0 -> 映射到 PWM（强光 -> 小占空比）
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(STAT_PIN, OUTPUT);

  // 设定初始状态
  analogWrite(LED_PWM_PIN, 0);
  digitalWrite(STAT_PIN, LOW);

  // 由于题目未提供串口/校准/阈值/计时等额外参数，这里采用默认线性映射行为
}

void loop() {
  // 读取环境光强度（0~1023）
  int raw = analogRead(LDR_PIN);

  // 防止异常范围
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;

  // 将强光对应较低 PWM：
  // 强光(大 raw) -> PWM(小)
  int pwm = map(raw, 0, 1023, 255, 0);

  analogWrite(LED_PWM_PIN, pwm);

  // 保留 D13 行为（不与亮度控制耦合，默认保持低）
  digitalWrite(STAT_PIN, LOW);
}

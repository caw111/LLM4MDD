#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LED_PWM_PIN = 9;   // D9 PWM LED
const uint8_t LED_DEBUG_PIN = 13; // D13 (no requirement specified beyond inventory)
const uint8_t LDR_PIN = A0;      // A0 光敏电阻

// 光控亮度：环境光越强，LED 越暗
// 采用简单的反向映射：LDR读值越大 -> PWM占空比越小
// 为避免极端值造成全亮/全灭，加入可配置的校准范围。
int ldrMin = 0;     // 将用于归一化下限（需在运行时自动更新）
int ldrMax = 1023;  // 将用于归一化上限（需在运行时自动更新）

// 自动校准/跟随：在前一段时间内寻找更合理的 min/max，之后仍随环境缓慢调整
static const uint32_t CALIBRATION_MS = 5000;
static const uint32_t UPDATE_INTERVAL_MS = 50;
static uint32_t t0 = 0;
static uint32_t lastUpdate = 0;

static int lastPWM = -1;

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(LED_DEBUG_PIN, OUTPUT);

  t0 = millis();

  // 初始校准（给一个宽范围，后续会跟随更新）
  ldrMin = 1023;
  ldrMax = 0;

  // 初始指示（可移除不影响需求，但保留一个状态输出）
  digitalWrite(LED_DEBUG_PIN, LOW);
}

void loop() {
  uint32_t now = millis();

  int raw = analogRead(LDR_PIN); // 0..1023

  // 在前 5 秒内快速建立 min/max；之后慢速跟随
  if (now - t0 < CALIBRATION_MS) {
    if (raw < ldrMin) ldrMin = raw;
    if (raw > ldrMax) ldrMax = raw;
  } else if (now - lastUpdate >= UPDATE_INTERVAL_MS) {
    lastUpdate = now;

    // 缓慢更新 min/max，避免被突发噪声锁死
    // 方式：使用小步逼近
    if (raw < ldrMin) ldrMin = raw;
    else ldrMin = (ldrMin * 99 + raw * 1) / 100;

    if (raw > ldrMax) ldrMax = raw;
    else ldrMax = (ldrMax * 99 + raw * 1) / 100;
  }

  // 防止 min/max 过于接近导致除零或映射失效
  int range = ldrMax - ldrMin;
  if (range < 5) range = 5;

  // 环境光越强(raw越大) -> LED 越暗(pwm越小)
  // 归一化到 0..1：brightness = (raw - ldrMin)/range
  // pwm = 255*(1 - brightness)
  long brightnessNumer = (long)raw - (long)ldrMin; // 可为负
  if (brightnessNumer < 0) brightnessNumer = 0;
  if (brightnessNumer > range) brightnessNumer = range;

  // pwm 与 Arduino Uno 的 analogWrite 0..255 对应
  int pwm = (int)(255L * (range - brightnessNumer) / range);

  // 轻微去抖：只有显著变化才写入（不改变行为逻辑，只减少写入）
  if (pwm != lastPWM) {
    analogWrite(LED_PWM_PIN, pwm);
    lastPWM = pwm;
  }

  // 可选调试：D13 显示当前是否在“较强光”状态（不影响主要需求的光控）
  // 强光时置高，弱光时置低
  int strongThreshold = ldrMin + (range * 2) / 3; // 约 66% 亮度位置
  digitalWrite(LED_DEBUG_PIN, (raw >= strongThreshold) ? HIGH : LOW);
}

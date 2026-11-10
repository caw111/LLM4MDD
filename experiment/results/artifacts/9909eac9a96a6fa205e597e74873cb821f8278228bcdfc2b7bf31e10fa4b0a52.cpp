#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t PIN_LDR = A0;      // 光敏电阻输入
const uint8_t PIN_PWM = 9;       // PWM LED 输出 D9
const uint8_t PIN_BUTTON = 2;   // 按钮 D2
const uint8_t PIN_TM_CLK = 4;     // TM1637 CLK=D4
const uint8_t PIN_TM_DIO = 5;     // TM1637 DIO=D5

// TM1637: 使用4位数码管显示
TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);

// --------- 行为参数（按需求：默认根据光照强度自动调节LED亮度并显示映射值，按钮切换手动冻结/恢复） ---------
static const uint16_t READ_SAMPLES = 8;     // 读光敏电阻平均次数
static const uint16_t DEBOUNCE_MS  = 50;     // 按键消抖

// 映射范围：将A0光照强度映射到 0~100（并显示 4位整数）
static const uint16_t MAP_IN_MIN = 0;
static const uint16_t MAP_IN_MAX = 1023;
static const uint16_t MAP_OUT_MIN = 0;
static const uint16_t MAP_OUT_MAX = 100;

// PWM 输出：把 0~100 映射到 0~255
static const uint16_t PWM_OUT_MIN = 0;
static const uint16_t PWM_OUT_MAX = 255;

// 手动冻结：保存冻结的显示/亮度值
volatile bool manualFrozen = false;
uint16_t frozenValue = 0; // 0~100

// 按键状态机
bool lastButtonStable = HIGH;         // 使用内部上拉：未按=HIGH
bool lastButtonReading = HIGH;
unsigned long lastDebounceTime = 0;

#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readLdrAverage();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t mapClamped(uint16_t x, uint16_t inMin, uint16_t inMax, uint16_t outMin, uint16_t outMax);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplayAndLed(uint16_t mappedVal0to100);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readLdrAverage() {
  uint32_t sum = 0;
  for (uint16_t i = 0; i < READ_SAMPLES; i++) {
    sum += analogRead(PIN_LDR);
    delay(1);
  }
  return (uint16_t)(sum / READ_SAMPLES);
}

uint16_t mapClamped(uint16_t x, uint16_t inMin, uint16_t inMax, uint16_t outMin, uint16_t outMax) {
  if (x <= inMin) return outMin;
  if (x >= inMax) return outMax;
  // Arduino 的 map() 使用 long，避免溢出即可
  long v = (long)(x - inMin) * (long)(outMax - outMin) / (long)(inMax - inMin) + outMin;
  if (v < outMin) v = outMin;
  if (v > outMax) v = outMax;
  return (uint16_t)v;
}

void updateDisplayAndLed(uint16_t mappedVal0to100) {
  uint16_t pwm = mapClamped(mappedVal0to100, MAP_OUT_MIN, MAP_OUT_MAX, PWM_OUT_MIN, PWM_OUT_MAX);
  analogWrite(PIN_PWM, pwm);

  // 显示当前映射值（0~100），用4位直接显示整数
  display.showNumberDec(mappedVal0to100, false, 4, 0);
}

void setup() {
  pinMode(PIN_PWM, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP); // D2 使用上拉：按下=LOW
  pinMode(13, OUTPUT); // 题面含 D13 设备角色，但不影响主要行为；保留其作为输出

  display.setBrightness(0x0f); // 亮度最大（不改变映射逻辑本身）

  // 初始化：先按自动模式显示当前光照映射
  uint16_t ldr = readLdrAverage();
  uint16_t mapped = mapClamped(ldr, MAP_IN_MIN, MAP_IN_MAX, MAP_OUT_MIN, MAP_OUT_MAX);

  manualFrozen = false;
  frozenValue = mapped;

  updateDisplayAndLed(mapped);
}

void loop() {
  // 1) 读取并处理按键：按一次切换 手动冻结 <-> 自动模式
  bool reading = digitalRead(PIN_BUTTON);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != lastButtonStable) {
      lastButtonStable = reading;

      // 下降沿（从HIGH到LOW）视为“按下”
      if (lastButtonStable == LOW) {
        manualFrozen = !manualFrozen;

        // 若切换到手动冻结：把当前自动计算值“冻结”
        if (manualFrozen) {
          uint16_t ldr = readLdrAverage();
          frozenValue = mapClamped(ldr, MAP_IN_MIN, MAP_IN_MAX, MAP_OUT_MIN, MAP_OUT_MAX);
          updateDisplayAndLed(frozenValue);
        } else {
          // 切回自动：立即更新一次（避免显示滞后）
          uint16_t ldr = readLdrAverage();
          uint16_t mapped = mapClamped(ldr, MAP_IN_MIN, MAP_IN_MAX, MAP_OUT_MIN, MAP_OUT_MAX);
          frozenValue = mapped;
          updateDisplayAndLed(mapped);
        }
      }
    }
  }

  // 2) 自动/手动模式下的行为
  if (!manualFrozen) {
    uint16_t ldr = readLdrAverage();
    uint16_t mapped = mapClamped(ldr, MAP_IN_MIN, MAP_IN_MAX, MAP_OUT_MIN, MAP_OUT_MAX);
    frozenValue = mapped; // 仅用于后续冻结时保持最近值一致性
    updateDisplayAndLed(mapped);
  } else {
    // 手动冻结：保持冻结值不变，只刷新一次即可，但为了确保时序一致性，低频刷新
    static unsigned long lastHoldUpdate = 0;
    const unsigned long HOLD_REFRESH_MS = 100;
    if (millis() - lastHoldUpdate >= HOLD_REFRESH_MS) {
      lastHoldUpdate = millis();
      updateDisplayAndLed(frozenValue);
    }
  }

  // 可选：保持 D13 不干扰（无明确需求，保持不变）
  digitalWrite(13, manualFrozen ? HIGH : LOW);

  delay(20);
}

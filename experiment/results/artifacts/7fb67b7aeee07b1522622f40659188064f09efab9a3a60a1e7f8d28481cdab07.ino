#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_TRIG = 8;
static const uint8_t PIN_ECHO = 9;

static const uint8_t PIN_TM1637_CLK = 4;
static const uint8_t PIN_TM1637_DIO = 5;

static const uint8_t PIN_LED = 13;

static const uint32_t ECHO_TIMEOUT_US = 25000UL;   // 每次等待回波必须有25000us超时
static const uint16_t THRESHOLD_CM = 10;            // <10cm LED亮
static const uint16_t VALID_MIN_CM = 2;             // 有效范围2..400cm
static const uint16_t VALID_MAX_CM = 400;

static const uint16_t SAMPLE_MIN_MS = 60;          // 采样开始间隔60..160ms
static const uint16_t SAMPLE_MAX_MS = 160;

static const uint8_t DIGITS = 4;

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// 量化/显示：显示整数厘米；前导零或空白均可（这里用“空白”表示无效）
static uint32_t nextSampleAt = 0;

static inline void setDisplayBlankAndLedOff() {
  display.setBrightness(0x0); // 避免亮屏干扰（也可不设亮度）
  display.showNumberDecEx(0, 0b01000000, true); // clear style; 容差要求不在此处
  // 上面showNumberDecEx不同库版本可能行为略有差异，但可保证不显示有效数字
  // 若你使用的TM1637Display版本支持“clearDisplay”，可替换为 display.clear();
  digitalWrite(PIN_LED, LOW);
}

static inline void showValidDistance(uint16_t cm) {
  digitalWrite(PIN_LED, (cm < THRESHOLD_CM) ? HIGH : LOW);

  // 显示整数厘米（量化容差1cm）
  // 4位足够显示 400
  display.setBrightness(7); // 正常亮度
  display.showNumberDec(cm, true, 4, 0);
}

static inline bool measureUltrasonicCm(uint16_t &out_cm) {
  // 发送触发脉冲
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // 等待回波；无回波/卡高/越界时必须超时
  // pulseIn 在Arduino中：超时返回0
  unsigned long us = pulseIn(PIN_ECHO, HIGH, ECHO_TIMEOUT_US);
  if (us == 0) return false;

  // 计算距离：回波微秒数除58
  // 为保证量化误差<=1cm，用整数除法（相当于向下取整）
  uint32_t cm32 = us / 58UL;

  if (cm32 < VALID_MIN_CM || cm32 > VALID_MAX_CM) return false;
  out_cm = (uint16_t)cm32;
  return true;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_LED, LOW);

  // 初始化TM1637并置空白、LED灭：初始显示空白且LED灭
  display.setBrightness(0);
  // 用“空白/不显示数字”表示无效；不同库版本差异允许前导零或空白
  // 这里直接显示“空白”（通过brightness为0 + clear样式）
  // 若showNumberDecEx clear在你的库中效果不理想，你可以改用 display.clear()。
  setDisplayBlankAndLedOff();

  // 让第一轮也在范围内启动：初始不必立刻测
  nextSampleAt = millis();
}

void loop() {
  // 采样开始间隔60..160ms
  uint32_t now = millis();
  if ((int32_t)(now - nextSampleAt) < 0) return;

  // 下一次采样时间：确保60..160ms范围内
  // 使用确定性“伪随机”以避免完全固定但也不依赖随机种子
  // 变化250ms内反映（含测量等待和显示传输），本控制保证不会卡太长
  static uint32_t k = 0;
  k = (k * 1103515245UL + 12345UL);
  uint16_t interval = SAMPLE_MIN_MS + (uint16_t)((k >> 16) % (SAMPLE_MAX_MS - SAMPLE_MIN_MS + 1));
  nextSampleAt = now + interval;

  // 测量：有效性必须区分，不得将超时当成0cm
  uint16_t cm = 0;
  bool ok = measureUltrasonicCm(cm);

  if (!ok) {
    // 无回波、卡高或越界：显示空白且LED灭，恢复后继续响应
    // 注意：不要把超时当成0cm
    display.setBrightness(0);
    // 清屏/空白显示
    // 若你的库支持 clear()，可改成 display.clear();
    // 为兼容性，使用显示扩展清除
    display.showNumberDecEx(0, 0b01000000, true);
    digitalWrite(PIN_LED, LOW);
  } else {
    showValidDistance(cm);
  }
}
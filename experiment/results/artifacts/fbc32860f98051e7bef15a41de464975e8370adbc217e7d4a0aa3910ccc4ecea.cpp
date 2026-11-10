#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t POT_PIN = A0;     // 电位器（A0）
const uint8_t LED_PIN = 9;      // LED（D9）

// TM1637: CLK=D4, DIO=D5
const uint8_t TM1637_CLK = 4;
const uint8_t TM1637_DIO = 5;

TM1637Display display(TM1637_CLK, TM1637_DIO);

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t clampU16(int32_t v, uint16_t lo, uint16_t hi);
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t clampU16(int32_t v, uint16_t lo, uint16_t hi) {
  if (v < (int32_t)lo) return lo;
  if (v > (int32_t)hi) return hi;
  return (uint16_t)v;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogReference(DEFAULT);

  display.setBrightness(0x0f, false); // initial: full brightness (auto overridden by loop)
  display.clearDisplay();
}

void loop() {
  // 读取电位器
  int raw = analogRead(POT_PIN); // 0..1023

  // 映射到 LED PWM 与 TM1637 亮度
  // LED PWM: 0..255
  uint8_t ledPwm = (uint32_t)raw * 255UL / 1023UL;
  analogWrite(LED_PIN, ledPwm);

  // TM1637 亮度: 0..7（常见范围为 0..7；库接口多为 0..7）
  uint8_t tmBright = (uint32_t)raw * 7UL / 1023UL;

  // 更新 TM1637 亮度
  display.setBrightness(tmBright);

  // 显示当前亮度数值（0-99）作为“控制亮度”的可见反馈
  // raw -> 0..99
  uint8_t percent = (uint32_t)raw * 99UL / 1023UL;
  uint8_t tens = percent / 10;
  uint8_t ones = percent % 10;

  // 显示两位：tens|ones（数码管为4位，这里用最高两位/或直接双位显示）
  // TM1637Display::showNumberDecEx 支持前导空白；这里用两位显示：XX
  display.showNumberDecEx((int)percent, 0b01000000, true);
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t POT_PIN = A0;   // 电位器（A0）
const uint8_t LED_PIN = 9;    // LED（D9）

// TM1637: CLK=D4, DIO=D5（按已解析接线）
const uint8_t TM1637_CLK = 4;
const uint8_t TM1637_DIO = 5;

TM1637Display display(TM1637_CLK, TM1637_DIO);

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint16_t clampU16(int32_t v, uint16_t lo, uint16_t hi);
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogReference(DEFAULT);

  // 初始化：库的亮度设置；随后 loop() 会根据电位器刷新
  display.setBrightness(0x0f, false);
}

static uint16_t clampU16(int32_t v, uint16_t lo, uint16_t hi) {
  if (v < (int32_t)lo) return lo;
  if (v > (int32_t)hi) return hi;
  return (uint16_t)v;
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
  display.setBrightness(tmBright);

  // 显示当前亮度数值（0-99）作为“控制亮度”的可见反馈
  uint8_t percent = (uint32_t)raw * 99UL / 1023UL;
  // 用两位显示：tens|ones
  display.showNumberDecEx((int)percent, 0b01000000, true);
}

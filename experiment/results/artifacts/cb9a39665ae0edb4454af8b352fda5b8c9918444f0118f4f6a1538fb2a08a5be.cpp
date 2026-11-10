#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <TM1637Display.h>

// TM1637 四位数码管：CLK=D4，DIO=D5
// Arduino UNO: D4=4, D5=5
const uint8_t TM1637_CLK_PIN = 4;
const uint8_t TM1637_DIO_PIN = 5;

TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // 初始化显示器：使用默认段码/位点，亮度由库内部默认值决定
  display.setBrightness(0x0f); // 0..7 通常可用；此处取最大以确保可见
}

void loop() {
  static uint16_t n = 0; // 0..9999 循环
  static unsigned long lastMs = 0;

  unsigned long now = millis();
  if (now - lastMs >= 1000) {
    lastMs = now;

    // 每秒递增显示一个数字（四位数码管：0..9999循环）
    n = (n + 1) % 10000;

    // 显示 n
    display.showNumberDec(n, false); // false: 不要前导零
  }
}

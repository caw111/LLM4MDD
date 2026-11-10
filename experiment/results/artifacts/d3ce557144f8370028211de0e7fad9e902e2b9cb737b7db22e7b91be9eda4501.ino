#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments (UNO)
static const uint8_t PIN_TRIG = 8;   // HC-SR04 TRIG = D8
static const uint8_t PIN_ECHO = 9;   // HC-SR04 ECHO = D9
static const uint8_t PIN_CLK  = 4;   // TM1637 CLK = D4
static const uint8_t PIN_DIO  = 5;   // TM1637 DIO = D5
static const uint8_t PIN_LED  = 13;  // LED D13, HIGH = ON

TM1637Display display(PIN_CLK, PIN_DIO);

// 每次等待回波必须有25000us超时
static const unsigned long ECHO_TIMEOUT_US = 25000UL;

// 有效范围 2..400cm（基于测量距离，不是显示取整值）
static const float VALID_CM_MIN = 2.0f;
static const float VALID_CM_MAX = 400.0f;

// LED阈值：测量距离严格小于10cm => LED亮；>=10cm => LED灭
static const float LED_THRESHOLD_CM = 10.0f;

// 采样开始间隔 60..160ms；变化250ms内反映（测量等待+显示传输都算在内）
static const unsigned long START_INTERVAL_MS_MIN = 60;
static const unsigned long START_INTERVAL_MS_MAX = 160;

// 显示：4位数（整数厘米），前导零或空白均可 -> 这里用“左对齐并允许前导零关闭”的方式显示整数
static void showBlank() {
  uint8_t blankSegs[4] = {0x00, 0x00, 0x00, 0x00};
  display.setSegments(blankSegs);
}

static inline void triggerUltrasonic() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
}

// 返回：是否成功获得一次回波脉宽；echoTimeUs为回波高电平持续时间(微秒)
static bool measureEchoTimeUS(unsigned long &echoTimeUs) {
  // 等待回波起始（ECHO由LOW->HIGH）
  const unsigned long t0 = micros();
  while (digitalRead(PIN_ECHO) == LOW) {
    if ((micros() - t0) >= ECHO_TIMEOUT_US) return false; // 无回波或等待超时
  }

  // 测量回波宽度（ECHO保持HIGH）
  const unsigned long m0 = micros();
  while (digitalRead(PIN_ECHO) == HIGH) {
    if ((micros() - m0) >= ECHO_TIMEOUT_US) return false; // 卡高或测量超时
  }

  echoTimeUs = micros() - m0;
  return true;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_LED, LOW); // 初始 LED 灭

  display.setBrightness(0x0f);

  // 初始显示为空白
  showBlank();

  // 随机间隔种子
  randomSeed(analogRead(A0));
}

void loop() {
  static unsigned long nextStartMs = 0;
  static bool first = true;

  if (first) {
    nextStartMs = millis();
    first = false;
  }

  const unsigned long nowMs = millis();
  if ((long)(nowMs - nextStartMs) >= 0) {
    // 本次测量开始后，安排下一次开始间隔（60..160ms）
    const unsigned long interval =
      (unsigned long)random(START_INTERVAL_MS_MIN, START_INTERVAL_MS_MAX + 1);
    nextStartMs = nowMs + interval;

    // 默认：无效则空白且LED灭；有效则按规则显示与判定
    digitalWrite(PIN_LED, LOW);
    showBlank();

    triggerUltrasonic();

    unsigned long echoTimeUs = 0;
    const bool ok = measureEchoTimeUS(echoTimeUs);

    if (!ok) {
      // 无回波/卡高/越界：显示空白且LED灭；恢复后继续响应
      showBlank();
      digitalWrite(PIN_LED, LOW);
      return;
    }

    // 回波微秒数除58 => 距离(单位cm)
    const float distCmMeasured = (float)echoTimeUs / 58.0f;

    // 有效范围严格：2..400cm（使用测量距离判断）
    const bool valid = (distCmMeasured >= VALID_CM_MIN) && (distCmMeasured <= VALID_CM_MAX);
    if (!valid) {
      showBlank();
      digitalWrite(PIN_LED, LOW);
      return;
    }

    // LED判定：严格小于10cm亮；>=10cm灭（使用测量距离判断，不用显示取整）
    if (distCmMeasured < LED_THRESHOLD_CM) digitalWrite(PIN_LED, HIGH);
    else digitalWrite(PIN_LED, LOW);

    // 显示：整数厘米（量化容差1cm；前导零或空白均可）
    // 采用四舍五入到最近整数
    long distCmInt = (long)lround(distCmMeasured);

    // 数值显示范围兜底：超出则按无效处理（显示空白/LED灭）
    if (distCmInt < 0 || distCmInt > 9999) {
      showBlank();
      digitalWrite(PIN_LED, LOW);
      return;
    }

    display.showNumberDecEx((int)distCmInt, 0, true); // 允许前导零关闭/右对齐取决于库实现
  }
}
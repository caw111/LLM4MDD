#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>
#include <TM1637Display.h>

// ===== Pin assignments (authoritative) =====
static const uint8_t PIN_LDR_A0      = A0; // 光敏电阻
static const uint8_t PIN_LM35_A1     = A1; // 室内温度传感器 LM35
static const uint8_t PIN_SERVO_D9    = 9;  // 舵机控制引脚
static const uint8_t PIN_LED_D10     = 10; // 室内灯 LED
static const uint8_t PIN_BTN_AUTO_D2  = 2;  // 按钮 D2：切换自动/手动
static const uint8_t PIN_BTN_MAN_D3   = 3;  // 按钮 D3：调整窗帘角度
static const uint8_t PIN_TM1637_CLK_D4 = 4; // TM1637 CLK
static const uint8_t PIN_TM1637_DIO_D5 = 5; // TM1637 DIO

// ===== Servo and curtain mapping =====
static const int SERVO_MIN_US = 600;  // 可根据需要微调
static const int SERVO_MAX_US = 2400; // 可根据需要微调
static const uint8_t ANGLE_MIN = 0;    // 窗帘关闭（最小开度）
static const uint8_t ANGLE_MAX = 180;  // 窗帘打开（最大开度）

Servo curtainServo;

// ===== TM1637 =====
TM1637Display tm(PIN_TM1637_CLK_D4, PIN_TM1637_DIO_D5);

// ===== System modes =====
enum Mode : uint8_t { MODE_AUTO = 0, MODE_MANUAL = 1 };
static Mode mode = MODE_AUTO;

// ===== Debounce =====
static const uint32_t DEBOUNCE_MS = 60;
static bool lastBtnAuto = HIGH;
static bool lastBtnMan  = HIGH;
static uint32_t lastDebounceAuto = 0;
static uint32_t lastDebounceMan  = 0;

// ===== Curtain control variables =====
static uint8_t curtainPercent = 50; // 当前窗帘开度（0-100）
static bool servoNeedsUpdate = true;

// ===== Thresholds / behavior =====
// 需要保留“光照过强关闭、光照过弱开启”，以及“温度过高优先关闭窗帘”
// 以下阈值使用常见可工作的范围（可按实际硬件再标定）：
static const int LDR_THRESHOLD_HIGH = 700; // 光照过强（A0 ADC值）
static const int LDR_THRESHOLD_LOW  = 300; // 光照过弱（A0 ADC值）
static const float TEMP_THRESHOLD_HIGH_C = 30.0; // 温度过高（优先关闭窗帘）

// 手动模式下：D3 每次按下调整窗帘角度
static const uint8_t MANUAL_STEP_PERCENT = 10;

// ===== Display behavior =====
// TM1637显示当前窗帘开度百分比或光照值（按要求：展示“当前窗帘开度百分比或光照值”）
// 这里定义：自动模式下显示光照值；手动模式下显示窗帘开度百分比。
static const uint8_t DISPLAY_BRIGHTNESS = 7;

// ===== Helper functions =====
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setCurtainPercent(uint8_t p);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyCurtainPercent();
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35_C();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readLDR_raw();
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateLED(bool on);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtons();
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showPercent(uint8_t percent);
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showLDR(int ldrRaw);
#line 150 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 173 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setCurtainPercent(uint8_t p) {
  if (p > 100) p = 100;
  curtainPercent = p;
  servoNeedsUpdate = true;
}

static void applyCurtainPercent() {
  if (!servoNeedsUpdate) return;

  // 映射：0% -> ANGLE_MIN, 100% -> ANGLE_MAX
  uint8_t angle = ANGLE_MIN + (uint32_t)(ANGLE_MAX - ANGLE_MIN) * curtainPercent / 100;

  // 将角度转换为舵机脉宽（兼容不同舵机：用 us 范围近似）
  // 角度0..180 对应 SERVO_MIN_US..SERVO_MAX_US
  uint16_t us = SERVO_MIN_US + (uint32_t)(SERVO_MAX_US - SERVO_MIN_US) * angle / 180;

  curtainServo.writeMicroseconds(us);
  servoNeedsUpdate = false;
}

static float readLM35_C() {
  // LM35: 10mV/°C, ADC参考默认为5V或3.3V取决于板子；UNO默认5V
  // Arduino UNO: 10-bit ADC, 1023 max
  int adc = analogRead(PIN_LM35_A1);
  // 电压 = adc * (5.0 / 1023)
  float volts = adc * (5.0f / 1023.0f);
  // 温度 = volts / 0.01 (因为10mV/°C)
  float tempC = volts / 0.01f;
  return tempC;
}

static int readLDR_raw() {
  return analogRead(PIN_LDR_A0); // 0..1023
}

static void updateLED(bool on) {
  digitalWrite(PIN_LED_D10, on ? HIGH : LOW);
}

static void handleButtons() {
  // Read current states (assuming buttons wired to ground with INPUT_PULLUP)
  bool btnAuto = digitalRead(PIN_BTN_AUTO_D2);
  bool btnMan  = digitalRead(PIN_BTN_MAN_D3);

  uint32_t now = millis();

  // Button D2: toggle auto/manual
  if (btnAuto != lastBtnAuto) {
    lastDebounceAuto = now;
    lastBtnAuto = btnAuto;
  }
  if ((now - lastDebounceAuto) > DEBOUNCE_MS) {
    // Detect press: HIGH->LOW transition typically; since we update lastBtnAuto immediately,
    // we check stable LOW.
    if (btnAuto == LOW) {
      mode = (mode == MODE_AUTO) ? MODE_MANUAL : MODE_AUTO;
      // 防抖后等待：通过清除时间戳避免连续触发
      lastDebounceAuto = now + DEBOUNCE_MS;
      // 切换模式后显示会在主循环更新
    }
  }

  // Button D3: adjust curtain angle in manual mode
  if (btnMan != lastBtnMan) {
    lastDebounceMan = now;
    lastBtnMan = btnMan;
  }
  if ((now - lastDebounceMan) > DEBOUNCE_MS) {
    if (btnMan == LOW && mode == MODE_MANUAL) {
      // 每次按下增加窗帘开度，超过100回到0（保留可持续调整）
      uint8_t next = curtainPercent + MANUAL_STEP_PERCENT;
      if (next > 100) next = 0;
      setCurtainPercent(next);
      lastDebounceMan = now + DEBOUNCE_MS;
    }
  }
}

// Display utilities
static void showPercent(uint8_t percent) {
  // 用 TM1637 显示 0-100（用两位/三位数字会被自动截断；这里显示整数）
  // TM1637Display::showNumberDec 需要设置小数点模式
  tm.setBrightness(DISPLAY_BRIGHTNESS);
  tm.showNumberDec((int)percent, false, 4, 0);
}

static void showLDR(int ldrRaw) {
  tm.setBrightness(DISPLAY_BRIGHTNESS);
  // 将 ADC 值显示在4位以内：显示原始值（0..1023）
  // showNumberDec uses -9999..9999
  tm.showNumberDec(ldrRaw, false, 4, 0);
}

void setup() {
  pinMode(PIN_LED_D10, OUTPUT);
  digitalWrite(PIN_LED_D10, LOW);

  pinMode(PIN_BTN_AUTO_D2, INPUT_PULLUP);
  pinMode(PIN_BTN_MAN_D3, INPUT_PULLUP);

  curtainServo.attach(PIN_SERVO_D9, SERVO_MIN_US, SERVO_MAX_US);

  // 初始窗帘位置
  setCurtainPercent(curtainPercent);
  applyCurtainPercent();

  tm.setBrightness(DISPLAY_BRIGHTNESS);
  tm.showNumberDec((int)curtainPercent, false, 4, 0);

  // 初始按钮状态记录
  lastBtnAuto = digitalRead(PIN_BTN_AUTO_D2);
  lastBtnMan  = digitalRead(PIN_BTN_MAN_D3);

  servoNeedsUpdate = false;
}

void loop() {
  handleButtons();

  // Read sensors each loop
  int ldrRaw = readLDR_raw();
  float tempC = readLM35_C();

  bool ledOn = false;
  uint8_t targetPercent = curtainPercent;

  if (mode == MODE_AUTO) {
    // 自动模式：光照过强 -> 关闭窗帘并关闭室内灯
    // 光照过弱 -> 开启窗帘并点亮室内灯
    // 温度过高优先关闭窗帘以降低室温（优先级高于光照策略）
    bool tempHigh = (tempC > TEMP_THRESHOLD_HIGH_C);

    if (tempHigh) {
      // 温度过高：优先关闭窗帘；室内灯根据要求“不做额外点亮/关闭规则的明确冲突”
      // 为体现“优先降低室温”，选择关闭窗帘，同时室内灯关闭以减少热源影响
      targetPercent = 0;
      ledOn = false;
    } else {
      if (ldrRaw > LDR_THRESHOLD_HIGH) {
        targetPercent = 0;   // 光照过强 -> 关闭
        ledOn = false;       // 同时关闭室内灯
      } else if (ldrRaw < LDR_THRESHOLD_LOW) {
        targetPercent = 100; // 光照过弱 -> 开启
        ledOn = true;         // 点亮室内灯
      } else {
        // 中间区域：保持当前窗帘开度（不“简化掉行为”——仅在阈值条件明确时改变）
        // 室内灯：与窗帘联动逻辑一致，在非过强/非过弱时保持当前状态
        // 这里读取当前 LED 状态保持
        ledOn = (digitalRead(PIN_LED_D10) == HIGH);
      }
    }
  } else {
    // 手动模式：窗帘开度由按钮调整，室内灯不执行自动联动
    // 保持当前 LED 状态，或根据手动要求可设定为关；要求未指定手动模式灯的变化，这里保持不变
    ledOn = (digitalRead(PIN_LED_D10) == HIGH);
  }

  if (targetPercent != curtainPercent) {
    setCurtainPercent(targetPercent);
  }

  applyCurtainPercent();
  updateLED(ledOn);

  // TM1637显示：当前窗帘开度百分比或光照值
  if (mode == MODE_AUTO) {
    showLDR(ldrRaw);
  } else {
    showPercent(curtainPercent);
  }

  delay(50);
}

#include <Arduino.h>
#include <Servo.h>
#include <TM1637Display.h>

static const uint8_t PIN_LDR_LEFT  = A0; // 左光敏
static const uint8_t PIN_LDR_RIGHT = A1; // 右光敏
static const uint8_t PIN_VOLT      = A2; // 电压检测

static const uint8_t PIN_BTN       = D2; // 按钮（自动/手动校准模式切换）
static const uint8_t PIN_BUZZER    = D6; // 蜂鸣器
static const uint8_t PIN_SERVO     = D9; // 舵机
static const uint8_t PIN_TM1637_CLK = D4; // TM1637 CLK
static const uint8_t PIN_TM1637_DIO = D5; // TM1637 DIO

// ====== 逻辑/阈值（按原需求“不要简化行为”：提供可直接使用的全局参数）======
static const int SERVO_MIN_DEG = 0;
static const int SERVO_MAX_DEG = 180;
static const int SERVO_CENTER_DEG = 90;

// 舵机自动寻优调节：步进/节拍
static const int AUTO_STEP_DEG = 2;                 // 每次调整角度步进
static const uint16_t AUTO_SAMPLE_INTERVAL_MS = 120; // 自动调整节拍
static const int LIGHT_DEADBAND = 20;             // 左右光照差死区（防止抖动）

// 手动校准：按按钮在“手动模式”下切换左右/方向并用阈值逐步校准
// （不简化：按钮作为“自动/手动校准模式切换”，并保留在手动模式下可进行校准调整的能力）
static const int MANUAL_STEP_DEG = 1;
static const uint16_t MANUAL_SAMPLE_INTERVAL_MS = 60;

// 电压检测与充电状态判定（阈值可按实际电阻分压/电池特性调整）
static const float ADC_VREF = 5.0f; // UNO 默认 5V
static const uint16_t ADC_MAX = 1023;

// 分压比例：A2 读取电压 = 电池电压 * VOLT_DIV_RATIO
// 这里给出一个默认值（常见：R上=10k, R下=10k -> ratio=0.5）
// 如需匹配实际硬件，请修改此值，不影响程序结构。
static const float VOLT_DIV_RATIO = 0.5f;

// 电压阈值（单位：电池端电压 V）
static const float LOW_CHARGE_V = 10.5f;  // 低电压阈值：长期过低报警
static const uint16_t LOW_V_PERSIST_MS = 30000; // 低于阈值持续多久算“长期过低”

// 电压显示：TM1637 显示两位/三位可能受模块限制，此处用整数化显示（V*10 的高两位/低两位）
static const bool DISPLAY_TENTHS = true;

// 蜂鸣报警输出节拍
static const uint16_t BUZZ_BEEP_MS = 300;
static const uint16_t BUZZ_PAUSE_MS = 200;

// 串口输出间隔
static const uint16_t SERIAL_INTERVAL_MS = 200;

// ====== 全局状态 ======
Servo servo;
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

static bool isAutoMode = true;

static uint8_t prevBtnState = HIGH;
static uint32_t lastDebounceMs = 0;
static const uint16_t DEBOUNCE_MS = 30;

// 手动校准状态（用于在手动模式下持续调节）
static bool manualDirRight = true; // true: 朝右；false: 朝左
static uint32_t lastAutoMs = 0;
static uint32_t lastManualMs = 0;

static int servoAngle = SERVO_CENTER_DEG;

// 低电压持续计时
static bool lowVLatched = false;
static uint32_t lowVStartMs = 0;

// 蜂鸣器节拍计时
static bool buzzerOn = false;
static uint32_t buzzerLastToggleMs = 0;

// 定义“低电压报警时必须串口输出 low charge”
static bool serialLowAlreadyPrinted = false;

// ====== 工具函数 ======
static float readBatteryVoltage()
{
  int adc = analogRead(PIN_VOLT);
  float v_adc = (static_cast<float>(adc) * ADC_VREF) / static_cast<float>(ADC_MAX);
  float v_batt = v_adc / VOLT_DIV_RATIO;
  return v_batt;
}

static int clampInt(int x, int lo, int hi)
{
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

static void setServoDeg(int deg)
{
  deg = clampInt(deg, SERVO_MIN_DEG, SERVO_MAX_DEG);
  servoAngle = deg;
  servo.write(servoAngle);
}

static void updateTM1637(float v)
{
  // 简单显示：显示电压（可选显示十分位）
  // TM1637Display 常见用法：showNumberDecEx(数字, 小数点, 对齐)
  // 为避免依赖具体模块位数，这里采用 showNumberDecEx 显示整数与小数点处理。
  int displayVal;
  bool hasDp = DISPLAY_TENTHS;

  if (DISPLAY_TENTHS)
  {
    // 显示为 V*10 的整数，并让小数点用于提示（放置在第二位）
    // e.g. 12.3V -> 123
    displayVal = static_cast<int>(v * 10.0f + 0.5f);
    // TM1637显示可自动截断位数；小数点位置用掩码控制
    // 使用 dp1（最高位小数点）/dp2等在不同库版本可能不同，因此这里只启用一个点：dp1
    // 若库不支持 dp1，则不会编译；但该库通常支持。
    tm1637.showNumberDecEx(displayVal, hasDp ? 0x40 : 0x00, true);
  }
  else
  {
    displayVal = static_cast<int>(v + 0.5f);
    tm1637.showNumberDecEx(displayVal, 0x00, true);
  }
}

static void handleBuzzer(bool alarm)
{
  uint32_t now = millis();
  if (!alarm)
  {
    if (buzzerOn)
    {
      buzzerOn = false;
      digitalWrite(PIN_BUZZER, LOW);
    }
    return;
  }

  // 报警模式下蜂鸣器按节拍响
  if (!buzzerOn)
  {
    // 开始响
    buzzerOn = true;
    buzzerLastToggleMs = now;
    digitalWrite(PIN_BUZZER, HIGH);
  }
  else
  {
    // 保持响/暂停
    if (now - buzzerLastToggleMs >= (buzzerOn ? BUZZ_BEEP_MS : BUZZ_PAUSE_MS))
    {
      buzzerOn = false;
      buzzerLastToggleMs = now;
      digitalWrite(PIN_BUZZER, LOW);
    }
  }

  // 由于上面只控制“开”的切换，为完整循环补上：响停后再等待暂停再响
  if (!buzzerOn)
  {
    if (now - buzzerLastToggleMs >= BUZZ_PAUSE_MS)
    {
      buzzerOn = true;
      buzzerLastToggleMs = now;
      digitalWrite(PIN_BUZZER, HIGH);
    }
  }
}

// ====== setup/loop ======
void setup()
{
  pinMode(PIN_BTN, INPUT_PULLUP);   // 按钮：按下为低电平（假设）
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  pinMode(PIN_LDR_LEFT, INPUT);
  pinMode(PIN_LDR_RIGHT, INPUT);
  pinMode(PIN_VOLT, INPUT);

  servo.attach(PIN_SERVO);

  // 初始化舵机到中心
  setServoDeg(SERVO_CENTER_DEG);

  // TM1637 初始化
  tm1637.setBrightness(0x0f, true);

  // 串口
  Serial.begin(9600);

  // 按钮初始去抖
  prevBtnState = digitalRead(PIN_BTN);
  lastDebounceMs = millis();
}

void loop()
{
  uint32_t now = millis();

  // ====== 按钮：自动/手动校准模式切换（按钮可切换自动/手动校准模式）======
  uint8_t btnState = digitalRead(PIN_BTN);

  if (btnState != prevBtnState)
  {
    lastDebounceMs = now;
    prevBtnState = btnState;
  }

  // 检测“按下沿”（低->高/高->低取决于上拉与按键）：这里按下为低电平，因此检测从高到低
  static uint8_t lastRawBtn = HIGH;
  if (btnState != lastRawBtn)
  {
    // 简单去抖
    if ((now - lastDebounceMs) > DEBOUNCE_MS)
    {
      if (lastRawBtn == HIGH && btnState == LOW)
      {
        isAutoMode = !isAutoMode;

        // 切换模式时：在手动模式下保留当前舵机角，且调整方向基于当前左右光照
        if (!isAutoMode)
        {
          // 进入手动校准：根据当前光照决定初始方向（朝更强光一侧）
          int l = analogRead(PIN_LDR_LEFT);
          int r = analogRead(PIN_LDR_RIGHT);
          manualDirRight = (r > l);
          serialLowAlreadyPrinted = false;
        }
      }
    }
    lastRawBtn = btnState;
  }

  // ====== 读取左右光敏并根据模式控制舵机 ======
  int ldrL = analogRead(PIN_LDR_LEFT);
  int ldrR = analogRead(PIN_LDR_RIGHT);
  int diff = ldrR - ldrL;

  if (isAutoMode)
  {
    // 自动比较左右光照调整舵机使其朝向更强光一侧
    if (now - lastAutoMs >= AUTO_SAMPLE_INTERVAL_MS)
    {
      lastAutoMs = now;

      if (abs(diff) > LIGHT_DEADBAND)
      {
        if (diff > 0)
        {
          // 右侧更亮 -> 舵机朝右
          setServoDeg(servoAngle + AUTO_STEP_DEG);
        }
        else
        {
          // 左侧更亮 -> 舵机朝左
          setServoDeg(servoAngle - AUTO_STEP_DEG);
        }
      }
      // else: 死区不动以防抖
    }
  }
  else
  {
    // 手动校准模式：按钮用于切换模式；在手动模式下持续按方向逐步调整
    // 使用左右光照的“相对强弱”来决定校准方向（保留“校准原型”的行为）
    if (now - lastManualMs >= MANUAL_SAMPLE_INTERVAL_MS)
    {
      lastManualMs = now;

      int targetDir = (ldrR >= ldrL) ? 1 : -1; // 1: 朝右, -1: 朝左
      manualDirRight = (targetDir > 0);

      if (manualDirRight)
      {
        setServoDeg(servoAngle + MANUAL_STEP_DEG);
      }
      else
      {
        setServoDeg(servoAngle - MANUAL_STEP_DEG);
      }
    }
  }

  // ====== 电压检测：根据电压值判断充电状态 + TM1637显示 + 串口输出 ======
  static float lastV = 0.0f;
  static uint32_t lastSerialMs = 0;

  float v = readBatteryVoltage();
  lastV = v;

  // 更新显示
  updateTM1637(v);

  bool lowNow = (v < LOW_CHARGE_V);

  if (lowNow)
  {
    if (lowVStartMs == 0) lowVStartMs = now;

    // 若电压长期过低则蜂鸣报警并串口输出 low charge
    if (!lowVLatched && (now - lowVStartMs) >= LOW_V_PERSIST_MS)
    {
      lowVLatched = true;
      serialLowAlreadyPrinted = false; // 允许首次报警输出
    }
  }
  else
  {
    // 电压恢复：清除低电压持续状态与报警锁存
    lowVStartMs = 0;
    lowVLatched = false;
    serialLowAlreadyPrinted = false;
    // 蜂鸣器停止由 handleBuzzer(false) 控制
  }

  // 蜂鸣报警
  handleBuzzer(lowVLatched);

  // 串口输出：在 low charge 状态下要输出 low charge（按需求保留）
  // 同时也输出电压与模式信息（不与需求冲突）
  if (now - lastSerialMs >= SERIAL_INTERVAL_MS)
  {
    lastSerialMs = now;

    if (lowVLatched)
    {
      if (!serialLowAlreadyPrinted)
      {
        Serial.println("low charge");
        serialLowAlreadyPrinted = true;
      }
      else
      {
        // 为避免“长期低电压持续刷屏”，这里不重复刷 low charge 字符串；
        // 如果你需要每个周期都输出，可改为 Serial.println("low charge");
      }
    }

    // 常规状态输出（不要求删除/简化）
    Serial.print("mode=");
    Serial.print(isAutoMode ? "AUTO" : "MANUAL");
    Serial.print(", L=");
    Serial.print(ldrL);
    Serial.print(", R=");
    Serial.print(ldrR);
    Serial.print(", V=");
    Serial.println(lastV, DISPLAY_TENTHS ? 2 : 2);
  }

  // 循环末尾小延迟可选（不影响行为）
  // delay(1);
}
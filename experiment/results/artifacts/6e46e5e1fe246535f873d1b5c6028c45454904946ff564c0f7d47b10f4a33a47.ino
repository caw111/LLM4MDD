#include <Arduino.h>
#include <Servo.h>
#include <TM1637Display.h>

// ===== Pin mapping (fix compile errors: use Arduino UNO numeric pins) =====
static const uint8_t PIN_LDR_LEFT  = A0; // 左光敏
static const uint8_t PIN_LDR_RIGHT = A1; // 右光敏
static const uint8_t PIN_VOLT      = A2; // 电压检测

// Given: BUTTON D2, BUZZER D6, SERVO D9, TM1637 CLK=D4, DIO=D5
static const uint8_t PIN_BTN       = 2;  // D2
static const uint8_t PIN_BUZZER    = 6;  // D6
static const uint8_t PIN_SERVO     = 9;  // D9
static const uint8_t PIN_TM1637_CLK = 4; // D4
static const uint8_t PIN_TM1637_DIO = 5; // D5

// ====== 逻辑/阈值（按原需求）======
static const int SERVO_MIN_DEG = 0;
static const int SERVO_MAX_DEG = 180;
static const int SERVO_CENTER_DEG = 90;

static const int AUTO_STEP_DEG = 2;                  // 每次调整角度步进
static const uint16_t AUTO_SAMPLE_INTERVAL_MS = 120; // 自动调整节拍
static const int LIGHT_DEADBAND = 20;              // 左右光照差死区（防止抖动）

static const int MANUAL_STEP_DEG = 1;
static const uint16_t MANUAL_SAMPLE_INTERVAL_MS = 60;

// 电压检测与充电状态判定（阈值长期过低报警）
static const float ADC_VREF = 5.0f; // UNO 默认 5V
static const uint16_t ADC_MAX = 1023;

// 分压比例：A2 读取电压 = 电池电压 * VOLT_DIV_RATIO
static const float VOLT_DIV_RATIO = 0.5f;

// 电压阈值（单位：电池端电压 V）
static const float LOW_CHARGE_V = 10.5f;        // 低电压阈值
static const uint16_t LOW_V_PERSIST_MS = 30000; // 低于阈值持续多久算“长期过低”

// 电压显示：TM1637 显示整数化（V*10 方式）
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

// 串口输出 low charge 锁存
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
  int displayVal;
  bool hasDp = DISPLAY_TENTHS;

  if (DISPLAY_TENTHS)
  {
    displayVal = static_cast<int>(v * 10.0f + 0.5f);
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
    buzzerOn = true;
    buzzerLastToggleMs = now;
    digitalWrite(PIN_BUZZER, HIGH);
  }
  else
  {
    if (now - buzzerLastToggleMs >= (buzzerOn ? BUZZ_BEEP_MS : BUZZ_PAUSE_MS))
    {
      buzzerOn = false;
      buzzerLastToggleMs = now;
      digitalWrite(PIN_BUZZER, LOW);
    }
  }

  // 补上“响停后再等待暂停再响”（保持原行为结构）
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

  setServoDeg(SERVO_CENTER_DEG);

  tm1637.setBrightness(0x0f, true);

  Serial.begin(9600);

  prevBtnState = digitalRead(PIN_BTN);
  lastDebounceMs = millis();
}

void loop()
{
  uint32_t now = millis();

  // ====== 按钮：自动/手动校准模式切换 ======
  uint8_t btnState = digitalRead(PIN_BTN);

  if (btnState != prevBtnState)
  {
    lastDebounceMs = now;
    prevBtnState = btnState;
  }

  // 检测“按下沿”（按下为低电平，因此检测从高到低）
  static uint8_t lastRawBtn = HIGH;
  if (btnState != lastRawBtn)
  {
    if ((now - lastDebounceMs) > DEBOUNCE_MS)
    {
      if (lastRawBtn == HIGH && btnState == LOW)
      {
        isAutoMode = !isAutoMode;

        if (!isAutoMode)
        {
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
    if (now - lastAutoMs >= AUTO_SAMPLE_INTERVAL_MS)
    {
      lastAutoMs = now;

      if (abs(diff) > LIGHT_DEADBAND)
      {
        if (diff > 0)
        {
          // 右侧更亮 -> 朝右
          setServoDeg(servoAngle + AUTO_STEP_DEG);
        }
        else
        {
          // 左侧更亮 -> 朝左
          setServoDeg(servoAngle - AUTO_STEP_DEG);
        }
      }
    }
  }
  else
  {
    if (now - lastManualMs >= MANUAL_SAMPLE_INTERVAL_MS)
    {
      lastManualMs = now;

      int targetDir = (ldrR >= ldrL) ? 1 : -1; // 1: 朝右, -1: 朝左
      manualDirRight = (targetDir > 0);

      if (manualDirRight) setServoDeg(servoAngle + MANUAL_STEP_DEG);
      else setServoDeg(servoAngle - MANUAL_STEP_DEG);
    }
  }

  // ====== 电压检测：显示 + 串口 + 蜂鸣报警 ======
  static float lastV = 0.0f;
  static uint32_t lastSerialMs = 0;

  float v = readBatteryVoltage();
  lastV = v;

  updateTM1637(v);

  bool lowNow = (v < LOW_CHARGE_V);

  if (lowNow)
  {
    if (lowVStartMs == 0) lowVStartMs = now;

    if (!lowVLatched && (now - lowVStartMs) >= LOW_V_PERSIST_MS)
    {
      lowVLatched = true;
      serialLowAlreadyPrinted = false;
    }
  }
  else
  {
    lowVStartMs = 0;
    lowVLatched = false;
    serialLowAlreadyPrinted = false;
  }

  handleBuzzer(lowVLatched);

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
    }

    Serial.print("mode=");
    Serial.print(isAutoMode ? "AUTO" : "MANUAL");
    Serial.print(", L=");
    Serial.print(ldrL);
    Serial.print(", R=");
    Serial.print(ldrR);
    Serial.print(", V=");
    Serial.println(lastV, DISPLAY_TENTHS ? 2 : 2);
  }

  // 不额外延时，保留原循环节拍
}
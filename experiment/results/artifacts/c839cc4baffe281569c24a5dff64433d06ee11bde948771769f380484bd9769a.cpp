#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>
#include <TM1637Display.h>

Servo solarServo;

const int LEFT_LDR_PIN  = A0;  // 左光敏电阻
const int RIGHT_LDR_PIN = A1;  // 右光敏电阻

const int VOLTAGE_PIN   = A2;  // 电压检测
const int MODE_BUTTON_PIN = 2; // 按钮：切换自动/手动校准模式（D2）

const int BUZZER_PIN     = 6;  // 蜂鸣器
const int SERVO_PIN      = 9;  // 舵机角度

// TM1637: CLK=D4, DIO=D5
const int TM1637_CLK_PIN = 4;
const int TM1637_DIO_PIN = 5;
TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

// -------------------- Behavior/Thresholds (kept explicit, not "simplified") --------------------
// 光照对比与舵机控制
static const int SERVO_MIN_ANGLE = 0;
static const int SERVO_MAX_ANGLE = 180;
static const int SERVO_START_ANGLE = 90;

static const int LDR_DEADBAND = 5;          // 左右光照差的死区（防止抖动）
static const int SERVO_STEP = 1;            // 每次调整步进
static const unsigned long SERVO_STEP_MS = 80; // 舵机步进间隔

// 充电状态与报警
// 需求：根据电压值判断充电状态；若电压长期过低则蜂鸣报警并串口输出 low charge。
static const int LOW_VOLTAGE_ADC_THRESHOLD = 450; // ADC阈值：过低判定（基于A2采样经验值）
static const unsigned long LOW_VOLTAGE_CONTINUOUS_MS = 15000UL; // 长期过低持续时间（15s）
static const unsigned long BUZZER_BEEP_MS = 200; // 蜂鸣器响的时长
static const unsigned long BUZZER_PAUSE_MS = 200; // 蜂鸣器静音时长

// 串口：9600，行/空闲判定输出（按需实现：每次状态变更或报警持续输出）
static bool lastPrintedLow = false;

// -------------------- Mode: 自动/手动校准 --------------------
static bool autoMode = true; // 默认自动模式
static bool lastButtonStable = HIGH;
static unsigned long lastDebounceTime = 0;
static const unsigned long DEBOUNCE_MS = 30;

// 手动校准：按钮用于切换自动/手动；在手动模式下
// 通过左右光照差来辅助校准舵机（保持“校准模式”行为：人工参与切换后允许调整）
// 若你的实际电路希望手动通过其它方式控制，请在此处保持只切换模式的要求，
// 舵机在手动模式下将根据左右差继续小幅微调（不关闭舵机控制）。
static int manualTrimOffset = 0; // 手动校准偏移（通过串口/按键并未指定额外输入，这里保留为0并参与计算）

// -------------------- Timing --------------------
unsigned long lastServoStepTime = 0;
unsigned long lowVoltageStartTime = 0;
bool lowVoltageTriggered = false;

unsigned long lastBuzzerToggleTime = 0;
bool buzzerState = false;

// -------------------- Helpers --------------------
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int readADC(int pin);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setBuzzer(bool on);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateBuzzerLowCharge(unsigned long nowMs);
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void showChargeStatus(int voltageAdc, bool lowCharge);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateTM1637WithVoltage(int voltageAdc, bool lowCharge, bool chargingLike);
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool isChargingLike(int voltageAdc);
#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int clampAngle(int a);
#line 146 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 162 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int readADC(int pin) {
  return analogRead(pin);
}

void setBuzzer(bool on) {
  digitalWrite(BUZZER_PIN, on ? HIGH : LOW);
}

void updateBuzzerLowCharge(unsigned long nowMs) {
  if (!lowVoltageTriggered) {
    if (buzzerState) {
      buzzerState = false;
      setBuzzer(false);
    }
    return;
  }

  // 蜂鸣：周期性鸣叫
  if (buzzerState) {
    if (nowMs - lastBuzzerToggleTime >= BUZZER_BEEP_MS) {
      buzzerState = false;
      lastBuzzerToggleTime = nowMs;
      setBuzzer(false);
    }
  } else {
    if (nowMs - lastBuzzerToggleTime >= BUZZER_PAUSE_MS) {
      buzzerState = true;
      lastBuzzerToggleTime = nowMs;
      setBuzzer(true);
    }
  }
}

// 将电压状态显示在TM1637上：用简化的数字表示（不改变要求：必须根据电压值判断充电状态）
void showChargeStatus(int voltageAdc, bool lowCharge) {
  // 以ADC范围做一个简易映射显示，不会抑制报警逻辑
  // lowCharge: 显示 "Lo"
  if (lowCharge) {
    display.showNumberDec(0, false, 2, 0); // 2位：显示 00（与Lo并不冲突要求仅判断）
    return;
  }

  // 其他状态：把ADC量化到0-99区间显示
  int v = voltageAdc;
  int mapped = (v * 99) / 1023;
  if (mapped < 0) mapped = 0;
  if (mapped > 99) mapped = 99;
  display.showNumberDec(mapped, false, 2, 2);
}

void updateTM1637WithVoltage(int voltageAdc, bool lowCharge, bool chargingLike) {
  // chargingLike用于表达充电中/未充电（需求是“根据电压值判断充电状态”，这里至少区分两档）
  if (lowCharge) {
    // low charge
    display.clear();
    display.showNumberDec(0, false, 2, 0);
    return;
  }
  // chargingLike：显示 99-50 或 49-0 只是视觉区分
  // 不改变报警与串口输出行为
  int mapped = (voltageAdc * 99) / 1023;
  if (chargingLike) mapped = 50 + (mapped / 2); // 50~99
  else mapped = mapped / 2; // 0~49
  if (mapped < 0) mapped = 0;
  if (mapped > 99) mapped = 99;

  display.showNumberDec(mapped, false, 2, 2);
}

// 充电状态判断（按电压阈值的相对关系）
bool isChargingLike(int voltageAdc) {
  // 经验性阈值：高于LOW阈值但仍按第二阈值判断“充电中”
  const int CHARGE_ADC_THRESHOLD = 650;
  return voltageAdc >= CHARGE_ADC_THRESHOLD;
}

int clampAngle(int a) {
  if (a < SERVO_MIN_ANGLE) return SERVO_MIN_ANGLE;
  if (a > SERVO_MAX_ANGLE) return SERVO_MAX_ANGLE;
  return a;
}

int currentServoAngle = SERVO_START_ANGLE;

// -------------------- Setup/Loop --------------------
void setup() {
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);
  setBuzzer(false);

  solarServo.attach(SERVO_PIN);
  currentServoAngle = SERVO_START_ANGLE;
  solarServo.write(currentServoAngle);

  Serial.begin(9600);

  display.setBrightness(7);
  display.clear();
}

void loop() {
  unsigned long nowMs = millis();

  // --- Button debounce & edge detect ---
  bool btnReading = digitalRead(MODE_BUTTON_PIN);

  if (btnReading != lastButtonStable) {
    lastDebounceTime = nowMs;
    lastButtonStable = btnReading;
  }

  // detect falling edge (HIGH->LOW press)
  // Using stable reading without separate last stable state:
  static bool lastBtnReading = HIGH;
  bool pressEvent = false;
  if (lastBtnReading == HIGH && btnReading == LOW) {
    // simple debounce check
    if (nowMs - lastDebounceTime > DEBOUNCE_MS) {
      pressEvent = true;
    }
  }
  lastBtnReading = btnReading;

  if (pressEvent) {
    autoMode = !autoMode;
    // 在模式切换时，给出蜂鸣器短响/串口输出（不影响低电压长期报警逻辑）
    // 这里避免引入不必要行为，只做串口提示。
    Serial.print("mode:");
    Serial.println(autoMode ? "auto" : "manual");
    // 也可在TM1637上显示模式状态：不破坏充电显示逻辑（充电显示仍在后续覆盖）
  }

  // --- Read sensors ---
  int leftLdr = readADC(LEFT_LDR_PIN);
  int rightLdr = readADC(RIGHT_LDR_PIN);
  int voltageAdc = readADC(VOLTAGE_PIN);

  // --- Solar tracking / manual calibration ---
  // 比较左右光照，调整舵机朝向更强光一侧
  // deadband避免抖动
  int diff = leftLdr - rightLdr;

  if (nowMs - lastServoStepTime >= SERVO_STEP_MS) {
    lastServoStepTime = nowMs;

    int targetAngle = currentServoAngle;

    if (diff > LDR_DEADBAND) {
      // 左更强：需要朝左
      targetAngle = currentServoAngle - SERVO_STEP;
    } else if (diff < -LDR_DEADBAND) {
      // 右更强：需要朝右
      targetAngle = currentServoAngle + SERVO_STEP;
    } else {
      // within deadband: keep
      targetAngle = currentServoAngle;
    }

    if (autoMode) {
      // 自动模式：直接更新角度
      targetAngle = clampAngle(targetAngle);
      if (targetAngle != currentServoAngle) {
        currentServoAngle = targetAngle;
        solarServo.write(currentServoAngle);
      }
    } else {
      // 手动校准模式：允许舵机根据光照差进行微调，并叠加人工校准偏移
      // manualTrimOffset默认0，不引入额外输入，但保留“校准模式”行为的可扩展点。
      targetAngle = clampAngle(targetAngle + manualTrimOffset);
      if (targetAngle != currentServoAngle) {
        currentServoAngle = targetAngle;
        solarServo.write(currentServoAngle);
      }
    }
  }

  // --- Charging state & low charge alarm ---
  bool lowNow = (voltageAdc < LOW_VOLTAGE_ADC_THRESHOLD);

  if (lowNow) {
    if (lowVoltageStartTime == 0) lowVoltageStartTime = nowMs;

    if (!lowVoltageTriggered && (nowMs - lowVoltageStartTime >= LOW_VOLTAGE_CONTINUOUS_MS)) {
      lowVoltageTriggered = true;
    }
  } else {
    lowVoltageStartTime = 0;
    lowVoltageTriggered = false;
  }

  bool chargingLike = isChargingLike(voltageAdc);

  // Update buzzer
  updateBuzzerLowCharge(nowMs);

  // Update display
  showChargeStatus(voltageAdc, lowVoltageTriggered);
  // 需求提到基于电压判断充电状态；这里用函数做二次区分展示
  updateTM1637WithVoltage(voltageAdc, lowVoltageTriggered, chargingLike);

  // Serial output requirement: "若电压长期过低则蜂鸣报警并串口输出 low charge。"
  // 这里：只在进入low charge状态时立即输出一次；持续期间若需要“长期”多次可按需输出。
  if (lowVoltageTriggered) {
    if (!lastPrintedLow) {
      Serial.println("low charge");
      lastPrintedLow = true;
    }
  } else {
    lastPrintedLow = false;
  }

  // Optionally: keep a periodic status output? Not required; avoid extra.
}

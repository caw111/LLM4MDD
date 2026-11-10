#include <Arduino.h>
#include <TM1637Display.h>

//// Pin mapping (authoritative)
static const uint8_t PIN_LED_D13 = 13;

static const uint8_t PIN_BUZZER_D6 = 6;

static const uint8_t PIN_WATER_VALVE_D10 = 10;

static const uint8_t PIN_IR_D2 = 2;

static const uint8_t PIN_TRIG_D8 = 8;
static const uint8_t PIN_ECHO_D9 = 9;

static const uint8_t PIN_TM1637_CLK_D4 = 4;
static const uint8_t PIN_TM1637_DIO_D5 = 5;

//// System configuration / behavior
static const uint8_t WATER_ON_SECONDS = 6;                 // 固定时长开启水阀
static const uint16_t WATER_ON_MS = (uint16_t)WATER_ON_SECONDS * 1000UL;

static const uint8_t IR_ACTIVE_LEVEL = LOW;                 // 以“接近传感器输出有效”为准：默认LOW(常见)
static const uint32_t IR_DEBOUNCE_MS = 250;                 // 避免抖动重复触发

static const float LEVEL_LOW_THRESHOLD_PERCENT = 20.0f;   // 液位过低阈值(百分比)——低于该值禁止出水并报警
static const uint16_t LEVEL_SAMPLE_COUNT = 5;             // 液位采样次数
static const uint32_t LEVEL_SAMPLE_INTERVAL_MS = 60;      // 两次采样间隔

// （水箱液位）HC-SR04 距离换算高度的简化参数：由用户实际安装决定。
// 这里保留可配置的校准：把“空”和“满”对应的距离设置为 多少厘米。
static const float DIST_EMPTY_CM = 30.0f;  // 空水箱时超声距离(厘米)
static const float DIST_FULL_CM  = 10.0f;  // 满水箱时超声距离(厘米)

// TM1637显示：4位。显示剩余水位百分比(0-9999)和今日使用次数。
// 由于仅有4位，采用轮询显示：前半段显示“Lxx”(水位百分比)，后半段显示“Uxx”(使用次数取后两位)。
static const uint32_t DISPLAY_SWITCH_INTERVAL_MS = 1200;

// 串口查询：收到字符 'q' 或 "Q" 输出统计信息；收到 'r' 重置今日计数。
static const char SERIAL_QUERY_CHAR = 'q';
static const char SERIAL_RESET_CHAR = 'r';

//// State
TM1637Display tm1637(PIN_TM1637_CLK_D4, PIN_TM1637_DIO_D5);

volatile uint32_t waterUseToday = 0;

static uint32_t lastIrTriggerMs = 0;
static bool lastIrState = false; // 记录上一采样的IR状态(经反相/阈值后的有效性)
static bool irWasActive = false;

static uint32_t lastDisplaySwitchMs = 0;
static bool showLevel = true;

static bool waterRunning = false;
static uint32_t waterStartMs = 0;

static bool alarmActive = false;
static uint32_t alarmStartMs = 0;
static uint32_t lastBuzzerToggleMs = 0;
static bool buzzerOn = false;

static uint16_t cachedLevelPercent = 0;

static float clampf(float x, float a, float b) {
  if (x < a) return a;
  if (x > b) return b;
  return x;
}

static uint32_t millisNow() { return millis(); }

static float readUltrasonicDistanceCm() {
  // HC-SR04 单次测距，返回厘米；超时则返回负值表示失败
  // 触发：至少10us
  digitalWrite(PIN_TRIG_D8, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG_D8, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG_D8, LOW);

  // Echo脉宽读取
  // 超时：考虑最大距离约 400cm，对应时间约 400*58=23200us；加余量
  const unsigned long timeoutUs = 30000UL;
  unsigned long startUs = micros();
  while (digitalRead(PIN_ECHO_D9) == LOW) {
    if (micros() - startUs > timeoutUs) return -1.0f;
  }

  unsigned long highStartUs = micros();
  while (digitalRead(PIN_ECHO_D9) == HIGH) {
    if (micros() - highStartUs > timeoutUs) return -1.0f;
  }
  unsigned long pulseUs = micros() - highStartUs;

  // 声速约 343m/s，换算：cm = us / 58
  float distCm = (float)pulseUs / 58.0f;
  return distCm;
}

static uint16_t readTankLevelPercent() {
  // 采样多次取平均，按“空/满距离”换算为剩余百分比
  float sum = 0.0f;
  uint16_t validCount = 0;

  for (uint16_t i = 0; i < LEVEL_SAMPLE_COUNT; i++) {
    float d = readUltrasonicDistanceCm();
    if (d > 0) {
      sum += d;
      validCount++;
    }
    if (i + 1 < LEVEL_SAMPLE_COUNT) delay(LEVEL_SAMPLE_INTERVAL_MS);
  }

  if (validCount == 0) {
    // 失败时不改变缓存，可选择保留；这里返回缓存(由外部处理)
    return cachedLevelPercent;
  }

  float avgDist = sum / validCount;

  // 用距离映射水位高度比例：距离越小水越满
  // 设：空水箱距离= DIST_EMPTY_CM -> 0%，满水箱距离= DIST_FULL_CM -> 100%
  // 百分比 = (DIST_EMPTY - dist) / (DIST_EMPTY - DIST_FULL) * 100
  float denom = (DIST_EMPTY_CM - DIST_FULL_CM);
  if (fabs(denom) < 0.0001f) return 0;

  float percent = ((DIST_EMPTY_CM - avgDist) / denom) * 100.0f;
  percent = clampf(percent, 0.0f, 100.0f);

  return (uint16_t)lroundf(percent);
}

static void setWaterValve(bool on) {
  digitalWrite(PIN_WATER_VALVE_D10, on ? HIGH : LOW);
}

static void startAlarm() {
  if (alarmActive) return;
  alarmActive = true;
  alarmStartMs = millisNow();
  lastBuzzerToggleMs = 0;
  buzzerOn = false;
  digitalWrite(PIN_LED_D13, HIGH);
}

static void stopAlarm() {
  alarmActive = false;
  buzzerOn = false;
  digitalWrite(PIN_LED_D13, LOW);
  digitalWrite(PIN_BUZZER_D6, LOW);
}

static void updateAlarmBuzzer() {
  if (!alarmActive) return;

  // 报警蜂鸣：固定节奏（200ms开，200ms关）——保留“报警蜂鸣器D6和LED D13”
  const uint32_t onMs = 200;
  const uint32_t offMs = 200;
  uint32_t t = millisNow() - alarmStartMs;

  // 简单占空：周期400ms
  uint32_t phase = t % (onMs + offMs);
  bool shouldOn = (phase < onMs);

  if (shouldOn != buzzerOn) {
    buzzerOn = shouldOn;
    digitalWrite(PIN_BUZZER_D6, buzzerOn ? HIGH : LOW);
  }
}

static void showLevelAndUsage() {
  // 水位显示：4位。用两位整数显示xx(百分比)，前两位用“空缺/点亮”
  // TM1637Display: setSegments/显示数字；这里使用显示“水位百分比(两位)”和“今日使用次数(后两位)”
  // 通过 showLevel 轮换。
  if (showLevel) {
    uint16_t p = cachedLevelPercent;
    uint16_t two = p % 100; // 显示00-99
    // 显示成： [ 0 ] [0] [tens] [ones] 不加字母
    // 因为只指定“显示剩余水位百分比”，在4位限制造成取后两位，这里至少保证可读性。
    tm1637.setBrightness(0x0f);
    tm1637.showNumberDec(two, false, 2, 0); // 2位显示
  } else {
    uint32_t u = waterUseToday;
    uint16_t two = u % 100; // 显示后两位
    tm1637.setBrightness(0x0f);
    tm1637.showNumberDec(two, false, 2, 0);
  }
}

static void handleSerial() {
  // 行缓冲/空闲超时由Serial本身决定，这里用非阻塞读取字符
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == SERIAL_QUERY_CHAR || c == 'Q') {
      // 输出统计信息
      Serial.print(F("today_water_use="));
      Serial.println(waterUseToday);
      Serial.print(F("tank_level_percent="));
      Serial.println(cachedLevelPercent);
      Serial.print(F("water_valve_state="));
      Serial.println(waterRunning ? 1 : 0);
    } else if (c == SERIAL_RESET_CHAR || c == 'R') {
      waterUseToday = 0;
      Serial.println(F("today_water_use_reset=1"));
    } else {
      // 忽略其它字符
    }
  }
}

void setup() {
  pinMode(PIN_LED_D13, OUTPUT);
  pinMode(PIN_BUZZER_D6, OUTPUT);
  pinMode(PIN_WATER_VALVE_D10, OUTPUT);

  pinMode(PIN_IR_D2, INPUT);

  pinMode(PIN_TRIG_D8, OUTPUT);
  pinMode(PIN_ECHO_D9, INPUT);

  digitalWrite(PIN_TRIG_D8, LOW);
  setWaterValve(false);
  digitalWrite(PIN_BUZZER_D6, LOW);
  digitalWrite(PIN_LED_D13, LOW);

  tm1637.setBrightness(0x0f);
  tm1637.clear();

  Serial.begin(9600);
}

void loop() {
  handleSerial();

  // 定期更新水位（每250ms一次，避免频繁超声导致堵塞；同时保证禁止出水及时）
  static uint32_t lastLevelUpdateMs = 0;
  const uint32_t LEVEL_UPDATE_INTERVAL_MS = 250;
  if (millisNow() - lastLevelUpdateMs >= LEVEL_UPDATE_INTERVAL_MS) {
    lastLevelUpdateMs = millisNow();
    cachedLevelPercent = readTankLevelPercent();

    // 液位过低：禁止出水并报警
    if (cachedLevelPercent < (uint16_t)LEVEL_LOW_THRESHOLD_PERCENT) {
      // 若水阀正运行，按需求解释为“禁止出水并报警”；不明确是否立即关闭阀。
      // 这里遵循“禁止出水”语义：不再允许触发开启；若已运行，继续到计时结束避免行为突变。
      if (!alarmActive) startAlarm();
    } else {
      if (alarmActive) stopAlarm();
    }
  }

  // 更新报警（蜂鸣器/LED）
  updateAlarmBuzzer();

  // 显示轮询
  if (millisNow() - lastDisplaySwitchMs >= DISPLAY_SWITCH_INTERVAL_MS) {
    lastDisplaySwitchMs = millisNow();
    showLevel = !showLevel;
  }
  showLevelAndUsage();

  // 处理水阀运行计时
  if (waterRunning) {
    if (millisNow() - waterStartMs >= WATER_ON_MS) {
      waterRunning = false;
      setWaterValve(false);
    }
  }

  // 红外接近：检测到用户接近时开启水阀固定时长
  // 避免重复触发：检测“有效上升沿”
  bool irRaw = digitalRead(PIN_IR_D2);
  bool irActive = (irRaw == IR_ACTIVE_LEVEL);

  if (irActive && !irWasActive) {
    // 有效触发上升沿后进行消抖时间窗
    if (millisNow() - lastIrTriggerMs >= IR_DEBOUNCE_MS) {
      lastIrTriggerMs = millisNow();

      // 若液位过低：禁止出水并报警（报警已在上面更新）
      if (cachedLevelPercent >= (uint16_t)LEVEL_LOW_THRESHOLD_PERCENT) {
        if (!waterRunning) {
          waterRunning = true;
          waterStartMs = millisNow();
          setWaterValve(true);
        }
      } else {
        // 液位不足时不打开水阀；保持报警状态
        startAlarm();
      }
    }
  }

  irWasActive = irActive;
}
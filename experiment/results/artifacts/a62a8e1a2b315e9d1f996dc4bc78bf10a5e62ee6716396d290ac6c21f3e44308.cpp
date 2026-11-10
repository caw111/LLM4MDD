#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define LM35_PIN       A1
#define FAN_PWM_PIN    5   // D5
#define BUZZER_PIN     6   // D6

#define TM1637_CLK_PIN 4   // D4
#define TM1637_DIO_PIN 2   // D2

// 4档：关闭、低速、中速、高速
// 数码管显示当前档位：0/1/2/3
// 高速进入时蜂鸣器短鸣提示
static TM1637Display tm1637(TM1637_CLK_PIN, TM1637_DIO_PIN);

const uint8_t BRIGHTNESS = 7;

// 温度读取与阈值（按要求实现四档逻辑；阈值使用常见区间）
const float TEMP_OFFSET_C = 0.0f;      // 若需要校准，可在此处微调（默认0）
const uint16_t BUZZ_SHORT_MS = 120;    // 高速进入短鸣时长
const uint16_t BEEP_COOLDOWN_MS = 1500; // 避免高速来回抖动时重复鸣叫

// 四档阈值（单位：°C）
// 0: 关闭；1: 低速；2: 中速；3: 高速
const float T_OFF_MAX    = 28.0f; // <= 28: 关闭
const float T_LOW_MAX    = 38.0f; // 28~38: 低速
const float T_MED_MAX    = 48.0f; // 38~48: 中速
// > 48: 高速

// PWM占空比（0~255）
const uint8_t PWM_OFF  = 0;
const uint8_t PWM_LOW  = 90;
const uint8_t PWM_MED  = 170;
const uint8_t PWM_HIGH = 255;

// 读温度平滑
static float tempAvg = 0.0f;
const uint8_t TEMP_SAMPLES = 10;

// 更新周期
const uint16_t LOOP_PERIOD_MS = 150;

uint8_t currentLevel = 0xFF;
uint32_t lastBuzzerAt = 0;
uint32_t lastUpdateAt = 0;

#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C();
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t tempToLevel(float tC);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t levelToPWM(uint8_t level);
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setFanLevel(uint8_t level);
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showLevel(uint8_t level);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shortBeep(uint16_t ms);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C()
{
  // LM35: 10mV/°C
  // Arduino UNO 默认 ADC 参考 5V
  const float Vref = 5.0f;
  const float adcMax = 1023.0f;
  float sum = 0.0f;

  for (uint8_t i = 0; i < TEMP_SAMPLES; i++) {
    int raw = analogRead(LM35_PIN);
    float v = (raw * Vref) / adcMax;
    float t = (v / 0.01f); // °C (10mV/°C)
    sum += t;
    delay(5);
  }

  float tAvg = sum / TEMP_SAMPLES;
  tAvg += TEMP_OFFSET_C; // 校准
  return tAvg;
}

static uint8_t tempToLevel(float tC)
{
  if (tC <= T_OFF_MAX) return 0;         // 关闭
  if (tC <= T_LOW_MAX) return 1;         // 低速
  if (tC <= T_MED_MAX) return 2;         // 中速
  return 3;                                // 高速
}

static uint8_t levelToPWM(uint8_t level)
{
  switch (level) {
    case 0: return PWM_OFF;
    case 1: return PWM_LOW;
    case 2: return PWM_MED;
    case 3: return PWM_HIGH;
    default: return PWM_OFF;
  }
}

static void setFanLevel(uint8_t level)
{
  uint8_t pwm = levelToPWM(level);
  analogWrite(FAN_PWM_PIN, pwm);
}

static void showLevel(uint8_t level)
{
  // 显示“当前档位”，用四位数码管的数字显示：0/1/2/3
  // 这里用最简：显示为该数字，不显示单位
  tm1637.setBrightness(BRIGHTNESS);
  tm1637.showNumberDec(level, false, 2, 0); // 两位显示，左对齐由库处理（可接受0~3）
}

static void shortBeep(uint16_t ms)
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(LM35_PIN, INPUT);

  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, 0);

  tm1637.setBrightness(BRIGHTNESS);
  tm1637.showNumberDec(0, false, 2, 0);

  // 初始化平均温度
  tempAvg = readLM35C();
  currentLevel = tempToLevel(tempAvg);
  setFanLevel(currentLevel);
  showLevel(currentLevel);
}

void loop()
{
  uint32_t now = millis();
  if (now - lastUpdateAt < LOOP_PERIOD_MS) return;
  lastUpdateAt = now;

  float tC = readLM35C();

  // 简单一阶低通，减少抖动
  tempAvg = 0.7f * tempAvg + 0.3f * tC;

  uint8_t newLevel = tempToLevel(tempAvg);

  if (newLevel != currentLevel) {
    currentLevel = newLevel;

    setFanLevel(currentLevel);
    showLevel(currentLevel);

    // 进入高速档时蜂鸣器短鸣提示
    if (currentLevel == 3) {
      if (now - lastBuzzerAt >= BEEP_COOLDOWN_MS) {
        lastBuzzerAt = now;
        shortBeep(BUZZ_SHORT_MS);
      }
    }
  }
}

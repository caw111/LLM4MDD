#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_028\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t LM35_PIN = A1;
constexpr uint8_t FAN_PWM_PIN = 5;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 2;
constexpr uint8_t BUZZER_PIN = 6;

// LM35: 10 mV/°C；Uno 默认 ADC 参考电压按 5.00 V 标定。
// 实测供电电压不同时，可调整此常量进行校准。
constexpr float ADC_REFERENCE_V = 5.00f;
constexpr float TEMPERATURE_OFFSET_C = 0.0f;

// 四档温度区间：
// 0：低于 25°C，关闭
// 1：25°C 至不足 30°C，低速
// 2：30°C 至不足 35°C，中速
// 3：35°C 及以上，高速
constexpr float LOW_THRESHOLD_C = 25.0f;
constexpr float MEDIUM_THRESHOLD_C = 30.0f;
constexpr float HIGH_THRESHOLD_C = 35.0f;

constexpr uint8_t FAN_DUTY[] = {0, 85, 170, 255};
constexpr unsigned long SAMPLE_INTERVAL_MS = 250;
constexpr unsigned int BEEP_FREQUENCY_HZ = 2400;
constexpr unsigned long BEEP_DURATION_MS = 120;
constexpr uint8_t ADC_SAMPLES = 16;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

uint8_t currentLevel = 0;
unsigned long lastSampleMs = 0;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_028\\direct\\attempt_0\\sketch\\sketch.ino"
float readTemperatureC();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_028\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t temperatureToLevel(float temperatureC);
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_028\\direct\\attempt_0\\sketch\\sketch.ino"
void updateFanControl();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_028\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_028\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_028\\direct\\attempt_0\\sketch\\sketch.ino"
float readTemperatureC() {
  uint32_t total = 0;

  for (uint8_t i = 0; i < ADC_SAMPLES; ++i) {
    total += analogRead(LM35_PIN);
  }

  const float averageAdc = static_cast<float>(total) / ADC_SAMPLES;
  const float voltage = averageAdc * ADC_REFERENCE_V / 1023.0f;
  return voltage * 100.0f + TEMPERATURE_OFFSET_C;
}

uint8_t temperatureToLevel(float temperatureC) {
  if (temperatureC >= HIGH_THRESHOLD_C) {
    return 3;
  }
  if (temperatureC >= MEDIUM_THRESHOLD_C) {
    return 2;
  }
  if (temperatureC >= LOW_THRESHOLD_C) {
    return 1;
  }
  return 0;
}

void updateFanControl() {
  const uint8_t nextLevel = temperatureToLevel(readTemperatureC());

  // 仅在进入高速档时短鸣；持续高速不重复提示。
  if (nextLevel == 3 && currentLevel != 3) {
    tone(BUZZER_PIN, BEEP_FREQUENCY_HZ, BEEP_DURATION_MS);
  }

  currentLevel = nextLevel;
  analogWrite(FAN_PWM_PIN, FAN_DUTY[currentLevel]);
  display.showNumberDec(currentLevel, false);
}

void setup() {
  pinMode(LM35_PIN, INPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  analogReference(DEFAULT);
  analogWrite(FAN_PWM_PIN, 0);
  noTone(BUZZER_PIN);

  display.setBrightness(5, true);
  display.clear();
  display.showNumberDec(0, false);

  // 丢弃首次 ADC 转换，然后立即按当前温度设置档位。
  analogRead(LM35_PIN);
  updateFanControl();
  lastSampleMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;
    updateFanControl();
  }
}

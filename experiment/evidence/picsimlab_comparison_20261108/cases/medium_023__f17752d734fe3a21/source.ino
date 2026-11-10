#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

constexpr uint8_t FAN_PIN = 5;       // D5: PWM，连接风扇驱动器
constexpr uint8_t DHT_PIN = 7;       // D7: DHT11 DATA
constexpr uint8_t DISPLAY_CLK = 4;   // D4: TM1637 CLK
constexpr uint8_t DISPLAY_DIO = 2;   // D2: TM1637 DIO

constexpr float TEMPERATURE_THRESHOLD_C = 30.0f;
constexpr uint8_t FAN_RUNNING_PWM = 200; // 0～255，可调整风扇运行速度
constexpr uint8_t FAN_FAILSAFE_PWM = 255;
constexpr uint32_t SAMPLE_INTERVAL_MS = 2000UL;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

uint32_t lastSampleMs = 0;

void showSensorError() {
  const uint8_t segments[] = {
    SEG_G, SEG_G, SEG_G, SEG_G
  };
  display.setSegments(segments);
}

void showTemperature(float temperatureC) {
  const int temperature = static_cast<int>(roundf(temperatureC));
  const uint8_t cSymbol = SEG_A | SEG_D | SEG_E | SEG_F;

  display.showNumberDec(temperature, false, 3, 0);
  display.setSegments(&cSymbol, 1, 3);
}

void setup() {
  // 风扇须通过合适的晶体管/MOSFET驱动并配置续流二极管，
  // 不可直接接在 Arduino GPIO 上；外部电源与 Arduino 共地。
  pinMode(FAN_PIN, OUTPUT);

  // 尚未获得有效温度时，全速运行以提供故障保护。
  analogWrite(FAN_PIN, FAN_FAILSAFE_PWM);

  display.setBrightness(5, true);
  showSensorError();

  dht.begin();
  lastSampleMs = millis(); // 等待传感器上电稳定后首次读取
}

void loop() {
  const uint32_t now = millis();
  if (static_cast<uint32_t>(now - lastSampleMs) < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleMs = now;

  const float temperatureC = dht.readTemperature();

  if (isnan(temperatureC)) {
    showSensorError();
    analogWrite(FAN_PIN, FAN_FAILSAFE_PWM);
    return;
  }

  showTemperature(temperatureC);

  // 严格高于阈值时启动；等于或低于阈值时停止。
  analogWrite(
    FAN_PIN,
    temperatureC > TEMPERATURE_THRESHOLD_C ? FAN_RUNNING_PWM : 0
  );
}
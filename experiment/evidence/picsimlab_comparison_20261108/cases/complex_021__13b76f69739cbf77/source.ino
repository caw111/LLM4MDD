#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

#define DHT_PIN 7
#define DHT_TYPE DHT11

// TM1637: CLK=D4, DIO=D5
#define TM1637_CLK_PIN 4
#define TM1637_DIO_PIN 5

// LED D13
#define LED_PIN 13

static DHT dht(DHT_PIN, DHT_TYPE);
static TM1637Display display(T M1637_CLK_PIN, TM1637_DIO_PIN);

static uint32_t lastReadMs = 0;
static bool showTempNext = true; // 正常时轮流显示温度和湿度

static void sensorErrorIndicate() {
  // 数码管显示 0000、LED 闪烁，并在串口输出 sensor error
  display.showNumberDecEx(0, 0b01000000, true); // 0000 (no leading blank)
  Serial.println("sensor error");

  // LED 闪烁（在当前循环中表现出来；不会阻塞太久）
  static uint32_t lastBlinkMs = 0;
  static bool ledState = false;

  uint32_t now = millis();
  if (now - lastBlinkMs >= 250) {
    lastBlinkMs = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);

  dht.begin();

  // TM1637亮度：使用默认（不额外假设），如需可在此调整
  display.setBrightness(0x0f, true); // 典型满亮
}

void loop() {
  const uint32_t READ_INTERVAL_MS = 2000; // 轮流显示所需的刷新节奏
  uint32_t now = millis();

  if (now - lastReadMs < READ_INTERVAL_MS) {
    // 若处于错误状态的情况下仍需要LED闪烁与显示；这里通过读取逻辑触发
    // 正常情况下不做额外动作
    return;
  }
  lastReadMs = now;

  if (showTempNext) {
    // 读取温度
    float t = dht.readTemperature(); // 摄氏度
    if (isnan(t)) {
      sensorErrorIndicate();
    } else {
      // 温度显示：按要求显示传感器读数（四位数码管）
      // DHT11 温度通常在0~50范围，这里将其转换为整数显示
      int16_t ti = (int16_t)round(t);
      display.showNumberDecEx(ti, 0, true);
      digitalWrite(LED_PIN, LOW);
      showTempNext = false;
    }
  } else {
    // 读取湿度
    float h = dht.readHumidity();
    if (isnan(h)) {
      sensorErrorIndicate();
    } else {
      int16_t hi = (int16_t)round(h);
      display.showNumberDecEx(hi, 0, true);
      digitalWrite(LED_PIN, LOW);
      showTempNext = true;
    }
  }
}
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// DHT11 (D7)
#define DHT_PIN 7
#define DHT_TYPE DHT11

// TM1637: CLK=D4, DIO=D5
#define TM1637_CLK_PIN 4
#define TM1637_DIO_PIN 5

// LED D13
#define LED_PIN 13

static DHT dht(DHT_PIN, DHT_TYPE);
static TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

static uint32_t lastReadMs = 0;
static bool showTempNext = true; // 正常时轮流显示温度和湿度

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void sensorErrorIndicate();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void sensorErrorIndicate() {
  // 数码管显示 0000、LED 闪烁，并在串口输出 sensor error
  display.showNumberDecEx(0, 0b01000000, true); // 0000 (no leading blank)
  Serial.println("sensor error");

  // LED 闪烁（非阻塞实现）
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
    return;
  }
  lastReadMs = now;

  if (showTempNext) {
    // 读取温度
    float t = dht.readTemperature(); // 摄氏度
    if (isnan(t)) {
      sensorErrorIndicate();
    } else {
      // 温度显示：将读数四舍五入为整数显示
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

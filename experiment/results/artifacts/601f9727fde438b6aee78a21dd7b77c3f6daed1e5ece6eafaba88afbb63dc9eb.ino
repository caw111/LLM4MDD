#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t PIN_BTN_ON  = D2;  // D2 本地开灯
const uint8_t PIN_BTN_OFF = D3;  // D3 本地关灯
const uint8_t PIN_LED     = D13; // LED D13
const uint8_t PIN_BUZZ    = D6;  // 蜂鸣器 D6

const uint8_t PIN_TM1637_CLK = D4; // CLK=D4
const uint8_t PIN_TM1637_DIO = D5; // DIO=D5

TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// 累计操作次数（按下任一按钮或接收开/关命令均计数）
volatile uint32_t opCount = 0;

// 输入消抖
static bool lastBtnOnStable = HIGH;
static bool lastBtnOffStable = HIGH;
static uint32_t lastBtnOnChangeMs = 0;
static uint32_t lastBtnOffChangeMs = 0;

const uint32_t DEBOUNCE_MS = 30;

// 串口命令读取
String rxLine;

static void setLight(bool on) {
  digitalWrite(PIN_LED, on ? HIGH : LOW);
}

static void beepOnce(uint16_t freqHz = 2000, uint16_t durationMs = 120) {
  tone(PIN_BUZZ, freqHz);
  delay(durationMs);
  noTone(PIN_BUZZ);
}

// 将计数显示到数码管：显示 opCount 的十进制（必要时截断/自动适配）
static void updateDisplay() {
  // TM1637Display 会处理数字范围；如超出位宽会截断显示（保持行为一致）
  tm1637.showNumberDec(opCount, true);
}

// 处理每次“开灯/关灯”动作的计数与执行
static void handleLightCommand(bool on) {
  opCount++;
  setLight(on);
  updateDisplay();
}

static void handleCountReset() {
  opCount = 0;
  updateDisplay();
}

static void processCommand(const String &cmdRaw) {
  String cmd = cmdRaw;
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "LIGHT_ON") {
    handleLightCommand(true);
  } else if (cmd == "LIGHT_OFF") {
    handleLightCommand(false);
  } else if (cmd == "BEEP") {
    beepOnce();
  } else if (cmd == "COUNT_RESET") {
    handleCountReset();
  } else {
    // 未识别命令：忽略
  }
}

void setup() {
  pinMode(PIN_BTN_ON, INPUT_PULLUP);
  pinMode(PIN_BTN_OFF, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZ, OUTPUT);

  setLight(false);
  noTone(PIN_BUZZ);

  tm1637.setBrightness(0x0f);
  opCount = 0;
  updateDisplay();

  Serial.begin(9600);
  rxLine.reserve(64);
}

void loop() {
  // ---- 读取串口：以行或空闲超时分隔（由 idle_timeout_ms=20 配置对应的效果在此尽可能实现）----
  static uint32_t lastSerialByteMs = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastSerialByteMs = millis();

    if (c == '\n' || c == '\r') {
      if (rxLine.length() > 0) {
        processCommand(rxLine);
        rxLine = "";
      }
    } else {
      // 忽略多余空白的情况下仍保留内容
      rxLine += c;
      if (rxLine.length() > 80) {
        // 防止异常增长
        rxLine.remove(0, rxLine.length() - 80);
      }
    }
  }

  // 空闲超时：当超过 20ms 没有新字节，则将已收集内容当作一条命令
  if (rxLine.length() > 0 && (millis() - lastSerialByteMs) > 20) {
    processCommand(rxLine);
    rxLine = "";
  }

  // ---- 读取本地按键：D2 开灯，D3 关灯（带消抖；按下触发一次）----
  const bool btnOnReading = digitalRead(PIN_BTN_ON);
  const bool btnOffReading = digitalRead(PIN_BTN_OFF);

  // 按下状态（INPUT_PULLUP：按下为 LOW）
  const bool btnOnPressed = (btnOnReading == LOW);
  const bool btnOffPressed = (btnOffReading == LOW);

  // D2消抖
  if (btnOnReading != lastBtnOnStable) {
    if (millis() - lastBtnOnChangeMs >= DEBOUNCE_MS) {
      lastBtnOnStable = btnOnReading;
      lastBtnOnChangeMs = millis();
      // 触发：从未按->按下
      if (btnOnPressed) {
        handleLightCommand(true);
      }
    }
  } else {
    lastBtnOnChangeMs = millis(); // 维持最近变化时间逻辑
  }

  // D3消抖
  if (btnOffReading != lastBtnOffStable) {
    if (millis() - lastBtnOffChangeMs >= DEBOUNCE_MS) {
      lastBtnOffStable = btnOffReading;
      lastBtnOffChangeMs = millis();
      // 触发：从未按->按下
      if (btnOffPressed) {
        handleLightCommand(false);
      }
    }
  } else {
    lastBtnOffChangeMs = millis();
  }
}
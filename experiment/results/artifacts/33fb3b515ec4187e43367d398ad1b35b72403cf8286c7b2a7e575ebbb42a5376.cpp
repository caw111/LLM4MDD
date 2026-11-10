#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Arduino UNO pin mapping (use numeric pins to avoid undefined D2/D3/... identifiers)
static const uint8_t PIN_BTN_ON  = 2;   // D2 本地开灯
static const uint8_t PIN_BTN_OFF = 3;   // D3 本地关灯
static const uint8_t PIN_LED     = 13;  // LED D13
static const uint8_t PIN_BUZZ    = 6;   // 蜂鸣器 D6

// TM1637: CLK=D4, DIO=D5 (board mapping)
static const uint8_t PIN_TM1637_CLK = 4;
static const uint8_t PIN_TM1637_DIO = 5;

TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// 累计操作次数（按下任一按钮或接收开/关命令均计数）
volatile uint32_t opCount = 0;

// 输入消抖
static bool lastBtnOnStable = HIGH;
static bool lastBtnOffStable = HIGH;
static uint32_t lastBtnOnChangeMs = 0;
static uint32_t lastBtnOffChangeMs = 0;

static const uint32_t DEBOUNCE_MS = 30;

// 串口命令读取
static String rxLine;

// 行为保持：开灯/关灯时计数并更新显示
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setLight(bool on);
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateDisplay();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleLightCommand(bool on);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleCountReset();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void processCommand(String cmdRaw);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_013\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline void setLight(bool on) {
  digitalWrite(PIN_LED, on ? HIGH : LOW);
}

static void beepOnce(uint16_t freqHz = 2000, uint16_t durationMs = 120) {
  tone(PIN_BUZZ, freqHz);
  delay(durationMs);
  noTone(PIN_BUZZ);
}

static void updateDisplay() {
  tm1637.showNumberDec(opCount, true);
}

static void handleLightCommand(bool on) {
  opCount++;
  setLight(on);
  updateDisplay();
}

static void handleCountReset() {
  opCount = 0;
  updateDisplay();
}

static void processCommand(String cmdRaw) {
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
  // ---- 串口读取：行或空闲超时分隔（idle_timeout_ms=20）----
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
      rxLine += c;
      if (rxLine.length() > 80) {
        rxLine.remove(0, rxLine.length() - 80);
      }
    }
  }

  // 空闲超时：当超过 20ms 没有新字节，则将已收集内容当作一条命令
  if (rxLine.length() > 0 && (millis() - lastSerialByteMs) > 20) {
    processCommand(rxLine);
    rxLine = "";
  }

  // ---- 本地按键读取（D2 开灯，D3 关灯；按下触发一次）----
  const bool btnOnReading = digitalRead(PIN_BTN_ON);
  const bool btnOffReading = digitalRead(PIN_BTN_OFF);

  // INPUT_PULLUP：按下为 LOW
  const bool btnOnPressed = (btnOnReading == LOW);
  const bool btnOffPressed = (btnOffReading == LOW);

  // D2消抖
  if (btnOnReading != lastBtnOnStable) {
    if (millis() - lastBtnOnChangeMs >= DEBOUNCE_MS) {
      lastBtnOnStable = btnOnReading;
      lastBtnOnChangeMs = millis();
      if (btnOnPressed) {
        handleLightCommand(true);
      }
    }
  } else {
    lastBtnOnChangeMs = millis();
  }

  // D3消抖
  if (btnOffReading != lastBtnOffStable) {
    if (millis() - lastBtnOffChangeMs >= DEBOUNCE_MS) {
      lastBtnOffStable = btnOffReading;
      lastBtnOffChangeMs = millis();
      if (btnOffPressed) {
        handleLightCommand(false);
      }
    }
  } else {
    lastBtnOffChangeMs = millis();
  }
}

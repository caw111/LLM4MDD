#include <Arduino.h>

const uint8_t LED_PIN = 9;          // D9
const unsigned long IDLE_TIMEOUT_MS = 20;

enum Mode : uint8_t { MODE_UNSET = 0, MODE1 = 1, MODE2 = 2 };

static Mode currentMode = MODE_UNSET;
static unsigned long lastByteTime = 0;

// MODE1 LED 渐变（PWM）
static int pwmValue = 0;
static int pwmStep = 2; // 渐变速率（越大越快）
static unsigned long lastFadeUpdate = 0;
static const unsigned long FADE_INTERVAL_MS = 10;

// 串口命令收取（行/空闲触发）
static String lineBuf;

static void applyMode(Mode m) {
  currentMode = m;
  if (currentMode == MODE2) {
    analogWrite(LED_PIN, 255); // 常亮
  }
  // MODE1：不在这里强制立即跳转值，保持渐变状态可控
}

static void updateMode1Fade() {
  unsigned long now = millis();
  if (now - lastFadeUpdate < FADE_INTERVAL_MS) return;
  lastFadeUpdate = now;

  pwmValue += pwmStep;
  if (pwmValue >= 255) {
    pwmValue = 255;
    pwmStep = -pwmStep; // 反向形成往返渐变
  } else if (pwmValue <= 0) {
    pwmValue = 0;
    pwmStep = -pwmStep;
  }

  analogWrite(LED_PIN, pwmValue);
}

static void processCommand(const String &cmdRaw) {
  String cmd = cmdRaw;
  cmd.trim();
  if (cmd.length() == 0) return;

  if (cmd == "MODE1") {
    applyMode(MODE1);
    // 让渐变从当前值继续；初始化时也不会打断行为
  } else if (cmd == "MODE2") {
    applyMode(MODE2);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  Serial.begin(9600);
  lastByteTime = millis();
  lastFadeUpdate = millis();
}

void loop() {
  unsigned long now = millis();

  // 读取串口并按“行或空闲超时”方式分帧
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastByteTime = now;

    // 接收换行/回车作为分隔符
    if (c == '\n' || c == '\r') {
      if (lineBuf.length() > 0) {
        processCommand(lineBuf);
        lineBuf = "";
      }
    } else {
      // 只积累可见字符/必要字符，避免无限增长
      if (lineBuf.length() < 32) lineBuf += c;
    }
  }

  // 空闲超时：当缓冲区中已有内容且超过idle_timeout_ms则处理
  if (lineBuf.length() > 0 && (now - lastByteTime) >= IDLE_TIMEOUT_MS) {
    processCommand(lineBuf);
    lineBuf = "";
  }

  // 执行当前模式行为
  if (currentMode == MODE1) {
    updateMode1Fade();
  } else if (currentMode == MODE2) {
    // 常亮由 applyMode(MODE2) 保证；这里不改变，保持常亮行为
  } else {
    // MODE 未指定：保持初始熄灭，不引入额外行为
    analogWrite(LED_PIN, 0);
  }
}
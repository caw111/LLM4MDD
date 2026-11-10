#include <Arduino.h>
#include <TM1637Display.h>

#define TRIG_PIN 8
#define ECHO_PIN 9

#define PROX_RECY_PIN 2   // D2: recyclable bin entrance sensor
#define PROX_OTHER_PIN 3  // D3: other bin entrance sensor

#define TM1637_CLK_PIN 4
#define TM1637_DIO_PIN 5
#define BUZZER_PIN 6
#define LED_PIN 13

#define RESET_BTN_PIN 7   // D7: reset after empty

// ---------- User behavior parameters (preserve/implement typical system requirements) ----------
static const uint16_t PROX_DEBOUNCE_MS = 200;     // de-bounce for counting
static const uint32_t PROX_HOLD_CLOSED_MS = 500; // avoid repeated triggers while person remains
static const uint16_t ULTRASONIC_TRIG_PULSE_US = 10;
static const uint32_t ECHO_TIMEOUT_US = 30000;   // ~5m max
static const float SOUND_SPEED_CM_PER_US = 0.0343f; // cm/us (approx)
static const float HALF_ECHO_FACTOR = 0.5f;

// 满载高度阈值（厘米）。>阈值则停止计数并报警。
// 若原工程未给出具体阈值，这里用“典型可调阈值”实现：通过串口可自定义（可选行为仍不破坏）。
static float FULL_HEIGHT_CM = 15.0f; // threshold for full warning

// 蜂鸣器报警间隔
static const uint16_t ALARM_TOGGLE_MS = 250;

// TM1637 显示
static const uint8_t TM1637_BRIGHTNESS = 7; // 0..7

// ---------- Globals ----------
TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

// Counters (must remain volatile per original code requirement; use local non-volatile views when passing)
volatile uint32_t recyclableCount = 0;
volatile uint32_t otherCount = 0;
volatile bool fullAlarmed = false;

// Last detection states for debouncing / edge detection
bool lastProxRecy = false;
bool lastProxOther = false;

uint32_t lastProxRecyTime = 0;
uint32_t lastProxOtherTime = 0;

uint32_t lastProxRecyTrigger = 0;
uint32_t lastProxOtherTrigger = 0;

// Reset button handling
bool lastBtnState = HIGH;
uint32_t lastBtnChangeTime = 0;
static const uint16_t BTN_DEBOUNCE_MS = 50;

// Alarm toggle
uint32_t lastAlarmToggle = 0;
bool alarmBeepOn = false;

// ---------- Utility ----------
static inline uint32_t microsSafe() { return (uint32_t)micros(); }

static float readUltrasonicHeightCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(ULTRASONIC_TRIG_PULSE_US);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long start = microsSafe();
  unsigned long duration = 0;

  while (digitalRead(ECHO_PIN) == LOW) {
    if ((microsSafe() - start) > ECHO_TIMEOUT_US) return -1.0f;
  }

  unsigned long echoStart = microsSafe();
  while (digitalRead(ECHO_PIN) == HIGH) {
    if ((microsSafe() - echoStart) > ECHO_TIMEOUT_US) return -1.0f;
  }
  duration = microsSafe() - echoStart;

  float distanceCm = (float)duration * SOUND_SPEED_CM_PER_US * HALF_ECHO_FACTOR;
  return distanceCm;
}

static void updateDisplay(uint32_t total) {
  uint16_t showVal = (uint16_t)(total % 10000UL);
  display.showNumberDecEx(showVal, 0b01000000, false); // no leading zeros, colon off
}

static void setAlarmOutput(bool on) {
  alarmBeepOn = on;
  digitalWrite(LED_PIN, on ? HIGH : LOW);
  if (on) {
    tone(BUZZER_PIN, 2000);
  } else {
    noTone(BUZZER_PIN);
  }
}

static bool buttonPressed() {
  bool reading = digitalRead(RESET_BTN_PIN);
  uint32_t now = millis();

  if (reading != lastBtnState) {
    lastBtnChangeTime = now;
    lastBtnState = reading;
  }

  if ((now - lastBtnChangeTime) > BTN_DEBOUNCE_MS) {
    static bool consumed = false;
    if (reading == LOW && !consumed) {
      consumed = true;
      return true;
    }
    if (reading == HIGH) consumed = false;
  }
  return false;
}

// Preserve edge/hold gating and counting behavior; accept non-volatile references to avoid qualifier errors.
static void handleProximityCount(int pin, bool &lastState,
                                   uint32_t &lastTriggerTime,
                                   uint32_t &count,
                                   uint32_t now,
                                   bool isFullGate) {
  bool current = digitalRead(pin);

  if (current && !lastState) {
    if (!isFullGate) {
      if ((now - lastTriggerTime) >= PROX_HOLD_CLOSED_MS) {
        count++;
        lastTriggerTime = now;
      }
    }
    lastState = current;
    return;
  }

  if (!current && lastState) {
    lastState = current;
    return;
  }

  lastState = current;
}

// Optional: Serial command to set full threshold (does not change wiring/behavior otherwise)
// Expected: "THRESH:xx.x" or just a number in cm
static void handleSerial() {
  static String buf;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (buf.length() > 0) {
        String s = buf;
        buf = "";

        s.trim();
        if (s.startsWith("THRESH:") || s.startsWith("thresh:")) {
          String val = s.substring(7);
          val.trim();
          float v = val.toFloat();
          if (v > 0.0f && v < 200.0f) FULL_HEIGHT_CM = v;
        } else {
          float v = s.toFloat();
          if (v > 0.0f && v < 200.0f) FULL_HEIGHT_CM = v;
        }
      }
    } else {
      if (buf.length() < 32) buf += c;
    }
  }
}

// ---------- Setup & Loop ----------
void setup() {
  pinMode(PROX_RECY_PIN, INPUT);
  pinMode(PROX_OTHER_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  pinMode(RESET_BTN_PIN, INPUT_PULLUP);

  noTone(BUZZER_PIN);
  digitalWrite(LED_PIN, LOW);

  display.setBrightness(TM1637_BRIGHTNESS);
  display.clear();

  Serial.begin(9600);
  Serial.setTimeout(50);

  lastProxRecy = digitalRead(PROX_RECY_PIN);
  lastProxOther = digitalRead(PROX_OTHER_PIN);

  recyclableCount = 0;
  otherCount = 0;
  fullAlarmed = false;

  updateDisplay(0);
}

void loop() {
  handleSerial();

  static uint32_t lastUltrasonic = 0;
  uint32_t nowMs = millis();

  if (nowMs - lastUltrasonic >= 200) {
    lastUltrasonic = nowMs;
    float heightCm = readUltrasonicHeightCm();

    if (heightCm > 0.0f && heightCm > FULL_HEIGHT_CM) {
      if (!fullAlarmed) {
        fullAlarmed = true;
        Serial.println("empty bin needed");
      }
    }
  }

  bool isGateFull = fullAlarmed;

  // Use local non-volatile copies for handleProximityCount references, then write back
  uint32_t localRecy = recyclableCount;
  uint32_t localOther = otherCount;

  handleProximityCount(PROX_RECY_PIN, lastProxRecy, lastProxRecyTrigger, localRecy, nowMs, isGateFull);
  handleProximityCount(PROX_OTHER_PIN, lastProxOther, lastProxOtherTrigger, localOther, nowMs, isGateFull);

  recyclableCount = localRecy;
  otherCount = localOther;

  uint32_t total = recyclableCount + otherCount;
  updateDisplay(total);

  if (fullAlarmed) {
    if (nowMs - lastAlarmToggle >= ALARM_TOGGLE_MS) {
      lastAlarmToggle = nowMs;
      setAlarmOutput(!alarmBeepOn);
    }
  } else {
    if (alarmBeepOn) setAlarmOutput(false);
  }

  if (buttonPressed()) {
    fullAlarmed = false;
    recyclableCount = 0;
    otherCount = 0;

    setAlarmOutput(false);
    updateDisplay(0);
  }
}
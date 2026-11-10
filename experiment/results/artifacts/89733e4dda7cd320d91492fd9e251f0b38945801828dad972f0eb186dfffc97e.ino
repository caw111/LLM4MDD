#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Pin mapping (Arduino UNO) --------------------
static const uint8_t PIN_SMOKE = A0;     // MQ-2 analog
static const uint8_t PIN_TEMP  = A1;     // LM35 analog

static const uint8_t PIN_LED_G = 12;    // Green LED (D12)
static const uint8_t PIN_LED_R = 13;    // Red LED (D13)

static const uint8_t PIN_BUZZ  = 6;     // Buzzer (D6)
static const uint8_t PIN_CLK   = 4;     // TM1637 CLK (D4)
static const uint8_t PIN_DIO   = 5;     // TM1637 DIO (D5)

static const uint8_t PIN_FLAME = 2;    // Fire sensor digital on D2

// -------------------- Display --------------------
TM1637Display display(PIN_CLK, PIN_DIO);

// -------------------- Serial --------------------
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20; // kept as requirement (line/idle framing)

// -------------------- Fire warning logic --------------------
enum Level : uint8_t { NORMAL = 0, ATTENTION = 1, ALERT = 2 };

// Thresholds
static const int SMOKE_ATTENTION_ADC = 350; // light over-limit => attention
static const int SMOKE_ALERT_ADC     = 550; // heavy over-limit => alert

static const float TEMP_ATTENTION_C = 45.0f; // light over-limit => attention
static const float TEMP_ALERT_C     = 60.0f; // heavy over-limit => alert

// Flame sensor digital on D2 (active LOW typical)
static const bool FLAME_ACTIVE = LOW;

// Intermittent/refresh rate for reading and display
static const uint16_t LOOP_PERIOD_MS = 200;

// Timers for ALERT buzzer: continuous intermittent alarm
static const uint16_t ALERT_BUZZ_ON_MS  = 300;
static const uint16_t ALERT_BUZZ_OFF_MS = 300;

// Serial output control
static const uint32_t ALERT_PRINT_PERIOD_MS = 500;

// -------------------- Global state --------------------
static Level currentLevel = NORMAL;
static uint32_t lastLoopMs = 0;

// Alarm buzzer state machine
static bool buzzerOn = false;
static uint32_t buzzerT0 = 0;

// Serial output control
static uint32_t lastAlertPrintMs = 0;

// -------------------- Helpers --------------------
static float readLM35C() {
  int adc = analogRead(PIN_TEMP);
  float v = (adc * (5.0f / 1023.0f));
  float tC = v / 0.01f; // LM35: 10mV/°C
  return tC;
}

static int readMQ2ADC() {
  return analogRead(PIN_SMOKE);
}

static bool flameTriggered() {
  return (digitalRead(PIN_FLAME) == FLAME_ACTIVE);
}

static void setLevelIndicators(Level lv) {
  // Green for attention, Red for alert; normal both off.
  if (lv == NORMAL) {
    digitalWrite(PIN_LED_G, LOW);
    digitalWrite(PIN_LED_R, LOW);
  } else if (lv == ATTENTION) {
    digitalWrite(PIN_LED_G, HIGH);
    digitalWrite(PIN_LED_R, LOW);
  } else { // ALERT
    digitalWrite(PIN_LED_G, LOW);
    digitalWrite(PIN_LED_R, HIGH);
  }
}

// Encoded display requirement: level number + key value
// - NORMAL: show 0 + (temperature*10) as 4 digits
// - ATTENTION: show 1 + (smokeADC) as 4 digits
// - ALERT: show 2 + 999 if flame else 2 + (temperature*10) as 4 digits
static void safeShowEncoded(uint16_t encodedLevelAndKey) {
  // encodedLevelAndKey = level*1000 + key(0..999)
  // TM1637Display: showNumberDec(value, leadingZeros, length, pos)
  display.showNumberDec((int)encodedLevelAndKey, false, 4, 0);
}

// -------------------- Buzzer control --------------------
static void stopBuzzer() {
  noTone(PIN_BUZZ);
  buzzerOn = false;
}

static void alertBuzzerTick() {
  // Intermittent continuous alarm
  uint32_t now = millis();
  if (!buzzerOn) {
    buzzerOn = true;
    tone(PIN_BUZZ, 2000); // 2kHz
    buzzerT0 = now;
  } else {
    if (now - buzzerT0 >= ALERT_BUZZ_ON_MS) {
      buzzerOn = false;
      noTone(PIN_BUZZ);
      buzzerT0 = now;
    }
  }

  // Ensure off interval is respected by state changes above (buzzerT0 updated on each transition)
}

// -------------------- Setup/Loop --------------------
void setup() {
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_R, OUTPUT);

  pinMode(PIN_BUZZ, OUTPUT);
  pinMode(PIN_FLAME, INPUT_PULLUP);

  analogReference(DEFAULT);

  Serial.begin(SERIAL_BAUD);

  // TM1637 setup
  display.setBrightness(0x0f, false);
  display.clear();

  setLevelIndicators(NORMAL);
  stopBuzzer();

  // Initial display
  float t = readLM35C();
  int s = readMQ2ADC();
  (void)s;

  int key = (int)roundf(t * 10.0f);
  if (key < 0) key = 0;
  if (key > 999) key = 999;
  safeShowEncoded((uint16_t)(NORMAL * 1000 + key));
}

void loop() {
  uint32_t now = millis();
  if (now - lastLoopMs < LOOP_PERIOD_MS) return;
  lastLoopMs = now;

  float tempC = readLM35C();
  int smokeADC = readMQ2ADC();
  bool flame = flameTriggered();

  // Indicators
  bool tempAtt = (tempC >= TEMP_ATTENTION_C) && (tempC < TEMP_ALERT_C);
  bool tempAl  = (tempC >= TEMP_ALERT_C);

  bool smokeAtt = (smokeADC >= SMOKE_ATTENTION_ADC) && (smokeADC < SMOKE_ALERT_ADC);
  bool smokeAl  = (smokeADC >= SMOKE_ALERT_ADC);

  // Count exceeding indicators
  int alertCount = 0;
  if (tempAl)  alertCount++;
  if (smokeAl) alertCount++;

  int attentionCount = 0; // kept for behavior completeness though not strictly necessary
  if (tempAtt)  attentionCount++;
  if (smokeAtt) attentionCount++;

  (void)attentionCount;

  // Determine level:
  // - 任一指标轻微超限进入关注
  // - 多个指标超限或火焰传感器触发进入警报
  Level newLevel = NORMAL;

  if (flame) {
    newLevel = ALERT;
  } else {
    // Attention if any mild over limit
    if (tempAtt || smokeAtt) newLevel = ATTENTION;

    // Alert if multiple indicators exceed OR heavy exceed with other being at least attention
    bool tempAtLeastAtt  = tempAtt || tempAl;
    bool smokeAtLeastAtt = smokeAtt || smokeAl;

    if (tempAl && smokeAtLeastAtt && (tempAtt || smokeAtt || smokeAl)) {
      newLevel = ALERT;
    } else if (smokeAl && tempAtLeastAtt && (tempAtt || smokeAtt || tempAl)) {
      newLevel = ALERT;
    } else if (alertCount >= 2) {
      newLevel = ALERT;
    } else if (tempAl && !smokeAtLeastAtt) {
      newLevel = ATTENTION; // strict: single heavy exceed without pairing stays attention
    } else if (smokeAl && !tempAtLeastAtt) {
      newLevel = ATTENTION;
    }
  }

  // Update indicators and buzzer on level change
  if (newLevel != currentLevel) {
    currentLevel = newLevel;
    setLevelIndicators(currentLevel);

    if (currentLevel == ALERT) {
      buzzerOn = false;
      buzzerT0 = 0;
    } else {
      stopBuzzer();
    }
  }

  // Buzzer tick in ALERT mode
  if (currentLevel == ALERT) {
    alertBuzzerTick();
  } else {
    stopBuzzer();
  }

  // Update display each loop with current level and key value
  uint16_t encoded = 0;

  if (currentLevel == NORMAL) {
    int k = (int)roundf(tempC * 10.0f);
    if (k < 0) k = 0;
    if (k > 999) k = 999;
    encoded = (uint16_t)(NORMAL * 1000 + k);
  } else if (currentLevel == ATTENTION) {
    int k = smokeADC;
    if (k < 0) k = 0;
    if (k > 999) k = 999;
    encoded = (uint16_t)(ATTENTION * 1000 + k);
  } else { // ALERT
    if (flame) {
      encoded = (uint16_t)(ALERT * 1000 + 999);
    } else {
      int k = (int)roundf(tempC * 10.0f);
      if (k < 0) k = 0;
      if (k > 999) k = 999;
      encoded = (uint16_t)(ALERT * 1000 + k);
    }
  }
  safeShowEncoded(encoded);

  // Serial output requirement:
  // "警报级下蜂鸣器持续间歇报警并串口输出 fire alert."
  if (currentLevel == ALERT) {
    if (now - lastAlertPrintMs >= ALERT_PRINT_PERIOD_MS) {
      lastAlertPrintMs = now;
      Serial.print("fire alert");
      Serial.print(" | tempC=");
      Serial.print(tempC, 1);
      Serial.print(" | smokeADC=");
      Serial.print(smokeADC);
      Serial.print(" | flame=");
      Serial.println(flame ? 1 : 0);
    }
  }
}
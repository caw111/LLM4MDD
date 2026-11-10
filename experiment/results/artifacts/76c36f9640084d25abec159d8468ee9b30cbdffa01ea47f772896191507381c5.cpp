#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Pin mapping (authoritative) --------------------
static const uint8_t PIN_SMOKE = A0;     // MQ-2 analog
static const uint8_t PIN_TEMP  = A1;     // LM35 analog

static const uint8_t PIN_LED_G = D12;    // Green LED
static const uint8_t PIN_LED_R = D13;    // Red LED

static const uint8_t PIN_BUZZ  = D6;      // Buzzer (tone output)
static const uint8_t PIN_CLK   = D4;      // TM1637 CLK
static const uint8_t PIN_DIO   = D5;      // TM1637 DIO

// -------------------- Display --------------------
TM1637Display display(PIN_CLK, PIN_DIO);

// -------------------- Serial --------------------
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20; // kept as requirement (line/idle framing)

// -------------------- Fire warning logic --------------------
enum Level : uint8_t { NORMAL = 0, ATTENTION = 1, ALERT = 2 };

// Thresholds (calibration/tuning can be adjusted here)
static const int SMOKE_ADC_MIN = 0;          // not used, but kept for clarity
static const int SMOKE_ATTENTION_ADC = 350;  // light over-limit => attention
static const int SMOKE_ALERT_ADC     = 550;  // heavy over-limit => alert

static const float TEMP_ATTENTION_C = 45.0f; // light over-limit => attention
static const float TEMP_ALERT_C     = 60.0f; // heavy over-limit => alert

// Flame sensor digital on D2 (as requirement). We assume active LOW (common for flame modules).
static const uint8_t PIN_FLAME = D2;
static const bool FLAME_ACTIVE = LOW;

// Timers for ALERT buzzer: continuous intermittent alarm
static const uint16_t ALERT_BUZZ_ON_MS  = 300;
static const uint16_t ALERT_BUZZ_OFF_MS = 300;

// Intermittent/refresh rate for reading and display
static const uint16_t LOOP_PERIOD_MS = 200;

// -------------------- Global state --------------------
static Level currentLevel = NORMAL;

static uint32_t lastLoopMs = 0;

// Alarm buzzer state machine
static bool buzzerOn = false;
static uint32_t buzzerT0 = 0;

// Serial output control to avoid excessive printing
static uint32_t lastAlertPrintMs = 0;
static const uint32_t ALERT_PRINT_PERIOD_MS = 500; // repeated "fire alert" in alert level

// -------------------- Helpers --------------------
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readMQ2ADC();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool flameTriggered();
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLevelIndicators(Level lv);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayLevelAndKeyValue(Level lv, float tempC, int smokeADC, bool flame);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void safeShowNumber(int value4digits);
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void stopBuzzer();
#line 148 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void alertBuzzerTick();
#line 174 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool anyAttention();
#line 179 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 203 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C() {
  int adc = analogRead(PIN_TEMP);
  // UNO ADC: 10-bit, Vref=5V
  // LM35: 10mV/°C => V = T * 0.01
  float v = (adc * (5.0f / 1023.0f));
  float tC = v / 0.01f;
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

static void displayLevelAndKeyValue(Level lv, float tempC, int smokeADC, bool flame) {
  // Display requirements:
  // "数码管显示当前等级编号和关键值"
  // We'll show:
  // - left digit: level number (0/1/2)
  // - remaining digits: key value (prefer flame as special code when present)
  // To fit TM1637 (typically 4 digits), we encode:
  //   NORMAL(0): show temperature*10 (e.g., 25.3 => 253) clipped
  //   ATTENTION(1): show smoke ADC (0-999) clipped to 3 digits
  //   ALERT(2): show flame=triggered => 9999 else show temperature*10 (still "关键值")
  //
  // Use setSegments / showNumberDecEx with signless. We'll use colon/dp off.
  display.clear();

  int key = 0;
  if (lv == NORMAL) {
    // temperature*10 for precision
    key = (int)roundf(tempC * 10.0f);
    if (key < 0) key = 0;
    if (key > 999) key = 999;
    // show L + key as 4 digits: [L][key(3digits)]
    int val = (int)lv * 1000 + key; // 0xxx..3xxx
    display.showNumberDecEx(val, 0b00000000, true, 4, 0); // raw mode (library supports var signatures)
  } else if (lv == ATTENTION) {
    // smoke ADC as key (3 digits)
    key = smokeADC;
    if (key < 0) key = 0;
    if (key > 999) key = 999;
    int val = (int)lv * 1000 + key;
    display.showNumberDecEx(val, 0b00000000, true, 4, 0);
  } else { // ALERT
    if (flame) {
      // show 2 + 999 = 2999 to indicate flame critical
      int val = 2000 + 999;
      display.showNumberDecEx(val, 0b00000000, true, 4, 0);
    } else {
      key = (int)roundf(tempC * 10.0f);
      if (key < 0) key = 0;
      if (key > 999) key = 999;
      int val = 2000 + key; // 2xxx
      display.showNumberDecEx(val, 0b00000000, true, 4, 0);
    }
  }
}

// Alternative: some TM1637Display versions differ in showNumberDecEx signature.
// Provide a fallback wrapper to ensure compilation.
static void safeShowNumber(int value4digits) {
  // display.showNumberDec(value, leadingZero, length, pos)
  // We'll attempt the most widely available overload.
  display.showNumberDec(value4digits, false, 4, 0);
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
    // Start tone
    buzzerOn = true;
    tone(PIN_BUZZ, 2000); // 2kHz
    buzzerT0 = now;
  } else {
    // Tone is on
    if (now - buzzerT0 >= ALERT_BUZZ_ON_MS) {
      buzzerOn = false;
      noTone(PIN_BUZZ);
      buzzerT0 = now;
    }
  }

  // Keep off time
  if (!buzzerOn) {
    if (now - buzzerT0 >= ALERT_BUZZ_OFF_MS) {
      // next tick will turn on
      // nothing else here
    }
  }
}

static bool anyAttention() {
  return false;
}

// -------------------- Setup/Loop --------------------
void setup() {
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_R, OUTPUT);

  pinMode(PIN_BUZZ, OUTPUT);
  pinMode(PIN_FLAME, INPUT_PULLUP); // common for active LOW flame modules

  analogReference(DEFAULT);

  Serial.begin(SERIAL_BAUD);

  // TM1637 setup
  display.setBrightness(0x0f, false); // full brightness
  display.clear();

  // Initial
  setLevelIndicators(NORMAL);
  stopBuzzer();
  // Display initial
  float t = readLM35C();
  int s = readMQ2ADC();
  safeShowNumber(0 * 1000 + (int)roundf(t * 10.0f));
}

void loop() {
  uint32_t now = millis();
  if (now - lastLoopMs < LOOP_PERIOD_MS) return;
  lastLoopMs = now;

  float tempC = readLM35C();
  int smokeADC = readMQ2ADC();
  bool flame = flameTriggered();

  // Determine attention/alert based on requirements:
  // - 任一指标轻微超限进入关注
  // - 多个指标超限或火焰传感器触发进入警报
  //
  // We'll treat:
  // - Temperature: attention if >= TEMP_ATTENTION_C and < TEMP_ALERT_C; alert if >= TEMP_ALERT_C
  // - Smoke: attention if >= SMOKE_ATTENTION_ADC and < SMOKE_ALERT_ADC; alert if >= SMOKE_ALERT_ADC
  // - Flame sensor: if triggered => ALERT regardless of other indicators

  bool tempAtt = (tempC >= TEMP_ATTENTION_C) && (tempC < TEMP_ALERT_C);
  bool tempAl  = (tempC >= TEMP_ALERT_C);

  bool smokeAtt = (smokeADC >= SMOKE_ATTENTION_ADC) && (smokeADC < SMOKE_ALERT_ADC);
  bool smokeAl  = (smokeADC >= SMOKE_ALERT_ADC);

  // Count exceeding indicators (for alert escalation requirement)
  int alertCount = 0;
  int attentionCount = 0;

  if (tempAl) { alertCount++; }
  if (smokeAl) { alertCount++; }

  if (tempAtt) { attentionCount++; }
  if (smokeAtt) { attentionCount++; }
  if (tempAtt || tempAl) {
    // temp contributes to attention already counted above for mild only; alert handled separately
    // no additional action here
  }
  if (smokeAtt || smokeAl) {
    // same
  }

  Level newLevel = NORMAL;
  if (flame) {
    newLevel = ALERT;
  } else {
    // Attention if any mild over limit
    if (tempAtt || smokeAtt) {
      newLevel = ATTENTION;
    }

    // Alert if multiple indicators exceed or any heavy exceed
    // "多个指标超限或火焰传感器触发进入警报"
    // We'll treat heavy exceed as one indicator, and "multiple" as >=2 of (tempAl, smokeAl) OR
    // one heavy exceed together with the other indicator being at least attention.
    bool tempAtLeastAtt = tempAtt || tempAl;
    bool smokeAtLeastAtt = smokeAtt || smokeAl;

    if (tempAl && smokeAtLeastAtt && (tempAtt || smokeAtt || smokeAl)) {
      // This covers both "multiple thresholds" and ensures behavior with combined conditions.
      newLevel = ALERT;
    } else if (smokeAl && tempAtLeastAtt && (tempAtt || smokeAtt || tempAl)) {
      newLevel = ALERT;
    } else if (alertCount >= 2) {
      newLevel = ALERT;
    } else if (tempAl && !smokeAtLeastAtt) {
      // Single heavy exceed: still qualifies as "multiple indicators超限" not met,
      // but requirement says "多个指标超限进入警报". However, typical behavior might
      // escalate on heavy exceed alone. To preserve a strict reading, keep ATTENTION
      // unless paired. But requirement also says "任一指标轻微超限关注，多个指标超限或火焰触发警报".
      // So heavy exceed without pairing -> ATTENTION (not ALERT).
      newLevel = ATTENTION;
    } else if (smokeAl && !tempAtLeastAtt) {
      newLevel = ATTENTION;
    }
  }

  // Update outputs only when level changes or always maintain alarm behavior
  if (newLevel != currentLevel) {
    currentLevel = newLevel;
    setLevelIndicators(currentLevel);

    if (currentLevel == ALERT) {
      // Start buzzer tick state
      buzzerOn = false;
      buzzerT0 = 0;
    } else {
      stopBuzzer();
    }
  }

  // Always tick buzzer if in ALERT
  if (currentLevel == ALERT) {
    alertBuzzerTick();
  } else {
    stopBuzzer();
  }

  // Display current level and key value
  // If TM1637 showNumberDecEx signature differs, use safeShowNumber.
  // We'll compute the same encoded 4-digit value used above.
  int keyEncoded = 0;
  if (currentLevel == NORMAL) {
    int k = (int)roundf(tempC * 10.0f);
    if (k < 0) k = 0;
    if (k > 999) k = 999;
    keyEncoded = 0 * 1000 + k;
  } else if (currentLevel == ATTENTION) {
    int k = smokeADC;
    if (k < 0) k = 0;
    if (k > 999) k = 999;
    keyEncoded = 1 * 1000 + k;
  } else { // ALERT
    if (flame) keyEncoded = 2 * 1000 + 999;
    else {
      int k = (int)roundf(tempC * 10.0f);
      if (k < 0) k = 0;
      if (k > 999) k = 999;
      keyEncoded = 2 * 1000 + k;
    }
  }
  safeShowNumber(keyEncoded);

  // Serial output: "警报级下蜂鸣器持续间歇报警并串口输出 fire alert."
  if (currentLevel == ALERT) {
    // Print at a controlled rate to match "持续间歇" without flooding
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

  // Optional: keep serial line idle framing hint by not printing otherwise.
  // (No additional buffering required on Arduino UNO.)
}

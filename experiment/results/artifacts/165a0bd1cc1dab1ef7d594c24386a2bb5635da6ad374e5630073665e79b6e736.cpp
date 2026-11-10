#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------- Pin assignments (authoritative) --------
static const uint8_t PIN_TEMP_LM35   = A1; // LM35 analog in
static const uint8_t PIN_SMOKE_MQ2    = A0; // MQ-2 analog in

static const uint8_t PIN_FLAME_D2    = 2;  // Fire/flame digital input (D2)
static const uint8_t PIN_BUZZER_D6    = 6;  // Buzzer (D6)
static const uint8_t PIN_LED_GREEN   = 12; // Green LED (D12)
static const uint8_t PIN_LED_RED     = 13; // Red LED (D13)

// TM1637: CLK=D4, DIO=D5
static const uint8_t PIN_TM1637_CLK = 4;
static const uint8_t PIN_TM1637_DIO = 5;

// -------- Display --------
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// -------- System behavior thresholds (preserved behavior model) --------
// Notes:
// - Requirement says: one metric slightly over -> "关注"; multiple or flame -> "警报".
// - Because no numeric thresholds were provided, we use common, practical calibration
//   with explicit constants below; behavior remains consistent and deterministic.
// - If your sensors need different thresholds, adjust these constants.
static const int SMOKE_ADC_CAUTION = 350;  // MQ-2 caution threshold (ADC)
static const int SMOKE_ADC_ALARM   = 500;  // MQ-2 alarm threshold (ADC)

static const float TEMP_CAUTION_C  = 45.0f; // LM35 caution threshold in Celsius
static const float TEMP_ALARM_C    = 60.0f; // LM35 alarm threshold in Celsius

// Flame digital input threshold is direct (HIGH triggers alarm)
static const uint8_t FLAME_ACTIVE_STATE = HIGH;

// -------- Alarm / Buzzer pattern (intermittent, continuous in alarm) --------
static const uint16_t BUZZ_ON_MS  = 250;
static const uint16_t BUZZ_OFF_MS = 250;

// -------- Reading timing --------
static const uint16_t SAMPLE_INTERVAL_MS = 200;

// -------- Serial --------
static const uint32_t SERIAL_BAUD = 9600;
static uint32_t lastSampleMs = 0;

// -------- Buzzer state --------
static uint32_t lastBuzzToggleMs = 0;
static bool buzzerOn = false;

// -------- Helper: read temperature (LM35) --------
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readSmokeADC();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static Level computeLevel(bool flame, float tempC, int smokeADC, bool &tempCaution, bool &tempAlarm, bool &smokeCaution, bool &smokeAlarm);
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLeds(Level level);
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t scaleKeyForDisplay(float tempC, int smokeADC, Level level, bool tempCaution, bool tempAlarm, bool smokeCaution, bool smokeAlarm);
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay(Level level, float tempC, int smokeADC, bool tempCaution, bool tempAlarm, bool smokeCaution, bool smokeAlarm);
#line 178 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void serialFireAlertIfNeeded(Level level);
#line 196 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBuzzer(Level level);
#line 220 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 240 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC() {
  int adc = analogRead(PIN_TEMP_LM35); // 0..1023
  // LM35: 10mV/°C, typical with 5V: Vref/1023*adc => volts, C=volts/0.01
  float voltage = (adc * (5.0f / 1023.0f));
  float tempC = voltage / 0.01f; // 10mV per °C
  return tempC;
}

// -------- Helper: read smoke (MQ-2) --------
static int readSmokeADC() {
  return analogRead(PIN_SMOKE_MQ2); // direct ADC-based thresholding
}

// -------- Level mapping --------
enum Level : uint8_t { NORMAL = 1, CAUTION = 2, ALARM = 3 };

// Decide level:
// - If flame triggers -> ALARM immediately
// - Else: count how many metrics are alarm-level or caution-level
//   * One metric slightly over -> CAUTION
//   * Multiple metrics over (caution or alarm) or any alarm-level -> ALARM (as required: multiple 超限 或火焰触发)
// To meet "multiple indicators超限 enters alarm", we treat:
static Level computeLevel(bool flame,
                           float tempC,
                           int smokeADC,
                           bool &tempCaution, bool &tempAlarm,
                           bool &smokeCaution, bool &smokeAlarm) {
  tempCaution = (tempC >= TEMP_CAUTION_C && tempC < TEMP_ALARM_C);
  tempAlarm   = (tempC >= TEMP_ALARM_C);

  smokeCaution = (smokeADC >= SMOKE_ADC_CAUTION && smokeADC < SMOKE_ADC_ALARM);
  smokeAlarm   = (smokeADC >= SMOKE_ADC_ALARM);

  if (flame) return ALARM;

  // Count "exceeded" indicators (any caution or alarm)
  int exceededCount = 0;
  if (tempCaution) exceededCount++;
  if (tempAlarm) exceededCount++;
  if (smokeCaution) exceededCount++;
  if (smokeAlarm) exceededCount++;

  // Multiple exceeded -> ALARM
  if (exceededCount >= 2) return ALARM;

  // Any alarm-level -> ALARM (single metric)
  if (tempAlarm || smokeAlarm) return ALARM;

  // Single caution -> CAUTION
  if (tempCaution || smokeCaution) return CAUTION;

  // Otherwise NORMAL
  return NORMAL;
}

static void setLeds(Level level) {
  // Green: 正常/关注，Red: 警报
  if (level == ALARM) {
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
  } else if (level == CAUTION) {
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
  } else {
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, LOW);
  }
}

// Display format:
// - Requirement: show current level ID and key value on 7-seg.
// We'll show:
//   * digits: level (1/2/3) on left; key value scaled on right (two digits).
// - Since TM1637 is 4 digits, we use:
//   - 4-digit display: [level][key_hi][key_lo?]
// We'll implement a stable 4-digit layout:
//   * NORMAL(1): "1" + "T" not possible. So show temperature*1? We'll use:
//       NORMAL: "1" + tempC integer mod 100 (00..99)
//       CAUTION: "2" + tempC integer mod 100 OR smoke ADC scaled (based on which is the key)
//       ALARM:   "3" + smokeADC scaled mod 100 (typically higher)
// - To satisfy "显示当前等级编号和关键值" we ensure the number and a key value are always updated.
static uint16_t scaleKeyForDisplay(float tempC, int smokeADC, Level level,
                                    bool tempCaution, bool tempAlarm,
                                    bool smokeCaution, bool smokeAlarm) {
  // Choose key value priority:
  // - If alarm because flame -> show smoke key (scaled) if available; else temp
  // - If smoke alarm/caution -> show smoke scaled (smokeADC/10)
  // - If temp alarm/caution -> show temp integer
  // - NORMAL -> show temperature integer
  float keyFloat = 0.0f;

  bool showSmoke = smokeAlarm || smokeCaution;
  bool showTemp = tempAlarm || tempCaution;

  if (level == ALARM) {
    if (showSmoke) keyFloat = smokeADC / 10.0f;       // 0..102 -> mod 100
    else if (showTemp) keyFloat = tempC;
    else keyFloat = tempC;
  } else if (level == CAUTION) {
    if (showSmoke) keyFloat = smokeADC / 10.0f;
    else keyFloat = tempC;
  } else { // NORMAL
    keyFloat = tempC;
  }

  int key = (int)keyFloat;
  if (key < 0) key = 0;
  return (uint16_t)(key % 100); // fit 2 digits for readability
}

static void updateDisplay(Level level, float tempC, int smokeADC,
                           bool tempCaution, bool tempAlarm,
                           bool smokeCaution, bool smokeAlarm) {
  uint16_t key2 = scaleKeyForDisplay(tempC, smokeADC, level,
                                     tempCaution, tempAlarm,
                                     smokeCaution, smokeAlarm);

  // Layout: LKKK? We'll use 4 digits: level as first digit, then key2 as last two digits,
  // and keep two middle digits as 00 for clarity: [level][00][key2(two digits)]
  // Example: level=2 key=37 => "20037"? too many. Instead we do: level*100 + key2 on 4 digits
  // Since level in {1,2,3} and key2 0..99 => range 100..399, fits 4 digits.
  uint16_t shown = (uint16_t)level * 100u + key2; // e.g., 2 -> 200..299
  display.showNumberDecEx(shown, 0b01000000, true); // leading zero false
}

// Serial output for alarm:
// - Requirement: "警报级下蜂鸣器持续间歇报警并串口输出 fire alert."
static void serialFireAlertIfNeeded(Level level) {
  static Level lastLevel = NORMAL;
  if (level == ALARM) {
    if (lastLevel != ALARM) {
      Serial.println("fire alert");
    } else {
      // To avoid flooding, still output periodically during alarm
      static uint32_t lastPrint = 0;
      uint32_t now = millis();
      if (now - lastPrint >= 1000) {
        Serial.println("fire alert");
        lastPrint = now;
      }
    }
  }
  lastLevel = level;
}

static void updateBuzzer(Level level) {
  if (level != ALARM) {
    // Ensure buzzer off in non-alarm
    digitalWrite(PIN_BUZZER_D6, LOW);
    buzzerOn = false;
    return;
  }

  uint32_t now = millis();
  if (!buzzerOn) {
    if (now - lastBuzzToggleMs >= BUZZ_OFF_MS) {
      digitalWrite(PIN_BUZZER_D6, HIGH);
      buzzerOn = true;
      lastBuzzToggleMs = now;
    }
  } else {
    if (now - lastBuzzToggleMs >= BUZZ_ON_MS) {
      digitalWrite(PIN_BUZZER_D6, LOW);
      buzzerOn = false;
      lastBuzzToggleMs = now;
    }
  }
}

void setup() {
  pinMode(PIN_FLAME_D2, INPUT); // flame sensor digital input
  pinMode(PIN_BUZZER_D6, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);

  digitalWrite(PIN_BUZZER_D6, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);

  Serial.begin(SERIAL_BAUD);

  display.setBrightness(0x0f);
  display.clear();

  // Prime timers
  lastSampleMs = millis();
  lastBuzzToggleMs = millis();
}

void loop() {
  uint32_t now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS) return;
  lastSampleMs = now;

  // Read sensors
  float tempC = readTemperatureC();
  int smokeADC = readSmokeADC();
  bool flame = (digitalRead(PIN_FLAME_D2) == FLAME_ACTIVE_STATE);

  // Compute level
  bool tempCaution = false, tempAlarm = false;
  bool smokeCaution = false, smokeAlarm = false;

  Level level = computeLevel(flame, tempC, smokeADC,
                             tempCaution, tempAlarm,
                             smokeCaution, smokeAlarm);

  // Update outputs
  setLeds(level);
  updateBuzzer(level);
  serialFireAlertIfNeeded(level);
  updateDisplay(level, tempC, smokeADC, tempCaution, tempAlarm, smokeCaution, smokeAlarm);

  // Additional serial info (not forbidden; keep minimal).
  // We'll output diagnostic line at a slower cadence during caution/normal.
  static Level lastDiagLevel = NORMAL;
  static uint32_t lastDiagPrint = 0;
  if (level != ALARM) {
    if (level != lastDiagLevel || (now - lastDiagPrint >= 2000)) {
      lastDiagLevel = level;
      lastDiagPrint = now;

      Serial.print("level=");
      Serial.print((int)level);
      Serial.print(", tempC=");
      Serial.print(tempC, 1);
      Serial.print(", smokeADC=");
      Serial.print(smokeADC);
      Serial.print(", flame=");
      Serial.println(flame ? 1 : 0);
    }
  }
}

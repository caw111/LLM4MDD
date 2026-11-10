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
static const int SMOKE_ADC_CAUTION = 350;  // MQ-2 caution threshold (ADC)
static const int SMOKE_ADC_ALARM   = 500;  // MQ-2 alarm threshold (ADC)

static const float TEMP_CAUTION_C  = 45.0f; // LM35 caution threshold in Celsius
static const float TEMP_ALARM_C    = 60.0f; // LM35 alarm threshold in Celsius

static const uint8_t FLAME_ACTIVE_STATE = HIGH;

// -------- Alarm / Buzzer pattern --------
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

// -------- Level mapping --------
enum Level : uint8_t { NORMAL = 1, CAUTION = 2, ALARM = 3 };

// -------- Helper: read temperature (LM35) --------
static float readTemperatureC() {
  int adc = analogRead(PIN_TEMP_LM35); // 0..1023
  float voltage = (adc * (5.0f / 1023.0f));
  float tempC = voltage / 0.01f; // 10mV per °C
  return tempC;
}

// -------- Helper: read smoke (MQ-2) --------
static int readSmokeADC() {
  return analogRead(PIN_SMOKE_MQ2);
}

// Decide level per requirement:
// - Any metric slightly over -> CAUTION
// - Multiple metrics over OR flame -> ALARM
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

  int exceededCount = 0;
  if (tempCaution) exceededCount++;
  if (tempAlarm) exceededCount++;
  if (smokeCaution) exceededCount++;
  if (smokeAlarm) exceededCount++;

  if (exceededCount >= 2) return ALARM;
  if (tempAlarm || smokeAlarm) return ALARM;
  if (tempCaution || smokeCaution) return CAUTION;

  return NORMAL;
}

static void setLeds(Level level) {
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

// Display shows current level ID and a key value.
// Use 4 digits by showing: shown = level*100 + key(0..99)
static uint16_t scaleKeyForDisplay(float tempC, int smokeADC, Level level,
                                    bool tempCaution, bool tempAlarm,
                                    bool smokeCaution, bool smokeAlarm) {
  float keyFloat = 0.0f;

  bool showSmoke = smokeAlarm || smokeCaution;
  bool showTemp  = tempAlarm  || tempCaution;

  if (level == ALARM) {
    if (showSmoke) keyFloat = smokeADC / 10.0f;
    else if (showTemp) keyFloat = tempC;
    else keyFloat = tempC;
  } else if (level == CAUTION) {
    if (showSmoke) keyFloat = smokeADC / 10.0f;
    else keyFloat = tempC;
  } else {
    keyFloat = tempC;
  }

  int key = (int)keyFloat;
  if (key < 0) key = 0;
  return (uint16_t)(key % 100);
}

static void updateDisplay(Level level, float tempC, int smokeADC,
                           bool tempCaution, bool tempAlarm,
                           bool smokeCaution, bool smokeAlarm) {
  uint16_t key2 = scaleKeyForDisplay(tempC, smokeADC, level,
                                     tempCaution, tempAlarm,
                                     smokeCaution, smokeAlarm);

  uint16_t shown = (uint16_t)level * 100u + key2; // range 100..399
  display.showNumberDecEx(shown, 0b01000000, true);  // no leading zeros
}

// Serial output for alarm:
// - Under ALARM: intermittent continuous alarm + serial "fire alert".
static void serialFireAlertIfNeeded(Level level) {
  static Level lastLevel = NORMAL;

  if (level == ALARM) {
    if (lastLevel != ALARM) {
      Serial.println("fire alert");
    } else {
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
  pinMode(PIN_FLAME_D2, INPUT);
  pinMode(PIN_BUZZER_D6, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);

  digitalWrite(PIN_BUZZER_D6, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);

  Serial.begin(SERIAL_BAUD);

  display.setBrightness(0x0f);
  display.clear();

  lastSampleMs = millis();
  lastBuzzToggleMs = millis();
}

void loop() {
  uint32_t now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS) return;
  lastSampleMs = now;

  float tempC = readTemperatureC();
  int smokeADC = readSmokeADC();
  bool flame = (digitalRead(PIN_FLAME_D2) == FLAME_ACTIVE_STATE);

  bool tempCaution = false, tempAlarm = false;
  bool smokeCaution = false, smokeAlarm = false;

  Level level = computeLevel(flame, tempC, smokeADC,
                             tempCaution, tempAlarm,
                             smokeCaution, smokeAlarm);

  setLeds(level);
  updateBuzzer(level);
  serialFireAlertIfNeeded(level);
  updateDisplay(level, tempC, smokeADC, tempCaution, tempAlarm, smokeCaution, smokeAlarm);

  // Minimal additional diagnostics (not required to be removed)
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
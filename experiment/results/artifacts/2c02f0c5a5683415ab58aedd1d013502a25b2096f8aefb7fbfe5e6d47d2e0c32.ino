#include <Arduino.h>
#include <TM1637Display.h>

// ========= Pin mapping (authoritative, as provided) =========
static const uint8_t PIN_LM35_A1 = A1;        // analog input
static const uint8_t PIN_BTN1_D2 = 2;        // digital input
static const uint8_t PIN_BTN2_D3 = 3;        // digital input
static const uint8_t PIN_BTN3_D4 = 4;        // digital input

static const uint8_t PIN_LED_D13 = 13;       // digital output
static const uint8_t PIN_BUZZER_D6 = 6;     // tone output

static const uint8_t PIN_TM1637_CLK_D7 = 7;  // CLK=D7
static const uint8_t PIN_TM1637_DIO_D5 = 5;  // DIO=D5

static const uint32_t SERIAL_BAUD = 9600;

// ========= TM1637 =========
TM1637Display display(PIN_TM1637_CLK_D7, PIN_TM1637_DIO_D5);

// ========= LM35 calibration & thresholds =========
static const float VREF_mV = 5000.0f;
static const float LM35_mV_per_C = 10.0f;

// Optional hysteresis for beep threshold stability
static const float TEMP_HYSTERESIS_C = 0.3f;

// ========= Cooking stages =========
static const uint8_t STAGES = 3;
enum StageIndex : uint8_t { PREHEAT = 0, COOK = 1, KEEP_WARM = 2 };

struct StageConfig {
  float targetTempC;
  uint32_t durationSec;
};

StageConfig stages[STAGES] = {
  { 60.0f, 180 },   // Preheat: 60°C, 3 min
  { 180.0f, 300 },  // Cook: 180°C, 5 min
  { 70.0f, 300 }    // Keep warm: 70°C, 5 min
};

// ========= State machine =========
enum SystemState : uint8_t {
  IDLE = 0,
  CONFIG = 1,
  RUNNING = 2,
  FINISHED = 3
};

static SystemState sysState = IDLE;
static StageIndex currentStage = PREHEAT;

// Timer bookkeeping
static uint32_t stageStartMs = 0;
static uint32_t stageEndMs = 0;
static bool stageTempReachedBeeped = false;

// A periodic display refresh
static uint32_t lastDisplayMs = 0;
static const uint32_t DISPLAY_REFRESH_MS = 100;

// ========= Buzzer control =========
static uint32_t buzzerUntilMs = 0;

static void buzzerBeep(uint16_t durationMs = 150, uint16_t frequencyHz = 2000) {
  buzzerUntilMs = millis() + durationMs;
  tone(PIN_BUZZER_D6, frequencyHz, durationMs);
}

static void buzzerUpdate() {
  if (buzzerUntilMs != 0 && (int32_t)(millis() - buzzerUntilMs) >= 0) {
    noTone(PIN_BUZZER_D6);
    buzzerUntilMs = 0;
  }
}

// ========= Buttons (with debounce) =========
struct Button {
  uint8_t pin;
  bool lastStable;
  bool lastRaw;
  uint32_t lastDebounceMs;
};

static Button btn2 = { PIN_BTN1_D2, HIGH, HIGH, 0 };
static Button btn3 = { PIN_BTN2_D3, HIGH, HIGH, 0 };
static Button btn4 = { PIN_BTN3_D4, HIGH, HIGH, 0 };

static void beginButton(Button &b) {
  pinMode(b.pin, INPUT_PULLUP);
  b.lastStable = digitalRead(b.pin);
  b.lastRaw = b.lastStable;
  b.lastDebounceMs = 0;
}

// returns true when a press event (transition to LOW after debounce) occurs
static bool updateButtonState(Button &b) {
  bool raw = digitalRead(b.pin);

  if (raw != b.lastRaw) {
    b.lastDebounceMs = millis();
    b.lastRaw = raw;
  }

  if ((millis() - b.lastDebounceMs) > 25) {
    if (raw != b.lastStable) {
      b.lastStable = raw;
      return (b.lastStable == LOW);
    }
  }
  return false;
}

static uint32_t nextDurationPresetSec(uint32_t current) {
  const uint32_t presets[] = { 60, 120, 180, 240, 300, 420, 600, 900 };
  const uint8_t N = sizeof(presets) / sizeof(presets[0]);

  for (uint8_t i = 0; i < N; i++) {
    if (presets[i] == current) return presets[(i + 1) % N];
  }
  return presets[0];
}

// ========= Temperature reading =========
static float readTemperatureC() {
  int adc = analogRead(PIN_LM35_A1);
  float mV = (adc * VREF_mV) / 1023.0f;
  float tempC = mV / LM35_mV_per_C;
  return tempC;
}

// ========= Display formatting =========
static void showStageAndRemainingSafe(StageIndex stage, uint32_t remainingSec) {
  uint8_t stageNumber = (stage == PREHEAT) ? 1 : (stage == COOK) ? 2 : 3;
  uint32_t r = remainingSec;
  if (r > 999) r = 999;

  uint16_t val = (uint16_t)stageNumber * 1000u + (uint16_t)r;
  display.showNumberDecEx(val, 0, true);
}

// ========= Logging =========
static void logStageConfig() {
  Serial.print(F("[CFG] Preheat: T="));
  Serial.print(stages[PREHEAT].targetTempC, 1);
  Serial.print(F("C, t="));
  Serial.print(stages[PREHEAT].durationSec);
  Serial.println(F("s"));

  Serial.print(F("[CFG] Cook:    T="));
  Serial.print(stages[COOK].targetTempC, 1);
  Serial.print(F("C, t="));
  Serial.print(stages[COOK].durationSec);
  Serial.println(F("s"));

  Serial.print(F("[CFG] Keep:    T="));
  Serial.print(stages[KEEP_WARM].targetTempC, 1);
  Serial.print(F("C, t="));
  Serial.print(stages[KEEP_WARM].durationSec);
  Serial.println(F("s"));
}

static void startStage(StageIndex st) {
  currentStage = st;
  stageTempReachedBeeped = false;
  stageStartMs = millis();
  stageEndMs = stageStartMs + stages[currentStage].durationSec * 1000UL;

  Serial.print(F("[RUN] Stage "));
  Serial.print((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
  Serial.print(F(" start. TargetT="));
  Serial.print(stages[currentStage].targetTempC, 1);
  Serial.print(F("C, Duration="));
  Serial.print(stages[currentStage].durationSec);
  Serial.println(F("s"));

  digitalWrite(PIN_LED_D13, HIGH);
}

// ========= Setup =========
void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial) { }

  pinMode(PIN_LED_D13, OUTPUT);
  digitalWrite(PIN_LED_D13, LOW);

  pinMode(PIN_BUZZER_D6, OUTPUT);
  noTone(PIN_BUZZER_D6);

  beginButton(btn2);
  beginButton(btn3);
  beginButton(btn4);

  display.setBrightness(0x0f);
  display.clear();

  Serial.println(F("=== 三阶段烹饪辅助系统启动 ==="));
  logStageConfig();

  sysState = CONFIG;
  currentStage = PREHEAT;
  Serial.println(F("[STATE] CONFIG (可设置各阶段目标温度/持续时间)"));
}

// ========= Loop =========
void loop() {
  buzzerUpdate();

  // Buttons update
  bool press2 = updateButtonState(btn2); // increase target temp in CONFIG
  bool press3 = updateButtonState(btn3); // decrease target temp in CONFIG
  bool press4 = updateButtonState(btn4); // duration preset + stage advance/start

  // Read temperature continuously
  float tempC = readTemperatureC();

  // ====== Configuration handling ======
  if (sysState == CONFIG || sysState == IDLE) {
    if (millis() - lastDisplayMs >= DISPLAY_REFRESH_MS) {
      lastDisplayMs = millis();
      showStageAndRemainingSafe(currentStage, stages[currentStage].durationSec);
    }

    // D2/D3 adjust target temperature for current stage
    float step = (currentStage == PREHEAT) ? 1.0f : (currentStage == COOK) ? 5.0f : 1.0f;

    if (press2) {
      stages[currentStage].targetTempC += step;
      if (stages[currentStage].targetTempC > 350.0f) stages[currentStage].targetTempC = 350.0f;

      Serial.print(F("[CFG] Stage "));
      Serial.print((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
      Serial.print(F(" targetTempC++ => "));
      Serial.print(stages[currentStage].targetTempC, 1);
      Serial.println(F("C"));
    }

    if (press3) {
      stages[currentStage].targetTempC -= step;
      if (stages[currentStage].targetTempC < 0.0f) stages[currentStage].targetTempC = 0.0f;

      Serial.print(F("[CFG] Stage "));
      Serial.print((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
      Serial.print(F(" targetTempC-- => "));
      Serial.print(stages[currentStage].targetTempC, 1);
      Serial.println(F("C"));
    }

    if (press4) {
      stages[currentStage].durationSec = nextDurationPresetSec(stages[currentStage].durationSec);

      Serial.print(F("[CFG] Stage "));
      Serial.print((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
      Serial.print(F(" duration preset => "));
      Serial.print(stages[currentStage].durationSec);
      Serial.println(F("s"));

      // advance stage after duration set
      if (currentStage < (StageIndex)(STAGES - 1)) {
        currentStage = (StageIndex)(currentStage + 1);
        Serial.print(F("[CFG] Move to next stage: "));
        Serial.println((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
      } else {
        sysState = RUNNING;
        display.clear();
        digitalWrite(PIN_LED_D13, LOW);

        Serial.println(F("[STATE] Start RUNNING"));
        logStageConfig();

        startStage(PREHEAT);
      }
    }

    static uint32_t lastCfgTempLogMs = 0;
    if (millis() - lastCfgTempLogMs >= 1000) {
      lastCfgTempLogMs = millis();
      Serial.print(F("[CFG] Current temp="));
      Serial.print(tempC, 1);
      Serial.print(F("C, Editing stage "));
      Serial.print((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
      Serial.print(F(" (Ttarget="));
      Serial.print(stages[currentStage].targetTempC, 1);
      Serial.print(F("C, t="));
      Serial.print(stages[currentStage].durationSec);
      Serial.println(F("s)"));
    }

    return;
  }

  // ====== Running handling ======
  if (sysState == RUNNING) {
    uint32_t now = millis();

    uint32_t remainingSec = 0;
    if (now < stageEndMs) {
      remainingSec = (stageEndMs - now) / 1000UL;
    }

    if (now - lastDisplayMs >= DISPLAY_REFRESH_MS) {
      lastDisplayMs = now;
      showStageAndRemainingSafe(currentStage, remainingSec);
    }

    // Beep when temperature reaches target (once per stage)
    float target = stages[currentStage].targetTempC;
    bool reached = tempC >= (target - TEMP_HYSTERESIS_C);

    if (reached && !stageTempReachedBeeped) {
      stageTempReachedBeeped = true;

      Serial.print(F("[BEEP] Stage "));
      Serial.print((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
      Serial.print(F(" target reached. temp="));
      Serial.print(tempC, 1);
      Serial.print(F("C >= "));
      Serial.print(target, 1);
      Serial.println(F("C"));

      buzzerBeep(200, 2200);
      digitalWrite(PIN_LED_D13, HIGH);
    }

    // Periodic temp log during run
    static uint32_t lastRunTempLogMs = 0;
    if (now - lastRunTempLogMs >= 1500) {
      lastRunTempLogMs = now;
      Serial.print(F("[RUN] Stage "));
      Serial.print((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
      Serial.print(F(": temp="));
      Serial.print(tempC, 1);
      Serial.print(F("C, target="));
      Serial.print(target, 1);
      Serial.print(F("C, remaining="));
      Serial.print(remainingSec);
      Serial.println(F("s"));
    }

    // Stage end -> automatic transition
    if (now >= stageEndMs) {
      Serial.print(F("[RUN] Stage "));
      Serial.print((currentStage == PREHEAT) ? 1 : (currentStage == COOK) ? 2 : 3);
      Serial.println(F(" end. Remaining=0s"));

      buzzerBeep(250, 2500);
      digitalWrite(PIN_LED_D13, LOW);

      if (currentStage < (StageIndex)(STAGES - 1)) {
        startStage((StageIndex)(currentStage + 1));
      } else {
        sysState = FINISHED;
        Serial.println(F("[STATE] FINISHED (all stages complete)"));
        display.clear();

        for (int i = 0; i < 3; i++) {
          buzzerBeep(120, 2600);
          delay(160);
        }
        digitalWrite(PIN_LED_D13, HIGH);
      }
    }

    return;
  }

  // ====== Finished handling ======
  if (sysState == FINISHED) {
    if (millis() - lastDisplayMs >= 300) {
      lastDisplayMs = millis();
      showStageAndRemainingSafe(KEEP_WARM, 0);
    }

    static uint32_t lastBlinkMs = 0;
    if (millis() - lastBlinkMs >= 500) {
      lastBlinkMs = millis();
      digitalWrite(PIN_LED_D13, !digitalRead(PIN_LED_D13));
    }

    return;
  }
}
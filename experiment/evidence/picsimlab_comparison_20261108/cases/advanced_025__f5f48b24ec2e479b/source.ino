#include <Arduino.h>
#include <TM1637Display.h>

// ========= Pin mapping (authoritative, as provided) =========
static const uint8_t PIN_LM35_A1 = A1;  // analog input
static const uint8_t PIN_BTN1_D2  = 2;   // stage param buttons / control
static const uint8_t PIN_BTN2_D3  = 3;
static const uint8_t PIN_BTN3_D4  = 4;

static const uint8_t PIN_LED_D13  = 13;

static const uint8_t PIN_BUZZER_D6  = 6;

static const uint8_t PIN_TM1637_CLK_D7 = 7;
static const uint8_t PIN_TM1637_DIO_D5 = 5;

static const uint32_t SERIAL_BAUD = 9600;

// ========= TM1637 =========
TM1637Display display(PIN_TM1637_CLK_D7, PIN_TM1637_DIO_D5);

// ========= LM35 calibration & thresholds =========
// LM35 outputs 10mV/°C. With UNO 5V ADC: °C = (ADC/1023)*5000mV / 10mV
// To be robust, keep 5.0V assumption; behavior can be adjusted here if needed.
static const float VREF_mV = 5000.0f;
static const float LM35_mV_per_C = 10.0f;

// We will treat "temperature reaches target" with optional hysteresis to avoid chatter.
static const float TEMP_HYSTERESIS_C = 0.3f;

// ========= Cooking stages =========
// The sketch supports 3 stages: Preheat, Cook, Keep Warm.
// Each stage has adjustable target temperature and duration.
// NOTE: Without explicit default values, we provide safe meaningful defaults
// and allow live adjustment via D2/D3/D4 buttons (defined below).
//
// Button behavior (D2/D3/D4):
// - D2: Increase target temperature (for current stage) while stopped or during config.
// - D3: Decrease target temperature (for current stage) while stopped or during config.
// - D4: Set duration to next preset / advance state in a controlled way.
//
// Since the circuit description only lists pins, we preserve required behaviors:
// "每个阶段可设置目标温度和持续时间" -> implement interactive setting.
// "阶段切换" -> automatic when time ends, and beep when reaching temperature.
// "串口输出完整流程日志" -> implemented.
// If you want different button semantics, update the handlers.
// =========

// Defaults (can be modified via buttons)
static const uint8_t STAGES = 3;
enum StageIndex : uint8_t { PREHEAT = 0, COOK = 1, KEEP_WARM = 2 };

struct StageConfig {
  float targetTempC;
  uint32_t durationSec;
};

StageConfig stages[STAGES] = {
  { 60.0f, 180 },   // Preheat: 60°C, 3 min
  { 180.0f, 300 }, // Cook: 180°C, 5 min (typical for some oven)
  { 70.0f, 300 }    // Keep warm: 70°C, 5 min
};

// ========= State machine =========
enum SystemState : uint8_t {
  IDLE = 0,
  CONFIG = 1,     // adjusting stage configs (shown on display)
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
  // tone() is time-limited; ensure stop in case of early state transitions
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

static bool updateButton(Button &b) {
  bool raw = digitalRead(b.pin);
  if (raw != b.lastRaw) {
    b.lastDebounceMs = millis();
    b.lastRaw = raw;
  }
  if ((millis() - b.lastDebounceMs) > 25) {
    if (raw != b.lastStable) {
      b.lastStable = raw;
      // We consider press when transitioning to LOW (due to pull-up)
      return (b.lastStable == LOW);
    }
  }
  return false;
}

static uint32_t nextDurationPresetSec(uint32_t current) {
  // Presets that span reasonable cooking times.
  // This provides "持续时间可设置" even without additional input like encoder.
  // Used by D4 while in CONFIG.
  const uint32_t presets[] = { 60, 120, 180, 240, 300, 420, 600, 900 };
  const uint8_t N = sizeof(presets) / sizeof(presets[0]);

  for (uint8_t i = 0; i < N; i++) {
    if (presets[i] == current) {
      return presets[(i + 1) % N];
    }
  }
  // If current not in presets, snap to first preset
  return presets[0];
}

// ========= Temperature reading =========
static float readTemperatureC() {
  int adc = analogRead(PIN_LM35_A1);
  // adc -> mV -> °C
  float mV = (adc * VREF_mV) / 1023.0f;
  float tempC = mV / LM35_mV_per_C;
  return tempC;
}

// ========= Display formatting =========
// Need: "数码管显示当前阶段编号和剩余时间"
// We'll use 4 digits: [stage][tens][ones][x?]. With TM1637 4-digit typical.
// We'll map as: digit1=stage (1..3) and last three digits show remaining seconds capped to 999.
// If remaining exceeds 999, show 999.
static void showStageAndRemaining(StageIndex stage, uint32_t remainingSec) {
  uint8_t stageNumber = (stage == PREHEAT) ? 1 : (stage == COOK) ? 2 : 3;

  uint32_t r = remainingSec;
  if (r > 999) r = 999;

  uint8_t d1 = stageNumber;          // thousands digit
  uint8_t d2 = (r / 100) % 10;      // hundreds
  uint8_t d3 = (r / 10) % 10;       // tens
  uint8_t d4 = (r % 10);            // ones

  // TM1637Display supports showNumberDecEx; but stage number in thousands digit with leading zeros control
  // We'll directly show a 4-digit number: stageNumber*1000 + r
  uint16_t val = (uint16_t)d1 * 1000u + (uint16_t)r;
  display.showNumberDecEx(val, 0b01000000, true); // dots off; leading zeros handled by segment bits
  // Note: we set the "leading zeros" by using the third arg true; segment flags vary.
  // This still produces the intended digit layout on most TM1637 libraries.
}

// Override dots setting to reduce accidental segments:
// Many TM1637 libs ignore unused bits; this keeps stable output.
static void showStageAndRemainingSafe(StageIndex stage, uint32_t remainingSec) {
  uint8_t stageNumber = (stage == PREHEAT) ? 1 : (stage == COOK) ? 2 : 3;
  uint32_t r = remainingSec;
  if (r > 999) r = 999;

  uint16_t val = (uint16_t)stageNumber * 1000u + (uint16_t)r;
  // showNumberDecEx expects up to 4 digits. Keep colon/dot off.
  display.showNumberDecEx(val, 0, true);
}

// ========= Logging =========
static void logLine(const __FlashStringHelper *msg) {
  Serial.println(msg);
}

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
  while (!Serial) { /* wait */ }

  pinMode(PIN_LED_D13, OUTPUT);
  digitalWrite(PIN_LED_D13, LOW);

  // Buzzer
  pinMode(PIN_BUZZER_D6, OUTPUT);
  noTone(PIN_BUZZER_D6);

  // Buttons
  beginButton(btn2);
  beginButton(btn3);
  beginButton(btn4);

  // TM1637
  display.setBrightness(0x0f); // 0..7 depends on library; keep full brightness when supported
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

  // Update buttons
  bool press2 = updateButton(btn2); // increase temp or maybe start?
  bool press3 = updateButton(btn3); // decrease temp
  bool press4 = updateButton(btn4); // next preset / advance / start

  // Read temperature continuously
  float tempC = readTemperatureC();

  // ====== Configuration handling ======
  if (sysState == CONFIG || sysState == IDLE) {
    // In CONFIG, show "stage number + remaining time" as 1000..???.
    // For config mode, interpret remaining as durationSec capped.
    if (millis() - lastDisplayMs >= DISPLAY_REFRESH_MS) {
      lastDisplayMs = millis();
      showStageAndRemainingSafe(currentStage, stages[currentStage].durationSec);
    }

    // D2/D3 adjust target temp of current stage in steps.
    // Step size depends on stage: preheat small increments, cook larger.
    float step = (currentStage == PREHEAT) ? 1.0f : (currentStage == COOK) ? 5.0f : 1.0f;

    if (press2) {
      stages[currentStage].targetTempC += step;
      // Clamp to reasonable bounds
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

    // D4:
    // - First press: advance duration preset for current stage
    // - Long sequences: after adjusting keep stage, wrap to start system.
    // To preserve "do not simplify away behavior", we keep deterministic:
    // Press D4:
    //   - change duration preset for current stage
    //   - then advance to next stage (cycles through 3 stages)
    //   - after KEEP_WARM, next D4 starts RUNNING.
    if (press4) {
      // update duration preset
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
        // all configured, start running
        sysState = RUNNING;
        display.clear();
        digitalWrite(PIN_LED_D13, LOW);

        Serial.println(F("[STATE] Start RUNNING"));
        logStageConfig();

        startStage(PREHEAT);
      }
    }

    // Keep a periodic temperature log in config mode (lightweight)
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

    // Remaining time
    int32_t remainingMs = (int32_t)(stageEndMs - now);
    if (remainingMs < 0) remainingMs = 0;
    uint32_t remainingSec = remainingMs / 1000UL;

    // Display stage and remaining time
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

    // Periodic temp log during run (to provide "完整流程日志")
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
      Serial.print(F(" end. Remaining=0s"));
      Serial.println();

      // End-of-stage beep
      buzzerBeep(250, 2500);
      digitalWrite(PIN_LED_D13, LOW);

      if (currentStage < (StageIndex)(STAGES - 1)) {
        // Next stage
        startStage((StageIndex)(currentStage + 1));
      } else {
        // All done
        sysState = FINISHED;
        Serial.println(F("[STATE] FINISHED (all stages complete)"));
        display.clear();
        // Keep a finishing beep pattern
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
    // Show stage "3" and 000 as a simple indication
    if (millis() - lastDisplayMs >= 300) {
      lastDisplayMs = millis();
      showStageAndRemainingSafe(KEEP_WARM, 0);
    }
    // Optional: blink LED
    static uint32_t lastBlinkMs = 0;
    if (millis() - lastBlinkMs >= 500) {
      lastBlinkMs = millis();
      digitalWrite(PIN_LED_D13, !digitalRead(PIN_LED_D13));
    }
    return;
  }
}
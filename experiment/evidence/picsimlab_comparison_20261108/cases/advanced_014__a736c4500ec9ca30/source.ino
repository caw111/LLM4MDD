#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_VIBRATION = 2;   // D2
static const uint8_t PIN_BUTTON    = 3;   // D3
static const uint8_t PIN_LED        = 13;  // D13
static const uint8_t PIN_BUZZER     = 6;   // D6
static const uint8_t PIN_WATER_A0   = A0;  // A0

static const uint8_t TM1637_CLK = 4;  // D4
static const uint8_t TM1637_DIO = 5;  // D5

// ------------ Washing stage logic / thresholds ------------
static const uint32_t STAGE_DEBOUNCE_MS = 80;

// Vibration detection
// Use a simple amplitude/proxy threshold on vibration sensor output.
// If your vibration module outputs analog, keep it digital with comparator threshold;
// but requirement says "振动传感器 D2" so we treat it as digital input.
static const uint32_t NO_VIBRATION_MAX_MS = 180000UL;  // "long time" without vibration -> fault

// Water level thresholds (A0 analog)
static const int WATER_EMPTY_TH = 250;   // below => empty/abnormal
static const int WATER_LOW_TH   = 450;   // low => partial fill
static const int WATER_OK_TH    = 650;   // adequate water

// Water abnormal criteria
static const uint32_t WATER_ABNORMAL_MAX_MS = 120000UL; // abnormal for long time -> fault

// Stage durations
static const uint32_t WATER_FILL_MIN_MS  = 15000UL; // keep in "进水" at least some time
static const uint32_t WASH_MIN_MS         = 60000UL; // "洗涤"
static const uint32_t SPIN_MIN_MS        = 30000UL; // "甩干"

// Fault detection tracking
static uint32_t tLastVibrationMs = 0;
static uint32_t tWaterAbnormalStartMs = 0;

// Stage numbering displayed on TM1637
// 1: 进水, 2: 洗涤, 3: 甩干, 4: 结束
enum Stage : uint8_t { ST_FILL = 1, ST_WASH = 2, ST_SPIN = 3, ST_END = 4 };

static Stage curStage = ST_FILL;
static Stage lastStableStage = ST_FILL;
static uint32_t stageEnterMs = 0;
static uint32_t stageStableSinceMs = 0;

// End confirmation
static bool endLatched = false;
static bool buzzerActive = false;

// Button confirmation
static const bool BUTTON_ACTIVE_LOW = true;
static const uint32_t BUTTON_DEBOUNCE_US = 20000UL;
static uint32_t btnLastChangeUs = 0;
static bool btnLastReading = true;

// Display
TM1637Display display(TM1637_CLK, TM1637_DIO);

// ---------- Helpers ----------
static inline int readWaterRaw() {
  return analogRead(PIN_WATER_A0);
}

static inline bool readVibration() {
  return digitalRead(PIN_VIBRATION) == HIGH;
}

static inline bool readButtonRaw() {
  bool r = (digitalRead(PIN_BUTTON) == HIGH);
  if (BUTTON_ACTIVE_LOW) return !r;
  return r;
}

static void setBuzzer(bool on) {
  // Requirement: buzzer on in end stage and flash LED
  if (on) {
    // For a buzzer, tone works well. Keep a fixed frequency.
    tone(PIN_BUZZER, 2000);
  } else {
    noTone(PIN_BUZZER);
  }
}

static void displayStageNumber(Stage s) {
  // Show stage number 1..4
  display.showNumberDec((int)s, false, 4, 0); // 4 digits with leading zeros suppressed
}

static bool isWaterAbnormal(int waterRaw) {
  // "水位异常" when too low or extremely high outside range.
  if (waterRaw < WATER_EMPTY_TH) return true;
  // If you want upper bound too, set a generous threshold:
  if (waterRaw > 900) return true;
  return false;
}

static bool isWaterEnoughForNext(int waterRaw) {
  // Enough to proceed from fill to wash/spin.
  return waterRaw >= WATER_OK_TH;
}

static bool isWaterLow(int waterRaw) {
  return waterRaw < WATER_LOW_TH;
}

// ---------- Setup / Loop ----------
void setup() {
  pinMode(PIN_VIBRATION, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP); // default for D3 (active-low)
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  analogReference(DEFAULT);

  Serial.begin(9600);

  display.setBrightness(0x0f, true); // max with auto dim
  display.clear();

  tLastVibrationMs = millis();
  stageEnterMs = millis();
  stageStableSinceMs = millis();
  lastStableStage = curStage;
  endLatched = false;
  buzzerActive = false;

  setBuzzer(false);
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  uint32_t nowMs = millis();

  // Read inputs
  bool vib = readVibration();
  int waterRaw = readWaterRaw();

  // Button debounced (confirm end stage)
  bool btnPressed = false;
  bool btnReading = readButtonRaw();
  // debouncing
  if (btnReading != btnLastReading) {
    uint32_t nowUs = micros();
    if (nowUs - btnLastChangeUs > BUTTON_DEBOUNCE_US) {
      btnLastChangeUs = nowUs;
      btnLastReading = btnReading;
      // detect rising edge of "pressed" (active-high after inversion)
      if (btnReading) btnPressed = true;
    }
  }

  // Update vibration fault timer
  if (vib) {
    tLastVibrationMs = nowMs;
  }

  // Update water abnormal timer
  bool waterAbn = isWaterAbnormal(waterRaw);
  if (waterAbn) {
    if (tWaterAbnormalStartMs == 0) tWaterAbnormalStartMs = nowMs;
  } else {
    tWaterAbnormalStartMs = 0;
  }

  // Handle end stage latching behavior
  if (curStage == ST_END) {
    endLatched = true;

    // Buzzer鸣叫并闪烁LED直到用户按键确认
    // Use non-blocking blinking
    const uint32_t blinkPeriodMs = 500;
    uint32_t phase = (nowMs / blinkPeriodMs) & 0x01;

    if (phase == 0) {
      digitalWrite(PIN_LED, HIGH);
      setBuzzer(true);
    } else {
      digitalWrite(PIN_LED, LOW);
      setBuzzer(true); // buzzer continues鸣叫
    }

    displayStageNumber(ST_END);

    if (btnPressed) {
      // Stop alarm and reset to fill stage
      endLatched = false;
      buzzerActive = false;
      setBuzzer(false);
      digitalWrite(PIN_LED, LOW);

      // Reset tracking timers and stage
      curStage = ST_FILL;
      lastStableStage = ST_FILL;
      stageEnterMs = nowMs;
      stageStableSinceMs = nowMs;
      tWaterAbnormalStartMs = 0;
      tLastVibrationMs = nowMs;
      display.clear();
    }

    // No further stage logic while ending
    return;
  }

  // Fault handling: 若长时间无振动且水位异常，则串口输出 wash fault
  // (and do not force end unless user presses; stage continues, preserving behavior)
  static bool faultPrinted = false;
  bool faultCondition = (nowMs - tLastVibrationMs > NO_VIBRATION_MAX_MS) && (tWaterAbnormalStartMs != 0) &&
                         (nowMs - tWaterAbnormalStartMs > WATER_ABNORMAL_MAX_MS);

  if (faultCondition && !faultPrinted) {
    Serial.println("wash fault");
    faultPrinted = true;
  }
  if (!faultCondition) {
    faultPrinted = false;
  }

  // Determine potential stage transitions based on vibration/water
  // Basic state machine:
  // - Fill: wait until water enough, plus min fill time
  // - Wash: require vibration present (for some time) while water is not too low
  // - Spin: require less water requirement and vibration strong/present for spin min time
  // - End: when wash/spin mins met and conditions indicate finish (vibration absent indicates done)
  //
  // Note: We preserve requested behavior by using vibration and water to judge stage.

  // Debounced stage transitions
  Stage nextStage = curStage;

  // Current stable stage entry gating
  uint32_t stageElapsedMs = nowMs - stageEnterMs;

  if (curStage == ST_FILL) {
    // Stay in fill until water enough and min time elapsed
    if (stageElapsedMs >= WATER_FILL_MIN_MS && isWaterEnoughForNext(waterRaw)) {
      nextStage = ST_WASH;
    } else {
      // If water is abnormal for long time already, keep stage, allow fault printing
      nextStage = ST_FILL;
    }
  } else if (curStage == ST_WASH) {
    // Must have vibration occasionally and water not too low
    // If water becomes too low early, treat as still wash (fault may print)
    bool vibOccurring = vib;

    // Transition to spin when:
    // - wash time elapsed minimum
    // - vibration has been present recently
    bool recentVibration = (nowMs - tLastVibrationMs < 30000UL);
    if (stageElapsedMs >= WASH_MIN_MS && vibOccurring && recentVibration && !isWaterLow(waterRaw)) {
      nextStage = ST_SPIN;
    }
  } else if (curStage == ST_SPIN) {
    bool recentVibration = (nowMs - tLastVibrationMs < 30000UL);
    // Transition to end when:
    // - spin time minimum elapsed
    // - vibration stops (indicates done)
    bool vibrationStopped = (nowMs - tLastVibrationMs > 60000UL);
    if (stageElapsedMs >= SPIN_MIN_MS && vibrationStopped && recentVibration == false) {
      nextStage = ST_END;
    } else {
      nextStage = ST_SPIN;
    }
  }

  // Stage transition debouncing
  if (nextStage != curStage) {
    // candidate stage
    if (nextStage != lastStableStage) {
      lastStableStage = nextStage;
      stageStableSinceMs = nowMs;
    }
    // confirm candidate stable for debounce window
    if (nowMs - stageStableSinceMs >= STAGE_DEBOUNCE_MS) {
      curStage = nextStage;
      stageEnterMs = nowMs;
      stageStableSinceMs = nowMs;

      // When entering a stage, update display
      displayStageNumber(curStage);

      // Turn off alarm/buzzer except end stage
      setBuzzer(false);
      digitalWrite(PIN_LED, LOW);
    }
  } else {
    // keep stable; update display for current stage
    displayStageNumber(curStage);
  }

  // Non-end: keep LED off except maybe show stage indicator? Requirement only specifies display stage
  // So LED used only for end-stage blinking.
  digitalWrite(PIN_LED, LOW);

  // Ensure buzzer off during non-end
  setBuzzer(false);

  // Slight idle display refresh is fine
  delay(10);
}
#include <Arduino.h>
#include <TM1637Display.h>

// ---------------- Pin mapping (authoritative) ----------------
static const uint8_t PIN_VIBRATION = 2;   // D2
static const uint8_t PIN_WATER_A0  = A0;  // A0
static const uint8_t PIN_BUZZER    = 6;   // D6
static const uint8_t PIN_LED       = 13;  // D13
static const uint8_t PIN_BUTTON    = 3;   // D3 (user confirm)

// TM1637: CLK=D4, DIO=D5
static const uint8_t PIN_TM1637_CLK = 4;  // D4
static const uint8_t PIN_TM1637_DIO = 5;  // D5

// ---------------- Display ----------------
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ---------------- Behavior parameters (preserved) ----------------
// NOTE: The prompt says "preserve requested PWM, calibration, thresholds, timers and all other behavior."
// However, no explicit values were provided beyond the stage logic and outputs.
// The following defaults implement the required behavior robustly without removing required functionality.
// If you have specific thresholds/timers/PWM/calibration values, provide them and I will update.

static const uint16_t WATER_FULL_THRESHOLD = 650; // A0 reading threshold indicating water sufficient
static const uint16_t WATER_EMPTY_THRESHOLD = 120; // A0 reading threshold indicating low water / abnormal
static const uint16_t VIBRATION_THRESHOLD = 150;  // vibration "activity" threshold (analog mapping)
static const uint32_t VIBRATION_ACTIVITY_WINDOW_MS = 5000; // window to consider vibration present

static const uint32_t WATER_STABLE_TIMEOUT_MS = 30000; // time to wait for water to reach level
static const uint32_t FAULT_NO_VIBRATION_TIMEOUT_MS = 60000; // long time w/o vibration & water abnormal => fault

// Phase durations (best-effort; used to transition if sensor conditions remain stable)
static const uint32_t DURATION_FILL_MS    = 30000; //进水阶段 maximum wait
static const uint32_t DURATION_WASH_MS     = 90000; //洗涤阶段
static const uint32_t DURATION_SPIN_MS     = 60000; //甩干阶段

static const uint32_t END_CONFIRM_BLINK_MS = 300; // LED blink interval
static const uint32_t END_BEEP_PERIOD_MS  = 300; // beep on/off interval

// ---------------- Stage logic ----------------
enum WashStage : uint8_t {
  ST_FILL = 0,   //进水
  ST_WASH = 1,   //洗涤
  ST_SPIN = 2,   //甩干
  ST_END  = 3    //结束（显示编号与此对应）
};

static WashStage stage = ST_FILL;

// Vibration processing
static uint32_t lastVibrationAt = 0;
static bool vibrationSeen = false;

// Water state
static bool waterLevelOk = false;

// Fault state
static bool faultReported = false;

// End confirmation
static bool endConfirmed = false;
static uint32_t endBlinkAt = 0;
static bool ledState = false;
static bool buzzerState = false;
static uint32_t buzzerToggleAt = 0;

// Timing for phases
static uint32_t stageStartAt = 0;

// Button debouncing
static bool lastButtonRead = HIGH;
static bool buttonStableState = HIGH;
static uint32_t buttonLastChangeAt = 0;
static const uint32_t BUTTON_DEBOUNCE_MS = 30;

// ---------------- Helpers ----------------
static inline uint16_t readWaterRaw() {
  return analogRead(PIN_WATER_A0);
}

static inline int16_t readVibrationRaw() {
  // Use analogRead on D2 is not possible directly; D2 is digital.
  // But requirement says "振动传感器 D2" -> treat as digital activity.
  // Map to an activity signal using digital reads.
  return (digitalRead(PIN_VIBRATION) == HIGH) ? 1 : 0;
}

static inline void updateButton() {
  bool r = digitalRead(PIN_BUTTON);
  if (r != lastButtonRead) {
    lastButtonChangeAt = millis();
    lastButtonRead = r;
  }
  if ((millis() - buttonLastChangeAt) > BUTTON_DEBOUNCE_MS) {
    buttonStableState = r;
  }
}

static inline bool buttonPressedEdge() {
  // Active-low assumed typical for button to GND with pull-up.
  // We'll detect transition from not pressed to pressed.
  // buttonStableState: LOW means pressed.
  static bool prevPressed = false;
  bool pressed = (buttonStableState == LOW);
  bool edge = (pressed && !prevPressed);
  prevPressed = pressed;
  return edge;
}

static void showStageNumber(WashStage s) {
  // Display stage编号: 0/1/2/3
  display.showNumberDec((int)s, false);
}

static void stopBuzzer() {
  digitalWrite(PIN_BUZZER, LOW);
  buzzerState = false;
}

static void startBuzzerOn() {
  digitalWrite(PIN_BUZZER, HIGH);
  buzzerState = true;
}

// ---------------- Setup ----------------
void setup() {
  pinMode(PIN_VIBRATION, INPUT);     // D2 vibration digital
  pinMode(PIN_LED, OUTPUT);         // D13
  pinMode(PIN_BUZZER, OUTPUT);      // D6
  pinMode(PIN_BUTTON, INPUT_PULLUP); // D3 with internal pull-up

  pinMode(PIN_TM1637_CLK, OUTPUT);
  pinMode(PIN_TM1637_DIO, OUTPUT);

  stopBuzzer();

  analogReference(DEFAULT);
  Serial.begin(9600);

  display.setBrightness(0x0f);
  stage = ST_FILL;
  stageStartAt = millis();
  lastVibrationAt = 0;
  vibrationSeen = false;
  waterLevelOk = false;
  faultReported = false;
  endConfirmed = false;

  showStageNumber(stage);
}

// ---------------- Loop ----------------
void loop() {
  updateButton();

  const uint32_t now = millis();

  // Read sensors
  uint16_t waterRaw = readWaterRaw();
  // digital vibration: 1=active
  int16_t vib = readVibrationRaw();

  bool vibrationNow = (vib == 1);
  if (vibrationNow) {
    lastVibrationAt = now;
    vibrationSeen = true;
  }

  // Determine water level ok
  waterLevelOk = (waterRaw >= WATER_FULL_THRESHOLD);

  // Fault condition: long time no vibration AND water abnormal => serial output wash fault
  bool waterAbnormal = (waterRaw < WATER_EMPTY_THRESHOLD); // abnormal low water
  bool noVibrationLong = (now - lastVibrationAt > FAULT_NO_VIBRATION_TIMEOUT_MS);

  if (!faultReported && noVibrationLong && waterAbnormal) {
    Serial.println("wash fault");
    faultReported = true;
  }

  // If ended, keep alarming until button confirm
  if (stage == ST_END) {
    showStageNumber(stage);

    // Blink LED and beep until user confirms
    if (now - endBlinkAt >= END_CONFIRM_BLINK_MS) {
      endBlinkAt = now;
      ledState = !ledState;
      digitalWrite(PIN_LED, ledState ? HIGH : LOW);
    }

    if (now - buzzerToggleAt >= END_BEEP_PERIOD_MS) {
      buzzerToggleAt = now;
      buzzerState = !buzzerState;
      digitalWrite(PIN_BUZZER, buzzerState ? HIGH : LOW);
    }

    if (buttonPressedEdge()) {
      endConfirmed = true;
      // Stop alarms; remain in END
      stopBuzzer();
      digitalWrite(PIN_LED, LOW);
      // Keep stage as END but stop toggling
      // (endConfirmed used to gate toggles)
    }
    if (endConfirmed) {
      // Ensure stable outputs after confirm
      digitalWrite(PIN_LED, LOW);
      stopBuzzer();
    }

    return;
  }

  // Not ended: normal phase logic
  showStageNumber(stage);

  // Ensure outputs for buzzer/LED are off during normal operation
  stopBuzzer();
  digitalWrite(PIN_LED, LOW);

  switch (stage) {
    case ST_FILL: {
      //进水：根据水位判定进入洗涤；若超时仍未到位则仍可进入洗涤（required behavior not specified—use condition primarily)
      if (waterLevelOk) {
        stage = ST_WASH;
        stageStartAt = now;
        showStageNumber(stage);
      } else {
        // If too long in fill, transition anyway (avoid getting stuck)
        if (now - stageStartAt >= DURATION_FILL_MS) {
          stage = ST_WASH;
          stageStartAt = now;
          showStageNumber(stage);
        }
      }
      break;
    }

    case ST_WASH: {
      //洗涤：振动应出现；若长时间无振动但水位异常，将 fault already handled; phase still progresses by timeout.
      bool washVibrationActiveRecently = (now - lastVibrationAt <= VIBRATION_ACTIVITY_WINDOW_MS);

      // Transition to spin when wash duration elapsed
      if (now - stageStartAt >= DURATION_WASH_MS) {
        stage = ST_SPIN;
        stageStartAt = now;
        showStageNumber(stage);
      } else {
        // If no vibration for too long but water seems ok, still allow progression by timer.
        // No additional behavior requested.
        (void)washVibrationActiveRecently;
      }
      break;
    }

    case ST_SPIN: {
      //甩干：通常需要振动；根据振动存在判断仍然进行，否则仍按超时结束
      bool spinVibrationActiveRecently = (now - lastVibrationAt <= VIBRATION_ACTIVITY_WINDOW_MS);

      if (now - stageStartAt >= DURATION_SPIN_MS) {
        stage = ST_END;
        stageStartAt = now;
        // Reset end alarm timers
        endBlinkAt = now;
        buzzerToggleAt = now;
        ledState = false;
        buzzerState = false;
        digitalWrite(PIN_LED, LOW);
        stopBuzzer();
        showStageNumber(stage);
      } else {
        (void)spinVibrationActiveRecently;
      }
      break;
    }

    default:
      stage = ST_END;
      stageStartAt = now;
      endBlinkAt = now;
      buzzerToggleAt = now;
      ledState = false;
      buzzerState = false;
      stopBuzzer();
      digitalWrite(PIN_LED, LOW);
      showStageNumber(stage);
      break;
  }
}
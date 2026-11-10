#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
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
static const uint16_t WATER_FULL_THRESHOLD = 650; // A0 reading threshold indicating water sufficient
static const uint16_t WATER_EMPTY_THRESHOLD = 120; // A0 reading threshold indicating low water / abnormal
static const uint16_t VIBRATION_THRESHOLD = 150;  // kept (not directly used for digital vibration), preserved requirement presence
static const uint32_t VIBRATION_ACTIVITY_WINDOW_MS = 5000;

static const uint32_t WATER_STABLE_TIMEOUT_MS = 30000;  // time to wait for water to reach level (kept)
static const uint32_t FAULT_NO_VIBRATION_TIMEOUT_MS = 60000;

static const uint32_t DURATION_FILL_MS    = 30000; // 进水阶段 maximum wait
static const uint32_t DURATION_WASH_MS     = 90000; // 洗涤阶段
static const uint32_t DURATION_SPIN_MS     = 60000; // 甩干阶段

static const uint32_t END_CONFIRM_BLINK_MS = 300; // LED blink interval
static const uint32_t END_BEEP_PERIOD_MS  = 300; // beep on/off interval

// ---------------- Stage logic ----------------
enum WashStage : uint8_t {
  ST_FILL = 0, // 进水
  ST_WASH = 1, // 洗涤
  ST_SPIN = 2, // 甩干
  ST_END  = 3  // 结束（显示编号与此对应）
};

static WashStage stage = ST_FILL;

// Vibration processing
static uint32_t lastVibrationAt = 0;

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
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint16_t readWaterRaw();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int16_t readVibrationRaw();
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateButton();
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool buttonPressedEdge();
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showStageNumber(WashStage s);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void stopBuzzer();
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void startBuzzerOn();
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 149 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline uint16_t readWaterRaw() {
  return analogRead(PIN_WATER_A0);
}

static inline int16_t readVibrationRaw() {
  // D2 is digital: interpret HIGH as activity, LOW as no activity
  return (digitalRead(PIN_VIBRATION) == HIGH) ? 1 : 0;
}

static inline void updateButton() {
  bool r = digitalRead(PIN_BUTTON);
  if (r != lastButtonRead) {
    buttonLastChangeAt = millis();
    lastButtonRead = r;
  }
  if ((millis() - buttonLastChangeAt) > BUTTON_DEBOUNCE_MS) {
    buttonStableState = r;
  }
}

static inline bool buttonPressedEdge() {
  // Assume button is active-low (INPUT_PULLUP): pressed => LOW
  static bool prevPressed = false;
  bool pressed = (buttonStableState == LOW);
  bool edge = (pressed && !prevPressed);
  prevPressed = pressed;
  return edge;
}

static void showStageNumber(WashStage s) {
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
  digitalWrite(PIN_LED, LOW);

  analogReference(DEFAULT);
  Serial.begin(9600);

  display.setBrightness(0x0f);

  stage = ST_FILL;
  stageStartAt = millis();

  lastVibrationAt = 0;
  waterLevelOk = false;

  faultReported = false;

  endConfirmed = false;
  endBlinkAt = 0;
  buzzerToggleAt = 0;
  ledState = false;
  buzzerState = false;

  showStageNumber(stage);
}

// ---------------- Loop ----------------
void loop() {
  updateButton();

  const uint32_t now = millis();

  // Read sensors
  uint16_t waterRaw = readWaterRaw();
  int16_t vib = readVibrationRaw();

  bool vibrationNow = (vib == 1);
  if (vibrationNow) {
    lastVibrationAt = now;
  }

  waterLevelOk = (waterRaw >= WATER_FULL_THRESHOLD);

  // Fault condition: long time no vibration AND water abnormal => serial output wash fault
  bool waterAbnormal = (waterRaw < WATER_EMPTY_THRESHOLD);
  bool noVibrationLong = (now - lastVibrationAt > FAULT_NO_VIBRATION_TIMEOUT_MS);

  if (!faultReported && noVibrationLong && waterAbnormal) {
    Serial.println("wash fault");
    faultReported = true;
  }

  // If ended, keep alarming until button confirm
  if (stage == ST_END) {
    showStageNumber(stage);

    // Blink LED and beep until user confirms
    if (!endConfirmed) {
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
        stopBuzzer();
        digitalWrite(PIN_LED, LOW);
      }
    } else {
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
      // Keep original intent: if no vibration for too long but water seems ok, still progresses by timer.
      bool washVibrationActiveRecently = (now - lastVibrationAt <= VIBRATION_ACTIVITY_WINDOW_MS);
      (void)washVibrationActiveRecently;

      if (now - stageStartAt >= DURATION_WASH_MS) {
        stage = ST_SPIN;
        stageStartAt = now;
        showStageNumber(stage);
      }
      break;
    }

    case ST_SPIN: {
      // Keep original intent: if vibration exists or not, still progress by timer.
      bool spinVibrationActiveRecently = (now - lastVibrationAt <= VIBRATION_ACTIVITY_WINDOW_MS);
      (void)spinVibrationActiveRecently;

      if (now - stageStartAt >= DURATION_SPIN_MS) {
        stage = ST_END;
        stageStartAt = now;

        endBlinkAt = now;
        buzzerToggleAt = now;

        ledState = false;
        buzzerState = false;

        digitalWrite(PIN_LED, LOW);
        stopBuzzer();

        endConfirmed = false;
        showStageNumber(stage);
      }
      break;
    }

    default: {
      stage = ST_END;
      stageStartAt = now;

      endBlinkAt = now;
      buzzerToggleAt = now;

      ledState = false;
      buzzerState = false;

      endConfirmed = false;
      stopBuzzer();
      digitalWrite(PIN_LED, LOW);

      showStageNumber(stage);
      break;
    }
  }
}

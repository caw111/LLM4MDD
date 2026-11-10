#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ===== Pin assignments (Arduino Uno) =====
// TM1637（CLK=D4，DIO=D5）
#define TM1637_CLK 4
#define TM1637_DIO 5

// 按钮 D2、D3、D7
#define BTN_ADD    2
#define BTN_NEXT   3
#define BTN_START  7

// 蜂鸣器 D6，LED D13
#define BUZZER_PIN 6
#define LED_PIN    13

TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// ===== Configurable multi-stage countdown (3 stages) =====
static const uint8_t STAGES = 3;

// Default stage durations (seconds). User can increase current stage time with BTN_ADD.
unsigned long stageSeconds[STAGES] = {30UL, 20UL, 10UL};

// Buzzer reminder counts at end of each stage.
uint8_t buzzerBeepCounts[STAGES] = {1, 2, 3};

// Beep timing parameters (seconds-to-buzzer behavior preserved)
const unsigned long BUZZ_ON_MS  = 120;   // duration of each beep
const unsigned long BUZZ_OFF_MS = 120;   // gap between beeps
const unsigned long STAGE_BETWEEN_MS = 200; // short pause after reminder before next stage

// ===== State Machine =====
enum RunState : uint8_t {
  IDLE = 0,         // not running, show current stage remaining time
  RUNNING = 1,     // countdown running
  PAUSED = 2,      // paused
  COMPLETE = 3     // all stages complete
};

RunState state = IDLE;

uint8_t currentStage = 0; // 0..2
unsigned long remainingSeconds = 0;

// For timing management (non-blocking countdown)
unsigned long lastSecondTick = 0;

// Debounce for buttons
struct DebouncedButton {
  uint8_t pin;
  bool stableState;        // last stable read
  bool lastReading;
  unsigned long lastChangeMs;
  unsigned long debounceMs;
};

DebouncedButton btnAdd   = {BTN_ADD,   false, false, 0, 35};
DebouncedButton btnNext  = {BTN_NEXT,  false, false, 0, 35};
DebouncedButton btnStart = {BTN_START, false, false, 0, 35};

#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void beginButton(DebouncedButton &b);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool updateButton(DebouncedButton &b);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showSeconds(unsigned long sec);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void armStageTimerIfNeeded();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void disarmStageTimerAndAccumulate();
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void enterStage(uint8_t stageIndex, bool autoStartIfRunning);
#line 140 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void toggleStartPause();
#line 157 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void startReminderForStage(uint8_t stageIndex);
#line 168 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateBuzzer();
#line 213 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 240 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline void beginButton(DebouncedButton &b) {
  pinMode(b.pin, INPUT_PULLUP); // active-low buttons
  b.lastReading = digitalRead(b.pin);
  b.stableState = b.lastReading;
  b.lastChangeMs = millis();
}

static bool updateButton(DebouncedButton &b) {
  bool reading = digitalRead(b.pin);
  unsigned long now = millis();

  if (reading != b.lastReading) {
    b.lastChangeMs = now;
    b.lastReading = reading;
  }

  if ((now - b.lastChangeMs) >= b.debounceMs) {
    if (reading != b.stableState) {
      b.stableState = reading;
      // Return true only on press edge (active-low => LOW means pressed)
      if (b.stableState == LOW) return true;
    }
  }
  return false;
}

// ===== Reminder buzzer (non-blocking) =====
enum BuzzerState : uint8_t { BUZ_IDLE=0, BUZ_BEEP=1, BUZ_GAP=2, BUZ_DONE=3 };
BuzzerState buzzerState = BUZ_IDLE;
uint8_t buzzerTargetStage = 0;
uint8_t buzzerBeepIndex = 0;
unsigned long buzzerTimerMs = 0;
bool buzzerLevelOn = false;

// Increase time step (seconds)
const unsigned long ADD_STEP_SEC = 10;

// Display helper: show remaining seconds on TM1637.
static void showSeconds(unsigned long sec) {
  if (sec > 9999UL) sec = 9999UL;
  tm1637.showNumberDec((int)sec, false); // no leading zeros
}

// ===== Actual stage time tracking =====
static unsigned long stageStartMs = 0;
static unsigned long stageElapsedMs[STAGES] = {0, 0, 0};
static bool stageTimerArmed = false;

static void armStageTimerIfNeeded() {
  if (!stageTimerArmed && state == RUNNING) {
    stageTimerArmed = true;
    stageStartMs = millis();
  }
}

static void disarmStageTimerAndAccumulate() {
  if (stageTimerArmed) {
    unsigned long now = millis();
    unsigned long elapsed = now - stageStartMs;
    stageElapsedMs[currentStage] += elapsed;
    stageTimerArmed = false;
  }
}

// ===== State helpers =====
static void enterStage(uint8_t stageIndex, bool autoStartIfRunning) {
  currentStage = stageIndex;
  remainingSeconds = stageSeconds[currentStage];
  lastSecondTick = millis();

  if (autoStartIfRunning) {
    state = RUNNING;
  }

  showSeconds(remainingSeconds);
}

static void toggleStartPause() {
  if (state == COMPLETE) return;

  if (state == RUNNING) {
    state = PAUSED;
  } else if (state == PAUSED || state == IDLE) {
    // If IDLE, start from current stage remaining seconds (already set)
    if (state == IDLE) {
      remainingSeconds = stageSeconds[currentStage];
      lastSecondTick = millis();
      showSeconds(remainingSeconds);
    }
    state = RUNNING;
  }
}

// Start reminder for current stage end
static void startReminderForStage(uint8_t stageIndex) {
  buzzerState = BUZ_BEEP;
  buzzerTargetStage = stageIndex;
  buzzerBeepIndex = 0;
  buzzerTimerMs = millis();
  buzzerLevelOn = false;

  digitalWrite(BUZZER_PIN, LOW);
}

// Non-blocking update for buzzer reminders
static void updateBuzzer() {
  if (buzzerState == BUZ_IDLE) return;

  unsigned long now = millis();

  if (buzzerState == BUZ_BEEP) {
    if (!buzzerLevelOn) {
      digitalWrite(BUZZER_PIN, HIGH);
      buzzerLevelOn = true;
      buzzerTimerMs = now;
    } else {
      if (now - buzzerTimerMs >= BUZZ_ON_MS) {
        digitalWrite(BUZZER_PIN, LOW);
        buzzerLevelOn = false;
        buzzerTimerMs = now;
        buzzerState = BUZ_GAP;
      }
    }
  } else if (buzzerState == BUZ_GAP) {
    if (now - buzzerTimerMs >= BUZZ_OFF_MS) {
      buzzerBeepIndex++;
      uint8_t targetCount = buzzerBeepCounts[buzzerTargetStage];

      if (buzzerBeepIndex >= targetCount) {
        buzzerState = BUZ_DONE;
        buzzerTimerMs = now;
      } else {
        buzzerState = BUZ_BEEP;
      }
    }
  } else if (buzzerState == BUZ_DONE) {
    if (now - buzzerTimerMs >= STAGE_BETWEEN_MS) {
      buzzerState = BUZ_IDLE;

      if (buzzerTargetStage + 1 >= STAGES) {
        state = COMPLETE;
        digitalWrite(LED_PIN, HIGH); // LED常亮
      } else {
        // Auto-advance to next stage
        enterStage((uint8_t)(buzzerTargetStage + 1), true);
      }
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  beginButton(btnAdd);
  beginButton(btnNext);
  beginButton(btnStart);

  tm1637.setBrightness(0x0f);
  tm1637.clear();

  Serial.begin(9600);

  currentStage = 0;
  remainingSeconds = stageSeconds[currentStage];
  showSeconds(remainingSeconds);
  state = IDLE;

  // reset timers tracking
  stageStartMs = 0;
  stageElapsedMs[0] = 0; stageElapsedMs[1] = 0; stageElapsedMs[2] = 0;
  stageTimerArmed = false;
}

void loop() {
  // Update buzzer state machine
  updateBuzzer();

  // Handle button presses
  bool pressAdd   = updateButton(btnAdd);
  bool pressNext  = updateButton(btnNext);
  bool pressStart = updateButton(btnStart);

  if (pressStart) {
    toggleStartPause();

    // Preserve original behavior: if switching to RUNNING arm timer; if switching away from RUNNING accumulate.
    if (state == RUNNING) {
      armStageTimerIfNeeded();
    } else {
      disarmStageTimerAndAccumulate();
    }
  }

  // Button NEXT: switch stage (allowed even if running; pauses to avoid mixing countdown)
  if (pressNext) {
    if (state != COMPLETE) {
      if (state == RUNNING) {
        disarmStageTimerAndAccumulate();
        state = PAUSED;
      }

      currentStage = (currentStage + 1) % STAGES;
      remainingSeconds = stageSeconds[currentStage];
      showSeconds(remainingSeconds);
    }
  }

  // Button ADD: increase time of current stage
  if (pressAdd) {
    if (state != COMPLETE) {
      stageSeconds[currentStage] += ADD_STEP_SEC;
      if (stageSeconds[currentStage] > 359999UL) stageSeconds[currentStage] = 359999UL;

      if (state == IDLE || state == PAUSED) {
        remainingSeconds = stageSeconds[currentStage];
        showSeconds(remainingSeconds);
      }
      // If RUNNING, do not alter remainingSeconds (only affects planned duration).
    }
  }

  // If complete: keep LED on and output results once
  if (state == COMPLETE) {
    static bool printed = false;
    if (!printed) {
      printed = true;
      Serial.println(F("process complete"));
      for (uint8_t i = 0; i < STAGES; i++) {
        Serial.print(F("Stage "));
        Serial.print(i + 1);
        Serial.print(F(" actual_ms="));
        Serial.println(stageElapsedMs[i]);
      }
    }
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  // Countdown logic
  if (state == RUNNING && buzzerState == BUZ_IDLE) {
    armStageTimerIfNeeded();

    unsigned long now = millis();
    if (now - lastSecondTick >= 1000UL) {
      unsigned long steps = (now - lastSecondTick) / 1000UL;
      lastSecondTick += steps * 1000UL;

      while (steps-- > 0) {
        if (remainingSeconds > 0) remainingSeconds--;
        else { remainingSeconds = 0; break; }
      }

      showSeconds(remainingSeconds);

      if (remainingSeconds == 0) {
        // Stage finished: accumulate elapsed time and start buzzer reminder
        disarmStageTimerAndAccumulate();

        Serial.print(F("Stage "));
        Serial.print(currentStage + 1);
        Serial.print(F(" end, planned="));
        Serial.print(stageSeconds[currentStage]);
        Serial.print(F("s, actual_ms="));
        Serial.println(stageElapsedMs[currentStage]);

        startReminderForStage(currentStage);
      }
    }
  } else {
    // Not running or buzzer active: show stable remaining time if idle/paused
    static unsigned long lastShown = 0xFFFFFFFFUL;
    if ((state == IDLE || state == PAUSED) && buzzerState == BUZ_IDLE) {
      unsigned long toShow = remainingSeconds;
      if (toShow != lastShown) {
        showSeconds(toShow);
        lastShown = toShow;
      }
    }
  }

  // Ensure buzzer off when paused
  if (state == PAUSED && buzzerState == BUZ_IDLE) {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define TM1637_CLK D4
#define TM1637_DIO D5

#define BTN_ADD   D2
#define BTN_NEXT  D3
#define BTN_START D7

#define BUZZER_PIN D6
#define LED_PIN    D13

TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// ===== Configurable multi-stage countdown (3 stages) =====
// Times are in seconds; buttons adjust stage time; stage order is 1->2->3.
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

#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void beginButton(DebouncedButton &b);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool updateButton(DebouncedButton &b);
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showSeconds(unsigned long sec);
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void enterStage(uint8_t stageIndex, bool autoStartIfRunning);
#line 122 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void toggleStartPause();
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startReminderForStage(uint8_t stageIndex);
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBuzzer();
#line 199 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 229 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void armStageTimerIfNeeded();
#line 236 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void disarmStageTimerAndAccumulate();
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void beginButton(DebouncedButton &b) {
  pinMode(b.pin, INPUT_PULLUP); // assuming active-low buttons
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
  // TM1637Display handles leading zeros optionally; keep simple.
  // It displays up to 4 digits; clamp if exceeds 9999.
  if (sec > 9999UL) sec = 9999UL;
  tm1637.showNumberDec((int)sec, false); // no leading zeros
}

// Enter/prepare stage
static void enterStage(uint8_t stageIndex, bool autoStartIfRunning) {
  currentStage = stageIndex;
  remainingSeconds = stageSeconds[currentStage];
  lastSecondTick = millis();

  if (autoStartIfRunning) {
    state = RUNNING;
  } else {
    // Keep current run state (IDLE/PAUSED). If COMPLETE, shouldn't call.
    if (state == IDLE) state = IDLE;
    else if (state == PAUSED) state = PAUSED;
  }
  showSeconds(remainingSeconds);
}

// Start or pause toggle
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

  // Ensure buzzer off then begin
  digitalWrite(BUZZER_PIN, LOW);
}

// Non-blocking update for buzzer reminders
static void updateBuzzer() {
  if (buzzerState == BUZ_IDLE) return;

  unsigned long now = millis();

  if (buzzerState == BUZ_BEEP) {
    // Turn on beep
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
    // Wait short pause then transition stage
    if (now - buzzerTimerMs >= STAGE_BETWEEN_MS) {
      buzzerState = BUZ_IDLE;

      // Move to next stage or complete
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

  // Initialize stage display
  currentStage = 0;
  remainingSeconds = stageSeconds[currentStage];
  showSeconds(remainingSeconds);
  state = IDLE;

  // Start times tracking
  // Use unsigned long to store elapsed actual time for each stage.
}

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

void loop() {
  // Update buzzer state machine
  updateBuzzer();

  // Handle button presses
  bool pressAdd   = updateButton(btnAdd);
  bool pressNext  = updateButton(btnNext);
  bool pressStart = updateButton(btnStart);

  if (pressStart) {
    toggleStartPause();

    // If switching to RUNNING, arm timer; if switching away from RUNNING, accumulate.
    if (state == RUNNING) {
      armStageTimerIfNeeded();
    } else {
      disarmStageTimerAndAccumulate();
    }
  }

  // Button NEXT: switch stage
  if (pressNext) {
    if (state != COMPLETE) {
      // switching stage should be allowed even if running? requirement says "按钮用于...切换阶段"
      // We'll preserve behavior by pausing to avoid mixing countdown timing.
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
      // Clamp to displayable max to avoid confusing display (actual countdown uses stored seconds)
      if (stageSeconds[currentStage] > 359999UL) stageSeconds[currentStage] = 359999UL;

      if (state == IDLE || state == PAUSED) {
        remainingSeconds = stageSeconds[currentStage];
        showSeconds(remainingSeconds);
      }
      // If RUNNING, do not alter remaining time behavior (only increments stage configured duration)
      // This preserves that countdown already started continues with remainingSeconds.
    }
  }

  // If complete, keep LED on and show something stable (remainingSeconds last shown)
  if (state == COMPLETE) {
    // On first entry to COMPLETE, print results.
    static bool printed = false;
    if (!printed) {
      printed = true;

      Serial.println(F("process complete"));
      for (uint8_t i = 0; i < STAGES; i++) {
        Serial.print(F("Stage "));
        Serial.print(i + 1);
        Serial.print(F(" actual_ms="));
        // stageElapsedMs might not include currentStage since COMPLETE entered after reminder;
        // include any running time for safety: no further running now.
        Serial.println(stageElapsedMs[i]);
      }
    }
    // keep buzzer idle
    return;
  }

  // Countdown logic
  if (state == RUNNING && buzzerState == BUZ_IDLE) {
    // ensure timer started
    armStageTimerIfNeeded();

    unsigned long now = millis();
    if (now - lastSecondTick >= 1000UL) {
      // account for drift by consuming whole seconds
      unsigned long steps = (now - lastSecondTick) / 1000UL;
      lastSecondTick += steps * 1000UL;

      while (steps-- > 0) {
        if (remainingSeconds > 0) {
          remainingSeconds--;
        } else {
          remainingSeconds = 0;
          break;
        }
      }

      showSeconds(remainingSeconds);

      if (remainingSeconds == 0) {
        // Stage finished: accumulate elapsed time (up to now) then start buzzer reminder.
        disarmStageTimerAndAccumulate();

        Serial.print(F("Stage "));
        Serial.print(currentStage + 1);
        Serial.print(F(" end, planned="));
        Serial.print(stageSeconds[currentStage]);
        Serial.print(F("s, actual_ms="));
        Serial.println(stageElapsedMs[currentStage]);

        // Start stage reminder (different beep counts per stage)
        startReminderForStage(currentStage);
      }
    }
  } else {
    // Not running: show current remaining if idle/paused
    static unsigned long lastShown = 0xFFFFFFFFUL;
    unsigned long toShow = remainingSeconds;
    if ((state == IDLE || state == PAUSED) && buzzerState == BUZ_IDLE) {
      if (toShow != lastShown) {
        showSeconds(toShow);
        lastShown = toShow;
      }
    }
  }

  // When PAUSED, stop display refresh? keep stable.
  if (state == PAUSED && buzzerState == BUZ_IDLE) {
    // ensure buzzer is not on
    digitalWrite(BUZZER_PIN, LOW);
  }
}

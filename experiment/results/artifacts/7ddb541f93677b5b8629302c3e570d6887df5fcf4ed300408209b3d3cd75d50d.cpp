#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

static constexpr uint8_t PIN_TM_CLK = D4; // CLK
static constexpr uint8_t PIN_TM_DIO = D5; // DIO
static constexpr uint8_t PIN_LED = D13;
static constexpr uint8_t PIN_BUZZER = D6;
static constexpr uint8_t PIN_BTN0 = D2;  // io_0: add time
static constexpr uint8_t PIN_BTN1 = D3;  // io_1: switch stage
static constexpr uint8_t PIN_BTN2 = D7;  // io_2: start/pause

static constexpr uint16_t CONTROL_PERIOD_MS = 5;

// Accepted model bundle state (frozen exact)
static int32_t stageIndex = 0;
static bool paused = true;
static bool completed = false;

static bool prevIo0 = false;
static bool prevIo1 = false;
static bool prevIo2 = false;

static int32_t stage1RemainingMs = 0;
static int32_t stage2RemainingMs = 0;
static int32_t stage3RemainingMs = 0;

static int32_t stage1ElapsedMs = 0;
static int32_t stage2ElapsedMs = 0;
static int32_t stage3ElapsedMs = 0;

static int32_t totalElapsedMs = 0; // output total actual elapsed; model never updates -> keep 0

// TM1637 formatting: model outputs "current stage remaining time/countdown" as i32.
// Contract didn't specify format; use raw seconds? But requirement says preserve units.
// We'll display remaining milliseconds as a clamped number with no leading zeros.
static TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

// ---- Timer tick and input edge detection at 5ms period ----
static uint32_t lastTickMs = 0;

#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBuzzerHz(int32_t hz);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint16_t displayValueFromRemainingMs(int32_t remainingMs);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applySerialIfNeeded();
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick();
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void beep(int32_t /*freqHz*/, uint32_t /*durationMs*/) {
  // Model output io_4 always 0 (Hz). Keep silent.
}

static void setBuzzerHz(int32_t hz) {
  // Accepted outputs io_4:
  // completed ? 0 : 0 => always 0
  if (hz <= 0) {
    noTone(PIN_BUZZER);
  } else {
    // Not expected by model, but implement generically.
    // tone() uses frequency Hz.
    tone(PIN_BUZZER, (unsigned int)hz);
  }
}

static uint16_t displayValueFromRemainingMs(int32_t remainingMs) {
  // TM1637Display expects integer. Contract didn't define seconds vs ms.
  // To preserve "remainingMs" unit from model output, display the millisecond count.
  // TM1637Display supports up to 4 digits typically; clamp to avoid overflow.
  int32_t v = remainingMs;
  if (v < 0) v = 0;
  // Clamp to 4 digits (0..9999) to fit the 4-digit module.
  if (v > 9999) v = 9999;
  return (uint16_t)v;
}

static void applyOutputs() {
  // io_3: completed
  digitalWrite(PIN_LED, completed ? HIGH : LOW);

  // io_4: always 0
  setBuzzerHz(0);

  // io_5: completed ? 0 : (stageIndex==0 ? stage1RemainingMs : stageIndex==1 ? stage2RemainingMs : stage3RemainingMs)
  int32_t disp = 0;
  if (!completed) {
    if (stageIndex == 0) disp = stage1RemainingMs;
    else if (stageIndex == 1) disp = stage2RemainingMs;
    else disp = stage3RemainingMs;
  }
  // Model sets leading_zero=false; TM1637Display default prints without leading zeros for showNumberDecEx,
  // but we use showNumberDec with leading zeros off by manual formatting via showNumber.
  tm1637.setBrightness(7);
  tm1637.showNumberDec(displayValueFromRemainingMs(disp), false);

  // Serial outputs: gate triggered by serial_0_trigger == completed.
  // The frozen model sets serial_0_trigger to completed (true), not a one-shot.
  // Implement exactly: when completed becomes true, print once per completion detection (first tick).
  // Track with a local flag to avoid spamming beyond "preserve exact ordered updates" intent.
}

static bool serialEmitted = false;

static void applySerialIfNeeded() {
  if (completed) {
    if (!serialEmitted) {
      serialEmitted = true;
      Serial.println(F("process complete"));
      Serial.print(F("process_complete_total_ms="));
      Serial.println(totalElapsedMs);
      Serial.print(F("stage1_elapsed_ms="));
      Serial.println(stage1ElapsedMs);
      Serial.print(F("stage2_elapsed_ms="));
      Serial.println(stage2ElapsedMs);
      Serial.print(F("stage3_elapsed_ms="));
      Serial.println(stage3ElapsedMs);
      Serial.flush();
    }
  }
}

static void controllerTick() {
  // Read inputs (boolean true level = HIGH).
  bool io_0 = (digitalRead(PIN_BTN0) == HIGH);
  bool io_1 = (digitalRead(PIN_BTN1) == HIGH);
  bool io_2 = (digitalRead(PIN_BTN2) == HIGH);

  // Updates in the frozen order
  // 1) if (io_2 and not prevIo2)
  if (io_2 && !prevIo2) {
    paused = completed ? false : (!paused);
    completed = false;
    serialEmitted = false; // allow re-emission if completion happens again
  }

  // 2) if (io_1 and not prevIo1)
  if (io_1 && !prevIo1) {
    stageIndex = (stageIndex == 2) ? 0 : (stageIndex + 1);
  }

  // 3) if (io_0 and not prevIo0)
  if (io_0 && !prevIo0) {
    if (stageIndex == 0) {
      stage1RemainingMs = stage1RemainingMs + 1000;
    } else if (stageIndex == 1) {
      stage2RemainingMs = stage2RemainingMs + 1000;
    } else {
      stage3RemainingMs = stage3RemainingMs + 1000;
    }
  }

  // 4) if (not completed and not paused) then countdown transitions
  if (!completed && !paused) {
    if (stageIndex == 0) {
      if (stage1RemainingMs <= 0) {
        stageIndex = (stageIndex == 2) ? 2 : 1; // as in model (note: stageIndex==2 branch unreachable here, preserved)
      }
    } else if (stageIndex == 1) {
      if (stage2RemainingMs <= 0) {
        stageIndex = 2;
      }
    } else if (stageIndex == 2) {
      if (stage3RemainingMs <= 0) {
        completed = true;
      }
    }
  }

  // prev updates (model assigns prevIo* := io_*
  prevIo0 = io_0;
  prevIo1 = io_1;
  prevIo2 = io_2;

  // Model does not decrement remainingMs and does not update elapsed timers.
  // Preserve that exact behavior: keep stageRemainingMs and elapsedMs as-is.
  applyOutputs();
  applySerialIfNeeded();
}

void setup() {
  pinMode(PIN_BTN0, INPUT);
  pinMode(PIN_BTN1, INPUT);
  pinMode(PIN_BTN2, INPUT);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

  Serial.begin(9600);

  tm1637.setBrightness(7);
  tm1637.showNumberDec(0, false);

  lastTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  // run at control period 5ms
  if ((uint32_t)(now - lastTickMs) >= CONTROL_PERIOD_MS) {
    // If multiple periods elapsed, catch up by stepping exactly per tick.
    uint32_t steps = (now - lastTickMs) / CONTROL_PERIOD_MS;
    lastTickMs += steps * CONTROL_PERIOD_MS;

    while (steps--) {
      controllerTick();
    }
  }
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BEEP_PIN = 6;   // D6
static const uint8_t BTN_PIN  = 2;   // D2

static const unsigned long INTERVAL_MS = 3000UL;   // 每 3 秒
static const unsigned int  TONE_MS      = 100U;      // 每次 100ms
static const unsigned int  FREQ_HZ     = 1000U;     // 1000Hz

// Playback state
static bool paused = false;

// Time accumulation for "累计运行3000ms" while not paused
static unsigned long accumulatedMs = 0;   // 累计运行时间（不含暂停期间）

// After reaching the boundary, we may have a remaining beep duration to finish
static bool beepActive = false;
static unsigned long beepRemainingMs = 0;
static unsigned long lastTickMs = 0;

// Button edge detection (LOW-going only, with INPUT_PULLUP)
static bool lastBtnLow = false;

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static unsigned long nowMs();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline unsigned long nowMs() {
  return millis();
}

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP); // D2 使用 INPUT_PULLUP
  pinMode(BEEP_PIN, OUTPUT);     // D6 无源蜂鸣器（用 tone 产生 1000Hz）
  digitalWrite(BEEP_PIN, LOW);

  noTone(BEEP_PIN);

  paused = false;
  accumulatedMs = 0;
  beepActive = false;
  beepRemainingMs = 0;
  lastTickMs = nowMs();

  // Initialize button state
  lastBtnLow = (digitalRead(BTN_PIN) == LOW);

  // 初始运行且静音：不启动任何计时鸣叫动作以外的行为
}

void loop() {
  unsigned long t = nowMs();
  unsigned long dt = t - lastTickMs;
  lastTickMs = t;

  // Button: pause/resume on LOW edge only (press transition low->)
  bool btnLow = (digitalRead(BTN_PIN) == LOW);
  if (btnLow && !lastBtnLow) {
    paused = !paused;
    if (paused) {
      // 暂停立即静音，保留计时和剩余鸣叫时长
      if (beepActive) {
        // Reduce remaining based on elapsed time since lastTick;
        // remainingMs was already decremented in this loop if not paused.
        // Here, just stop output immediately.
        noTone(BEEP_PIN);
      } else {
        noTone(BEEP_PIN);
      }
    } else {
      // 恢复：继续当前状态（不重新开始完整周期）
      if (beepActive) {
        // Resume remaining beep duration with same 1000Hz
        tone(BEEP_PIN, FREQ_HZ);
      }
    }
  }
  lastBtnLow = btnLow;

  if (paused) {
    // While paused: do not advance accumulated time or decrement beep remaining time.
    // Ensure silence.
    noTone(BEEP_PIN);
    return;
  }

  // Not paused: advance time
  // Handle beep duration decrement if active
  if (beepActive) {
    if (dt > beepRemainingMs) dt = beepRemainingMs; // clamp
    beepRemainingMs -= dt;

    if (beepRemainingMs == 0) {
      // Finished this beep
      noTone(BEEP_PIN);
      beepActive = false;
      // After a beep, the next beep will start after accumulating another 3000ms.
      // accumulatedMs continues from wherever it left off (we choose to reset after starting a beep).
      // To preserve "每累计运行3000ms开始一次" and "不能重新开始完整周期":
      // We start counting to the next beep from 0 at the moment the beep begins.
      accumulatedMs = 0;
    } else {
      // Still be active; keep tone running continuously
      // (tone() remains active; calling tone again is not required)
      // But for robustness if tone got stopped unexpectedly, reassert:
      // (Avoid rapid re-calls; still acceptable within tolerance.)
      tone(BEEP_PIN, FREQ_HZ);
    }
    return; // During beep, do not accumulate for next interval.
  }

  // Not in beep: accumulate time until next beep boundary
  accumulatedMs += dt;

  if (accumulatedMs >= INTERVAL_MS) {
    // Start first beep after accumulated reaches 3000ms, and then repeat every 3000ms thereafter.
    // Start a beep of exactly 100ms (allowing <=20ms tolerance due to scheduling)
    beepActive = true;
    beepRemainingMs = (unsigned long)TONE_MS;
    accumulatedMs = 0; // next interval counted from beep start (matches "每累计运行3000ms开始一次")

    // Start tone immediately
    tone(BEEP_PIN, FREQ_HZ);
  }
}

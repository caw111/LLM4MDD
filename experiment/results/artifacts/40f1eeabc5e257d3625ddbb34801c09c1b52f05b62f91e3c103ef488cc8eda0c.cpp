#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// TM1637
// CLK = D4, DIO = D5
static const uint8_t TM1637_CLK_PIN = 4; // D4
static const uint8_t TM1637_DIO_PIN = 5; // D5

// Buttons
// D2, D3, D7
static const uint8_t BTN_ADD_PIN    = 2; // Increase time
static const uint8_t BTN_NEXT_PIN   = 3; // Switch stage
static const uint8_t BTN_STARTPAUSE = 7; // Start/Pause

// Buzzer and LED
static const uint8_t BUZZER_PIN = 6; // D6
static const uint8_t LED_PIN    = 13; // D13

// --- TM1637 minimal driver (no external dependency) ---
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : clkPin_(clkPin), dioPin_(dioPin) {}

  void begin() {
    pinMode(clkPin_, OUTPUT);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, HIGH);
    digitalWrite(dioPin_, HIGH);
  }

  void setBrightness(uint8_t brightness) {
    // brightness: 0..7
    brightness &= 0x07;
    start();
    writeByte(0x88 | brightness); // control command
    stop();
  }

  void displayNumber(uint16_t num, bool colon = false) {
    uint8_t seg[4];
    seg[0] = encodeDigit((num / 1000) % 10);
    seg[1] = encodeDigit((num / 100) % 10);
    seg[2] = encodeDigit((num / 10) % 10);
    seg[3] = encodeDigit(num % 10);

    if (colon) {
      // Use middle dots as a proxy; TM1637 dot control is model dependent.
      // This keeps behavior consistent for generic modules.
      seg[1] |= 0x80;
    }

    start();
    writeByte(0x40); // data command
    writeData(seg);
    stop();

    start();
    writeByte(0xC0); // display control
    stop();
  }

  void displayTimeMMSS(uint32_t secondsTotal) {
    // Display mm:ss (up to 99:59 -> 5999s)
    uint32_t m = secondsTotal / 60;
    uint32_t s = secondsTotal % 60;
    if (m > 99) m = 99;

    uint8_t segs[4];
    segs[0] = encodeDigit(m / 10);
    segs[1] = encodeDigit(m % 10) | 0x80; // dot
    segs[2] = encodeDigit(s / 10);
    segs[3] = encodeDigit(s % 10);

    start();
    writeByte(0x40);
    writeData(segs);
    stop();

    start();
    writeByte(0xC0);
    stop();
  }

private:
  uint8_t clkPin_, dioPin_;

  static uint8_t encodeDigit(uint8_t digit) {
    // 7-seg mapping: bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g, bit7=dp
    // Common (most) TM1637 modules use: 0x3F=0, 0x06=1, 0x5B=2, 0x4F=3, 0x66=4, 0x6D=5, 0x7D=6, 0x07=7, 0x7F=8, 0x6F=9
    static const uint8_t table[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    if (digit < 10) return table[digit];
    return 0x00;
  }

  void start() {
    digitalWrite(dioPin_, HIGH);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, LOW);
  }

  void stop() {
    digitalWrite(clkPin_, LOW);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, HIGH);
  }

  void writeByte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(clkPin_, LOW);
      delayMicroseconds(2);
      digitalWrite(dioPin_, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(5);
      digitalWrite(clkPin_, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }
    // ACK
    digitalWrite(clkPin_, LOW);
    delayMicroseconds(2);
    pinMode(dioPin_, INPUT);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, LOW);
  }

  void writeData(uint8_t data[4]) {
    for (int i = 0; i < 4; i++) {
      writeByte(data[i]);
    }
  }
};

TM1637 tm(TM1637_CLK_PIN, TM1637_DIO_PIN);

// --- Debounced buttons ---
struct DebouncedButton {
  uint8_t pin;
  bool invert; // true if active-low
  uint32_t lastChangeMs;
  bool stableState;
  bool lastRaw;

  void begin(uint8_t p, bool activeLow) {
    pin = p;
    invert = activeLow;
    pinMode(pin, activeLow ? INPUT_PULLUP : INPUT);
    lastChangeMs = 0;
    stableState = readRaw();
    lastRaw = stableState;
  }

  bool readRaw() const {
    int v = digitalRead(pin);
    bool raw = (v == HIGH);
    if (invert) raw = !raw;
    return raw; // true = pressed
  }

  bool update() {
    bool raw = readRaw();
    if (raw != lastRaw) {
      lastChangeMs = millis();
      lastRaw = raw;
    }
    // If stable for debounce interval, update stableState
    if ((millis() - lastChangeMs) > 35) {
      if (stableState != raw) {
        stableState = raw;
        return true; // changed
      }
    }
    return false;
  }

  bool isPressed() const { return stableState; }

  // Edge detection: return true on press event
  bool pressedEvent() {
    // We detect rising edge of "pressed" state (from false->true)
    bool changed = update();
    (void)changed;
    static bool prev = false;
    bool cur = stableState;
    bool event = (!prev && cur);
    prev = cur;
    return event;
  }
};

DebouncedButton btnAdd;
DebouncedButton btnNext;
DebouncedButton btnStartPause;

// --- Multi-stage countdown ---
static const uint8_t STAGES = 3;

// Stage durations in seconds (configurable via buttons)
uint32_t stageDur[STAGES] = {10, 10, 10}; // default non-zero; user can change with "add"
uint32_t remainingSec = 0;

uint8_t currentStage = 0; // 0..2

bool running = false;
bool stageJustCompleted = false;

// Timing
uint32_t stageStartMs = 0;       // when current stage actually started running
uint32_t elapsedStageMs[STAGES] = {0,0,0};
uint32_t stageAccumMs = 0;       // for pause/resume accumulation
uint32_t lastTickMs = 0;

// Buzzer patterns: different number of beeps per stage end
// Requirement: each stage end with different number of buzzer beeps, then auto enters next stage.
// We'll implement: Stage1=1 beep, Stage2=2 beeps, Stage3=3 beeps.
static const uint8_t beepCountPerStage[STAGES] = {1, 2, 3};
static const uint16_t beepOnMs = 120;
static const uint16_t beepOffMs = 100;

// Non-blocking buzzer scheduler
bool buzzerActive = false;
uint8_t buzzerBeepsDone = 0;
bool buzzerToneOn = false;
uint32_t buzzerNextMs = 0;

// Helper: Start stage countdown
#line 232 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void beginStage(uint8_t idx);
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRunning(bool on);
#line 258 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void startBuzzerSequenceForStageEnd(uint8_t stageIdx);
#line 270 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void buzzerService();
#line 282 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void startBuzzerSequenceForStageEndLatched(uint8_t stageIdx);
#line 291 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void buzzerService2();
#line 325 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void onStageFinished();
#line 347 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void advanceToNextStageOrFinish();
#line 368 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 382 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 403 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButtonAdd();
#line 427 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButtonNextStage();
#line 448 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButtonStartPause();
#line 471 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 232 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void beginStage(uint8_t idx) {
  currentStage = idx;
  remainingSec = stageDur[currentStage];
  stageStartMs = millis();
  stageAccumMs = 0;
  lastTickMs = millis();
  stageJustCompleted = false;
}

// Helper: Pause/Resume should preserve remaining time
void setRunning(bool on) {
  if (running == on) return;
  if (on) {
    running = true;
    // Adjust stageStartMs to now - current accumulated
    stageStartMs = millis();
    lastTickMs = millis();
  } else {
    // Pause: accumulate elapsed so far for reporting
    if (running) {
      stageAccumMs += (millis() - stageStartMs);
    }
    running = false;
  }
}

void startBuzzerSequenceForStageEnd(uint8_t stageIdx) {
  buzzerActive = true;
  buzzerBeepsDone = 0;
  buzzerToneOn = false;
  buzzerNextMs = millis();
  buzzerBeepsDone = 0;

  // store how many beeps needed by stage
  // (use beepCountPerStage array directly during scheduler)
  (void)stageIdx;
}

void buzzerService() {
  if (!buzzerActive) return;

  uint8_t targetBeep = beepCountPerStage[currentStage]; // currentStage may have changed; keep consistent by calling at end and servicing before stage changes?
  // To ensure correctness, we should latch targetBeep at start.
  // We'll instead determine targetBeep by latching a global.
}

// Latch buzzer pattern count at trigger
uint8_t buzzerTargetBeepCount = 0;
uint8_t buzzerStageIndexLatched = 0;

void startBuzzerSequenceForStageEndLatched(uint8_t stageIdx) {
  buzzerActive = true;
  buzzerBeepsDone = 0;
  buzzerToneOn = false;
  buzzerNextMs = millis();
  buzzerTargetBeepCount = beepCountPerStage[stageIdx];
  buzzerStageIndexLatched = stageIdx;
}

void buzzerService2() {
  if (!buzzerActive) return;

  uint32_t now = millis();
  if ((int32_t)(now - buzzerNextMs) < 0) return;

  if (!buzzerToneOn) {
    // Turn on
    if (buzzerBeepsDone >= buzzerTargetBeepCount) {
      // Done
      buzzerActive = false;
      digitalWrite(BUZZER_PIN, LOW);
      return;
    }
    // Start beep
    buzzerToneOn = true;
    buzzerNextMs = now + beepOnMs;

    // "different times" - keep consistent tone
    tone(BUZZER_PIN, 2000);
  } else {
    // Turn off
    buzzerToneOn = false;
    buzzerBeepsDone++;
    buzzerNextMs = now + beepOffMs;
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// Stage completion flow controller
bool waitingForBuzzerToFinish = false;
uint8_t stageIndexToAdvanceAfterBuzzer = 0;

void onStageFinished() {
  // Record actual elapsed time for this stage (ms)
  uint32_t now = millis();
  stageAccumMs += (now - stageStartMs);
  elapsedStageMs[currentStage] = stageAccumMs;

  // Serial output for stage actual time will be printed after buzzer? requirement says output process complete and each stage actual time.
  // We'll output immediately upon completion (buzzer doesn't affect correctness).
  Serial.print("Stage ");
  Serial.print(currentStage + 1);
  Serial.print(" elapsed ms: ");
  Serial.println(elapsedStageMs[currentStage]);

  // Start buzzer pattern for this stage end
  startBuzzerSequenceForStageEndLatched(currentStage);
  waitingForBuzzerToFinish = true;
  stageIndexToAdvanceAfterBuzzer = currentStage;

  // Remaining becomes 0
  remainingSec = 0;
}

void advanceToNextStageOrFinish() {
  // After buzzer completes, move to next stage or finish
  uint8_t finishedStage = stageIndexToAdvanceAfterBuzzer;
  waitingForBuzzerToFinish = false;

  uint8_t next = finishedStage + 1;
  if (next < STAGES) {
    beginStage(next);
  } else {
    // All stages complete
    setRunning(false);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("process complete");
    // Keep buzzer off
    buzzerActive = false;
    waitingForBuzzerToFinish = false;
  }
}

unsigned long lastDisplayUpdateMs = 0;

void updateDisplay() {
  // Display remaining time of current stage while running; when paused, keep current remaining.
  // When LED is on (finished), show total maybe.
  uint32_t totalRemaining = remainingSec;
  if (LED_PIN && digitalRead(LED_PIN) == HIGH) {
    // finished: show 00:00
    tm.displayTimeMMSS(0);
    return;
  }

  // mm:ss for remainingSec
  tm.displayTimeMMSS(totalRemaining);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  btnAdd.begin(BTN_ADD_PIN, true);      // active-low
  btnNext.begin(BTN_NEXT_PIN, true);    // active-low
  btnStartPause.begin(BTN_STARTPAUSE, true); // active-low

  tm.begin();
  tm.setBrightness(6);

  // Initial stage
  beginStage(0);
  running = false;
  updateDisplay();
}

void handleButtonAdd() {
  if (!btnAdd.pressedEvent()) return;

  // Increase time of current stage by 10 seconds (configurable behavior)
  // Preserve behavior: "increase time"
  // We'll increment by 10 seconds and cap at 99 minutes 59 seconds (5980s) for mm:ss display safety.
  uint32_t inc = 10;
  uint32_t maxSec = 99UL * 60UL + 59UL;
  uint32_t newDur = stageDur[currentStage] + inc;
  if (newDur > maxSec) newDur = maxSec;
  stageDur[currentStage] = newDur;

  if (!running) {
    // If not running, reflect immediately to remaining
    remainingSec = stageDur[currentStage];
  } else {
    // If running, do not retroactively change remainingSec (time already counting down),
    // but adding time should extend remaining of current stage.
    remainingSec = stageDur[currentStage];
  }

  updateDisplay();
}

void handleButtonNextStage() {
  if (!btnNext.pressedEvent()) return;

  // Switch stage
  // If LED finished, ignore
  if (digitalRead(LED_PIN) == HIGH) return;

  uint8_t nextStage = currentStage + 1;
  if (nextStage >= STAGES) nextStage = 0;

  // When switching stage, update remaining and stage timers.
  bool wasRunning = running;
  setRunning(false);
  beginStage(nextStage);
  if (wasRunning) {
    // Starting stage immediately if previously running
    setRunning(true);
  }
  updateDisplay();
}

void handleButtonStartPause() {
  if (!btnStartPause.pressedEvent()) return;

  // If finished, ignore
  if (digitalRead(LED_PIN) == HIGH) return;

  // If not running and currently at stage 0 with remaining 0, reset stage remaining from duration.
  if (!running) {
    // Start or resume
    setRunning(true);
    // If remaining is 0 (e.g., user reduced? not provided), set from configured duration
    if (remainingSec == 0) remainingSec = stageDur[currentStage];
    // ensure timer baseline exists
    stageStartMs = millis();
    lastTickMs = millis();
    stageAccumMs = 0;
    updateDisplay();
  } else {
    setRunning(false);
    updateDisplay();
  }
}

void loop() {
  handleButtonAdd();
  handleButtonNextStage();
  handleButtonStartPause();

  // Buzzer non-blocking service
  buzzerService2();

  // Stage progression tick
  if (running && !waitingForBuzzerToFinish) {
    uint32_t now = millis();
    if ((int32_t)(now - lastTickMs) >= 1000) {
      // update remaining by number of full seconds elapsed
      uint32_t elapsedSeconds = (now - lastTickMs) / 1000;
      lastTickMs += elapsedSeconds * 1000;

      if (elapsedSeconds > remainingSec) elapsedSeconds = remainingSec;
      remainingSec -= elapsedSeconds;

      // Display update
      updateDisplay();

      if (remainingSec == 0) {
        // Stage end: handle completion and start buzzer
        onStageFinished();
      }
    }
  }

  // When buzzer finishes after stage completion, advance
  if (waitingForBuzzerToFinish && !buzzerActive) {
    advanceToNextStageOrFinish();
    updateDisplay();
  }

  // Periodically keep display responsive
  uint32_t now = millis();
  if ((int32_t)(now - lastDisplayUpdateMs) >= 150) {
    lastDisplayUpdateMs = now;
    if (digitalRead(LED_PIN) != HIGH) {
      updateDisplay();
    }
  }
}

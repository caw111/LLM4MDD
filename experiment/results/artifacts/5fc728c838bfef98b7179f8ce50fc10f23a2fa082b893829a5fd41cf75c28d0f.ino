#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_BTN_INC = 2;   // D2 加一
static const uint8_t PIN_BTN_DEC = 3;   // D3 减一
static const uint8_t PIN_LED     = 13;  // D13 指示：偶数亮，奇数灭
static const uint8_t PIN_BUZZER  = 6;   // D6 蜂鸣器
static const uint8_t PIN_TM_CLK  = 4;   // D4 CLK
static const uint8_t PIN_TM_DIO  = 5;   // D5 DIO

TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

// --- State ---
static int8_t countValue = 0;          // 当前计数（显示与逻辑使用）
static bool locked = false;           // 达到15后报警并锁定，直到长按D3复位

// --- Debounce / long press ---
static bool lastIncReading = HIGH;
static bool lastDecReading = HIGH;
static bool stableInc = HIGH;
static bool stableDec = HIGH;

static unsigned long incLastChangeMs = 0;
static unsigned long decLastChangeMs = 0;

static bool incPressedEvent = false;
static bool decPressedEvent = false;

// long-press tracking (D3)
static bool decPressTracking = false;
static unsigned long decPressStartMs = 0;
static bool decLongPressFired = false;

// Timing (tuned for human buttons)
static const unsigned long DEBOUNCE_MS = 30;
static const unsigned long LONGPRESS_MS = 1200;

static void updateDebouncedButtons() {
  // Read raw (pullups => idle HIGH, pressed LOW)
  bool incReading = digitalRead(PIN_BTN_INC);
  bool decReading = digitalRead(PIN_BTN_DEC);

  unsigned long now = millis();

  // --- INC (D2) debounce ---
  if (incReading != lastIncReading) {
    lastIncChangeMs = now;
    lastIncReading = incReading;
  }
  if ((now - incLastChangeMs) >= DEBOUNCE_MS && incReading != stableInc) {
    stableInc = incReading;
    // falling edge: HIGH->LOW indicates press
    if (stableInc == LOW) {
      incPressedEvent = true;
    }
  }

  // --- DEC (D3) debounce ---
  if (decReading != lastDecReading) {
    decLastChangeMs = now;
    lastDecReading = decReading;
  }
  if ((now - decLastChangeMs) >= DEBOUNCE_MS && decReading != stableDec) {
    stableDec = decReading;

    if (stableDec == LOW) {
      // Press start
      decPressedEvent = true;      // also treat as a press event for single decrement
      decPressTracking = true;
      decPressStartMs = now;
      decLongPressFired = false;
    } else {
      // Release
      decPressTracking = false;
      decLongPressFired = false;
    }
  }

  // Long press detection (while held LOW)
  if (decPressTracking && !decLongPressFired) {
    if (now - decPressStartMs >= LONGPRESS_MS) {
      decLongPressFired = true;
      // fire long press event by applying reset logic in loop()
    }
  }
}

static void applyDisplayAndLed() {
  // Display current value on TM1637
  // Use 1-digit display (counts 0..15 but requirement states "current value";
  // with 0..15, we can show two-digit when needed; TM1637Display auto handles).
  tm1637.showNumberDec(countValue, false, 2, 0); // no leading zeros, 2 digits for 0..15

  // LED: even -> ON, odd -> OFF
  if ((countValue % 2) == 0) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }
}

static void alarmAndLockBehavior() {
  // When count reaches 15: beep and lock counting until long-press D3 reset.
  // Keep buzzing while locked.
  // Use non-blocking tone toggling by using tone() with fixed frequency.
  tone(PIN_BUZZER, 2000); // 2kHz steady alarm
}

static void stopAlarm() {
  noTone(PIN_BUZZER);
}

void setup() {
  pinMode(PIN_BTN_INC, INPUT_PULLUP);
  pinMode(PIN_BTN_DEC, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  stopAlarm();

  tm1637.setBrightness(7); // 0..7 (kept bright enough for visibility)

  // Initial states
  lastIncReading = digitalRead(PIN_BTN_INC);
  lastDecReading = digitalRead(PIN_BTN_DEC);
  stableInc = lastIncReading;
  stableDec = lastDecReading;
  incLastChangeMs = millis();
  decLastChangeMs = millis();

  countValue = 0;
  locked = false;

  applyDisplayAndLed();
}

void loop() {
  updateDebouncedButtons();

  // If locked, disable single-step operations and handle only long-press reset.
  if (locked) {
    alarmAndLockBehavior();

    // Long-press reset: long按D3复位
    if (decLongPressFired) {
      // Reset all: unlock and clear count
      locked = false;
      stopAlarm();
      countValue = 0;
      applyDisplayAndLed();

      // Consume the event so we don't immediately refire
      decLongPressFired = false;
    }

    // Clear one-shot press events if any happened while locked
    incPressedEvent = false;
    decPressedEvent = false;

    return;
  } else {
    stopAlarm();
  }

  // Handle INC (D2) single press: 加一
  if (incPressedEvent) {
    incPressedEvent = false;

    countValue++;
    if (countValue > 15) countValue = 15; // prevent beyond 15 since alarm locks at 15
    applyDisplayAndLed();

    // When count reaches 15: beep and lock
    if (countValue == 15) {
      locked = true;
      // alarm starts immediately in next loop iteration; also update now
      alarmAndLockBehavior();
    }
  }

  // Handle DEC (D3) single press: 减一
  if (decPressedEvent) {
    decPressedEvent = false;

    // Normal decrement only when not locked; requirement says long-press D3 resets from lock.
    countValue--;
    if (countValue < 0) countValue = 0;
    applyDisplayAndLed();
  }
}
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
static bool locked = false;            // 达到15后报警并锁定，直到长按D3复位

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

static void applyDisplayAndLed() {
  // 显示当前值（0..15，2位显示）
  tm1637.showNumberDec(countValue, false, 2, 0);

  // LED: even -> ON, odd -> OFF
  digitalWrite(PIN_LED, (countValue % 2 == 0) ? HIGH : LOW);
}

static void alarmAndLockBehavior() {
  // 当计数达到15时蜂鸣器报警并锁定计数
  tone(PIN_BUZZER, 2000); // 2kHz steady alarm
}

static void stopAlarm() {
  noTone(PIN_BUZZER);
}

static void updateDebouncedButtons() {
  // Read raw (pullups => idle HIGH, pressed LOW)
  bool incReading = digitalRead(PIN_BTN_INC);
  bool decReading = digitalRead(PIN_BTN_DEC);

  unsigned long now = millis();

  // --- INC (D2) debounce ---
  if (incReading != lastIncReading) {
    incLastChangeMs = now;
    lastIncReading = incReading;
  }
  if ((now - incLastChangeMs) >= DEBOUNCE_MS && incReading != stableInc) {
    stableInc = incReading;
    if (stableInc == LOW) {
      // falling edge indicates press
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
      decPressedEvent = true; // also count as a single decrement press
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
    }
  }
}

void setup() {
  pinMode(PIN_BTN_INC, INPUT_PULLUP);
  pinMode(PIN_BTN_DEC, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  stopAlarm();

  tm1637.setBrightness(7); // 0..7

  // Initial button readings
  lastIncReading = digitalRead(PIN_BTN_INC);
  lastDecReading = digitalRead(PIN_BTN_DEC);
  stableInc = lastIncReading;
  stableDec = lastDecReading;

  incLastChangeMs = millis();
  decLastChangeMs = millis();

  countValue = 0;
  locked = false;
  decPressTracking = false;
  decPressStartMs = 0;
  decLongPressFired = false;

  applyDisplayAndLed();
}

void loop() {
  updateDebouncedButtons();

  if (locked) {
    // Keep alarm sounding while locked
    alarmAndLockBehavior();

    // 长按 D3 复位
    if (decLongPressFired) {
      locked = false;
      stopAlarm();
      countValue = 0;
      applyDisplayAndLed();

      // Consume the long-press event
      decLongPressFired = false;
    }

    // Ignore any one-shot button events while locked (consume flags)
    incPressedEvent = false;
    decPressedEvent = false;
    return;
  } else {
    stopAlarm();
  }

  // D2: 加一
  if (incPressedEvent) {
    incPressedEvent = false;

    countValue++;
    if (countValue > 15) countValue = 15;
    applyDisplayAndLed();

    if (countValue == 15) {
      locked = true;
      alarmAndLockBehavior(); // start immediately
    }
  }

  // D3: 减一
  if (decPressedEvent) {
    decPressedEvent = false;

    countValue--;
    if (countValue < 0) countValue = 0;
    applyDisplayAndLed();
  }
}
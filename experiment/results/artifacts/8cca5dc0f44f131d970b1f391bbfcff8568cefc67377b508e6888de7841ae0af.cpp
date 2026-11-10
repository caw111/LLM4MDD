#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Arduino Uno core pin constants (D2 etc.) may not be available in some build environments.
// Use numeric pins to avoid compilation errors while preserving wiring intent.
static const uint8_t PIN_Q1 = 2;   // D2  Button for player 1
static const uint8_t PIN_Q2 = 3;   // D3  Button for player 2
static const uint8_t PIN_Q3 = 7;   // D7  Button for player 3
static const uint8_t PIN_Q4 = 8;   // D8  Button for player 4
static const uint8_t PIN_RESET = 9; // D9  Reset button (long-press 3s clears stats)

static const uint8_t PIN_LED = 13;   // D13 LED
static const uint8_t PIN_BUZZ = 6;    // D6  Buzzer (tone output)

static const uint8_t PIN_TM1637_CLK = 4; // D4 CLK
static const uint8_t PIN_TM1637_DIO = 5; // D5 DIO

// --- TM1637 ---
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// --- Configuration / behavior ---
static const uint32_t SERIAL_BAUD = 9600;
static const uint16_t TM1637_BRIGHTNESS = 7;

// Debounce settings
static const uint16_t DEBOUNCE_MS = 30;

// Buzzer confirmation: short鸣
static const uint16_t BUZZ_TONE_HZ = 2000;
static const uint16_t BUZZ_SHORT_MS = 80;

// Locking: once first player pressed, lock inputs until reset
static bool locked = false;
static uint8_t winner = 0; // 0=none, 1..4=player number

// Statistics (counts total number of wins per player)
static uint32_t stats[5] = {0, 0, 0, 0, 0}; // index 1..4 used
static uint32_t totalRounds = 0;

// Timing (for serial timestamp)
static uint32_t roundStartMs = 0; // when system becomes unlocked (after reset)

// Reset button press tracking for 3s long-press clear
static bool resetPrevStable = false;
static uint32_t resetLastChangeMs = 0;

// Player button debounce state
static bool playerPrevRaw[5] = {false, false, false, false, false};
static bool playerPrevStable[5] = {false, false, false, false, false};
static uint32_t playerLastChangeMs[5] = {0, 0, 0, 0, 0};

// Track reset press duration and long-handled
static uint32_t resetPressStartMs = 0;
static bool resetLongHandled = false;

// Button handling helpers
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readDebouncedPlayer(uint8_t idx, uint8_t pin);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readDebouncedReset(bool &stableStateOut);
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void buzzShort();
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void displayPlayer(uint8_t p);
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void clearStats();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void doResetUnlock(bool clearStatistics);
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 140 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readDebouncedPlayer(uint8_t idx, uint8_t pin) {
  bool raw = (digitalRead(pin) == LOW); // active-low buttons
  if (raw != playerPrevRaw[idx]) {
    playerPrevRaw[idx] = raw;
    playerLastChangeMs[idx] = millis();
  }
  if ((millis() - playerLastChangeMs[idx]) >= DEBOUNCE_MS) {
    playerPrevStable[idx] = raw;
  }
  return playerPrevStable[idx];
}

static bool readDebouncedReset(bool &stableStateOut) {
  bool raw = (digitalRead(PIN_RESET) == LOW); // active-low
  if (raw != resetPrevStable) {
    resetPrevStable = raw;
    resetLastChangeMs = millis();
  }
  if ((millis() - resetLastChangeMs) >= DEBOUNCE_MS) {
    stableStateOut = raw;
    return true;
  }
  stableStateOut = resetPrevStable;
  return false;
}

static void buzzShort() {
  tone(PIN_BUZZ, BUZZ_TONE_HZ);
  delay(BUZZ_SHORT_MS);
  noTone(PIN_BUZZ);
}

static void displayPlayer(uint8_t p) {
  // Display player number 1..4, otherwise show 0
  if (p >= 1 && p <= 4) {
    display.showNumberDec(p, false, 2, 0); // no leading zeros, 2 digits
  } else {
    display.showNumberDec(0, false, 2, 0);
  }
}

static void clearStats() {
  for (uint8_t i = 1; i <= 4; i++) stats[i] = 0;
  totalRounds = 0;
}

static void doResetUnlock(bool clearStatistics) {
  locked = false;
  winner = 0;
  roundStartMs = millis();
  digitalWrite(PIN_LED, LOW);
  displayPlayer(0);
  if (clearStatistics) clearStats();
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  display.setBrightness(TM1637_BRIGHTNESS);
  displayPlayer(0);

  pinMode(PIN_Q1, INPUT_PULLUP);
  pinMode(PIN_Q2, INPUT_PULLUP);
  pinMode(PIN_Q3, INPUT_PULLUP);
  pinMode(PIN_Q4, INPUT_PULLUP);

  pinMode(PIN_RESET, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BUZZ, OUTPUT);
  noTone(PIN_BUZZ);

  for (uint8_t i = 1; i <= 4; i++) {
    playerPrevRaw[i] = false;
    playerPrevStable[i] = false;
    playerLastChangeMs[i] = millis();
  }

  doResetUnlock(false);
}

void loop() {
  // --- Reset button handling (long press 3s clears cumulative stats) ---
  bool resetStable = false;
  readDebouncedReset(resetStable);

  // Behavior: in both locked/unlocked modes, long press clears stats + unlocks.
  if (resetStable && !resetLongHandled) {
    if (resetPressStartMs == 0) resetPressStartMs = millis();
    if ((millis() - resetPressStartMs) >= 3000UL) {
      resetLongHandled = true;
      doResetUnlock(true);
    }
  }

  if (!resetStable) {
    // Short press unlocks if long wasn't handled
    if (!resetLongHandled) {
      doResetUnlock(false);
    }
    resetPressStartMs = 0;
    resetLongHandled = false;
  }

  // --- If locked, ignore player inputs until reset happens ---
  if (locked) return;

  // --- Player buttons: record only the first pressed player number ---
  bool p1 = readDebouncedPlayer(1, PIN_Q1);
  bool p2 = readDebouncedPlayer(2, PIN_Q2);
  bool p3 = readDebouncedPlayer(3, PIN_Q3);
  bool p4 = readDebouncedPlayer(4, PIN_Q4);

  uint8_t candidate = 0;
  if (p1) candidate = 1;
  else if (p2) candidate = 2;
  else if (p3) candidate = 3;
  else if (p4) candidate = 4;

  if (candidate != 0) {
    locked = true;
    winner = candidate;

    digitalWrite(PIN_LED, HIGH);

    // Count stats
    stats[winner]++;
    totalRounds++;

    uint32_t lockedAtMs = millis();
    uint32_t t = lockedAtMs - roundStartMs; // 抢答时间戳(ms)
    Serial.print("抢答时间戳(ms)=");
    Serial.print(t);
    Serial.print(", 选手编号=");
    Serial.println(winner);

    // Buzzer short confirm
    buzzShort();

    // Display winner
    displayPlayer(winner);
  }
}

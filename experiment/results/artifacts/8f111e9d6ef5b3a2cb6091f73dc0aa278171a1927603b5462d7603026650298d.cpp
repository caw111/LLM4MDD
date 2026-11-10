#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// --- Pins (per authoritative wiring) ---
static const uint8_t PIN_Q1 = D2;   // Button for player 1
static const uint8_t PIN_Q2 = D3;   // Button for player 2
static const uint8_t PIN_Q3 = D7;   // Button for player 3
static const uint8_t PIN_Q4 = D8;   // Button for player 4
static const uint8_t PIN_RESET = D9; // Reset button (long-press 3s clears stats)

static const uint8_t PIN_LED = D13;  // Output LED
static const uint8_t PIN_BUZZ = D6;  // Buzzer

static const uint8_t PIN_TM1637_CLK = D4;
static const uint8_t PIN_TM1637_DIO = D5;

// --- TM1637 ---
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// --- Configuration / behavior ---
static const uint32_t SERIAL_BAUD = 9600;

// Keep all behavior explicit
static const uint16_t TM1637_BRIGHTNESS = 7;

// Debounce settings (do not remove behavior; just implement stable input)
static const uint16_t DEBOUNCE_MS = 30;

// Buzzer confirmation: "short鸣" (short beep)
static const uint16_t BUZZ_TONE_HZ = 2000;
static const uint16_t BUZZ_SHORT_MS = 80;

// Locking: once first player pressed, lock inputs until reset
static bool locked = false;
static uint8_t winner = 0; // 0=none, 1..4=player number

// Statistics (counts total number of wins per player)
static uint32_t stats[5] = {0, 0, 0, 0, 0};
static uint32_t totalRounds = 0;

// Timing (for serial timestamp)
static uint32_t roundStartMs = 0; // when system becomes unlocked (after reset)
static uint32_t lockedAtMs = 0;

// Reset button press tracking for 3s long-press clear
static bool resetPrevStable = false;
static uint32_t resetPressStartMs = 0;
static bool resetLongHandled = false;

// Button handling helpers
static uint32_t lastSampleMs = 0;

// Debounced state per player
static bool playerPrevRaw[5] = {false, false, false, false};
static bool playerPrevStable[5] = {false, false, false, false};
static uint32_t playerLastChangeMs[5] = {0, 0, 0, 0, 0};

// For reset button debouncing
static uint32_t resetLastChangeMs = 0;

// Read inputs with debounce
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readDebouncedPlayer(uint8_t idx, uint8_t pin);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readDebouncedReset(bool &stableStateOut);
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzShort();
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayPlayer(uint8_t p);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void clearStats();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void doResetUnlock(bool clearStatistics);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 152 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readDebouncedPlayer(uint8_t idx, uint8_t pin) {
  bool raw = (digitalRead(pin) == LOW); // assuming active-low buttons
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
    return true; // changed after debounce? (we also use stableStateOut anyway)
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
  // Display player number 1..4; else blank/0
  if (p >= 1 && p <= 4) {
    display.showNumberDec(p, false, 2, 0); // no leading zeros, 2 digits
  } else {
    display.showNumberDec(0, false, 2, 0);
  }
}

static void clearStats() {
  for (uint8_t i = 1; i <= 4; i++) stats[i] = 0;
  totalRounds = 0;
  // Keep display and lock state controlled by reset logic
}

static void doResetUnlock(bool clearStatistics) {
  locked = false;
  winner = 0;
  lockedAtMs = 0;
  roundStartMs = millis();
  digitalWrite(PIN_LED, LOW);
  displayPlayer(0);
  if (clearStatistics) clearStats();
}

void setup() {
  // Serial
  Serial.begin(SERIAL_BAUD);

  // TM1637 init
  display.setBrightness(TM1637_BRIGHTNESS);
  displayPlayer(0);

  // IO
  pinMode(PIN_Q1, INPUT_PULLUP);
  pinMode(PIN_Q2, INPUT_PULLUP);
  pinMode(PIN_Q3, INPUT_PULLUP);
  pinMode(PIN_Q4, INPUT_PULLUP);

  pinMode(PIN_RESET, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BUZZ, OUTPUT);
  noTone(PIN_BUZZ);

  // Initialize debounce tracking
  for (uint8_t i = 1; i <= 4; i++) {
    playerPrevRaw[i] = false;
    playerPrevStable[i] = false;
    playerLastChangeMs[i] = millis();
  }

  // Unlock initial state
  doResetUnlock(false);
}

void loop() {
  // --- Reset button handling (long press 3s clears cumulative stats) ---
  bool resetStable = false;
  readDebouncedReset(resetStable);

  if (!locked) {
    // When unlocked, we still must allow reset action
    // Track reset press duration
    if (resetStable && !resetLongHandled) {
      if (resetPressStartMs == 0) resetPressStartMs = millis();
      if ((millis() - resetPressStartMs) >= 3000UL) {
        // Long press: clear stats; then perform a normal unlock reset
        resetLongHandled = true;
        doResetUnlock(true);
      }
    }

    if (!resetStable) {
      resetPressStartMs = 0;
      resetLongHandled = false;
    }
  } else {
    // When locked, same reset logic; short press unlocks, long press clears stats then unlocks
    if (resetStable && !resetLongHandled) {
      if (resetPressStartMs == 0) resetPressStartMs = millis();
      if ((millis() - resetPressStartMs) >= 3000UL) {
        resetLongHandled = true;
        doResetUnlock(true);
      }
    }

    if (!resetStable) {
      // Short press unlock (if long wasn't handled)
      if (!resetLongHandled) {
        doResetUnlock(false);
      }
      resetPressStartMs = 0;
      resetLongHandled = false;
    }
  }

  // --- If locked, ignore player inputs until reset happens ---
  if (locked) return;

  // --- Player buttons: record only the first pressed player number ---
  // Detect rising edge (pressed event), not just level, to avoid repeated triggers
  bool p1 = readDebouncedPlayer(1, PIN_Q1);
  bool p2 = readDebouncedPlayer(2, PIN_Q2);
  bool p3 = readDebouncedPlayer(3, PIN_Q3);
  bool p4 = readDebouncedPlayer(4, PIN_Q4);

  // We'll consider "pressed" as active-low stable true.
  // To preserve "first按下", we check in fixed order within the loop.
  // If multiple are pressed simultaneously, the earlier in this order will be captured,
  // matching typical deterministic behavior.
  uint8_t candidate = 0;
  if (p1) candidate = 1;
  else if (p2) candidate = 2;
  else if (p3) candidate = 3;
  else if (p4) candidate = 4;

  if (candidate != 0) {
    locked = true;
    winner = candidate;
    lockedAtMs = millis();
    digitalWrite(PIN_LED, HIGH);

    // Count stats
    stats[winner]++;
    totalRounds++;

    // Timestamp: "抢答时间戳" and winner via serial
    // Use time since round start (or since unlock) for meaningful timestamp
    uint32_t t = lockedAtMs - roundStartMs;
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

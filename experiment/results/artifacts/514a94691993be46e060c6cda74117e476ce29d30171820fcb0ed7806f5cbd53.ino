#include <Arduino.h>
#include <TM1637Display.h>

// ---------------- Pin assignments (fixed for Arduino UNO) ----------------
static const uint8_t BTN_1 = 2;   // D2
static const uint8_t BTN_2 = 3;   // D3
static const uint8_t BTN_3 = 7;   // D7
static const uint8_t BTN_4 = 8;   // D8

static const uint8_t RESET_BTN = 9;   // D9 long-press 3s clears accumulated stats
static const uint8_t BUZZER_PIN = 6;  // D6
static const uint8_t LED_PIN = 13;     // D13

static const uint8_t TM_CLK = 4;       // D4
static const uint8_t TM_DIO = 5;       // D5

static const uint8_t NUM_TM_DIGITS = 4;

// TM1637 display
TM1637Display display(TM_CLK, TM_DIO);

// ---------------- Configuration / behavior ----------------
static const uint32_t RESET_HOLD_MS = 3000;     // long press clears accumulated stats
static const uint32_t DEBOUNCE_MS   = 30;       // input debounce
static const uint32_t LOCKOUT_MS    = 0;        // lock indefinitely until reset (required)

// ---------------- State ----------------
static bool locked = false;        // after first valid press, lock other inputs until reset
static uint8_t winner = 0;         // 1..4
static uint32_t pressTimeMs = 0;  // captured timestamp at winner press

static uint8_t lastStableBtn[5] = {HIGH, HIGH, HIGH, HIGH, HIGH}; // indices 1..4
static uint32_t lastChangeMs[5] = {0, 0, 0, 0, 0};

// Reset button long-press tracking
static bool resetPressActive = false;
static uint32_t resetPressStartMs = 0;

// Accumulated stats (cleared by 3s long press)
static uint32_t totalBets[5] = {0, 0, 0, 0, 0}; // indices 1..4

// ---------------- Helpers ----------------
static void beepShort(uint16_t durationMs = 80, uint16_t freq = 2000) {
  tone(BUZZER_PIN, freq, durationMs);
}

static void showNumber(uint8_t num) {
  // Display winner number on 4-digit TM1637 (right aligned).
  // Suppress leading zeros via the second parameter's colon/segment control used by this library.
  display.showNumberDecEx(num, 0b01000000, true, NUM_TM_DIGITS);
}

static bool readButtonPressed(uint8_t pin) {
  // Buttons are wired with INPUT_PULLUP: pressed => LOW
  return digitalRead(pin) == LOW;
}

static void updateParticipantDebounce(uint8_t idx, uint8_t pin, bool &stablePressed) {
  // stablePressed output means: debounced "pressed" state for this button.
  bool rawPressed = readButtonPressed(pin);       // true when pressed
  uint8_t prevStableLevel = lastStableBtn[idx];    // HIGH or LOW

  bool prevStablePressed = (prevStableLevel == LOW);

  // If raw pressed state differs from stable pressed state, debounce.
  if (rawPressed != prevStablePressed) {
    uint32_t now = millis();
    if (now - lastChangeMs[idx] >= DEBOUNCE_MS) {
      lastStableBtn[idx] = rawPressed ? LOW : HIGH;
      lastChangeMs[idx] = now;
      stablePressed = rawPressed;
    } else {
      // Keep previous stablePressed; do not update stable level yet.
      stablePressed = prevStablePressed;
    }
  } else {
    stablePressed = prevStablePressed;
  }
}

static void initParticipantDebounce() {
  uint8_t pins[5] = {0, BTN_1, BTN_2, BTN_3, BTN_4};
  uint32_t now = millis();
  for (uint8_t i = 1; i <= 4; i++) {
    uint8_t level = digitalRead(pins[i]);
    lastStableBtn[i] = level;
    lastChangeMs[i] = now;
  }
}

static void resetSystem(bool clearStats) {
  locked = false;
  winner = 0;
  pressTimeMs = 0;
  display.clear();
  digitalWrite(LED_PIN, LOW);

  if (clearStats) {
    for (uint8_t i = 1; i <= 4; i++) totalBets[i] = 0;
  }
}

// Returns true if locked became true and winner was set (caller handles display/serial/beep)
static bool tryCaptureWinner() {
  if (locked) return false;

  // Scan order 1..4. Requirement: only record the first pressed contestant number.
  // With this approach, if multiple are pressed, the one whose debounce first confirms
  // in scan order will be captured.
  uint8_t pins[5] = {0, BTN_1, BTN_2, BTN_3, BTN_4};
  for (uint8_t idx = 1; idx <= 4; idx++) {
    bool stablePressed = false;
    updateParticipantDebounce(idx, pins[idx], stablePressed);
    if (stablePressed) {
      winner = idx;
      return true;
    }
  }
  return false;
}

static void handleResetButton() {
  // Any press unlocks inputs.
  // Long-press 3 seconds clears accumulated抢答统计 (only once per hold).
  const bool DEBOUNCE = true;

  static uint8_t stableLevel = HIGH;
  static uint32_t lastStableChange = 0;
  static bool clearedOnHold = false;

  bool rawPressed = readButtonPressed(RESET_BTN); // true when pressed (LOW)
  uint8_t rawLevel = rawPressed ? LOW : HIGH;

  uint32_t now = millis();

  if (DEBOUNCE) {
    if (rawLevel != stableLevel) {
      if (now - lastStableChange >= DEBOUNCE_MS) {
        stableLevel = rawLevel;
        lastStableChange = now;
      }
    }
  }

  bool resetPressedStable = (stableLevel == LOW);

  if (resetPressedStable) {
    if (!resetPressActive) {
      resetPressActive = true;
      resetPressStartMs = now;
      clearedOnHold = false;

      // Unlock immediately (do not clear stats yet)
      resetSystem(false);
    } else {
      if (!clearedOnHold && (now - resetPressStartMs) >= RESET_HOLD_MS) {
        clearedOnHold = true;
        resetSystem(true);

        // Confirmation: show 0 briefly + short beep + LED pulse
        display.showNumberDecEx(0, 0b01000000, true, NUM_TM_DIGITS);
        beepShort(80, 2000);
        digitalWrite(LED_PIN, HIGH);
        delay(80);
        digitalWrite(LED_PIN, LOW);
      }
    }
  } else {
    resetPressActive = false;
  }
}

// ---------------- Arduino lifecycle ----------------
void setup() {
  Serial.begin(9600);

  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);
  pinMode(BTN_4, INPUT_PULLUP);

  pinMode(RESET_BTN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  display.setBrightness(0x0f); // max brightness
  display.clear();

  initParticipantDebounce();
  resetSystem(false); // starts unlocked
}

void loop() {
  handleResetButton();

  if (!locked) {
    if (tryCaptureWinner()) {
      locked = true;

      pressTimeMs = millis();

      // Update stats
      if (winner >= 1 && winner <= 4) {
        totalBets[winner]++;
      }

      // Short beep confirmation
      beepShort(80, 2000);

      // Display winner number
      showNumber(winner);

      // LED indicates locked (wired requirement preserved)
      digitalWrite(LED_PIN, HIGH);

      // Serial output:抢答时间戳和选手编号
      Serial.print(pressTimeMs);
      Serial.print(",");
      Serial.println(winner);
    }
  } else {
    // Locked: ignore participant inputs until reset
    // Keep buzzer quiet; tone auto-stops after duration.
  }
}
#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments (authoritative)
static const uint8_t BTN_1 = D2;
static const uint8_t BTN_2 = D3;
static const uint8_t BTN_3 = D7;
static const uint8_t BTN_4 = D8;

static const uint8_t RESET_BTN = D9;   // long-press 3s clears accumulated stats
static const uint8_t BUZZER_PIN = D6;

static const uint8_t LED_PIN = D13;

static const uint8_t TM_CLK = D4;
static const uint8_t TM_DIO = D5;

static const uint8_t NUM_TM_DIGITS = 4;

// TM1637 pins and display
TM1637Display display(TM_CLK, TM_DIO);

// ---------------- Configuration / behavior ----------------
static const uint32_t RESET_HOLD_MS = 3000;   // long press clears accumulated stats
static const uint32_t DEBOUNCE_MS   = 30;     // input debounce
static const uint32_t LOCKOUT_MS    = 0;      // lock indefinitely until reset (required)
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20; // provided; not required for code

// ---------------- State ----------------
static bool locked = false;      // after first valid press, lock other inputs until reset
static uint8_t winner = 0;       // 1..4
static uint32_t t_lock_ms = 0;   // time when locked (for internal timing)
static uint32_t pressTimeMs = 0; // captured timestamp at winner press (millis)

// Debounce tracking for each participant button
static uint8_t lastStable[5] = {HIGH, HIGH, HIGH, HIGH, HIGH}; // indices 1..4 and 0 unused
static uint32_t lastChangeMs[5] = {0, 0, 0, 0, 0};

// Reset button tracking for long press
static bool resetStable = false;
static uint32_t resetPressStartMs = 0;
static bool resetPressActive = false;

// Accumulated stats (cleared by 3s long press)
static uint32_t totalBets[5] = {0, 0, 0, 0, 0}; // indices 1..4

// ---------------- Helpers ----------------
static void beepShort(uint16_t durationMs = 80, uint16_t freq = 2000) {
  tone(BUZZER_PIN, freq, durationMs);
}

static void showNumber(uint8_t num) {
  // Requirements: system displays the first pressed player's number.
  // We'll display as right-aligned on 4-digit module.
  // Common approach: set "0x00" leading segments are blank.
  // Using showNumberDecEx with leading zeros suppressed.
  display.showNumberDecEx(num, (uint8_t)0b01000000, true, NUM_TM_DIGITS); 
  // If above flags are not supported as expected on your library version,
  // fallback to direct showNumberDec:
  // display.showNumberDec(num, true, NUM_TM_DIGITS);
}

static bool readButtonPressed(uint8_t pin) {
  // Assuming buttons pull input LOW when pressed (typical with INPUT_PULLUP).
  // Circuit note says "buttons D2,D3,D7,D8, D9 as reset". We'll use INPUT_PULLUP.
  return digitalRead(pin) == LOW;
}

static void updateDebounceParticipant(uint8_t idx, uint8_t pin, bool &stablePressed) {
  // idx: 1..4
  bool rawPressed = readButtonPressed(pin);
  uint8_t prevStable = lastStable[idx];

  // Convert stablePressed from prevStable:
  // lastStable stores stable "not pressed" or "pressed"? We'll store stable level (HIGH/LOW).
  // stablePressed = (lastStable[idx] == LOW)
  stablePressed = (prevStable == LOW);

  if ((rawPressed ? LOW : HIGH) != prevStable) {
    // Potential change
    uint32_t now = millis();
    if (now - lastChangeMs[idx] >= DEBOUNCE_MS) {
      // Accept change
      lastStable[idx] = rawPressed ? LOW : HIGH;
      lastChangeMs[idx] = now;
      stablePressed = rawPressed;
    } else {
      // Start debounce timer from first detection
      lastChangeMs[idx] = lastChangeMs[idx] ? lastChangeMs[idx] : millis();
    }
  } else {
    // No change, keep timer intact
  }
}

static void updateParticipantDebounceInit() {
  // Initialize stable levels
  uint8_t pins[5] = {0, BTN_1, BTN_2, BTN_3, BTN_4};
  for (uint8_t i = 1; i <= 4; i++) {
    uint8_t level = digitalRead(pins[i]);
    lastStable[i] = level;     // stable level
    lastChangeMs[i] = millis();
  }
}

static bool shouldTriggerWinner() {
  // Returns true if locked is false and one of the participant buttons is pressed (debounced).
  // Also sets global 'winner' with correct index.
  if (locked) return false;

  bool pPressed = false;
  bool stablePressed = false;

  // Check in order: 1..4.
  // Requirement: "only record the first pressed contestant number".
  // With scanning order, if multiple are pressed simultaneously,
  // the smallest index that becomes stable first will be recorded.
  // Debounce prevents jitter from producing multiple winners.
  uint8_t pins[5] = {0, BTN_1, BTN_2, BTN_3, BTN_4};
  for (uint8_t idx = 1; idx <= 4; idx++) {
    stablePressed = false;
    updateDebounceParticipant(idx, pins[idx], stablePressed);
    if (stablePressed) {
      winner = idx;
      return true;
    }
  }
  return false;
}

static void resetSystem(bool clearStats) {
  locked = false;
  winner = 0;
  pressTimeMs = 0;
  t_lock_ms = 0;
  display.clear();

  if (clearStats) {
    for (uint8_t i = 1; i <= 4; i++) totalBets[i] = 0;
  }

  digitalWrite(LED_PIN, LOW);
}

static void handleResetButton() {
  // Long-press 3 seconds clears accumulated抢答统计.
  // Any reset press should unlock inputs until release? Requirements: "button D9 as reset".
  // We'll treat: holding for 3s clears stats; regardless of duration, release or active triggers unlock.
  // But locked is only released when reset is pressed.
  bool rawResetPressed = readButtonPressed(RESET_BTN); // true when pressed (LOW)

  // Debounce reset key with simple method
  static uint8_t stableLevel = HIGH;
  static uint32_t lastStableChange = 0;

  uint8_t rawLevel = rawResetPressed ? LOW : HIGH;

  if (rawLevel != stableLevel) {
    uint32_t now = millis();
    if (now - lastStableChange >= DEBOUNCE_MS) {
      stableLevel = rawLevel;
      lastStableChange = now;
      resetStable = (stableLevel == LOW);
    }
  }

  if (resetStable) {
    if (!resetPressActive) {
      resetPressActive = true;
      resetPressStartMs = millis();
      // Immediate reset unlock behavior
      resetSystem(false);
    } else {
      // If held long enough and not yet applied, clear stats
      if ((millis() - resetPressStartMs) >= RESET_HOLD_MS) {
        // Apply stats clear only once per hold
        static bool clearedThisHold = false;
        if (!clearedThisHold) {
          resetSystem(true);
          clearedThisHold = true;
          // Provide a visible confirmation: show 0 briefly
          display.showNumberDecEx(0, 0, true);
          beepShort(60, 1800);
          digitalWrite(LED_PIN, HIGH);
          delay(60);
          digitalWrite(LED_PIN, LOW);
          clearedThisHold = true;
        }
      }
    }
  } else {
    // Released
    resetPressActive = false;
    resetPressStartMs = 0;
    // reset hold clear flag for next press
    // (need separate static inside block above; can't access here directly)
  }

  // Since clearedThisHold is inside the block, we need another mechanism:
  // We'll implement a more reliable clear-once guard globally:
}

// Global guard for clear-on-hold once
static bool clearedOnHold = false;

static void handleResetButton2() {
  bool rawResetPressed = readButtonPressed(RESET_BTN); // LOW when pressed
  uint32_t now = millis();

  // Debounce
  static uint8_t stableLevel = HIGH;
  static uint32_t lastStableChange = 0;

  uint8_t rawLevel = rawResetPressed ? LOW : HIGH;
  if (rawLevel != stableLevel) {
    if (now - lastStableChange >= DEBOUNCE_MS) {
      stableLevel = rawLevel;
      lastStableChange = now;
    }
  }
  resetStable = (stableLevel == LOW);

  if (resetStable) {
    if (!resetPressActive) {
      resetPressActive = true;
      resetPressStartMs = now;
      clearedOnHold = false;

      // Any reset press unlocks inputs
      resetSystem(false);
    } else {
      if (!clearedOnHold && (now - resetPressStartMs) >= RESET_HOLD_MS) {
        clearedOnHold = true;
        resetSystem(true);

        // Optional: short confirmation beep + show 0
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

  updateParticipantDebounceInit();

  // Start unlocked
  locked = false;
  winner = 0;
}

void loop() {
  // Handle reset button (unlock; clear stats if held 3s)
  handleResetButton2();

  // If unlocked, monitor contestant buttons
  if (!locked) {
    if (shouldTriggerWinner()) {
      // Lock system
      locked = true;
      t_lock_ms = millis();

      // Capture timestamp and winner
      pressTimeMs = t_lock_ms;

      // Update stats
      if (winner >= 1 && winner <= 4) totalBets[winner]++;

      // Short beep confirmation
      beepShort(80, 2000);

      // Show winner number
      showNumber(winner);

      // LED on to indicate locked (optional but required wiring; behavior preserved minimal)
      digitalWrite(LED_PIN, HIGH);

      // Serial output:抢答时间戳和选手编号
      // Format: timestamp_ms,winner
      Serial.print(pressTimeMs);
      Serial.print(",");
      Serial.println(winner);

      // Prevent retrigger due to still-pressed buttons:
      // Wait until all contestant buttons release before allowing another lock after reset.
      // However reset unlocks; so here we just keep locked.
    }
  } else {
    // Once locked, ignore contestant inputs until reset.
    // Maintain LED off if resetSystem clears it.
  }

  // Keep buzzer quiet after tone duration (tone auto-stops, but ensure no lingering)
  // (tone handles itself; noTone not strictly required)
}
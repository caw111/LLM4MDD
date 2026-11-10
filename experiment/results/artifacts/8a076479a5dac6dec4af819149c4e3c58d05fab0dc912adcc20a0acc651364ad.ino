#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t BED1_PIN = 2;   // D2
const uint8_t BED2_PIN = 3;   // D3
const uint8_t BED3_PIN = 4;   // D4
const uint8_t CONFIRM_PIN = 7; // D7

const uint8_t BUZZER_PIN = 6;  // D6 (tone output)
const uint8_t LED_PIN = 13;    // D13

const uint8_t TM_CLK_PIN = 8;  // D8
const uint8_t TM_DIO_PIN = 5;  // D5

// TM1637
TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// -------------------- Call/priority bookkeeping --------------------
static const uint8_t NUM_BEDS = 3;
static const uint8_t BED_NUM[NUM_BEDS] = {1, 2, 3}; // for display/log

// For each bed we track: called flag, call order index, call time, and whether "logged" for display/handling.
struct BedState {
  bool called;
  uint32_t callTimeMs;
  uint32_t order;      // increasing sequence number, smaller = higher priority
};

BedState beds[NUM_BEDS];

// Global sequence number for call ordering
uint32_t seqCounter = 0;

// Priority: smallest order among called beds
int8_t currentTopIndex = -1;

// When a bed is first called we can log that event.
// When confirm is pressed we clear the top call and log handling time.
bool lastBedInput[NUM_BEDS] = {false, false, false};
bool lastConfirmInput = false;

// -------------------- Debounce --------------------
const uint32_t DEBOUNCE_MS = 35;

struct DebouncedInput {
  uint8_t pin;
  bool lastStable;
  bool lastRaw;
  uint32_t lastChangeMs;
};

DebouncedInput bedInputs[NUM_BEDS] = {
  {BED1_PIN, false, false, 0},
  {BED2_PIN, false, false, 0},
  {BED3_PIN, false, false, 0}
};

DebouncedInput confirmInput = {CONFIRM_PIN, false, false, 0};

bool readDebouncedPressed(DebouncedInput &in) {
  // Returns true only on rising edge to pressed state (stable)
  bool raw = digitalRead(in.pin) == HIGH;
  uint32_t now = millis();

  if (raw != in.lastRaw) {
    in.lastRaw = raw;
    in.lastChangeMs = now;
  }

  if ((now - in.lastChangeMs) >= DEBOUNCE_MS) {
    if (in.lastStable != raw) {
      bool prev = in.lastStable;
      in.lastStable = raw;
      // pressed is stable HIGH, detect rising edge
      if (!prev && raw) return true;
    }
  }
  return false;
}

// -------------------- Display encoding --------------------
// TM1637Display default uses segments. We'll show:
// "B" + bed number + ":" + seconds (coarse) as two-digit decimal doesn't fit well.
// We'll implement display as: HH = bedNo and SS = seconds (mod 100).
// Show format: [bedNo][two digits seconds%100], using 4 digits like: d S S.
// digits: D0=bedNo, D1=sec/10, D2=sec%10, D3=0 (blank-like by using 0x00 -> no decimal point).
// For "秒数" requirement, we will display seconds modulo 100 continuously.
// Additionally, if you need full seconds > 99, can be extended; requirement says seconds, typical 4-digit display implies mod 100.
void updateDisplay(uint8_t bedNo, uint32_t waitSec) {
  uint8_t s = (uint8_t)(waitSec % 100);
  uint8_t tens = s / 10;
  uint8_t ones = s % 10;

  // Use 4 digits: [bedNo][tens][ones][0]
  // Many TM1637 libraries treat setSegments; TM1637Display::showNumberDecEx can place leading zeros.
  // We'll do showNumberDecEx with 4 digits: bedNo*1000 + tens*100 + ones*10
  // But that leaves last digit as 0 (not necessary). We'll set exact 4-digit number:
  // value = bedNo*1000 + tens*100 + ones*10
  // Example: bedNo=2, sec=5 => value=2000 + 0*100 + 5*10 =2050 -> digits "2050"
  uint16_t value = (uint16_t)bedNo * 1000u + (uint16_t)tens * 100u + (uint16_t)ones * 10u;
  // Use colon/decimal points: none required; keep plain.
  display.showNumberDecEx(value, 0b01000000, true, 4); // decimal point off; allow leading zeros
}

// -------------------- Alarm/Buzzer --------------------
bool alarmActive() {
  return currentTopIndex >= 0 && beds[currentTopIndex].called;
}

void buzzerPatternTick(uint32_t nowMs) {
  // Simple periodic beep while there is any pending call:
  // 500ms on / 500ms off
  if (!alarmActive()) {
    noTone(BUZZER_PIN);
    return;
  }
  uint32_t phase = nowMs % 1000UL;
  if (phase < 350) {
    tone(BUZZER_PIN, 2000); // 2kHz beep
  } else {
    noTone(BUZZER_PIN);
  }
}

// -------------------- Priority computation --------------------
int8_t computeTopIndex() {
  uint32_t bestOrder = 0xFFFFFFFFUL;
  int8_t bestIdx = -1;
  for (uint8_t i = 0; i < NUM_BEDS; i++) {
    if (beds[i].called) {
      if (beds[i].order < bestOrder) {
        bestOrder = beds[i].order;
        bestIdx = (int8_t)i;
      }
    }
  }
  return bestIdx;
}

// -------------------- Logging helpers --------------------
void logCallEvent(uint8_t bedNo, uint32_t waitMsNow) {
  // Output: "CALL bed=<n> wait=<ms>ms"
  // The wait now after press should be near 0; still records.
  Serial.print(F("CALL "));
  Serial.print(bedNo);
  Serial.print(F(" WAIT "));
  Serial.print(waitMsNow);
  Serial.println(F(" ms"));
}

void logConfirmEvent(uint8_t bedNo, uint32_t waitedMs) {
  Serial.print(F("CONFIRM "));
  Serial.print(bedNo);
  Serial.print(F(" HANDLED_WAIT "));
  Serial.print(waitedMs);
  Serial.println(F(" ms"));
}

void resetTopIndexIfNeeded() {
  currentTopIndex = computeTopIndex();
}

// -------------------- Setup/Loop --------------------
void setup() {
  Serial.begin(9600);
  // Inputs: assume external wiring uses pull-down or pull-up; use INPUT_PULLUP to be safe.
  // If your circuit uses pull-down, the signals will be inverted; this sketch assumes active HIGH.
  pinMode(BED1_PIN, INPUT_PULLUP);
  pinMode(BED2_PIN, INPUT_PULLUP);
  pinMode(BED3_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_PIN, INPUT_PULLUP);

  // Outputs
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // TM1637 init
  display.setBrightness(0x0f, true);

  // Initialize states
  for (uint8_t i = 0; i < NUM_BEDS; i++) {
    beds[i].called = false;
    beds[i].callTimeMs = 0;
    beds[i].order = 0xFFFFFFFFUL;
  }
  currentTopIndex = -1;
  seqCounter = 0;

  // Seed debouncer last raw stable
  for (uint8_t i = 0; i < NUM_BEDS; i++) {
    bool raw = digitalRead(bedInputs[i].pin) == HIGH;
    bedInputs[i].lastRaw = raw;
    bedInputs[i].lastStable = raw;
    bedInputs[i].lastChangeMs = millis();
  }
  {
    bool raw = digitalRead(confirmInput.pin) == HIGH;
    confirmInput.lastRaw = raw;
    confirmInput.lastStable = raw;
    confirmInput.lastChangeMs = millis();
  }

  // Clear display
  display.showNumberDecEx(0, 0, true);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  uint32_t now = millis();

  // Handle debounced bed button presses
  for (uint8_t i = 0; i < NUM_BEDS; i++) {
    if (readDebouncedPressed(bedInputs[i])) {
      // On any bed press: record call order and start waiting time.
      // If already called, do not change order (preserve "any bed pressed" behavior):
      // But requirement says "任一床位按下后系统记录呼叫顺序和等待时间并报警提示";
      // We'll allow re-press to create a new call record by re-arming this bed,
      // however beds are typically single-call; we'll treat re-press as "new call" by updating call time and order.
      beds[i].called = true;
      beds[i].callTimeMs = now;
      beds[i].order = seqCounter++;

      // Log call with waiting time at moment
      logCallEvent(BED_NUM[i], 0);

      // Update priority/top index
      resetTopIndexIfNeeded();
    }
  }

  // Handle confirm button press
  if (readDebouncedPressed(confirmInput)) {
    // Confirm: clear current highest priority call and output handling log.
    // Determine current top before clearing.
    int8_t top = computeTopIndex();
    if (top >= 0 && beds[top].called) {
      uint32_t waitedMs = now - beds[top].callTimeMs;
      uint8_t bedNo = BED_NUM[top];

      beds[top].called = false; // clear
      beds[top].callTimeMs = 0;
      beds[top].order = 0xFFFFFFFFUL;

      logConfirmEvent(bedNo, waitedMs);
    } else {
      // No pending calls; still log for traceability (optional but doesn't simplify behavior).
      Serial.println(F("CONFIRM NONE"));
    }

    // Update priority
    resetTopIndexIfNeeded();
  }

  // LED and alarm based on pending calls
  if (alarmActive()) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Buzzer pattern when there is any pending call
  buzzerPatternTick(now);

  // Update display: current waiting longest bed ID and seconds
  if (currentTopIndex >= 0 && beds[currentTopIndex].called) {
    uint32_t waitSec = (now - beds[currentTopIndex].callTimeMs) / 1000UL;
    updateDisplay(BED_NUM[currentTopIndex], waitSec);
  } else {
    // Show 0 when no calls pending
    display.showNumberDecEx(0, 0, true);
  }
}
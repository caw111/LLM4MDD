#include <Arduino.h>
#include <TM1637Display.h>

// NOTE: Arduino UNO pin constants are A0..A5, Bx/Cx/Dx not defined.
// Use numeric pin numbers as required by the wiring: D2=2, D4=4, etc.
static const uint8_t ENTRY_TRIG = 8;  // D8
static const uint8_t ENTRY_ECHO = 9;  // D9

static const uint8_t SLOT_TRIG  = 11; // D11
static const uint8_t SLOT_ECHO  = 12; // D12

static const uint8_t TM1637_CLK  = 4;  // D4
static const uint8_t TM1637_DIO  = 5;  // D5

static const uint8_t LED_GREEN  = 10;  // D10
static const uint8_t LED_RED    = 13;  // D13

static const uint8_t BUZZER_PIN = 6;   // D6

static const uint8_t BTN_PIN    = 2;   // D2 button

TM1637Display display(TM1637_CLK, TM1637_DIO);

// ------------------- System behavior parameters -------------------
static const uint8_t  NUM_SAMPLES   = 4;        // ultrasonic sampling average
static const uint16_t MAX_ECHO_US  = 25000;    // ~4.3m (safety)

// Occupancy threshold (cm) - determines whether a slot is considered occupied
static const uint16_t OCCUPIED_CM = 10;        // if slot distance <= 10cm => occupied

// Backing (reverse) protection: if reverse distance < 5cm => continuous alarm
static const uint16_t REVERSE_ALARM_CM = 5;

// Distance grading for buzzer frequency and LED提示 (cm)
static const uint16_t GRADE1_CM = 15; // far
static const uint16_t GRADE2_CM = 10; // occupied threshold-ish
static const uint16_t GRADE3_CM = 7;  // close
static const uint16_t GRADE4_CM = 4;  // very close

// ------------------- Display modes -------------------
enum DisplayMode : uint8_t {
  SHOW_ENTRY = 0,
  SHOW_SLOT  = 1,
  SHOW_COUNT = 2
};

static volatile DisplayMode mode = SHOW_ENTRY;

// ------------------- Timing / state -------------------
static uint32_t lastLoopMs = 0;

// Button debounce/state (assume INPUT_PULLUP, pressed = LOW)
static bool lastBtnState = HIGH;
static uint32_t lastBtnChangeMs = 0;

// Slot occupancy counting
static bool slotOccupiedPrev = false;
static uint32_t occupiedCount = 0;

// Buzzer control (non-blocking tone scheduling)
static bool buzzerActive = false;
static uint32_t buzzerLastToggleMs = 0;
static uint32_t buzzerHalfPeriodMs = 0;
static bool buzzerLevel = false;

// Alarm continuous state
static bool continuousAlarm = false;
static uint32_t continuousAlarmStartMs = 0;

// ------------------- Helpers -------------------
static uint16_t readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  uint32_t sumCm = 0;
  uint8_t valid = 0;

  for (uint8_t i = 0; i < NUM_SAMPLES; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    uint32_t duration = pulseIn(echoPin, HIGH, MAX_ECHO_US);
    if (duration == 0) continue;

    // duration in microseconds. Distance cm = duration / 58 (approx)
    uint16_t cm = (uint16_t)(duration / 58UL);
    if (cm == 0) cm = 1;
    sumCm += cm;
    valid++;
  }

  if (valid == 0) return 9999;
  return (uint16_t)(sumCm / valid);
}

static void setBuzzerFrequencyHz(uint16_t freqHz) {
  if (freqHz == 0) {
    buzzerActive = false;
    continuousAlarm = false;
    buzzerHalfPeriodMs = 0;
    buzzerLevel = false;
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  // half period in ms: (0.5/freq) seconds -> ms
  uint32_t hp = 1000UL / (2UL * (uint32_t)freqHz);
  if (hp == 0) hp = 1;

  buzzerHalfPeriodMs = hp;
  buzzerActive = true;
}

static void startContinuousAlarm() {
  continuousAlarm = true;
  continuousAlarmStartMs = millis();
  // "持续报警" => fixed high frequency
  setBuzzerFrequencyHz(2000);
}

static void updateBuzzerNonBlocking() {
  if (!buzzerActive) return;
  if (buzzerHalfPeriodMs == 0) return;

  uint32_t now = millis();
  if (now - buzzerLastToggleMs >= buzzerHalfPeriodMs) {
    buzzerLastToggleMs = now;
    buzzerLevel = !buzzerLevel;
    digitalWrite(BUZZER_PIN, buzzerLevel ? HIGH : LOW);
  }
}

static void renderDisplay(uint16_t entryCm, uint16_t slotCm, uint32_t count) {
  display.setBrightness(7);

  uint8_t dots = 0x00;
  uint16_t toShow = 0;

  if (mode == SHOW_ENTRY) {
    dots = 0x00; // no dots
    toShow = entryCm;
  } else if (mode == SHOW_SLOT) {
    dots = 0x01; // left dot
    toShow = slotCm;
  } else { // SHOW_COUNT
    dots = 0x03; // both dots
    toShow = (count > 9999) ? 9999 : (uint16_t)count;
  }

  if (toShow > 9999) toShow = 9999;
  display.showNumberDecEx(toShow, dots, false);
}

// ------------------- Main logic -------------------
static void applyIndicatorsAndProtection(uint16_t entryCm, uint16_t slotCm) {
  // 1) Backing protection: if detected reverse distance < 5cm => continuous alarm
  // Using slot distance as "reverse distance" (closest detection).
  if (slotCm < REVERSE_ALARM_CM) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    startContinuousAlarm();
    return;
  }

  // 2) Entry guidance: when entry distance is within some range => green guide
  bool nearEntrance = (entryCm <= 80);
  digitalWrite(LED_GREEN, nearEntrance ? HIGH : LOW);

  // 3) Slot occupancy indicator
  bool slotOccupied = (slotCm <= OCCUPIED_CM);
  digitalWrite(LED_RED, slotOccupied ? HIGH : LOW);

  // 4) Slot graded buzzer + LED提示
  if (!nearEntrance) {
    setBuzzerFrequencyHz(0); // buzzer off
  } else {
    uint16_t freq = 0;
    if (slotCm > GRADE1_CM) {
      freq = 0;
    } else if (slotCm > GRADE2_CM) {
      freq = 800;   // low
    } else if (slotCm > GRADE3_CM) {
      freq = 1200;  // medium
    } else if (slotCm > GRADE4_CM) {
      freq = 1600;  // high
    } else {
      freq = 2000;  // very close
    }
    setBuzzerFrequencyHz(freq);
  }
}

static void updateOccupiedCount(uint16_t slotCm) {
  bool slotOccupiedNow = (slotCm <= OCCUPIED_CM);
  if (!slotOccupiedPrev && slotOccupiedNow) {
    occupiedCount++;
  }
  slotOccupiedPrev = slotOccupiedNow;
}

static void handleButton() {
  bool btnState = digitalRead(BTN_PIN);

  // Basic change tracking
  if (btnState != lastBtnState) {
    lastBtnChangeMs = millis();
    lastBtnState = btnState;
  }

  // Debounce: stable for 30ms
  static bool stableState = HIGH;
  static uint32_t stableSince = 0;

  if (btnState != stableState) {
    stableSince = millis();
    stableState = btnState;
  }

  if (millis() - stableSince >= 30) {
    static bool pressedPrev = false;
    bool pressedNow = (stableState == LOW); // INPUT_PULLUP: pressed => LOW

    if (pressedNow && !pressedPrev) {
      // toggle mode
      if (mode == SHOW_ENTRY) mode = SHOW_SLOT;
      else if (mode == SHOW_SLOT) mode = SHOW_COUNT;
      else mode = SHOW_ENTRY;
    }
    pressedPrev = pressedNow;
  }
}

// ------------------- Arduino lifecycle -------------------
void setup() {
  pinMode(ENTRY_TRIG, OUTPUT);
  pinMode(ENTRY_ECHO, INPUT);

  pinMode(SLOT_TRIG, OUTPUT);
  pinMode(SLOT_ECHO, INPUT);

  pinMode(TM1637_CLK, OUTPUT);
  pinMode(TM1637_DIO, OUTPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);

  digitalWrite(ENTRY_TRIG, LOW);
  digitalWrite(SLOT_TRIG, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(7);
  display.showNumberDecEx(0, 0x00, false);
}

void loop() {
  handleButton();

  uint32_t now = millis();
  if (now - lastLoopMs < 60) {
    updateBuzzerNonBlocking();
    return;
  }
  lastLoopMs = now;

  uint16_t entryCm = readUltrasonicCm(ENTRY_TRIG, ENTRY_ECHO);
  uint16_t slotCm  = readUltrasonicCm(SLOT_TRIG, SLOT_ECHO);

  updateOccupiedCount(slotCm);
  applyIndicatorsAndProtection(entryCm, slotCm);
  renderDisplay(entryCm, slotCm, occupiedCount);

  updateBuzzerNonBlocking();
}
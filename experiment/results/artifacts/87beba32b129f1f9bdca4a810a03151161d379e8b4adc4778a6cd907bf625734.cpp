#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ------------------- Pin assignments (as required) -------------------
static const uint8_t ENTRY_TRIG = D8;
static const uint8_t ENTRY_ECHO = D9;

static const uint8_t SLOT_TRIG  = D11;
static const uint8_t SLOT_ECHO  = D12;

static const uint8_t TM1637_CLK = D4;
static const uint8_t TM1637_DIO = D5;

static const uint8_t LED_GREEN  = D10;
static const uint8_t LED_RED    = D13;

static const uint8_t BUZZER_PIN = D6;

static const uint8_t BTN_PIN    = D2; // button: toggle display modes

TM1637Display display(TM1637_CLK, TM1637_DIO);

// ------------------- System behavior parameters -------------------
static const uint8_t NUM_SAMPLES = 4;          // ultrasonic sampling average
static const uint16_t MAX_ECHO_US = 25000;    // ~4.3m (safety)
static const uint16_t SOUND_SPEED_CM_PER_US = 0; // unused; we compute directly

// Occupancy threshold (cm) - determines whether a slot is considered occupied
static const uint16_t OCCUPIED_CM = 10; // if slot distance <= 10cm => occupied

// Backing (reverse) protection: if reverse distance < 5cm => continuous alarm
static const uint16_t REVERSE_ALARM_CM = 5;

// Distance grading for buzzer frequency and LED提示 (cm)
// These define how buzzer/LED change as a car enters the slot.
static const uint16_t GRADE1_CM = 15; // far (approach)
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

// Button debounce
static uint32_t lastBtnChangeMs = 0;
static bool lastBtnState = HIGH;

// Slot occupancy counting
static bool slotOccupiedPrev = false;
static uint32_t occupiedCount = 0;

// Buzzer control (non-blocking tone scheduling)
static bool buzzerActive = false;
static uint32_t buzzerLastToggleMs = 0;
static uint32_t buzzerHalfPeriodMs = 0; // half period for square wave
static bool buzzerLevel = false;

// Alarm continuous state
static bool continuousAlarm = false;
static uint32_t continuousAlarmStartMs = 0;

// ------------------- Helpers -------------------
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readUltrasonicCm(uint8_t trigPin, uint8_t echoPin);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBuzzerFrequencyHz(uint16_t freqHz);
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startContinuousAlarm();
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void stopContinuousAlarmIfNeeded();
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBuzzerNonBlocking();
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void renderDisplay(uint16_t entryCm, uint16_t slotCm, uint32_t count);
#line 185 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyIndicatorsAndProtection(uint16_t entryCm, uint16_t slotCm);
#line 249 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateOccupiedCount(uint16_t slotCm);
#line 257 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButton();
#line 295 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 321 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  // Returns distance in cm; if out of range returns a large value
  uint32_t sumCm = 0;
  uint8_t valid = 0;

  for (uint8_t i = 0; i < NUM_SAMPLES; i++) {
    // trigger
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // measure pulse width
    // timeout prevents blocking if no echo
    uint32_t duration = pulseIn(echoPin, HIGH, MAX_ECHO_US);
    if (duration == 0) {
      continue;
    }
    // duration in microseconds. Distance cm = duration / 58 (approx)
    uint16_t cm = (uint16_t)(duration / 58UL);
    if (cm == 0) cm = 1; // avoid 0
    sumCm += cm;
    valid++;
  }

  if (valid == 0) return 9999;
  return (uint16_t)(sumCm / valid);
}

static void setBuzzerFrequencyHz(uint16_t freqHz) {
  // Configure buzzer to be toggled at half-period; freqHz==0 disables.
  if (freqHz == 0) {
    buzzerActive = false;
    continuousAlarm = false;
    buzzerHalfPeriodMs = 0;
    buzzerLevel = false;
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }
  // half period in ms: (0.5/freq) seconds -> ms
  // halfPeriodMs = 1000 / (2*freq)
  uint32_t hp = 1000UL / (2UL * (uint32_t)freqHz);
  if (hp == 0) hp = 1; // ensure minimal timing
  buzzerHalfPeriodMs = hp;
  buzzerActive = true;
}

static void startContinuousAlarm() {
  continuousAlarm = true;
  continuousAlarmStartMs = millis();
  // Use high fixed frequency; behavior says "持续报警".
  setBuzzerFrequencyHz(2000);
}

static void stopContinuousAlarmIfNeeded() {
  // leave continuousAlarm handling to caller which will call start/stop conditionally
}

static void updateBuzzerNonBlocking() {
  if (!buzzerActive) return;
  uint32_t now = millis();
  if (now - buzzerLastToggleMs >= buzzerHalfPeriodMs) {
    buzzerLastToggleMs = now;
    buzzerLevel = !buzzerLevel;
    digitalWrite(BUZZER_PIN, buzzerLevel ? HIGH : LOW);
  }
}

static void showNumber(uint16_t value, uint8_t brightness = 7, bool leadingZero = false) {
  display.setBrightness(brightness);
  // TM1637Display's showNumberDecEx supports leading zeros; but we use showNumberDec for simplicity
  if (leadingZero) {
    // showNumberDecEx(value, dots, leadingZeros)
    // In this library, showNumberDecEx has signature: showNumberDecEx(num, uint8_t dots, uint8_t leadingZeros);
    display.showNumberDecEx(value, 0x00, true);
  } else {
    display.showNumberDec(value, false);
  }
}

// Display mapping: show short labels by dots usage.
// We'll keep it numeric-only but adjust dots pattern to indicate mode.
static void renderDisplay(uint16_t entryCm, uint16_t slotCm, uint32_t count) {
  // Each mode shows numeric value:
  // - SHOW_ENTRY: entry distance
  // - SHOW_SLOT : slot distance
  // - SHOW_COUNT: occupied count (clamped to 4 digits)
  display.setBrightness(7);

  // Dots: entry=none, slot=left dot, count=both dots (visual aid)
  uint8_t dots = 0x00;
  uint16_t toShow = 0;

  if (mode == SHOW_ENTRY) {
    dots = 0x00; // no dots
    toShow = entryCm;
  } else if (mode == SHOW_SLOT) {
    dots = 0x01; // left dot
    toShow = slotCm;
  } else {
    dots = 0x03; // both dots
    if (count > 9999) toShow = 9999;
    else toShow = (uint16_t)count;
  }

  // Use showNumberDecEx for dots.
  // Limit to 0..9999
  if (toShow > 9999) toShow = 9999;
  display.showNumberDecEx(toShow, dots, false);
}

// ------------------- Main logic -------------------
static void applyIndicatorsAndProtection(uint16_t entryCm, uint16_t slotCm) {
  // Default LED states
  // Green: guide when vehicle near entrance
  // Red: occupancy indication / protection
  // Buzzer: adjust by distance grading when in the slot
  uint16_t effectiveSlotCm = slotCm;

  // 1) Backing protection: if detected reverse distance < 5cm => continuous alarm
  // Here we use slot distance as "reverse distance" (closest detection).
  if (effectiveSlotCm < REVERSE_ALARM_CM) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    startContinuousAlarm();
    return;
  }

  // 2) Entry guidance:
  // "车辆靠近入口时绿灯引导" => when entry distance is within some range,
  // we turn green on. When not near, keep green off.
  // Choose threshold: near within 80cm
  bool nearEntrance = (entryCm <= 80);

  if (nearEntrance) {
    digitalWrite(LED_GREEN, HIGH);
  } else {
    digitalWrite(LED_GREEN, LOW);
  }

  // 3) Slot occupancy and graded buzzer/LED提示
  bool slotOccupied = (effectiveSlotCm <= OCCUPIED_CM);

  // Update red LED to show occupancy (unless backing alarm active, handled above)
  digitalWrite(LED_RED, slotOccupied ? HIGH : LOW);

  // Determine buzzer frequency by grading when car "enters the slot"
  // We'll only drive buzzer when near entrance and slot is within a reasonable distance.
  // If far away, buzzer off.
  if (!nearEntrance) {
    setBuzzerFrequencyHz(0);
  } else {
    // Distance grading:
    // - > GRADE1_CM : buzzer off
    // - <= GRADE1_CM : low frequency
    // - <= GRADE2_CM : medium frequency
    // - <= GRADE3_CM : higher frequency + stronger LED (red already)
    // - <= GRADE4_CM : very high frequency
    uint16_t freq = 0;
    if (effectiveSlotCm > GRADE1_CM) {
      freq = 0;
    } else if (effectiveSlotCm > GRADE2_CM) {
      freq = 800;  // low
    } else if (effectiveSlotCm > GRADE3_CM) {
      freq = 1200; // medium
    } else if (effectiveSlotCm > GRADE4_CM) {
      freq = 1600; // high
    } else {
      // Note: backing alarm would have triggered and returned above,
      // but keep safe fallback.
      freq = 2000;
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
  // Button toggles display mode among entry distance, slot distance, occupied count.
  bool btnState = digitalRead(BTN_PIN);

  // Using pull-down/pull-up assumption from wiring:
  // We'll interpret LOW as pressed if using INPUT_PULLUP.
  // Requirements didn't specify wiring; typical UNO uses INPUT_PULLUP.
  // We'll configure INPUT_PULLUP in setup, so pressed => LOW.
  if (btnState != lastBtnState) {
    lastBtnChangeMs = millis();
    lastBtnState = btnState;
  }

  // Debounce: only consider if stable for 30ms
  static bool stableState = HIGH;
  static uint32_t stableSince = 0;

  if (btnState != stableState) {
    stableSince = millis();
    stableState = btnState;
  }

  if (millis() - stableSince >= 30) {
    // Press edge detection: stable pressed (LOW) and previously not pressed
    static bool pressedPrev = false;
    bool pressedNow = (stableState == LOW);

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

  // Measure both ultrasonic distances
  uint16_t entryCm = readUltrasonicCm(ENTRY_TRIG, ENTRY_ECHO);
  uint16_t slotCm  = readUltrasonicCm(SLOT_TRIG, SLOT_ECHO);

  // Occupancy count update
  updateOccupiedCount(slotCm);

  // Apply indicators and buzzer behavior
  applyIndicatorsAndProtection(entryCm, slotCm);

  // Render display according to mode
  renderDisplay(entryCm, slotCm, occupiedCount);

  // keep buzzer going
  updateBuzzerNonBlocking();
}

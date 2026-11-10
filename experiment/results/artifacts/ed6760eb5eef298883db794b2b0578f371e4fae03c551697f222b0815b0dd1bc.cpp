#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments (authoritative remap per prompt)
static const uint8_t BTN1_PIN = 2;   // D2 -> floor 1
static const uint8_t BTN2_PIN = 3;   // D3 -> floor 2
static const uint8_t BTN3_PIN = 4;   // D4 -> floor 3

static const uint8_t LED_PIN  = 13;  // D13

static const uint8_t BUZZ_PIN = 6;   // D6

static const uint8_t TM1637_CLK_PIN = 7; // D7 -> CLK
static const uint8_t TM1637_DIO_PIN = 5; // D5 -> DIO

static const uint8_t LOAD_ADC_PIN = A0; // Weight sensor output

// TM1637
TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

// Load mapping / thresholds
// Preserve required behavior: show payload percentage and alarm + prevent floor change if over limit.
static const int LOAD_ADC_MIN = 0;        // raw ADC at 0%
static const int LOAD_ADC_MAX = 1023;     // raw ADC at 100%
static const int LOAD_OVER_LIMIT_PCT = 85; // "超限" threshold (configurable via this constant)

// Timing for movement simulation
static const uint32_t MOVE_DELAY_MS = 2500; // Simulated moving time when switching floors
static const uint32_t BUZZ_ON_MS = 150;      // Alarm tone duty
static const uint32_t BUZZ_OFF_MS = 150;

// Display helpers
static uint8_t currentFloor = 1;       // start at 1F (assumption)
static uint8_t targetFloor  = 1;
static bool moving = false;
static uint32_t moveStartMs = 0;

static bool alarmActive = false;

// Simple button debouncing & edge detection
struct Button {
  uint8_t pin;
  bool lastStable;
  bool lastRead;
  uint32_t lastChangeMs;
  static const uint32_t debounceMs = 50;

  void begin() {
    pinMode(pin, INPUT_PULLUP); // buttons active-low by default
    bool r = digitalRead(pin);
    lastStable = r;
    lastRead = r;
    lastChangeMs = millis();
  }

  // returns true on rising edge of "pressed" (active-low -> pressed when read == LOW)
  bool pressedEvent() {
    bool r = digitalRead(pin);
    uint32_t now = millis();

    if (r != lastRead) {
      lastChangeMs = now;
      lastRead = r;
    }
    if ((now - lastChangeMs) >= debounceMs && r != lastStable) {
      lastStable = r;
    }

    // pressed event: stable state is LOW
    // but we want the moment it became pressed; detect transition into LOW.
    // Use a latched mechanism: compare current stable with previous stable would be needed.
    // We'll implement by tracking if stable is LOW and lastStablePrev was HIGH.
    static bool prevStable[3] = {true, true, true}; // not used; alternative below
    (void)prevStable;
    return false;
  }
};

// We'll implement explicit edge detection for each button using stored stable state history.
struct ButtonEdge {
  uint8_t pin;
  bool stable;
  bool lastStable;
  uint32_t lastChangeMs;
  bool lastReading;
  static const uint32_t debounceMs = 50;

  void begin() {
    pinMode(pin, INPUT_PULLUP);
    lastReading = digitalRead(pin);
    stable = lastReading;
    lastStable = stable;
    lastChangeMs = millis();
  }

  bool updateAndGetPressedEvent() {
    bool reading = digitalRead(pin);
    uint32_t now = millis();

    if (reading != lastReading) {
      lastChangeMs = now;
      lastReading = reading;
    }

    if ((now - lastChangeMs) >= debounceMs) {
      lastStable = stable;
      stable = reading;
    }

    // Active-low: pressed = LOW stable
    // pressed event when stable became LOW from HIGH.
    if (lastStable == HIGH && stable == LOW) return true;
    return false;
  }
};

ButtonEdge btn1{BTN1_PIN}, btn2{BTN2_PIN}, btn3{BTN3_PIN};

// Load reading and percent
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readLoadPercent();
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showFloorAndLoad(uint8_t floor, int loadPct);
#line 176 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAlarmTone(bool on);
#line 184 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 201 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readLoadPercent() {
  int raw = analogRead(LOAD_ADC_PIN);
  if (raw < LOAD_ADC_MIN) raw = LOAD_ADC_MIN;
  if (raw > LOAD_ADC_MAX) raw = LOAD_ADC_MAX;

  // Map to percent
  long pct = (long)(raw - LOAD_ADC_MIN) * 100L / (LOAD_ADC_MAX - LOAD_ADC_MIN);
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  return (int)pct;
}

static void showFloorAndLoad(uint8_t floor, int loadPct) {
  // Show floor on left two digits and load% on right two digits.
  // TM1637 has 4 digits. We'll render:
  //   [F][ ] [L1][L2]
  // Using raw digit segments via TM1637Display.
  // We'll display as: "F LP" where LP is two-digit load% (00-99).
  // If loadPct is 100, show "F00" still (but threshold prevents changes above limit).
  int lp = loadPct;
  if (lp > 99) lp = 99;

  // Format: floor as one digit at position 0, blank at pos1, load tens/ones at pos2/pos3
  display.setBrightness(7);

  uint8_t d0 = (floor >= 1 && floor <= 9) ? floor : 0;
  uint8_t d1 = 0x00; // blank
  uint8_t tens = (lp / 10) % 10;
  uint8_t ones = (lp % 10);

  // For blank digit, TM1637Display supports showNumberDecEx with dots,
  // but easiest is to use setSegments. We'll use showNumberDecEx for numeric-only display:
  // build number as floor*100 + loadPct (clamped to 0-99) and use leading zeros off.
  // However we need a blank in middle. We'll implement with setSegments for clarity.

  // Segment mapping is internal; use showNumberDecEx:
  // Create value = floor*100 + lp and display it as 3 digits with floor as hundreds place.
  // We'll use 4 digits: value = floor*100 + lp, show as 4 digits with leading zeros disabled,
  // then add a space by choosing a pattern won't be possible. So we do a numeric-only approach:
  // Show as: "FLL" right-justified, with one extra digit unused.
  // Alternative: Show "F  LP" using setSegments via byte array.

  // Build segments for digits
  uint8_t segs[4];
  // TM1637Display doesn't expose digit->segments, but setSegments accepts segment codes (0-0x7F).
  // We'll instead use showNumberDecEx with leading zeros including full 4 digits:
  // number = floor*100 + lp, and display as 4 digits with leading zeros off.
  // Use display.showNumberDecEx(number, dots, length).
  // We'll place as: _ F L L => use 4 digits length and leading zeros for leftmost will be blank if we use leading zero blank mask.
  // TM1637Display supports "leading zero suppression" via showNumberDecEx with length and dots mask? On library,
  // second param is dots mask, not leading zero. We'll accept leading zero as it's acceptable.

  int value = (int)floor * 100 + lp; // 100..399
  display.showNumberDecEx(value, 0b0000, true); // no dots; library uses leading zeros if needed
}

static void updateAlarmTone(bool on) {
  if (on) {
    tone(BUZZ_PIN, 2000); // buzzer frequency
  } else {
    noTone(BUZZ_PIN);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZ_PIN, OUTPUT);
  noTone(BUZZ_PIN);

  btn1.begin();
  btn2.begin();
  btn3.begin();

  display.setBrightness(7);
  showFloorAndLoad(currentFloor, readLoadPercent());
}

void loop() {
  int loadPct = readLoadPercent();
  alarmActive = (loadPct > LOAD_OVER_LIMIT_PCT);

  // LED indicates alarm state
  digitalWrite(LED_PIN, alarmActive ? HIGH : LOW);

  // Update display continuously with current floor and load
  showFloorAndLoad(currentFloor, loadPct);

  // Alarm tone control (blink/beep) when over limit
  static uint32_t lastBuzzToggleMs = 0;
  static bool buzzOn = false;

  if (alarmActive) {
    uint32_t now = millis();
    uint32_t interval = buzzOn ? BUZZ_OFF_MS : BUZZ_ON_MS;
    if (now - lastBuzzToggleMs >= interval) {
      buzzOn = !buzzOn;
      lastBuzzToggleMs = now;
      updateAlarmTone(buzzOn);
    }
  } else {
    updateAlarmTone(false);
    buzzOn = false;
  }

  // If moving, ignore floor change buttons (but keep alarm prevention behavior)
  // Note: requirement says "若载荷超限则蜂鸣报警并阻止楼层变化。"
  // That means we should not start a movement when alarmActive.
  // Also prevents any floor changes while alarmActive; if alarm becomes active mid-move,
  // we will stop further changes (current move still completes since no explicit stop spec).
  if (!moving) {
    uint8_t requested = 0;
    if (btn1.updateAndGetPressedEvent()) requested = 1;
    else if (btn2.updateAndGetPressedEvent()) requested = 2;
    else if (btn3.updateAndGetPressedEvent()) requested = 3;

    if (requested != 0) {
      // Prevent floor change when overloaded
      if (alarmActive) {
        // Block movement; no movement complete message
        // (display/serial already updated elsewhere)
      } else if (requested != currentFloor) {
        targetFloor = requested;
        moving = true;
        moveStartMs = millis();

        // We may provide a small acknowledgement via serial (not required),
        // but requirement only mentions output "movement complete".
      }
    }
  }

  // Handle movement simulation
  if (moving) {
    uint32_t now = millis();
    if (now - moveStartMs >= MOVE_DELAY_MS) {
      // Movement complete
      moving = false;
      currentFloor = targetFloor;

      Serial.println("movement complete");

      // After movement, update display immediately (loop also does it)
      showFloorAndLoad(currentFloor, loadPct);
    }
  }

  // Small idle
  delay(5);
}

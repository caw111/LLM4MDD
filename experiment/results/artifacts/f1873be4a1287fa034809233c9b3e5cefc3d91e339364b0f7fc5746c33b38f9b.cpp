#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ---- Pin assignments (Arduino UNO uses numeric pins; D* aliases may not exist) ----
// Entry module: Trig=D8, Echo=D9
static const uint8_t ENTRY_TRIG = 8;
static const uint8_t ENTRY_ECHO = 9;

// Slot module: Trig=D11, Echo=D12
static const uint8_t SLOT_TRIG = 11;
static const uint8_t SLOT_ECHO = 12;

// TM1637: CLK=D4, DIO=D5
static const uint8_t TM1637_CLK = 4;
static const uint8_t TM1637_DIO = 5;

// Red LED D13, Green LED D10, Buzzer D6, Button D2
static const uint8_t LED_GREEN = 10;
static const uint8_t LED_RED = 13;
static const uint8_t BUZZER_PIN = 6;
static const uint8_t BTN_PIN = 2;

TM1637Display display(TM1637_CLK, TM1637_DIO);

// --------- Helper: ultrasonic distance ----------
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readUltrasonicCm(uint8_t trigPin, uint8_t echoPin);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readButtonPressedEdge();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setLeds(bool greenOn, bool redOn);
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void buzzerOff();
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint16_t frequencyForDistance(float cm);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isVehicleApproachingEntrance(float entryCm);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isInSlotApproach(float slotCm);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateOccupancy(float slotCm);
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void displayNumberMode(float entryCm, float slotCm);
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 175 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 30000UL); // up to ~5m
  if (duration == 0) return -1.0f;
  float cm = (float)duration / 58.0f;
  return cm;
}

// --------- Display modes and parking guidance ----------
enum DisplayMode : uint8_t { SHOW_ENTRY = 0, SHOW_SLOT = 1, SHOW_COUNT = 2 };

// Button toggles display mode
static volatile DisplayMode mode = SHOW_ENTRY;

// Occupancy count
static uint32_t occupancyCount = 0;

// State tracking for occupancy detection and direction safety
static bool slotOccupied = false;

// For "倒车距离小于 5cm 则持续报警"
// In this system: treat slot distance as rear/backing proxy
static bool rearAlarmActive = false;

// For buzzer frequency control
static uint16_t buzzerHalfPeriodMs = 0; // kept to preserve structure (not required)

// --------- Button handling ----------
static bool lastBtnReading = HIGH; // INPUT_PULLUP
static unsigned long lastDebounceMs = 0;
static const unsigned long debounceDelayMs = 50;

static bool readButtonPressedEdge() {
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    lastDebounceMs = millis();
    lastBtnReading = reading;
  }
  if ((millis() - lastDebounceMs) > debounceDelayMs) {
    static bool stableState = HIGH;
    if (reading != stableState) {
      stableState = reading;
      if (stableState == LOW) return true; // pressed
    }
  }
  return false;
}

static void setLeds(bool greenOn, bool redOn) {
  digitalWrite(LED_GREEN, greenOn ? HIGH : LOW);
  digitalWrite(LED_RED, redOn ? HIGH : LOW);
}

static void buzzerOff() {
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

// ---- Frequency bins based on slot distance ----
static uint16_t frequencyForDistance(float cm) {
  if (cm < 0) return 0;
  if (cm < 6.0f) return 2000;  // near
  if (cm < 11.0f) return 1500;
  if (cm < 21.0f) return 1000;
  if (cm < 41.0f) return 700;
  if (cm < 81.0f) return 400;
  return 0;
}

static bool isVehicleApproachingEntrance(float entryCm) {
  if (entryCm < 0) return false;
  // Approaching threshold
  return entryCm <= 60.0f;
}

static bool isInSlotApproach(float slotCm) {
  if (slotCm < 0) return false;
  // consider in-slot when slot distance is within a reasonable range
  return slotCm <= 80.0f;
}

static void updateOccupancy(float slotCm) {
  // Occupied when slot distance is very small (<=10cm).
  bool nowOccupied = false;
  if (slotCm >= 0) nowOccupied = (slotCm <= 10.0f);

  if (!slotOccupied && nowOccupied) {
    occupancyCount++;
  }
  slotOccupied = nowOccupied;
}

static void displayNumberMode(float entryCm, float slotCm) {
  display.setBrightness(7, true);

  if (mode == SHOW_ENTRY) {
    int v = 0;
    if (entryCm >= 0) v = (int)entryCm;
    if (v > 9999) v = 9999;
    display.showNumberDec(v, false);
  } else if (mode == SHOW_SLOT) {
    int v = 0;
    if (slotCm >= 0) v = (int)slotCm;
    if (v > 9999) v = 9999;
    display.showNumberDec(v, false);
  } else {
    uint32_t v = occupancyCount % 10000UL;
    display.showNumberDec((int)v, false);
  }
}

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

  setLeds(false, false);
  buzzerOff();

  display.setBrightness(7, true);
  display.clear();

  lastBtnReading = digitalRead(BTN_PIN);
  lastDebounceMs = millis();

  mode = SHOW_ENTRY;

  digitalWrite(ENTRY_TRIG, LOW);
  digitalWrite(SLOT_TRIG, LOW);
}

void loop() {
  if (readButtonPressedEdge()) {
    if (mode == SHOW_ENTRY) mode = SHOW_SLOT;
    else if (mode == SHOW_SLOT) mode = SHOW_COUNT;
    else mode = SHOW_ENTRY;
  }

  float entryCm = readUltrasonicCm(ENTRY_TRIG, ENTRY_ECHO);
  float slotCm = readUltrasonicCm(SLOT_TRIG, SLOT_ECHO);

  updateOccupancy(slotCm);

  bool approachingEntrance = isVehicleApproachingEntrance(entryCm);
  bool inSlot = isInSlotApproach(slotCm);

  // "车辆靠近入口时绿灯引导"
  if (approachingEntrance) {
    if (rearAlarmActive) {
      setLeds(false, true);
    } else {
      if (slotOccupied) setLeds(false, true);
      else setLeds(true, false);
    }
  } else {
    if (rearAlarmActive) setLeds(false, true);
    else if (slotOccupied) setLeds(false, true);
    else setLeds(false, false);
  }

  // "当进入车位时根据距离分级调节蜂鸣器频率和 LED 提示"
  // "若检测到倒车距离小于 5cm 则持续报警。"
  bool rearTooClose = (slotCm >= 0 && slotCm < 5.0f);

  // Latch while condition holds
  rearAlarmActive = rearTooClose;

  if (rearAlarmActive) {
    tone(BUZZER_PIN, 2500); // fixed continuous alarm tone
  } else {
    if (inSlot && !slotOccupied) {
      uint16_t f = frequencyForDistance(slotCm);
      if (f > 0) {
        tone(BUZZER_PIN, f);

        // LED hint: if slotCm <= 20 -> red, else green
        if (slotCm >= 0 && slotCm <= 20.0f) {
          digitalWrite(LED_GREEN, LOW);
          digitalWrite(LED_RED, HIGH);
        } else {
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GREEN, HIGH);
        }
      } else {
        buzzerOff();
      }
    } else if (slotOccupied) {
      buzzerOff();
    } else {
      buzzerOff();
    }
  }

  // Keep occupancy state visible unless rearAlarm is active
  if (slotOccupied && !rearAlarmActive) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else if (!slotOccupied && !rearAlarmActive) {
    // Do not override the proximity hint that may have been set above.
  }

  displayNumberMode(entryCm, slotCm);

  delay(80);
}

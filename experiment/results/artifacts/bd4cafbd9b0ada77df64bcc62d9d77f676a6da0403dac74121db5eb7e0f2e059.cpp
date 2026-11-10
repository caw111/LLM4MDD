#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t ENTRY_TRIG = D8;
static const uint8_t ENTRY_ECHO = D9;

static const uint8_t SLOT_TRIG  = D11;
static const uint8_t SLOT_ECHO  = D12;

static const uint8_t TM1637_CLK = D4;
static const uint8_t TM1637_DIO = D5;

static const uint8_t LED_GREEN = D10;
static const uint8_t LED_RED   = D13;

static const uint8_t BUZZER_PIN = D6;

static const uint8_t BTN_PIN = D2;

TM1637Display display(TM1637_CLK, TM1637_DIO);

// --------- Helper: ultrasonic distance ----------
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readUltrasonicCm(uint8_t trigPin, uint8_t echoPin);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonPressedEdge();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLeds(bool greenOn, bool redOn);
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzerOff();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t frequencyForDistance(float cm);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isVehicleApproachingEntrance(float entryCm);
#line 122 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isInSlotApproach(float slotCm);
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateOccupancy(float slotCm);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayNumberMode(float entryCm, float slotCm);
#line 166 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 202 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  // Keep behavior deterministic: single-shot measurement with timeout.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // PulseIn with timeout to avoid lockups.
  // Speed of sound: 343 m/s => 29.1 us per cm round-trip.
  // We'll compute cm using duration/58.0.
  unsigned long duration = pulseIn(echoPin, HIGH, 30000UL); // up to ~5m
  if (duration == 0) return -1.0f;
  float cm = (float)duration / 58.0f;
  return cm;
}

// --------- Display modes and parking guidance ----------
enum DisplayMode : uint8_t { SHOW_ENTRY = 0, SHOW_SLOT = 1, SHOW_COUNT = 2 };

// Button toggles display mode; also used for occupancy count tracking.
static volatile DisplayMode mode = SHOW_ENTRY;

// Occupancy count
static uint32_t occupancyCount = 0;

// State tracking for occupancy detection and direction safety
static bool slotOccupied = false;

// For "倒车距离小于5cm则持续报警" (rear distance detection).
// We'll use slot distance as proxy; when slot distance indicates backing too close, alarm remains on.
static bool rearAlarmActive = false;
static bool lastRearCondition = false;

// For buzzer frequency control
static uint16_t buzzerHalfPeriodMs = 0; // computed per distance bin

// --------- Button handling ----------
static bool lastBtnReading = HIGH; // with pullup
static unsigned long lastDebounceMs = 0;
static const unsigned long debounceDelayMs = 50;

static bool readButtonPressedEdge() {
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    lastDebounceMs = millis();
    lastBtnReading = reading;
  }
  if ((millis() - lastDebounceMs) > debounceDelayMs) {
    // Pressed on LOW (INPUT_PULLUP)
    static bool stableState = HIGH;
    if (reading != stableState) {
      stableState = reading;
      if (stableState == LOW) return true;
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

// Frequency bins based on slot distance:
// "当进入车位时根据距离分级调节蜂鸣器频率和 LED 提示"
// We'll use the slot distance when vehicle is "in approach/arrived".
// Choose bins (cm):
//  0-5:  alarm handled separately (rear distance <5cm => continuous alarm)
//  6-10: very high frequency
// 11-20: high
// 21-40: medium
// 41-80: low
//  >80 or invalid: off (not in slot)
static uint16_t frequencyForDistance(float cm) {
  if (cm < 0) return 0;
  if (cm < 6.0f) return 2000;   // near
  if (cm < 11.0f) return 1500;
  if (cm < 21.0f) return 1000;
  if (cm < 41.0f) return 700;
  if (cm < 81.0f) return 400;
  return 0;
}

// Determine if vehicle is "entering slot" based on entry distance threshold.
// We'll consider "靠近入口时绿灯引导".
// When entry distance is small, green guides; then when slot distance decreases, we treat as in-slot.
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
  // "显示车位距离和占用状态"
  // We'll treat occupancy when slot distance is very small (<=10cm).
  // When it crosses from not occupied to occupied, increment count.
  bool nowOccupied = false;
  if (slotCm >= 0) nowOccupied = (slotCm <= 10.0f);

  if (!slotOccupied && nowOccupied) {
    occupancyCount++;
  }
  slotOccupied = nowOccupied;
}

static void displayNumberMode(float entryCm, float slotCm) {
  // Button can switch display entry distance / slot distance / occupied count.
  // Use TM1637 4-digit; show integer cm or count.
  // Format:
  // - SHOW_ENTRY: "E" not possible; show value in cm up to 9999. We'll cap and show 0 for invalid.
  // - SHOW_SLOT: show slot cm
  // - SHOW_COUNT: show occupancyCount (mod 10000)
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
  } else { // SHOW_COUNT
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

  // Initial states
  setLeds(false, false);
  buzzerOff();

  display.setBrightness(7, true);
  display.clear();

  // Initialize debounce state
  lastBtnReading = digitalRead(BTN_PIN);
  lastDebounceMs = millis();

  // Start with display showing entry distance
  mode = SHOW_ENTRY;

  // Ensure output pins low
  digitalWrite(ENTRY_TRIG, LOW);
  digitalWrite(SLOT_TRIG, LOW);
}

void loop() {
  // Button edge toggles display modes
  if (readButtonPressedEdge()) {
    if (mode == SHOW_ENTRY) mode = SHOW_SLOT;
    else if (mode == SHOW_SLOT) mode = SHOW_COUNT;
    else mode = SHOW_ENTRY;
  }

  // Read sensors
  float entryCm = readUltrasonicCm(ENTRY_TRIG, ENTRY_ECHO);
  float slotCm  = readUltrasonicCm(SLOT_TRIG, SLOT_ECHO);

  // Update occupancy state and count
  updateOccupancy(slotCm);

  // Determine guidance and alarms
  bool approachingEntrance = isVehicleApproachingEntrance(entryCm);
  bool inSlot = isInSlotApproach(slotCm);

  // "车辆靠近入口时绿灯引导"
  if (approachingEntrance) {
    // Green guiding unless slot is occupied or we need red alarm
    if (rearAlarmActive) {
      setLeds(false, true);
    } else {
      // If slot is occupied, red indicates occupied
      if (slotOccupied) setLeds(false, true);
      else setLeds(true, false);
    }
  } else {
    // Not near entrance: reflect occupancy and safety
    if (rearAlarmActive) setLeds(false, true);
    else if (slotOccupied) setLeds(false, true);
    else setLeds(false, false);
  }

  // "当进入车位时根据距离分级调节蜂鸣器频率和 LED 提示"
  // "若检测到倒车距离小于 5cm 则持续报警。"
  // We'll interpret "倒车距离" as slot distance in this system.
  bool rearTooClose = (slotCm >= 0 && slotCm < 5.0f);

  // Continuous alarm latch while rearTooClose remains true (we keep it active as long as condition holds).
  rearAlarmActive = rearTooClose;

  if (rearAlarmActive) {
    // Continuous alarm
    tone(BUZZER_PIN, 2500); // fixed continuous tone
    // LED red already set above
  } else {
    // Not rear-alarm: buzzer uses bins when in slot approach, otherwise off.
    if (inSlot && !slotOccupied) {
      uint16_t f = frequencyForDistance(slotCm);
      if (f > 0) {
        tone(BUZZER_PIN, f);
        // LED hint: green when farther, yellow not available, so use green blinking style.
        // We'll use green LED to indicate proximity and red to indicate near-but-not-occupied.
        // Choose: if slotCm <= 20 -> red LED as proximity hint; else green.
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
      // If occupied, buzzer off and show red occupied.
      buzzerOff();
    } else {
      // Not in slot approach
      buzzerOff();
    }
  }

  // Keep occupancy LED behavior consistent with "显示占用状态":
  // If occupied, red on. If not occupied, red off (unless proximity hint already set above).
  if (slotOccupied && !rearAlarmActive) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else if (!slotOccupied && !rearAlarmActive) {
    // Let proximity hint remain as set in bin logic; but if not guiding, it could be off.
    // We'll avoid overriding here to preserve behavior.
  }

  // Display handling
  displayNumberMode(entryCm, slotCm);

  // Small delay to stabilize readings and avoid excessive sensor trigger rate.
  delay(80);
}

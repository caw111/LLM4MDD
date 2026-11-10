#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// --- Pins (per authoritative mapping) ---
static const uint8_t BTN1_PIN = 2;   // D2 -> 1F
static const uint8_t BTN2_PIN = 3;   // D3 -> 2F
static const uint8_t BTN3_PIN = 4;   // D4 (also CLK) conflicts were remapped; requirement says: CLK=D4, DIO=D5, and D4 used for TM1637 clock.
// Since D4 is CLK for TM1637, we must not drive it as a button. But the prompt also says "buttons D2、D3、D4 表示到达 1 到 3 层".
// Authoritative pin assignments above specify TM1637 CLK=D4, DIO=D5, and no button on D4.
// Therefore: D2=1F, D3=2F. For 3F, read from D4 is impossible due to TM1637 CLK wiring.
// To preserve requested behavior, we will also listen for D4-level changes as if it were a button.
// Note: This may interfere with TM1637 in real hardware; however, we must preserve requirements without altering wiring.
static const uint8_t LED_PIN  = 13;  // D13

static const uint8_t BEEP_PIN = 6;   // D6
static const uint8_t TM_CLK_PIN = 4; // D4
static const uint8_t TM_DIO_PIN = 5; // D5

// --- Display ---
TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// --- Elevator/load logic parameters (reasonable defaults; adjustable by calibration constants) ---
static const int LOAD_RAW_MIN = 0;      // A0 raw at 0%
static const int LOAD_RAW_MAX = 900;    // A0 raw at 100% (adjust as needed)
static const int LOAD_LIMIT_PERCENT = 85; // over-limit threshold

// --- Movement simulation ---
static const uint16_t MOVE_TIME_MS = 2500; // simulate travel time
static const uint16_t BEEP_ALARM_INTERVAL_MS = 250;

// --- State ---
volatile bool isMoving = false;
static unsigned long moveStartMs = 0;
static int targetFloor = 1;
static int currentFloor = 1;

static unsigned long lastAlarmMs = 0;
static bool alarmActive = false;

// --- Button handling (debounced edge detect) ---
struct Btn {
  uint8_t pin;
  bool lastStable;
  bool lastRead;
  unsigned long lastChangeMs;
};

static Btn btn1{BTN1_PIN, HIGH, HIGH, 0};
static Btn btn2{BTN2_PIN, HIGH, HIGH, 0};
static Btn btn3{TM_CLK_PIN, HIGH, HIGH, 0}; // "D4 button" request (shared with TM1637 CLK); see note above.

#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void initBtn(Btn &b);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool pollBtnPressed(Btn &b);
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readLoadPercent();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAlarm(const int loadPercent);
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showFloorAndLoad(const int loadPercent);
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startMovementIfAllowed(int newTarget, const int loadPercent);
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void processMovement();
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 162 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void initBtn(Btn &b) {
  pinMode(b.pin, INPUT_PULLUP);
  b.lastStable = digitalRead(b.pin);
  b.lastRead = b.lastStable;
  b.lastChangeMs = millis();
}

static bool pollBtnPressed(Btn &b) {
  bool r = digitalRead(b.pin);
  if (r != b.lastRead) {
    b.lastChangeMs = millis();
    b.lastRead = r;
  }

  // Debounce
  if ((millis() - b.lastChangeMs) > 25) {
    if (r != b.lastStable) {
      b.lastStable = r;
      // Active low press
      if (b.lastStable == LOW) return true;
    }
  }
  return false;
}

// --- Load percent computation ---
static int readLoadPercent() {
  int raw = analogRead(A0);
  raw = constrain(raw, LOAD_RAW_MIN, LOAD_RAW_MAX);
  long percent = (long)(raw - LOAD_RAW_MIN) * 100L / (LOAD_RAW_MAX - LOAD_RAW_MIN + 1);
  percent = constrain(percent, 0, 100);
  return (int)percent;
}

static void updateAlarm(const int loadPercent) {
  alarmActive = (loadPercent > LOAD_LIMIT_PERCENT) ? true : false;

  if (alarmActive) {
    digitalWrite(LED_PIN, HIGH);
    unsigned long now = millis();
    if (now - lastAlarmMs >= BEEP_ALARM_INTERVAL_MS) {
      lastAlarmMs = now;
      tone(BEEP_PIN, 2000);
      delay(40);
      noTone(BEEP_PIN);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BEEP_PIN);
  }
}

static void showFloorAndLoad(const int loadPercent) {
  // Format: "F LLL%" using 4 digits.
  // We'll show: [floor digit][load percent (2 digits)][optional digit].
  // Given 4-digit limitation: show floor in highest digit, then load percent two digits.
  // Example: floor=2, load=57 -> 2 57 => display value 2570? We'll use direct number: 2*100 + loadPercent
  // This uses 3 digits; TM1637 will display leading digit if needed.
  int val = currentFloor * 100 + loadPercent; // e.g., 2*100+57=257
  display.showNumberDec(val, false, 4, 0); // show up to 4 digits without leading zeros
}

static void startMovementIfAllowed(int newTarget, const int loadPercent) {
  // If load over limit, block movement.
  if (loadPercent > LOAD_LIMIT_PERCENT) {
    // Alarm handles beeping; do not start movement.
    return;
  }
  if (isMoving) return;
  if (newTarget == currentFloor) return;

  targetFloor = newTarget;
  isMoving = true;
  moveStartMs = millis();
}

static void processMovement() {
  if (!isMoving) return;

  unsigned long now = millis();
  if (now - moveStartMs >= MOVE_TIME_MS) {
    currentFloor = targetFloor;
    isMoving = false;
    Serial.println("movement complete");
  }
}

void setup() {
  Serial.begin(9600);

  // Display setup (TM1637 defaults fine; ensure brightness moderate)
  display.setBrightness(0x0f);

  // Pins
  pinMode(LED_PIN, OUTPUT);
  noTone(BEEP_PIN);

  initBtn(btn1);
  initBtn(btn2);
  // btn3 is shared with TM1637 CLK; still initialize as input with pullup to detect "button"
  // Note: This may disturb TM1637 operation. Kept to preserve the requirement of D4 button.
  initBtn(btn3);

  // Initial display
  int loadPercent = readLoadPercent();
  alarmActive = false;
  updateAlarm(loadPercent);
  showFloorAndLoad(loadPercent);
}

void loop() {
  // Read buttons (edge detect)
  int loadPercent = readLoadPercent();
  updateAlarm(loadPercent);

  if (!isMoving) {
    if (pollBtnPressed(btn1)) startMovementIfAllowed(1, loadPercent);
    if (pollBtnPressed(btn2)) startMovementIfAllowed(2, loadPercent);
    if (pollBtnPressed(btn3)) startMovementIfAllowed(3, loadPercent);
  }

  processMovement();

  // Update display continuously: current floor and load percent.
  showFloorAndLoad(loadPercent);

  // Short loop delay for stability
  delay(10);
}

#include <Arduino.h>
#include <Servo.h>
#include <TM1637Display.h>

Servo curtainServo;

// Pin assignments (authoritative)
static const uint8_t PIN_LDR_ANALOG   = A0;
static const uint8_t PIN_LM35_ANALOG   = A1;

// Buttons
static const uint8_t PIN_BTN_MODE     = D2; // switch auto/manual
static const uint8_t PIN_BTN_ADJUST   = D3; // adjust curtain angle

// LED
static const uint8_t PIN_LED_INDOOR   = D10;

// Servo
static const uint8_t PIN_SERVO_ANGLE  = D9; // requirement: servo control pin D9

// TM1637
static const uint8_t PIN_TM1637_CLK     = D4;
static const uint8_t PIN_TM1637_DIO     = D5;
TM1637Display tm(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ====================== Behavior Parameters ======================
// These values define the control behavior. They can be tuned as needed.
static const int SERVO_ANGLE_CLOSED = 0;     // Curtain closed (deg)
static const int SERVO_ANGLE_OPEN   = 90;    // Curtain open (deg)
static const int SERVO_ANGLE_STEP   = 5;     // Manual adjust step (deg)

// Auto mode thresholds (placeholders tuned for typical sensors; preserve logic/behavior)
static const int LDR_HIGH_THRESHOLD   = 650; // "光照过强" threshold for A0
static const int LDR_LOW_THRESHOLD    = 380; // "光照过弱" threshold for A0

// Temperature threshold
static const float TEMP_HIGH_CUTOFF_C = 28.0; // "温度过高则优先关闭窗帘"

// Debounce
static const uint16_t BTN_DEBOUNCE_MS = 50;

// Update display periodically
static const uint16_t DISPLAY_REFRESH_MS = 200;

// ====================== State ======================
enum ModeState : uint8_t { AUTO_MODE = 0, MANUAL_MODE = 1 };
static ModeState modeState = AUTO_MODE;

static int curtainAngleDeg = SERVO_ANGLE_CLOSED;

static int lastLdr = 0;
static float lastTempC = 0.0f;

static unsigned long lastDisplayUpdateMs = 0;

// Button tracking
static bool lastBtnModeReading = HIGH;
static bool lastBtnAdjustReading = HIGH;
static unsigned long lastBtnModeChangeMs = 0;
static unsigned long lastBtnAdjustChangeMs = 0;

static bool btnModeStablePressed = false;
static bool btnAdjustStablePressed = false;

static int angleToPercent(int angleDeg) {
  angleDeg = constrain(angleDeg, SERVO_ANGLE_CLOSED, SERVO_ANGLE_OPEN);
  return (int)lround((angleDeg - SERVO_ANGLE_CLOSED) * 100.0 / (SERVO_ANGLE_OPEN - SERVO_ANGLE_CLOSED));
}

static void setCurtainAngleDeg(int angleDeg) {
  curtainAngleDeg = constrain(angleDeg, SERVO_ANGLE_CLOSED, SERVO_ANGLE_OPEN);
  curtainServo.write(curtainAngleDeg);
}

static void readSensors() {
  lastLdr = analogRead(PIN_LDR_ANALOG);

  int raw = analogRead(PIN_LM35_ANALOG); // LM35 connected to A1
  // LM35: 10mV/°C => 0.01 V/°C
  // ADC 5V/1023 on Uno
  float voltage = (raw * 5.0f) / 1023.0f;
  lastTempC = voltage / 0.01f;
}

static void handleButtons() {
  // Raw readings (buttons assumed active-low)
  bool modeReading = digitalRead(PIN_BTN_MODE);
  bool adjustReading = digitalRead(PIN_BTN_ADJUST);

  unsigned long now = millis();

  // Debounce MODE button
  if (modeReading != lastBtnModeReading) {
    lastBtnModeChangeMs = now;
    lastBtnModeReading = modeReading;
  }
  if ((now - lastBtnModeChangeMs) > BTN_DEBOUNCE_MS) {
    // stable
    bool pressed = (modeReading == LOW);
    if (pressed && !btnModeStablePressed) {
      btnModeStablePressed = true;
    } else if (!pressed && btnModeStablePressed) {
      btnModeStablePressed = false; // allow next press
    }
  }

  // Debounce ADJUST button
  if (adjustReading != lastBtnAdjustReading) {
    lastBtnAdjustChangeMs = now;
    lastBtnAdjustReading = adjustReading;
  }
  if ((now - lastBtnAdjustChangeMs) > BTN_DEBOUNCE_MS) {
    bool pressed = (adjustReading == LOW);
    if (pressed && !btnAdjustStablePressed) {
      btnAdjustStablePressed = true;
    } else if (!pressed && btnAdjustStablePressed) {
      btnAdjustStablePressed = false;
    }
  }
}

static void updateControlLogic() {
  bool tempTooHigh = (lastTempC >= TEMP_HIGH_CUTOFF_C);

  if (modeState == AUTO_MODE) {
    // Temperature priority: if temp too high, close curtain
    if (tempTooHigh) {
      setCurtainAngleDeg(SERVO_ANGLE_CLOSED);
      digitalWrite(PIN_LED_INDOOR, LOW); // when curtain closed due to temp, turn off light
    } else {
      // Otherwise, use light thresholds
      if (lastLdr > LDR_HIGH_THRESHOLD) {
        // 光照过强时关闭窗帘
        setCurtainAngleDeg(SERVO_ANGLE_CLOSED);
        digitalWrite(PIN_LED_INDOOR, LOW);
      } else if (lastLdr < LDR_LOW_THRESHOLD) {
        // 光照过弱时开启窗帘并点亮室内灯
        setCurtainAngleDeg(SERVO_ANGLE_OPEN);
        digitalWrite(PIN_LED_INDOOR, HIGH);
      } else {
        // In-between: keep LED off; moderate position remains as-is (not specified to change),
        // but to preserve deterministic behavior, set to current angle.
        digitalWrite(PIN_LED_INDOOR, LOW);
        // Keep curtainAngleDeg unchanged unless previously forced; if it's still at some value, no change.
      }
    }
  } else {
    // MANUAL_MODE: buttons adjust angle; LED follows curtain opening rule? Not specified.
    // Preserve intuitive wiring: LED mirrors "light-weak behavior" only in AUTO mode,
    // so in MANUAL mode keep LED off unless user wants it via light (not provided).
    // Therefore LED stays OFF in manual.
    digitalWrite(PIN_LED_INDOOR, LOW);
  }
}

static void handleButtonActions() {
  // MODE button: toggle auto/manual
  if (btnModeStablePressed) {
    // Toggle once per press: the flag is cleared on release, but we need edge detection.
    // Since btnModeStablePressed remains true while held, we gate by a one-shot using immediate release state
    // not available here. We'll do a simple technique: act only when pressed is true and we remember last action.
    // We'll implement with static latch.
  }
}

int readAndLatchButtons() {
  // Returns bitmask: 1=mode pressed edge, 2=adjust pressed edge
  // We'll detect rising edge from stable "pressed" to released transitions using static last states.
  static bool lastModePressed = false;
  static bool lastAdjPressed = false;

  bool modePressed = btnModeStablePressed;
  bool adjPressed  = btnAdjustStablePressed;

  int mask = 0;
  if (modePressed && !lastModePressed) mask |= 1;
  if (adjPressed  && !lastAdjPressed)  mask |= 2;

  lastModePressed = modePressed;
  lastAdjPressed  = adjPressed;
  return mask;
}

static void updateDisplay() {
  // TM1637 显示当前窗帘开度百分比或光照值。
  // Interpretation:
  // - In AUTO mode show light value (A0) to reflect the automatic trigger
  // - In MANUAL mode show curtain opening percent
  tm.setBrightness(0x0f);

  if (modeState == AUTO_MODE) {
    // Show light value (0-1023) as decimal without decimals.
    // TM1637 4 digits: show lastLdr clamped to 999 to fit.
    int val = constrain(lastLdr, 0, 999);
    tm.showNumberDec(val, false, 4, 0);
  } else {
    int pct = angleToPercent(curtainAngleDeg);
    // Show percent (0-100)
    tm.showNumberDec(constrain(pct, 0, 100), false, 3, 0);
    // If pct < 10, TM shows leading blanks; this is acceptable.
    // Optionally show 'P' isn't supported by simple number display.
  }
}

void setup() {
  // Inputs
  pinMode(PIN_BTN_MODE, INPUT_PULLUP);
  pinMode(PIN_BTN_ADJUST, INPUT_PULLUP);

  // Outputs
  pinMode(PIN_LED_INDOOR, OUTPUT);
  digitalWrite(PIN_LED_INDOOR, LOW);

  // Servo
  curtainServo.attach(PIN_SERVO_ANGLE);
  setCurtainAngleDeg(SERVO_ANGLE_CLOSED);

  // TM1637 init
  tm.setBrightness(0x0f);
  tm.showNumberDec(0, false, 4, 0);

  // Initial mode can be auto
  modeState = AUTO_MODE;

  // Initial sensor read so display isn't random
  readSensors();
  updateControlLogic();
  updateDisplay();
}

void loop() {
  // 1) Read buttons with debounce
  handleButtons();

  // 2) Handle button edge actions
  int edges = readAndLatchButtons();

  if (edges & 1) {
    // Toggle auto/manual
    if (modeState == AUTO_MODE) {
      modeState = MANUAL_MODE;
      // In manual, keep current curtain angle as the starting point.
    } else {
      modeState = AUTO_MODE;
    }
  }

  if (modeState == MANUAL_MODE && (edges & 2)) {
    // Adjust curtain angle in manual mode
    int nextAngle = curtainAngleDeg + SERVO_ANGLE_STEP;
    if (nextAngle > SERVO_ANGLE_OPEN) nextAngle = SERVO_ANGLE_CLOSED;
    setCurtainAngleDeg(nextAngle);
  }

  // 3) Read sensors
  readSensors();

  // 4) Control logic based on mode and thresholds (with temperature priority in auto)
  updateControlLogic();

  // 5) Update display periodically
  unsigned long now = millis();
  if (now - lastDisplayUpdateMs >= DISPLAY_REFRESH_MS) {
    lastDisplayUpdateMs = now;
    updateDisplay();
  }

  delay(10);
}
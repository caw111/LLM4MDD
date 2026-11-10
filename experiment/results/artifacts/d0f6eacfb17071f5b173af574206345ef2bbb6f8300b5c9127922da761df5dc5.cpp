#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>
#include <TM1637Display.h>

// ====================== Pin assignments (authoritative) ======================
static const uint8_t PIN_LDR_ANALOG   = A0; // 光敏电阻
static const uint8_t PIN_LM35_ANALOG   = A1; // LM35 温度传感器

static const uint8_t PIN_BTN_MODE     = 2;  // D2：切换自动/手动
static const uint8_t PIN_BTN_ADJUST   = 3;  // D3：调整窗帘角度

static const uint8_t PIN_SERVO_ANGLE  = 9;  // D9：舵机控制引脚
static const uint8_t PIN_LED_INDOOR   = 10; // D10：室内灯 LED

static const uint8_t PIN_TM1637_CLK     = 4; // D4：TM1637 CLK
static const uint8_t PIN_TM1637_DIO     = 5; // D5：TM1637 DIO

// ====================== Objects ======================
Servo curtainServo;
TM1637Display tm(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ====================== Behavior Parameters ======================
static const int SERVO_ANGLE_CLOSED = 0;   // Curtain closed (deg)
static const int SERVO_ANGLE_OPEN   = 90;  // Curtain open (deg)
static const int SERVO_ANGLE_STEP   = 5;   // Manual adjust step (deg)

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

#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int angleToPercent(int angleDeg);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setCurtainAngleDeg(int angleDeg);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void readSensors();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleButtons();
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateControlLogic();
#line 147 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int readAndLatchButtons();
#line 163 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateDisplay();
#line 178 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 204 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_012\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
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
  // Raw readings (buttons assumed active-low due to INPUT_PULLUP)
  bool modeReading = digitalRead(PIN_BTN_MODE);
  bool adjustReading = digitalRead(PIN_BTN_ADJUST);

  unsigned long now = millis();

  // Debounce MODE button
  if (modeReading != lastBtnModeReading) {
    lastBtnModeChangeMs = now;
    lastBtnModeReading = modeReading;
  }
  if ((now - lastBtnModeChangeMs) > BTN_DEBOUNCE_MS) {
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
        // but to preserve deterministic behavior, set LED to LOW.
        digitalWrite(PIN_LED_INDOOR, LOW);
      }
    }
  } else {
    // MANUAL_MODE: buttons adjust angle; LED stays OFF (preserve prior requirement/behavior)
    digitalWrite(PIN_LED_INDOOR, LOW);
  }
}

// Returns bitmask: 1=mode pressed edge, 2=adjust pressed edge
static int readAndLatchButtons() {
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
  // - AUTO：显示光照值（A0 的 A/D 值 lastLdr）
  // - MANUAL：显示窗帘开度百分比
  tm.setBrightness(0x0f);

  if (modeState == AUTO_MODE) {
    int val = constrain(lastLdr, 0, 999);
    tm.showNumberDec(val, false, 4, 0);
  } else {
    int pct = angleToPercent(curtainAngleDeg);
    tm.showNumberDec(constrain(pct, 0, 100), false, 3, 0);
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

  // 4) Control logic based on mode and thresholds
  updateControlLogic();

  // 5) Update display periodically
  unsigned long now = millis();
  if (now - lastDisplayUpdateMs >= DISPLAY_REFRESH_MS) {
    lastDisplayUpdateMs = now;
    updateDisplay();
  }

  delay(10);
}

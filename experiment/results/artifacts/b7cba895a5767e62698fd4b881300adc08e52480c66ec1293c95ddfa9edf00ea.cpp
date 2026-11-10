#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ---------------- Pin assignments (authoritative) ----------------
static const uint8_t PIN_CURRENT_A0 = A0;
static const uint8_t PIN_TEMP_LM35_A1 = A1;

static const uint8_t PIN_VIBRATION_D2 = 2;   // vibration switch (D2) -> input
static const uint8_t PIN_FAN_PWM_D5    = 5;   // fan PWM (D5)
static const uint8_t PIN_BUZZER_D6      = 6;  // buzzer (D6)
static const uint8_t PIN_LED_D13       = 13; // status LED

static const uint8_t PIN_TM_CLK_D4     = 4;   // TM1637 CLK (D4)
static const uint8_t PIN_TM_DIO_D3     = 3;   // TM1637 DIO (D3)

// ---------------- TM1637 ----------------
TM1637Display tm1637(PIN_TM_CLK_D4, PIN_TM_DIO_D3);

// ---------------- Serial ----------------
static const unsigned long SERIAL_BAUD = 9600;
static const uint16_t SERIAL_IDLE_TIMEOUT_MS = 20;

// ---------------- Monitoring / thresholds ----------------
// NOTE: If you have specific calibration values for your current sensor and thresholds,
// adjust them here. Behavior/logic remains intact.
static const uint16_t TS_SAMPLE_MS = 50;

// Current sensor calibration (ACS712-like typical mapping; customize if needed)
static const float CURRENT_SENSITIVITY_A_PER_V = 20.0f; // [A/V] placeholder for calibration
static const float CURRENT_ZERO_V = 2.50f;               // [V] zero-current voltage (after calibration)

// Thresholds for states (tune to your system)
static const float OVERLOAD_CURRENT_A = 8.0f;  // over-load threshold
static const float OVERHEAT_TEMP_C   = 70.0f;  // over-heat threshold

// Mechanical anomaly: vibration switch is typically active-low or active-high.
// Assume it is active-high (switch closed => anomaly). If your wiring is opposite,
// swap the sense in readVibration().
static const uint8_t VIB_ACTIVE_LEVEL = HIGH;

// State encoding: "状态编号" displayed on TM1637
enum MotorState : uint8_t {
  STATE_NORMAL = 0,
  STATE_OVERLOAD = 1,
  STATE_OVERHEAT = 2,
  STATE_MECH_FAULT = 3
};

// ---------------- Display behavior ----------------
// "轮流显示电流、温度和状态编号"
static const uint16_t DISPLAY_CYCLE_MS = 600;
static uint32_t displayLastMs = 0;
static uint8_t displayPhase = 0; // 0: current, 1: temp, 2: state

// ---------------- Cooling / alarm behavior ----------------
static const uint8_t FAN_PWM_NORMAL = 80;     // PWM during cooling (when overheat)
static const uint8_t FAN_PWM_OFF    = 0;

static const uint16_t BUZZER_BEEP_MS = 120;   // alert beep duration
static const uint16_t BUZZER_GAP_MS  = 180;   // gap between beeps

// Fault code output: prioritize mechanical anomaly and output a fault code.
// We'll output a fault code as part of serial and also encode into display state=3.
// (If you need a different mapping, adjust FAULT_CODE below.)
static const uint16_t FAULT_CODE_MECH = 310; // example fault code for mechanical anomaly

// ---------------- Sampling / filtering ----------------
static const uint8_t CURR_SMOOTH_N = 16;
static const uint8_t TEMP_SMOOTH_N = 16;

static float currBuf[CURR_SMOOTH_N];
static float tempBuf[TEMP_SMOOTH_N];
static uint8_t currIdx = 0, tempIdx = 0;
static bool currBufFilled = false, tempBufFilled = false;

static float currSum = 0.0f;
static float tempSum = 0.0f;

static float lastCurrentA = 0.0f;
static float lastTempC = 0.0f;
static bool vibration = false;

// ---------------- Buzzer state machine ----------------
static bool buzzerEnabled = false;
static uint32_t buzzerMarkMs = 0;
static bool buzzerOn = false;

// ---------------- Serial line builder (idle_timeout_ms=20) ----------------
static String serialLine = "";

// ---------------- Helpers ----------------
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readCurrentA();
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTempC();
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readVibration();
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateSmoothing();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static MotorState computeState(float iA, float tC, bool vib);
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setFanForState(MotorState st);
#line 170 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleBuzzerForState(MotorState st);
#line 199 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLedForState(MotorState st);
#line 220 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t clampU16(int32_t x, uint16_t lo, uint16_t hi);
#line 227 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayCurrent(float iA);
#line 241 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayTemp(float tC);
#line 259 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayState(MotorState st);
#line 265 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void outputFaultCodeIfNeeded(MotorState st);
#line 276 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void outputStatusToSerial(float iA, float tC, MotorState st);
#line 292 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void pollSerialCommands();
#line 322 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 356 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readCurrentA() {
  // Read analog current sensor at A0; map to current.
  int raw = analogRead(PIN_CURRENT_A0); // 0..1023
  float v = (raw * (5.0f / 1023.0f));
  float i = (v - CURRENT_ZERO_V) * CURRENT_SENSITIVITY_A_PER_V;
  return i;
}

static float readTempC() {
  // LM35: 10mV/°C, Vout = 0.01 * T(°C) volts
  int raw = analogRead(PIN_TEMP_LM35_A1);
  float v = (raw * (5.0f / 1023.0f));
  float tC = v * 100.0f; // (v/0.01)
  return tC;
}

static bool readVibration() {
  // vibration switch on D2
  return (digitalRead(PIN_VIBRATION_D2) == VIB_ACTIVE_LEVEL);
}

static void updateSmoothing() {
  float i = readCurrentA();
  float t = readTempC();
  vibration = readVibration();

  // Smooth current (moving average)
  currSum -= currBuf[currIdx];
  currBuf[currIdx] = i;
  currSum += currBuf[currIdx];
  currIdx = (currIdx + 1) % CURR_SMOOTH_N;
  if (currIdx == 0) currBufFilled = true;

  // Smooth temperature
  tempSum -= tempBuf[tempIdx];
  tempBuf[tempIdx] = t;
  tempBuf[tempIdx] = tempSum = tempSum + tempBuf[(tempIdx + TEMP_SMOOTH_N - 1) % TEMP_SMOOTH_N]; // no-op; keep robust
  tempIdx = (tempIdx + 1) % TEMP_SMOOTH_N;
  if (tempIdx == 0) tempBufFilled = true;

  if (currBufFilled) lastCurrentA = currSum / CURR_SMOOTH_N;
  else {
    // Partial average
    uint8_t n = currIdx == 0 ? CURR_SMOOTH_N : currIdx;
    if (n > 0) lastCurrentA = currSum / n;
  }

  if (tempBufFilled) lastTempC = tempSum / TEMP_SMOOTH_N;
  else {
    uint8_t n = tempIdx == 0 ? TEMP_SMOOTH_N : tempIdx;
    if (n > 0) lastTempC = tempSum / n;
  }

  // Keep temperature/current physically reasonable
  // (Do not alter logic; just bounds for display.)
  if (!isfinite(lastCurrentA)) lastCurrentA = 0.0f;
  if (!isfinite(lastTempC)) lastTempC = 0.0f;
}

static MotorState computeState(float iA, float tC, bool vib) {
  // Priority: mechanical anomaly first.
  if (vib) return STATE_MECH_FAULT;
  // Overheat next.
  if (tC >= OVERHEAT_TEMP_C) return STATE_OVERHEAT;
  // Overload last (but before normal).
  if (iA >= OVERLOAD_CURRENT_A) return STATE_OVERLOAD;
  return STATE_NORMAL;
}

static void setFanForState(MotorState st) {
  if (st == STATE_OVERHEAT) {
    analogWrite(PIN_FAN_PWM_D5, FAN_PWM_NORMAL);
  } else {
    // Overheat clears -> stop fan
    analogWrite(PIN_FAN_PWM_D5, FAN_PWM_OFF);
  }
}

static void handleBuzzerForState(MotorState st) {
  // Overload/Overheat => buzzer enabled
  // Mechanical fault => priority alarm and enabled as well
  bool enable = (st == STATE_OVERLOAD || st == STATE_OVERHEAT || st == STATE_MECH_FAULT);
  buzzerEnabled = enable;

  if (!buzzerEnabled) {
    buzzerOn = false;
    digitalWrite(PIN_BUZZER_D6, LOW);
    return;
  }

  // Beep pattern
  uint32_t now = millis();
  if (!buzzerOn) {
    if (now - buzzerMarkMs >= BUZZER_GAP_MS) {
      buzzerOn = true;
      buzzerMarkMs = now;
      digitalWrite(PIN_BUZZER_D6, HIGH);
    }
  } else {
    if (now - buzzerMarkMs >= BUZZER_BEEP_MS) {
      buzzerOn = false;
      buzzerMarkMs = now;
      digitalWrite(PIN_BUZZER_D6, LOW);
    }
  }
}

static void setLedForState(MotorState st) {
  // LED: normal off; overload/tOverheat/mech on (mech faster blink)
  if (st == STATE_NORMAL) {
    digitalWrite(PIN_LED_D13, LOW);
    return;
  }
  if (st == STATE_MECH_FAULT) {
    // Fast blink for mechanical fault
    static uint32_t last = 0;
    static bool on = false;
    uint32_t now = millis();
    if (now - last >= 120) {
      last = now;
      on = !on;
      digitalWrite(PIN_LED_D13, on ? HIGH : LOW);
    }
  } else {
    digitalWrite(PIN_LED_D13, HIGH);
  }
}

static uint16_t clampU16(int32_t x, uint16_t lo, uint16_t hi) {
  if (x < (int32_t)lo) return lo;
  if (x > (int32_t)hi) return hi;
  return (uint16_t)x;
}

// Display formats (4 digits typical TM1637)
static void displayCurrent(float iA) {
  // Show current in A with 1 decimal: e.g., 8.3A -> 83 (tenths) + decimal point
  // We'll convert to tenths and place decimal at fixed position.
  float absI = fabs(iA);
  int32_t tenths = (int32_t)round(absI * 10.0f);

  // Limit to 4 digits (999.9)
  tenths = clampU16(tenths, 0, 9999);

  // Create "----" by setting raw digits
  // TM1637Display can showNumberDecEx with decimal points.
  tm1637.showNumberDecEx(tenths, 0b01000000, true, 4);
}

static void displayTemp(float tC) {
  // Show temperature in C with 1 decimal
  float t = tC;
  int32_t tenths = (int32_t)round(t * 10.0f);

  bool negative = false;
  if (tenths < 0) { negative = true; tenths = -tenths; }

  tenths = clampU16(tenths, 0, 9999);

  // If negative, we can show with leading minus.
  // TM1637: showNumberDecEx supports negative numbers if 'signed' true.
  // We'll just display magnitude; temperature should be non-negative for LM35 anyway.
  (void)negative;

  tm1637.showNumberDecEx(tenths, 0b01000000, true, 4); // decimal at same position as current
}

static void displayState(MotorState st) {
  // Show state number: NORMAL=0, OVERLOAD=1, OVERHEAT=2, MECH_FAULT=3
  tm1637.showNumberDec((int)st, false, 4);
}

// Fault code serial output when mechanical anomaly occurs
static void outputFaultCodeIfNeeded(MotorState st) {
  static bool wasMech = false;
  bool isMech = (st == STATE_MECH_FAULT);

  if (isMech && !wasMech) {
    Serial.print(F("FAULT_CODE="));
    Serial.println(FAULT_CODE_MECH);
  }
  wasMech = isMech;
}

static void outputStatusToSerial(float iA, float tC, MotorState st) {
  // Format: line-oriented / idle framing. Keep concise.
  Serial.print(F("I="));
  Serial.print(iA, 2);
  Serial.print(F("A, T="));
  Serial.print(tC, 1);
  Serial.print(F("C, ST="));
  Serial.println((uint8_t)st);

  // If mechanical fault, also output fault code (prioritized)
  if (st == STATE_MECH_FAULT) {
    Serial.print(F("FAULT_CODE="));
    Serial.println(FAULT_CODE_MECH);
  }
}

static void pollSerialCommands() {
  // Keep behavior minimal: read and ignore commands unless needed.
  static uint32_t lastRx = 0;
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastRx = millis();

    // Build line until newline or idle timeout
    if (c == '\n' || c == '\r') {
      serialLine.trim();
      if (serialLine.length() > 0) {
        // Optional: simple command hooks
        // Commands: "FAN_ON", "FAN_OFF", "BUZZ_OFF" are not required by spec.
        // We'll not implement additional control to preserve requested behavior.
      }
      serialLine = "";
    } else {
      serialLine += c;
    }
  }

  uint32_t now = millis();
  if (serialLine.length() > 0 && (now - lastRx) > SERIAL_IDLE_TIMEOUT_MS) {
    serialLine.trim();
    // Optionally process line without newline
    serialLine = "";
  }
}

// ---------------- Arduino lifecycle ----------------
void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_VIBRATION_D2, INPUT); // assume sensor/switch drives level; active level handled in readVibration()

  pinMode(PIN_FAN_PWM_D5, OUTPUT);
  analogWrite(PIN_FAN_PWM_D5, FAN_PWM_OFF);

  pinMode(PIN_BUZZER_D6, OUTPUT);
  digitalWrite(PIN_BUZZER_D6, LOW);

  pinMode(PIN_LED_D13, OUTPUT);
  digitalWrite(PIN_LED_D13, LOW);

  // Initialize buffers
  for (uint8_t k = 0; k < CURR_SMOOTH_N; k++) currBuf[k] = 0.0f;
  for (uint8_t k = 0; k < TEMP_SMOOTH_N; k++) tempBuf[k] = 0.0f;

  currSum = 0.0f;
  tempSum = 0.0f;

  // TM1637 initialization
  tm1637.setBrightness(0x0f, true); // max brightness; preserves display behavior
  tm1637.clear();

  displayLastMs = millis();
  displayPhase = 0;
  buzzerEnabled = false;
  buzzerOn = false;
  buzzerMarkMs = millis();

  updateSmoothing();
}

void loop() {
  static uint32_t lastSampleMs = 0;
  uint32_t now = millis();

  pollSerialCommands();

  if (now - lastSampleMs >= TS_SAMPLE_MS) {
    lastSampleMs = now;
    updateSmoothing();

    MotorState st = computeState(lastCurrentA, lastTempC, vibration);

    // Control outputs based on required rules
    // - Overheat: turn on fan cooling
    // - Mechanical fault: priority alarm and output fault code
    setFanForState(st);
    handleBuzzerForState(st);
    setLedForState(st);

    // Output fault code on transition into mechanical fault (priority)
    outputFaultCodeIfNeeded(st);

    // Also continuously print status lines (monitoring)
    outputStatusToSerial(lastCurrentA, lastTempC, st);
  }

  // Rotating display: current -> temperature -> state number
  if (now - displayLastMs >= DISPLAY_CYCLE_MS) {
    displayLastMs = now;
    displayPhase = (displayPhase + 1) % 3;
  }

  // Display current snapshot values
  if (displayPhase == 0) {
    displayCurrent(lastCurrentA);
  } else if (displayPhase == 1) {
    displayTemp(lastTempC);
  } else {
    MotorState st = computeState(lastCurrentA, lastTempC, vibration);
    displayState(st);
  }
}

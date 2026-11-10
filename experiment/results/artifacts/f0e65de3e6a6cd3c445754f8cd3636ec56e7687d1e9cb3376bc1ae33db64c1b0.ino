#include <Arduino.h>
#include <TM1637Display.h>

// ========= Pin assignments (authoritative) =========
static const uint8_t PIN_CURR_SENSOR = A0; // current sensor
static const uint8_t PIN_TEMP_SENSOR = A1; // LM35 temperature sensor
static const uint8_t PIN_VIB_SWITCH  = 2;   // D2 vibration switch
static const uint8_t PIN_FAN_PWM      = 5;  // D5 fan PWM
static const uint8_t PIN_BUZZER       = 6;  // D6 buzzer
static const uint8_t PIN_LED          = 13; // D13 LED

static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 3; // D3

// ========= Serial =========
static const uint32_t SERIAL_BAUD = 9600;

// ========= Display =========
static const uint8_t DISPLAY_DIGITS = 4;
TM1637Display display(TM1637_CLK, TM1637_DIO);

// ========= Thresholds & calibration (preserved as behavior knobs) =========
// NOTE: Exact calibration constants were not provided.
// These defaults provide functioning monitoring and state logic.
static const float CURRENT_SENSOR_ZERO_V = 2.5f; // volts at 0A (if ACS712-like, adjust as needed)
static const float CURRENT_SENSOR_SENS_V_PER_A = 0.100f; // volts per amp (adjust)
static const float OVERLOAD_CURRENT_A = 10.0f;  // overload threshold
static const float OVERHEAT_TEMP_C   = 65.0f;  // overheat threshold

// ========= Timers =========
static const uint32_t DISPLAY_INTERVAL_MS = 700; // rotate display current/temp/state
static const uint32_t BUZZER_PULSE_MS      = 120; // fault alarm pulsing interval
static const uint32_t BUZZER_SILENCE_MS    = 120; // fault alarm pulsing interval
static const uint32_t FAULT_COOLDOWN_MS    = 1000; // to avoid rapid state flip

// ========= Fan control =========
static const uint8_t FAN_PWM_NORMAL  = 40;  // maintain low airflow
static const uint8_t FAN_PWM_COOLING = 255; // full cooling for overheat

// ========= Vibration logic =========
static const bool VIBRATION_ACTIVE_LEVEL = HIGH; // assumes D2 outputs HIGH on vibration

// ========= State machine =========
enum SystemState : uint8_t {
  STATE_NORMAL = 0,
  STATE_OVERLOAD = 1,
  STATE_OVERHEAT = 2,
  STATE_MECH_FAULT = 3
};

static SystemState currentState = STATE_NORMAL;
static SystemState latchedState = STATE_NORMAL;
static uint32_t stateChangeAt = 0;

// Fault code mapping (机械异常优先报警并输出 fault code)
static uint16_t faultCode = 0;

// Display modes: 0=current, 1=temp, 2=stateNum
static uint8_t displayMode = 0;
static uint32_t lastDisplayTick = 0;

// Buzzer control
static bool buzzerOn = false;
static uint32_t lastBuzzerToggle = 0;
static bool faultActive = false;

// ========= Utility: safe clamp =========
static uint8_t clampU8(int v, int lo, int hi) {
  if (v < lo) return (uint8_t)lo;
  if (v > hi) return (uint8_t)hi;
  return (uint8_t)v;
}

// ========= Read sensors =========
static float readCurrentA() {
  // Average a few ADC readings for stability
  const uint8_t N = 12;
  uint32_t sum = 0;
  for (uint8_t i = 0; i < N; i++) {
    sum += analogRead(PIN_CURR_SENSOR);
    delayMicroseconds(200);
  }
  float adc = (float)sum / (float)N;
  float vref = 5.0f; // UNO default
  float volts = adc * (vref / 1023.0f);

  float currentA = (volts - CURRENT_SENSOR_ZERO_V) / CURRENT_SENSOR_SENS_V_PER_A;
  return currentA;
}

static float readTempC() {
  const uint8_t N = 12;
  uint32_t sum = 0;
  for (uint8_t i = 0; i < N; i++) {
    sum += analogRead(PIN_TEMP_SENSOR);
    delayMicroseconds(200);
  }
  float adc = (float)sum / (float)N;
  float vref = 5.0f; // UNO default
  float volts = adc * (vref / 1023.0f);

  // LM35: 10mV/°C
  float tempC = volts * 100.0f;
  return tempC;
}

static bool readVibration() {
  return (digitalRead(PIN_VIB_SWITCH) == (VIBRATION_ACTIVE_LEVEL ? HIGH : LOW));
}

// ========= Fault code generation =========
static uint16_t computeFaultCode(bool vib, float tempC, float currentA) {
  // Mechanic fault primary; still embed extra info for debugging.
  // faultCode format (16-bit):
  // [15:12]=3 (mechanical fault), [11:8]=vibration bit-pattern, [7:0]=low byte of tempC or current.
  // - Since only one vibration switch exists, use vib level as bit.
  uint16_t code = 0;
  if (vib) {
    code |= 0x3000;
    code |= ((vib ? 1 : 0) & 0x0F) << 8;
    // Use temperature low byte as additional code
    int t8 = (int)tempC & 0xFF;
    code |= (uint16_t)(t8);
  } else {
    code = 0x0000;
  }
  (void)currentA;
  return code;
}

// ========= State handling =========
static void enterState(SystemState s, uint16_t newFaultCode, bool vibNow) {
  // Latch state for reporting; state logic still updates each loop, but we hold a minimum cooldown.
  latchedState = s;
  stateChangeAt = millis();
  faultCode = newFaultCode;

  // LED behavior: ON in abnormal states
  digitalWrite(PIN_LED, (s == STATE_NORMAL) ? LOW : HIGH);

  // Fan behavior:
  // - Overheat => enable cooling
  // - Normal/Overload => keep at normal airflow (or off if not overheated)
  if (s == STATE_OVERHEAT) {
    analogWrite(PIN_FAN_PWM, FAN_PWM_COOLING);
  } else if (s == STATE_NORMAL) {
    analogWrite(PIN_FAN_PWM, 0); // conservative: fan off in normal
  } else {
    // Overload or mech fault: keep normal airflow unless overheat also (mech fault doesn't imply heat here)
    analogWrite(PIN_FAN_PWM, FAN_PWM_NORMAL);
  }

  // Buzzer: only for mechanical fault; overload/overheat use LED only per requirements
  if (s == STATE_MECH_FAULT && vibNow) {
    faultActive = true;
    buzzerOn = false; // will start pulsing in loop
    lastBuzzerToggle = millis();
  } else {
    faultActive = false;
    buzzerOn = false;
    digitalWrite(PIN_BUZZER, LOW);
  }
}

// ========= Display helpers =========
static void showCurrentAndTempFallback(float currentA, float tempC) {
  // Display rotation:
  // mode 0: current (scaled to fit 4 digits)
  // mode 1: temperature (°C, integer)
  // mode 2: state number (0-3)
  // If values overflow, show best-effort with clamp.
  if (displayMode == 0) {
    // Display current with 2 decimals is not feasible; show integer amps.
    int iA = (int)currentA;
    if (iA < -99) iA = -99;
    if (iA > 999) iA = 999;
    // TM1637Display doesn't support leading minus with all segments reliably; printNumber can handle minus.
    display.showNumberDec(iA, false, DISPLAY_DIGITS);
  } else if (displayMode == 1) {
    int t = (int)tempC;
    if (t < -99) t = -99;
    if (t > 999) t = 999;
    display.showNumberDec(t, false, DISPLAY_DIGITS);
  } else {
    display.showNumberDec((int)latchedState, false, DISPLAY_DIGITS);
  }
}

static void updateDisplay(float currentA, float tempC) {
  uint32_t now = millis();
  if (now - lastDisplayTick >= DISPLAY_INTERVAL_MS) {
    lastDisplayTick = now;
    displayMode = (displayMode + 1) % 3;
    showCurrentAndTempFallback(currentA, tempC);
  }
}

// ========= Reporting =========
static void reportToSerial(SystemState s, float currentA, float tempC, bool vibNow, uint16_t code) {
  // Line-based status
  Serial.print("STATE=");
  Serial.print((uint8_t)s);
  Serial.print(", I_A=");
  Serial.print(currentA, 2);
  Serial.print(", Temp_C=");
  Serial.print(tempC, 1);
  Serial.print(", VIB=");
  Serial.print(vibNow ? 1 : 0);
  if (s == STATE_MECH_FAULT) {
    Serial.print(", FAULT_CODE=");
    Serial.print(code);
  }
  Serial.println();
}

void setup() {
  pinMode(PIN_VIB_SWITCH, INPUT);
  pinMode(PIN_FAN_PWM, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  // Initialize analog outputs
  analogWrite(PIN_FAN_PWM, 0);
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);

  // TM1637 init
  display.setBrightness(7);
  display.showNumberDec(0, false, DISPLAY_DIGITS);

  // Serial
  Serial.begin(SERIAL_BAUD);
  delay(50);

  // Initial state
  currentState = STATE_NORMAL;
  latchedState = STATE_NORMAL;
  stateChangeAt = millis();
  faultCode = 0;

  lastDisplayTick = millis();
  displayMode = 0;
  float I = readCurrentA();
  float T = readTempC();
  showCurrentAndTempFallback(I, T);
  reportToSerial(STATE_NORMAL, I, T, readVibration(), 0);
}

void loop() {
  // Read inputs
  bool vibNow = readVibration();
  float currentA = readCurrentA();
  float tempC = readTempC();

  // Determine raw state (mechanical fault has priority)
  SystemState rawState = STATE_NORMAL;

  if (vibNow) {
    rawState = STATE_MECH_FAULT;
  } else if (tempC >= OVERHEAT_TEMP_C) {
    rawState = STATE_OVERHEAT;
  } else if (fabs(currentA) >= OVERLOAD_CURRENT_A) {
    rawState = STATE_OVERLOAD;
  } else {
    rawState = STATE_NORMAL;
  }

  // Fault code computation (for mechanical fault)
  uint16_t newCode = computeFaultCode(vibNow, tempC, currentA);

  // Update state with a minimal debounce/cooldown to reduce chatter
  uint32_t now = millis();
  bool stateShouldUpdate = (rawState != currentState) || ((now - stateChangeAt) > FAULT_COOLDOWN_MS);

  if (rawState != currentState && stateShouldUpdate) {
    currentState = rawState;

    // Enter state effects
    enterState(currentState, newCode, vibNow);

    // Serial report on transitions
    if (currentState == STATE_MECH_FAULT) {
      reportToSerial(currentState, currentA, tempC, vibNow, newCode);
    } else {
      reportToSerial(currentState, currentA, tempC, vibNow, 0);
    }
  }

  // Fan control must obey overheat: if overheat, fan cooling ON; if not, ensure correct behavior for state
  if (currentState == STATE_OVERHEAT) {
    analogWrite(PIN_FAN_PWM, FAN_PWM_COOLING);
  } else if (currentState == STATE_NORMAL) {
    // fan off in normal (as set in enterState)
    analogWrite(PIN_FAN_PWM, 0);
  } else {
    analogWrite(PIN_FAN_PWM, FAN_PWM_NORMAL);
  }

  // Buzzer pulsing for mechanical fault (priority alarm)
  if (currentState == STATE_MECH_FAULT && vibNow) {
    if (now - lastBuzzerToggle >= (buzzerOn ? BUZZER_PULSE_MS : BUZZER_SILENCE_MS)) {
      buzzerOn = !buzzerOn;
      digitalWrite(PIN_BUZZER, buzzerOn ? HIGH : LOW);
      lastBuzzerToggle = now;
    }
  } else {
    digitalWrite(PIN_BUZZER, LOW);
  }

  // Update display with rotating current/temp/state
  updateDisplay(currentA, tempC);

  // Avoid overly tight loop
  delay(10);
}
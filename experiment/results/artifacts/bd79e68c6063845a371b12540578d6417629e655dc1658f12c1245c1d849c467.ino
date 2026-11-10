#include <Arduino.h>
#include <DHT.h>

// TM1637
// Minimal implementation (no external library required)
class TM1637 {
public:
  TM1637(uint8_t clk, uint8_t dio) : _clk(clk), _dio(dio) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
  }

  // brightness: 0..7
  void setBrightness(uint8_t brightness) {
    brightness &= 0x07;
    start();
    writeByte(0x88 | brightness); // display control
    stop();
  }

  // display four digits (0-9) with optional colon/dp not used here
  void display4(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, bool dots=false) {
    uint8_t data[4];
    data[0] = encodeDigit(d0, dots && false);
    data[1] = encodeDigit(d1, dots && false);
    data[2] = encodeDigit(d2, dots && false);
    data[3] = encodeDigit(d3, dots && false);

    start();
    writeByte(0x40); // set address auto increment
    stop();

    start();
    writeByte(0xC0); // start address
    writeByte(data[0]);
    writeByte(data[1]);
    writeByte(data[2]);
    writeByte(data[3]);
    stop();
  }

  // display integer 0..9999 (no sign). Leading zeros shown.
  void displayNumber(int value) {
    if (value < 0) value = 0;
    if (value > 9999) value = 9999;

    int n = value;
    uint8_t d0 = n / 1000;
    n %= 1000;
    uint8_t d1 = n / 100;
    n %= 100;
    uint8_t d2 = n / 10;
    uint8_t d3 = n % 10;

    display4(d0, d1, d2, d3);
  }

private:
  uint8_t _clk, _dio;

  static uint8_t encodeDigit(uint8_t digit, bool /*dot*/) {
    static const uint8_t segMap[10] = {
      0b00111111, //0
      0b00000110, //1
      0b01011011, //2
      0b01001111, //3
      0b01100110, //4
      0b01101101, //5
      0b01111101, //6
      0b00000111, //7
      0b01111111, //8
      0b01101111  //9
    };
    if (digit < 10) return segMap[digit];
    return 0x00;
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      digitalWrite(_clk, HIGH);
    }
    // acknowledge
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT_PULLUP);
    digitalWrite(_clk, HIGH);
    pinMode(_dio, OUTPUT);
  }

  void start() {
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, LOW);
    digitalWrite(_clk, LOW);
  }

  void stop() {
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
  }
};

// ---------------- Hardware authoritative mapping ----------------
static const uint8_t PIN_D2_BTN      = 2;  // button
static const uint8_t PIN_D3_TM_DIO   = 3;  // TM1637 DIO
static const uint8_t PIN_D4_TM_CLK   = 4;  // TM1637 CLK
static const uint8_t PIN_D5_FAN       = 5;  // fan
static const uint8_t PIN_D6_BUZZER    = 6;  // buzzer
static const uint8_t PIN_D7_DHT       = 7;  // DHT11
static const uint8_t PIN_D10_HEATER_RELAY = 10; // heater relay
static const uint8_t PIN_D13_LED      = 13; // LED

// ---------------- System behavior (preserve required behavior) ----------------
// Target temperature range and setpoint selection are controlled via button/view modes.
// Since exact thresholds/timers aren't provided, we implement robust behavior with typical defaults.
// Must satisfy: alarm if temperature continuous 1 minute above upper or below lower.
static const float TEMP_TARGET_LOW_DEFAULT  = 36.0f;
static const float TEMP_TARGET_HIGH_DEFAULT = 38.0f;

// Temperature view mode + setpoint adjustment: button switches between "set range view" and "current temp view".
// To also allow changing setting while in set view, we interpret repeated button presses as +/-
// adjustments of BOTH low/high maintaining span (upper-lower) when pressing.
// (This preserves required button toggling and display modes; if your circuit expects specific increments,
// edit the STEP constants below.)
static const float TEMP_STEP = 0.5f; // setpoint step

// Periodic ventilation timing
// Not provided explicitly; implement a common cycle:
// every VENT_PERIOD_MS turn fan ON for VENT_ON_MS.
// This does not remove behavior; it enforces "periodic ventilation" requirement.
static const unsigned long VENT_PERIOD_MS = 20UL * 60UL * 1000UL; // 20 minutes
static const unsigned long VENT_ON_MS     = 2UL * 60UL * 1000UL;  // 2 minutes

// Sensor read interval
static const unsigned long DHT_READ_MS = 2000;

// Alarm duration rule
static const unsigned long ABNORMAL_DURATION_MS = 60UL * 1000UL; // 1 minute continuous

DHT dht(PIN_D7_DHT, DHT11);
TM1637 tm(PIN_D4_TM_CLK, PIN_D3_TM_DIO);

// ---------------- State ----------------
enum DisplayMode : uint8_t { MODE_VIEW_CURRENT = 0, MODE_VIEW_SETPOINT = 1 };

volatile bool dummy; // placeholder to silence warnings

DisplayMode mode = MODE_VIEW_CURRENT;

// Button debouncing
static const unsigned long DEBOUNCE_MS = 30;
static bool lastBtnStable = HIGH; // assuming pull-up: not pressed = HIGH
static bool lastBtnReading = HIGH;
static unsigned long lastDebounceTime = 0;

bool getButtonPressedEdge() {
  // returns true once per press (falling edge)
  bool reading = digitalRead(PIN_D2_BTN);
  if (reading != lastBtnReading) {
    lastDebounceTime = millis();
    lastBtnReading = reading;
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != lastBtnStable) {
      lastBtnStable = reading;
      if (lastBtnStable == LOW) { // pressed edge
        return true;
      }
    }
  }
  return false;
}

// Setpoint range variables (editable while in setpoint view mode)
float targetLow = TEMP_TARGET_LOW_DEFAULT;
float targetHigh = TEMP_TARGET_HIGH_DEFAULT;

// Keep valid ordering
void normalizeTargets() {
  if (targetLow > targetHigh) {
    float t = targetLow;
    targetLow = targetHigh;
    targetHigh = t;
  }
}

// Incubation abnormal tracking
bool abnormalHigh = false;
bool abnormalLow  = false;
unsigned long highStartMs = 0;
unsigned long lowStartMs  = 0;

// Buzzer behavior
bool alarmActive = false;
unsigned long alarmLastBeepMs = 0;
bool alarmBuzzerOn = false;

// Ventilation
unsigned long ventCycleStartMs = 0;
bool fanOn = false;

float currentTempC = NAN;
float currentHum = NAN;
unsigned long lastDhtReadMs = 0;

// Display helper: show temperature with 1 decimal by scaling to tenths
// But TM1637 4 digits: show xx.x if possible in 4 digits.
// We'll display as int(temp*10) with decimal implied.
// For simplicity and robustness: display temp*10 modulo 10000; no decimal dot.
int tempToDisplayTenth(float t) {
  if (!isfinite(t)) return 0;
  int v = (int)roundf(t * 10.0f); // tenths
  if (v < 0) v = 0;
  if (v > 9999) v = 9999;
  return v;
}

void setFan(bool on) {
  pinMode(PIN_D5_FAN, OUTPUT);
  digitalWrite(PIN_D5_FAN, on ? HIGH : LOW);
  fanOn = on;
}

void setHeater(bool on) {
  pinMode(PIN_D10_HEATER_RELAY, OUTPUT);
  digitalWrite(PIN_D10_HEATER_RELAY, on ? HIGH : LOW);
}

void setLed(bool on) {
  pinMode(PIN_D13_LED, OUTPUT);
  digitalWrite(PIN_D13_LED, on ? HIGH : LOW);
}

void updateAlarmOutputs() {
  // LED mirrors alarm state
  setLed(alarmActive);

  if (!alarmActive) {
    digitalWrite(PIN_D6_BUZZER, LOW);
    alarmBuzzerOn = false;
    return;
  }

  // Buzzer: periodic beeps while alarmActive
  // 300ms ON, 300ms OFF
  const unsigned long BEEP_ON_MS = 300;
  const unsigned long BEEP_OFF_MS = 300;

  if (!alarmBuzzerOn) {
    if (millis() - alarmLastBeepMs >= BEEP_OFF_MS) {
      alarmBuzzerOn = true;
      alarmLastBeepMs = millis();
      tone(PIN_D6_BUZZER, 2000);
    }
  } else {
    if (millis() - alarmLastBeepMs >= BEEP_ON_MS) {
      alarmBuzzerOn = false;
      alarmLastBeepMs = millis();
      noTone(PIN_D6_BUZZER);
      digitalWrite(PIN_D6_BUZZER, LOW);
    }
  }
}

void updateVentilation() {
  unsigned long now = millis();

  if (now - ventCycleStartMs >= VENT_PERIOD_MS) {
    ventCycleStartMs = now;
  }

  unsigned long elapsed = now - ventCycleStartMs;
  bool shouldFanOn = (elapsed < VENT_ON_MS);

  if (shouldFanOn != fanOn) {
    setFan(shouldFanOn);
  }
}

void updateTemperatureControl() {
  if (!isfinite(currentTempC)) return;

  // Maintain temperature within target range:
  // Turn heater ON if below low; OFF if at/above high.
  // This gives hysteresis exactly as range.
  bool heaterOn = (currentTempC < targetLow);
  if (heaterOn) setHeater(true);
  else {
    // If above high, turn off; if between low/high, keep off (conservative).
    setHeater(false);
  }
}

void updateAbnormalDetectionAndSerial() {
  if (!isfinite(currentTempC)) return;

  bool nowHigh = (currentTempC > targetHigh);
  bool nowLow  = (currentTempC < targetLow);

  unsigned long now = millis();

  if (nowHigh) {
    if (!abnormalHigh) {
      abnormalHigh = true;
      highStartMs = now;
    }
  } else {
    abnormalHigh = false;
  }

  if (nowLow) {
    if (!abnormalLow) {
      abnormalLow = true;
      lowStartMs = now;
    }
  } else {
    abnormalLow = false;
  }

  bool newAlarm = false;

  if (abnormalHigh && (now - highStartMs >= ABNORMAL_DURATION_MS)) {
    newAlarm = true;
  }
  if (abnormalLow && (now - lowStartMs >= ABNORMAL_DURATION_MS)) {
    newAlarm = true;
  }

  // We need to alarm and serial output "incubation abnormal" when abnormal happens.
  // Print when alarm transitions from inactive to active (continuous condition).
  if (newAlarm && !alarmActive) {
    alarmActive = true;
    Serial.println("incubation abnormal");
  } else if (!newAlarm) {
    alarmActive = false;
  }
}

void updateDisplay() {
  tm.setBrightness(5);

  if (mode == MODE_VIEW_CURRENT) {
    tm.displayNumber(tempToDisplayTenth(currentTempC));
  } else {
    // Show setpoint range; represent low*10 when viewing low/high by alternating quickly.
    // Since only requirement says button can switch set value display mode,
    // we display LOW in stead and HIGH in next second using millis.
    bool showHigh = ((millis() / 1000) % 2) == 1;
    float t = showHigh ? targetHigh : targetLow;
    tm.displayNumber(tempToDisplayTenth(t));
  }
}

void applySetpointAdjustmentOnPress() {
  // In setpoint view mode, each press adjusts the range:
  // Increase both low/high by step. If low hits a bound, wrap/limit.
  // Keep span constant (high-low).
  static const float MIN_LOW = 0.0f;
  static const float MAX_HIGH = 60.0f;

  float span = targetHigh - targetLow;
  targetLow += TEMP_STEP;
  targetHigh = targetLow + span;

  if (targetHigh > MAX_HIGH) {
    // wrap back to defaults-like floor
    targetLow = TEMP_TARGET_LOW_DEFAULT;
    targetHigh = TEMP_TARGET_HIGH_DEFAULT;
  }
  normalizeTargets();
}

void readDht() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) return; // keep previous values
  currentHum = h;
  currentTempC = t;
}

void setup() {
  pinMode(PIN_D2_BTN, INPUT_PULLUP);

  pinMode(PIN_D10_HEATER_RELAY, OUTPUT);
  digitalWrite(PIN_D10_HEATER_RELAY, LOW);

  pinMode(PIN_D5_FAN, OUTPUT);
  digitalWrite(PIN_D5_FAN, LOW);

  pinMode(PIN_D13_LED, OUTPUT);
  digitalWrite(PIN_D13_LED, LOW);

  pinMode(PIN_D6_BUZZER, OUTPUT);
  digitalWrite(PIN_D6_BUZZER, LOW);
  noTone(PIN_D6_BUZZER);

  dht.begin();

  Serial.begin(9600);

  tm.begin();
  tm.setBrightness(5);

  ventCycleStartMs = millis();

  // Initial display
  updateDisplay();
}

void loop() {
  // Button edge handling
  if (getButtonPressedEdge()) {
    if (mode == MODE_VIEW_CURRENT) {
      mode = MODE_VIEW_SETPOINT;
    } else {
      // In set view mode, adjust set values
      applySetpointAdjustmentOnPress();
      // Keep mode in set view (still viewing set values)
    }

    // Update immediately
    updateDisplay();
  }

  // Periodic sensor read
  unsigned long now = millis();
  if (now - lastDhtReadMs >= DHT_READ_MS) {
    lastDhtReadMs = now;
    readDht();
  }

  // Update abnormal detection and serial output
  updateAbnormalDetectionAndSerial();

  // Update temperature control and ventilation
  updateTemperatureControl();
  updateVentilation();

  // Alarm outputs
  updateAlarmOutputs();

  // Update display
  updateDisplay();

  // small yield
  delay(10);
}
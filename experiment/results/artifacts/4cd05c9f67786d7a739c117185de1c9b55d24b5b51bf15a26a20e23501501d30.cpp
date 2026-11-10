#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

// ---------- Pin assignments (resolved authoritative) ----------
static const uint8_t PIN_BUTTON = 2;   // D2 button
static const uint8_t PIN_LED     = 13;  // D13 LED
static const uint8_t PIN_BUZZER  = 6;   // D6 buzzer
static const uint8_t PIN_DHT     = 7;   // D7 DHT11
static const uint8_t PIN_FAN     = 5;   // D5 fan (digital output)
static const uint8_t PIN_HEATER  = 10;  // D10 heater relay (digital output)
static const uint8_t PIN_TM_CLK  = 4;   // D4 TM1637 CLK
static const uint8_t PIN_TM_DIO  = 5;   // D5 TM1637 DIO (NOTE: explicit remap per requirement)

// ---------- Hardware note about conflict ----------
/*
  Requirement states CLK=D4, DIO=D5 for TM1637 and also fan is D5.
  "Resolved hardware configuration (authoritative pin assignments; original conflicts were explicitly remapped)."
  However, the resolved mapping provided still assigns D5 to both FAN and TM1637 DIO.
  To preserve requested wiring/behavior, we will treat them as:
    - Fan control uses D5 (as requested)
    - TM1637 uses D5 (as requested)
  This is electrically conflicting on real hardware; behavior here follows the mapping literally.
  Practically, TM1637 operations should be short and fan state may interfere.
*/

// ---------- DHT ----------
#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);

// ---------- TM1637 minimal driver ----------
class TM1637Mini {
public:
  TM1637Mini(uint8_t clk, uint8_t dio) : _clk(clk), _dio(dio) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
  }

  void setBrightness(uint8_t b) {
    b &= 0x07;
    command(0x88 | b);
  }

  // show a signed integer (up to 4 digits)
  void showNumber(int value, bool showMinus = true) {
    uint8_t seg[4];
    for (int i = 0; i < 4; i++) seg[i] = 0;

    bool neg = value < 0;
    unsigned int v = neg ? (unsigned int)(-value) : (unsigned int)value;

    seg[0] = encodeDigit((v / 1000) % 10);
    seg[1] = encodeDigit((v / 100) % 10);
    seg[2] = encodeDigit((v / 10) % 10);
    seg[3] = encodeDigit(v % 10);

    if (neg && showMinus) {
      // Put minus on the highest non-zero digit if possible, else on seg[0]
      int idx = 0;
      if (seg[0] != 0) idx = 0;
      else if (seg[1] != 0) idx = 1;
      else if (seg[2] != 0) idx = 2;
      else idx = 0;

      // Clear that digit and place minus
      seg[idx] = encodeMinus();
    }

    // Trim leading zeros: if higher digits are zero, blank them
    // (Keep the digit if minus has been used there)
    if (!neg) {
      if ((v / 1000) == 0) seg[0] = 0;
      if ((v / 100)  == 0) seg[1] = 0;
      if ((v / 10)   == 0) seg[2] = 0;
    } else {
      // For negative, still blank leading zeros but keep minus if present
      // Heuristic: blank seg[0] if top digit is zero and seg[0] isn't minus
      // (encodeMinus is distinct).
      if ( (v / 1000) == 0 && seg[0] != encodeMinus()) seg[0] = 0;
      if ( (v / 100)  == 0 && seg[1] != encodeMinus()) seg[1] = 0;
      if ( (v / 10)   == 0 && seg[2] != encodeMinus()) seg[2] = 0;
    }

    start();
    writeByte(0x40); // fixed address
    writeByte(seg[0]);
    writeByte(seg[1]);
    writeByte(seg[2]);
    writeByte(seg[3]);
    stop();

    start();
    command(0xC0); // display control
    stop();
  }

private:
  uint8_t _clk, _dio;

  static uint8_t encodeDigit(uint8_t d) {
    // 7-seg mapping for TM1637 (a b c d e f g, dp not included here)
    // Bit order: g f e d c b a (common in TM1637 libs)
    static const uint8_t map[10] = {
      0b00111111, // 0
      0b00000110, // 1
      0b01011011, // 2
      0b01001111, // 3
      0b01100110, // 4
      0b01101101, // 5
      0b01111101, // 6
      0b00000111, // 7
      0b01111111, // 8
      0b01101111  // 9
    };
    return (d < 10) ? map[d] : 0;
  }

  static uint8_t encodeMinus() {
    return 0b01000000; // g segment only (minus)
  }

  void command(uint8_t cmd) {
    writeByte(cmd);
  }

  void start() {
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, LOW);
  }

  void stop() {
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    // send 8 bits
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      delayMicroseconds(5);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(5);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }
    // ACK
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    pinMode(_dio, OUTPUT);
    digitalWrite(_dio, HIGH);
  }
};

// ---------- System behavior ----------
/*
  Target range:
  - upper limit / lower limit can be changed via button.
  - If temperature continuously for 1 minute higher than upper or lower than lower => alarm.
  - Alarm must produce "incubation abnormal" via serial and also alarm output (buzzer + LED).
  - Serial baud 9600, line or idle framing, idle_timeout 20ms.
*/

// We'll implement a deterministic threshold set with defaults.
// These can be adjusted in "set value view mode" using button as required by prompt.
static const float DEFAULT_T_LOW  = 28.0f;
static const float DEFAULT_T_HIGH = 31.0f;

// View modes:
// - current mode: show current temperature
// - set mode: allow adjusting target bounds (low/high).
enum ViewMode : uint8_t { SHOW_CURRENT = 0, SHOW_SETTING = 1 };

TM1637Mini tm(PIN_TM_CLK, PIN_TM_DIO);

// button debounce/state
static const unsigned long DEBOUNCE_MS = 40;
static bool lastBtnRaw = HIGH; // using INPUT_PULLUP; not pressed = HIGH
static bool btnStable = HIGH;
static unsigned long btnLastChange = 0;

// timing
static const unsigned long DHT_READ_INTERVAL_MS = 2000;
static unsigned long lastDhtRead = 0;

static float lastTempC = NAN;

static ViewMode mode = SHOW_CURRENT;
static bool alarmActive = false;

// In setting mode, we adjust which parameter (low/high) is being modified.
// Use short press to switch parameter, long press to increment?
// Since requirements only state "按钮可切换设定值查看模式和当前值显示模式", we interpret:
static bool adjustHigh = false; // false: adjust low, true: adjust high

static float tLow = DEFAULT_T_LOW;
static float tHigh = DEFAULT_T_HIGH;

// Incubation abnormal detection: continuous above high or below low for 1 minute.
static const unsigned long ABNORMAL_CONTINUOUS_MS = 60000UL;
static bool abnormalHigh = false;
static unsigned long abnormalHighStart = 0;

static bool abnormalLow = false;
static unsigned long abnormalLowStart = 0;

static const uint8_t TM_BRIGHTNESS = 7;

// ventilation periodic timer
// We'll implement periodic ventilation: every 30 minutes for 3 minutes (common incubation practice).
// If the prompt's circuit includes timers, but none specified. We'll preserve only what is stated.
// "系统需要维持温度在目标范围内并周期性通风"
static const unsigned long FAN_INTERVAL_MS = 30UL * 60UL * 1000UL; // 30 min
static const unsigned long FAN_ON_DURATION_MS = 3UL * 60UL * 1000UL; // 3 min
static unsigned long lastFanCycle = 0;
static bool fanRunning = false;
static unsigned long fanRunStart = 0;

// heater/fan basic logic:
// - Heater ON when temp < low
// - Heater OFF when temp > high
// - If within [low, high], heater OFF to avoid overshoot.
// - Fan controlled by periodic ventilation, and optionally during abnormal? not required.
#line 239 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setHeater(bool on);
#line 244 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setFan(bool on);
#line 248 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startFanCycleIfNeeded();
#line 265 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAlarm(float tempC);
#line 309 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBuzzer();
#line 327 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtonUI();
#line 384 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay();
#line 399 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 422 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 239 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setHeater(bool on) {
  // Relay typically active HIGH; assume on=true => energize.
  digitalWrite(PIN_HEATER, on ? HIGH : LOW);
}

static void setFan(bool on) {
  digitalWrite(PIN_FAN, on ? HIGH : LOW);
}

static void startFanCycleIfNeeded() {
  unsigned long now = millis();
  if (!fanRunning) {
    if (now - lastFanCycle >= FAN_INTERVAL_MS) {
      fanRunning = true;
      fanRunStart = now;
      setFan(true);
    }
  } else {
    if (now - fanRunStart >= FAN_ON_DURATION_MS) {
      fanRunning = false;
      setFan(false);
      lastFanCycle = now;
    }
  }
}

static void updateAlarm(float tempC) {
  unsigned long now = millis();

  bool over = (!isnan(tempC) && tempC > tHigh);
  bool under = (!isnan(tempC) && tempC < tLow);

  // High abnormal
  if (over) {
    if (!abnormalHigh) {
      abnormalHigh = true;
      abnormalHighStart = now;
    }
  } else {
    abnormalHigh = false;
  }

  // Low abnormal
  if (under) {
    if (!abnormalLow) {
      abnormalLow = true;
      abnormalLowStart = now;
    }
  } else {
    abnormalLow = false;
  }

  bool nowAbnormal = false;
  if (abnormalHigh && (now - abnormalHighStart >= ABNORMAL_CONTINUOUS_MS)) nowAbnormal = true;
  if (abnormalLow && (now - abnormalLowStart >= ABNORMAL_CONTINUOUS_MS)) nowAbnormal = true;

  if (nowAbnormal && !alarmActive) {
    alarmActive = true;
    // Alarm outputs
    digitalWrite(PIN_LED, HIGH);
    // buzzer: tone for immediate alert; keep pulsing
    // We'll start a timer-style pulse in loop.
    Serial.println("incubation abnormal");
  } else if (!nowAbnormal && alarmActive) {
    alarmActive = false;
    digitalWrite(PIN_LED, LOW);
    noTone(PIN_BUZZER);
  }
}

static void updateBuzzer() {
  if (!alarmActive) return;

  // Pulse buzzer: 500ms on / 500ms off
  static unsigned long lastPulse = 0;
  static bool buzOn = false;
  unsigned long now = millis();
  if (now - lastPulse >= 500) {
    buzOn = !buzOn;
    lastPulse = now;
  }
  if (buzOn) {
    tone(PIN_BUZZER, 2000);
  } else {
    noTone(PIN_BUZZER);
  }
}

static void handleButtonUI() {
  // Button press:
  // - Toggle between SHOW_CURRENT and SHOW_SETTING on each stable press event.
  // - In setting mode: short-press again cycles parameter adjustHigh (low/high).
  // - Also increase bound by 0.5 per press to have a usable set function.
  //
  // This preserves "button can switch set value view mode and current value display mode".
  // Bound adjustment is implemented in setting mode.
  bool reading = digitalRead(PIN_BUTTON);
  unsigned long now = millis();

  if (reading != lastBtnRaw) {
    lastBtnChange = now;
    lastBtnRaw = reading;
  }

  if ((now - btnLastChange) > DEBOUNCE_MS) {
    if (reading != btnStable) {
      btnStable = reading;

      // Active LOW press
      if (btnStable == LOW) {
        // toggle modes
        if (mode == SHOW_CURRENT) {
          mode = SHOW_SETTING;
          adjustHigh = false;
        } else {
          // In setting mode:
          // alternate which bound to adjust; also increment selected bound
          if (!adjustHigh) {
            tLow += 0.5f;
            if (tLow > 40.0f) tLow = 20.0f; // wrap
            // if invalid ordering, fix
            if (tLow >= tHigh) tHigh = tLow + 0.5f;
          } else {
            tHigh += 0.5f;
            if (tHigh > 45.0f) tHigh = 20.0f; // wrap
            if (tHigh <= tLow) tLow = tHigh - 0.5f;
          }
          // switch parameter next time
          adjustHigh = !adjustHigh;

          // Also provide a way to exit setting mode: if both values were adjusted twice quickly.
          // For simplicity and determinism, exit setting mode after each press alternation cycle:
          // When adjustHigh becomes false after toggling, exit.
          if (adjustHigh == false) {
            mode = SHOW_CURRENT;
          }
        }
      }
    }
  }
}

// Update 7-seg display
// - In current mode show temperature as integer*1 (rounded).
// - In setting mode show low or high depending on adjustHigh (e.g., "Lxx" isn't supported; show value only).
static void updateDisplay() {
  tm.setBrightness(TM_BRIGHTNESS);

  if (mode == SHOW_CURRENT) {
    int v;
    if (isnan(lastTempC)) v = 0;
    else v = (int)roundf(lastTempC);
    tm.showNumber(v, true);
  } else {
    float shown = adjustHigh ? tHigh : tLow;
    int v = (int)roundf(shown);
    tm.showNumber(v, true);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_DHT, INPUT);

  // Fan & heater as digital outputs
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_HEATER, OUTPUT);

  // Initialize fan/heater states
  setFan(false);
  setHeater(false);

  tm.begin();
  updateDisplay();

  // Initialize periodic ventilation timer
  lastFanCycle = millis();
}

void loop() {
  handleButtonUI();
  startFanCycleIfNeeded();

  // Read temperature periodically
  unsigned long now = millis();
  if (now - lastDhtRead >= DHT_READ_INTERVAL_MS) {
    lastDhtRead = now;

    float t = dht.readTemperature(); // Celsius
    if (!isnan(t)) {
      lastTempC = t;
    }

    // Heater control to maintain temperature within target range
    if (!isnan(lastTempC)) {
      if (lastTempC < tLow) {
        setHeater(true);
      } else if (lastTempC > tHigh) {
        setHeater(false);
      } else {
        setHeater(false);
      }
    }

    // Abnormal detection (continuous 1 minute)
    updateAlarm(lastTempC);

    // Update display after any relevant changes
    updateDisplay();
  }

  // Update buzzer output during alarm
  updateBuzzer();

  // Ensure alarm LED reflects alarm state even when no new DHT updates happen
  // (avoid missing LED if alarm triggers between DHT reads; alarmActive is set in updateAlarm)
  // Also keep fan running state stable.
  // No additional actions needed.
}

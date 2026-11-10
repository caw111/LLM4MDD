#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

// ---------- Pin assignments (authoritative) ----------
static const uint8_t PIN_BUTTON = 2;   // D2 button
static const uint8_t PIN_LED     = 13;  // D13 LED
static const uint8_t PIN_BUZZER  = 6;   // D6 buzzer (tone output)
static const uint8_t PIN_DHT     = 7;   // D7 DHT11
static const uint8_t PIN_FAN     = 5;   // D5 fan (digital output; note shared with TM1637 DIO per provided mapping)
static const uint8_t PIN_HEATER  = 10;  // D10 heater relay (digital output)
static const uint8_t PIN_TM_CLK  = 4;   // D4 TM1637 CLK
static const uint8_t PIN_TM_DIO  = 5;   // D5 TM1637 DIO (explicit remap per requirement)

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

  void showNumber(int value, bool showMinus = true) {
    uint8_t seg[4] = {0, 0, 0, 0};

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

      seg[idx] = encodeMinus();
    }

    // Blank leading zeros (except keep minus if it was placed)
    if (!neg) {
      if ((v / 1000) == 0) seg[0] = 0;
      if ((v / 100)  == 0) seg[1] = 0;
      if ((v / 10)   == 0) seg[2] = 0;
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
    // TM1637 7-seg mapping (g f e d c b a)
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
    return 0b01000000; // minus on g
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

static TM1637Mini tm(PIN_TM_CLK, PIN_TM_DIO);

// ---------- System parameters / behavior ----------
static const float DEFAULT_T_LOW  = 28.0f;
static const float DEFAULT_T_HIGH = 31.0f;

enum ViewMode : uint8_t { SHOW_CURRENT = 0, SHOW_SETTING = 1 };

static ViewMode mode = SHOW_CURRENT;
static bool adjustHigh = false; // in setting mode: false=adjust low, true=adjust high

static float tLow = DEFAULT_T_LOW;
static float tHigh = DEFAULT_T_HIGH;

static float lastTempC = NAN;

// Alarm: continuous 1 minute outside bounds
static const unsigned long ABNORMAL_CONTINUOUS_MS = 60000UL;
static bool alarmActive = false;
static bool abnormalHigh = false;
static unsigned long abnormalHighStart = 0;
static bool abnormalLow = false;
static unsigned long abnormalLowStart = 0;

// Ventilation periodic timer
static const unsigned long FAN_INTERVAL_MS = 30UL * 60UL * 1000UL;     // 30 minutes
static const unsigned long FAN_ON_DURATION_MS = 3UL * 60UL * 1000UL;   // 3 minutes
static unsigned long lastFanCycle = 0;
static bool fanRunning = false;
static unsigned long fanRunStart = 0;

// Timing / sampling
static const unsigned long DHT_READ_INTERVAL_MS = 2000;
static unsigned long lastDhtRead = 0;

// Button debounce/state
static const unsigned long DEBOUNCE_MS = 40;
static bool lastBtnRaw = HIGH; // INPUT_PULLUP: not pressed = HIGH
static bool btnStable = HIGH;
static unsigned long lastBtnChange = 0; // declared in scope

#line 186 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void setHeater(bool on);
#line 190 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void setFan(bool on);
#line 194 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void startFanCycleIfNeeded();
#line 211 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void updateAlarm(float tempC);
#line 252 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void updateBuzzer();
#line 269 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void handleButtonUI();
#line 310 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void updateDisplay();
#line 323 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
void setup();
#line 349 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
void loop();
#line 186 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void setHeater(bool on) {
  digitalWrite(PIN_HEATER, on ? HIGH : LOW); // relay active level assumed HIGH
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

  bool over  = (!isnan(tempC) && tempC > tHigh);
  bool under = (!isnan(tempC) && tempC < tLow);

  if (over) {
    if (!abnormalHigh) {
      abnormalHigh = true;
      abnormalHighStart = now;
    }
  } else {
    abnormalHigh = false;
  }

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
  if (abnormalLow  && (now - abnormalLowStart  >= ABNORMAL_CONTINUOUS_MS)) nowAbnormal = true;

  if (nowAbnormal && !alarmActive) {
    alarmActive = true;
    digitalWrite(PIN_LED, HIGH);

    // Serial required output
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

  if (buzOn) tone(PIN_BUZZER, 2000);
  else noTone(PIN_BUZZER);
}

static void handleButtonUI() {
  bool reading = digitalRead(PIN_BUTTON);
  unsigned long now = millis();

  if (reading != lastBtnRaw) {
    lastBtnChange = now;
    lastBtnRaw = reading;
  }

  if ((now - lastBtnChange) > DEBOUNCE_MS) {
    if (reading != btnStable) {
      btnStable = reading;

      if (btnStable == LOW) { // active-low press
        if (mode == SHOW_CURRENT) {
          mode = SHOW_SETTING;
          adjustHigh = false;
        } else {
          // In setting mode: update bounds on each press
          if (!adjustHigh) {
            tLow += 0.5f;
            if (tLow > 40.0f) tLow = 20.0f;
            if (tLow >= tHigh) tHigh = tLow + 0.5f;
          } else {
            tHigh += 0.5f;
            if (tHigh > 45.0f) tHigh = 20.0f;
            if (tHigh <= tLow) tLow = tHigh - 0.5f;
          }

          adjustHigh = !adjustHigh;

          // Exit setting mode after toggling selection back
          if (adjustHigh == false) {
            mode = SHOW_CURRENT;
          }
        }
      }
    }
  }
}

static void updateDisplay() {
  tm.setBrightness(7);

  if (mode == SHOW_CURRENT) {
    int v = isnan(lastTempC) ? 0 : (int)roundf(lastTempC);
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

  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_HEATER, OUTPUT);

  setFan(false);
  setHeater(false);
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

  dht.begin();

  tm.begin();
  updateDisplay();

  lastFanCycle = millis();
}

void loop() {
  handleButtonUI();
  startFanCycleIfNeeded();

  unsigned long now = millis();
  if (now - lastDhtRead >= DHT_READ_INTERVAL_MS) {
    lastDhtRead = now;

    float t = dht.readTemperature(); // Celsius
    if (!isnan(t)) lastTempC = t;

    // Heater logic: keep within range [tLow, tHigh]
    if (!isnan(lastTempC)) {
      if (lastTempC < tLow) setHeater(true);
      else if (lastTempC > tHigh) setHeater(false);
      else setHeater(false);
    }

    updateAlarm(lastTempC);
    updateDisplay();
  }

  updateBuzzer();
}

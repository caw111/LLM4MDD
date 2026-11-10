#include <Arduino.h>
#include <Wire.h>

static const uint8_t PIN_D2 = 2;   // 光电计数传感器输入
static const uint8_t PIN_D3 = 3;   // 人工停线输入
static const uint8_t PIN_D4 = 4;   // TM1637 CLK
static const uint8_t PIN_D5 = 5;   // TM1637 DIO
static const uint8_t PIN_D6 = 6;   // 蜂鸣器 tone 输出
static const uint8_t PIN_D7 = 7;   // 切换显示输入
static const uint8_t PIN_D13 = 13; // LED 输出

// ---- Minimal TM1637 implementation (number-only, no leading zero) ----
class TM1637Minimal {
public:
  TM1637Minimal(uint8_t clkPin, uint8_t dioPin) : clkPin_(clkPin), dioPin_(dioPin) {}

  void begin() {
    pinMode(clkPin_, OUTPUT);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, HIGH);
    digitalWrite(dioPin_, HIGH);
  }

  void setBrightness(uint8_t b) {
    // b: 0..7
    b &= 0x07;
    start();
    writeByte(0x40); // command
    writeByte(0x80 | b);
    stop();
  }

  void displayNumber(int32_t num, bool leadingZero, bool blankOnInvalid = true) {
    if (num < 0) {
      // For negative numbers, original model doesn't specify sign; blank if invalid.
      if (blankOnInvalid) {
        clear();
      }
      return;
    }

    uint8_t seg[4] = {0, 0, 0, 0};

    // Convert to up to 4 digits
    uint16_t n = (uint16_t)num;
    bool started = false;
    for (int i = 3; i >= 0; --i) {
      uint8_t digit = (uint8_t)(n % 10);
      n /= 10;

      if (digit != 0 || started || leadingZero) {
        seg[i] = digitToSeg_(digit);
        started = true;
      } else {
        seg[i] = 0x00; // blank
      }
    }
    if (!started) {
      // if num==0 and leadingZero==false, model still expects to show 0 or blank?
      // "invalid_policy":"blank" only for invalid; for 0 we should show 0.
      if (!leadingZero) {
        seg[3] = digitToSeg_(0);
      }
    }

    // Write digits (TM1637 expects 0x00 address..)
    start();
    writeByte(0x40); // data command
    stop();

    start();
    writeByte(0xC0); // starting address
    for (int i = 0; i < 4; ++i) {
      writeByte(seg[i]);
    }
    stop();
  }

  void clear() {
    start();
    writeByte(0x40);
    stop();
    start();
    writeByte(0xC0);
    for (int i = 0; i < 4; ++i) writeByte(0x00);
    stop();
  }

private:
  uint8_t clkPin_, dioPin_;

  void start() {
    digitalWrite(dioPin_, HIGH);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, LOW);
  }

  void stop() {
    digitalWrite(clkPin_, LOW);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; ++i) {
      digitalWrite(clkPin_, LOW);
      delayMicroseconds(3);
      digitalWrite(dioPin_, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      delayMicroseconds(3);
      digitalWrite(clkPin_, HIGH);
      delayMicroseconds(3);
    }
    digitalWrite(clkPin_, LOW);
    delayMicroseconds(3);
    pinMode(dioPin_, INPUT);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(10);
    pinMode(dioPin_, OUTPUT);
  }

  uint8_t digitToSeg_(uint8_t d) {
    // Common cathode segment map; TM1637 typical
    static const uint8_t segMap[10] = {
      0x3F, // 0
      0x06, // 1
      0x5B, // 2
      0x4F, // 3
      0x66, // 4
      0x6D, // 5
      0x7D, // 6
      0x07, // 7
      0x7F, // 8
      0x6F  // 9
    };
    if (d < 10) return segMap[d];
    return 0x00;
  }
};

// ---- Controller state (Accepted executable model) ----
struct ControllerState {
  int32_t targetPerMinute = 60;
  int32_t productCountTotal = 0;
  int32_t productCountActual = 0;
  int32_t productCountAtLastToggle = 0; // kept though not used in outputs
  int32_t stopCount = 0;
  int32_t missedInactivityMs = 0;
  bool alarmLatched = false;
  bool prevProductPulse = false;
  bool prevStopButtonPressed = false;
  int32_t displayMode = 0; // 0: total, 1: achieved(real), 2: stopCount
};

static ControllerState s;

// Inputs sampled each 10ms
static bool io_1_productPulse = false;      // D2
static bool io_2_stopButton = false;        // D3
static bool io_3_toggleButton = false;      // D7
static int32_t io_0_elapsed_ms = 10;        // delta since previous step, in ms

// Outputs
static bool io_4_alarm = false;
static int32_t io_5_buzzerHz = 0;
static int32_t io_6_tm1637Value = 0;

// Serial "trigger" outputs when alarmLatched is true
static bool serial_0_trigger = false;

// ---- Timing ----
static unsigned long lastStepMicros = 0;

TM1637Minimal tm1637(PIN_D4, PIN_D5);

// ---- Arduino tone: use noTone when silent; model expects i32 Hz or 0 ----
static void applyBuzzer(int32_t hz) {
  if (hz > 0) {
    tone(PIN_D6, (unsigned int)hz);
  } else {
    noTone(PIN_D6);
  }
}

static void applyLED(bool on) {
  digitalWrite(PIN_D13, on ? HIGH : LOW);
}

static void computeAndApply() {
  // ===== Tick update sequence (preserve exact order) =====

  // 1) if io_3 and not prevStopButtonPressed:
  if (io_3_toggleButton && !s.prevStopButtonPressed) {
    if (s.displayMode == 2) s.displayMode = 0;
    else s.displayMode = s.displayMode + 1;
  }

  // 2) if io_2
  if (io_2_stopButton) {
    s.missedInactivityMs = 0;
    if (!s.alarmLatched) {
      s.alarmLatched = false; // stays false
    }
  }

  // 3) if not io_2
  if (!io_2_stopButton) {
    s.missedInactivityMs = s.missedInactivityMs + io_0_elapsed_ms;
    if (!s.alarmLatched && s.missedInactivityMs >= 30000) {
      s.alarmLatched = true;
      s.stopCount = s.stopCount + 1;
    }
  }

  // 4) if io_1 and not prevProductPulse
  if (io_1_productPulse && !s.prevProductPulse) {
    s.productCountTotal = s.productCountTotal + 1;
    s.productCountActual = s.productCountActual + 1;
    s.missedInactivityMs = 0;
    s.alarmLatched = false;
  }

  // 5) assign prevProductPulse := io_1
  s.prevProductPulse = io_1_productPulse;

  // 6) assign prevStopButtonPressed := io_2
  s.prevStopButtonPressed = io_2_stopButton;

  // 7) if io_3 and not prevStopButtonPressed:
  if (io_3_toggleButton && !s.prevStopButtonPressed) {
    s.productCountAtLastToggle = s.productCountTotal;
  }

  // ===== Outputs =====
  io_4_alarm = s.alarmLatched;
  io_5_buzzerHz = (s.alarmLatched ? 2000 : 0);

  if (s.displayMode == 0) {
    io_6_tm1637Value = s.productCountTotal;
  } else if (s.displayMode == 1) {
    // floor(productCountActual * 100.0 / targetPerMinute)
    if (s.targetPerMinute == 0) {
      io_6_tm1637Value = 0; // safe; though targetPerMinute initial 60
    } else {
      double v = (double)s.productCountActual * 100.0 / (double)s.targetPerMinute;
      io_6_tm1637Value = (int32_t)floor(v);
    }
  } else {
    io_6_tm1637Value = s.stopCount;
  }

  serial_0_trigger = s.alarmLatched;

  applyLED(io_4_alarm);
  applyBuzzer(io_5_buzzerHz);
  tm1637.displayNumber(io_6_tm1637Value, false, true);

  // Serial report: all value ports output under same trigger condition.
  // Original contract: "output" and trigger; no specified format. We'll print one line when triggered.
  if (serial_0_trigger) {
    // Values:
    // serial_0_value_0: productCountTotal
    // serial_0_value_1: productCountActual * 100.0 / targetPerMinute (0.0 if targetPerMinute==0)
    // serial_0_value_2: stopCount
    // serial_0_value_3: productCountActual
    // serial_0_value_4: alarmLatched
    double rate = (s.targetPerMinute == 0) ? 0.0 : (double)s.productCountActual * 100.0 / (double)s.targetPerMinute;

    Serial.print("ALARM ");
    Serial.print((int32_t)s.productCountTotal);
    Serial.print(' ');
    Serial.print(rate, 2);
    Serial.print(' ');
    Serial.print((int32_t)s.stopCount);
    Serial.print(' ');
    Serial.print((int32_t)s.productCountActual);
    Serial.print(' ');
    Serial.print(s.alarmLatched ? 1 : 0);
    Serial.print('\n');
  }
}

void setup() {
  pinMode(PIN_D2, INPUT);  // digital_input active_high, pullup false per contract
  pinMode(PIN_D3, INPUT);  // digital_input active_high, pullup false per contract
  pinMode(PIN_D7, INPUT);  // digital_input active_high, pullup false per contract

  pinMode(PIN_D13, OUTPUT);
  pinMode(PIN_D6, OUTPUT);

  // Ensure noTone initially
  noTone(PIN_D6);

  Serial.begin(9600);

  tm1637.begin();
  tm1637.setBrightness(7); // brightness=7, leading_zero=false
  tm1637.displayNumber(0, false, true);

  lastStepMicros = micros();
}

void loop() {
  // 10ms control period scheduling
  const uint32_t periodMs = 10;
  unsigned long nowMicros = micros();
  unsigned long elapsedMicros = nowMicros - lastStepMicros;

  // If not enough time, return; deterministic scheduling by waiting exact 10ms boundaries
  if (elapsedMicros < (unsigned long)periodMs * 1000UL) return;

  // Compute delta in ms as i32 (model interface requires elapsed_ms delta_since_previous_step).
  // To preserve deterministic behavior, quantize to 10ms granularity and use fixed 10.
  // (No requirement about exact drift correction; model assumes 10ms base.)
  lastStepMicros += (unsigned long)periodMs * 1000UL;
  io_0_elapsed_ms = (int32_t)periodMs;

  // Sample inputs (active high, pullup false)
  io_1_productPulse = (digitalRead(PIN_D2) == HIGH);
  io_2_stopButton = (digitalRead(PIN_D3) == HIGH);
  io_3_toggleButton = (digitalRead(PIN_D7) == HIGH);

  computeAndApply();
}
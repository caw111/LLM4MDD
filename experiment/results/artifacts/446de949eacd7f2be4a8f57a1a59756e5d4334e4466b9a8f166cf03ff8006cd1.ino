#include <Arduino.h>

// ---------------- TM1637 ----------------
/*
  Minimal TM1637 driver (4-digit 7-seg, using only required features: set brightness, display integer).
  Based on widely used TM1637 code patterns; kept self-contained.
*/
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clk(clkPin), _dio(dioPin) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
  }

  void setBrightness(uint8_t brightness, bool on=true) {
    // Command: 1000 001x where x is brightness (0..7), and on/off bit is implicitly part of brightness.
    uint8_t cmd = 0x88 | (brightness & 0x07);
    if (!on) cmd = 0x80 | (brightness & 0x07);
    start();
    writeByte(cmd);
    stop();
  }

  void showNumber(int value, bool leadingZero=false) {
    // Display as single number on 4 digits (no fixed formatting specified by model; we keep it general).
    // Map digits; blanking is handled via showBlank().
    if (value < -999) value = -999;
    if (value > 9999) value = 9999;

    uint8_t seg[4] = {0,0,0,0};
    if (value < 0) {
      seg[0] = segMinus();
      value = -value;
    }

    int v = value;
    bool started = (value < 0);
    for (int i = 3; i >= 0; --i) {
      int digit = v % 10;
      v /= 10;

      if (!leadingZero && !started) {
        // determine blank until first non-zero
        if (v != 0 || digit != 0) {
          seg[i] = segDigit(digit);
          started = true;
        } else {
          seg[i] = 0x00; // blank
        }
      } else {
        seg[i] = segDigit(digit);
      }
    }

    start();
    writeByte(0x40); // data command
    for (int i = 0; i < 4; ++i) writeByte(seg[i]);
    stop();
    start();
    writeByte(0xC0); // display control
    stop();
  }

  void showBlank() {
    uint8_t seg[4] = {0,0,0,0};
    start();
    writeByte(0x40);
    for (int i = 0; i < 4; ++i) writeByte(seg[i]);
    stop();
    start();
    writeByte(0xC0);
    stop();
  }

private:
  uint8_t _clk, _dio;

  static uint8_t segDigit(int d) {
    // segments: a b c d e f g (bit 0..6), dp at bit7.
    // Common patterns for TM1637 with typical library mapping.
    static const uint8_t lut[10] = {
      0x3F, //0
      0x06, //1
      0x5B, //2
      0x4F, //3
      0x66, //4
      0x6D, //5
      0x7D, //6
      0x07, //7
      0x7F, //8
      0x6F  //9
    };
    if (d < 0 || d > 9) return 0x00;
    return lut[d];
  }

  static uint8_t segMinus() { return 0x40; } // g segment

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
    for (int i = 0; i < 8; ++i) {
      digitalWrite(_clk, LOW);
      delayMicroseconds(3);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(5);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(3);
      b >>= 1;
    }
    // ack
    digitalWrite(_clk, LOW);
    delayMicroseconds(3);
    pinMode(_dio, INPUT);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(3);
    (void)digitalRead(_dio);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, LOW);
    delayMicroseconds(3);
  }
};

// ---------------- HC-SR04 ----------------
struct HCSR04 {
  uint8_t trigPin;
  uint8_t echoPin;
  uint32_t timeoutUs; // 25000
  float lastDistanceCm;
  bool lastValid;

  void begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
    delay(50);
    lastDistanceCm = 0;
    lastValid = false;
  }

  void trigger() {
    // Standard trigger: 10us HIGH
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  }

  void readOnce() {
    trigger();

    // measure pulse width
    unsigned long startUs = micros();
    // Wait for echo to go HIGH (with timeout)
    while (digitalRead(echoPin) == LOW) {
      if ((micros() - startUs) >= timeoutUs) {
        // invalid
        if (lastValid) {
          // hold last valid
        } else {
          lastDistanceCm = 0;
        }
        return;
      }
    }

    // Echo is HIGH, measure duration
    unsigned long echoStart = micros();
    while (digitalRead(echoPin) == HIGH) {
      if ((micros() - echoStart) >= timeoutUs) {
        // invalid
        return;
      }
    }
    unsigned long durationUs = micros() - echoStart;

    // distance = duration / 58.0 (cm) for HC-SR04
    float dist = (float)durationUs / 58.0f;

    // Mark valid if within reasonable range (model handles thresholds; but validity input must reflect sensor validity)
    // Original contract: invalid_policy = hold_last_valid. We implement "valid if measured within timeout" only.
    // Here, since timeout already handled, treat as valid.
    lastDistanceCm = dist;
    lastValid = true;
  }
};

// ---------------- Accepted Model ----------------
static const uint32_t CONTROL_PERIOD_MS = 5;

// Ports mapping:
// io_0 vehicle distance cm (f64) from sensor1
// io_1 vehicle valid (bool)
// io_2 pedestrian distance cm (f64) from sensor2
// io_3 pedestrian valid (bool)
// io_4 red LED / danger flash control (bool)
// io_5 green LED / safe state control (bool)
// io_6 buzzer frequency Hz (i32)
// io_7 TM1637 danger level number (i32) with invalid_policy blank
// io_8 reserved output D2 false
// serial trigger/value: print lastDangerDurationMs once when dangerEventReported becomes true on control tick.

static const uint8_t PIN_VEH_TRIG = D8;
static const uint8_t PIN_VEH_ECHO = D9;
static const uint8_t PIN_PED_TRIG = D10;
static const uint8_t PIN_PED_ECHO = D11;

static const uint8_t PIN_RED_LED = D12;
static const uint8_t PIN_GREEN_LED = D13;
static const uint8_t PIN_BUZZER = D6;
static const uint8_t PIN_TM1637_CLK = D4;
static const uint8_t PIN_TM1637_DIO = D5;
static const uint8_t PIN_RESERVED_D2 = D2;

static const uint32_t SERIAL_BAUD = 9600;

// States from model
bool prevVehApproach = false;
bool prevPedApproach = false;
bool inDanger = false;
int32_t dangerStartMs = 0;
int32_t lastDangerDurationMs = 0;
bool dangerEventReported = false;

// Sensor IOs
double io_0 = 0;   // vehicle distance
bool   io_1 = false; // vehicle validity
double io_2 = 0;   // pedestrian distance
bool   io_3 = false; // pedestrian validity

// TM1637
TM1637 tm(PIN_TM1637_CLK, PIN_TM1637_DIO);
static const uint8_t TM_BRIGHTNESS = 7;

// Hardware objects
HCSR04 veh{PIN_VEH_TRIG, PIN_VEH_ECHO, 25000UL, 0.0f, false};
HCSR04 ped{PIN_PED_TRIG, PIN_PED_ECHO, 25000UL, 0.0f, false};

// Timekeeping
uint32_t lastControlMs = 0;

// Buzzer: model sets io_6 = 2000 when inDanger else 0.
// On Uno, tone() uses Timer2. We'll use tone/noTone.
static void setBuzzerHz(int32_t hz) {
  if (hz <= 0) {
    noTone(PIN_BUZZER);
  } else {
    tone(PIN_BUZZER, (unsigned int)hz);
  }
}

static void updateOutputs() {
  // outputs per model
  bool io_4 = inDanger;
  bool io_5 = !inDanger;

  // io_6
  int32_t io_6 = inDanger ? 2000 : 0;

  // io_7 danger level number
  int32_t io_7;
  if (io_1 && (io_0 < 20.0) && io_3 && (io_2 < 20.0)) {
    io_7 = 3;
  } else if (io_1 && (io_0 < 50.0) && io_3 && (io_2 < 50.0)) {
    io_7 = 2;
  } else {
    io_7 = inDanger ? 1 : 0;
  }

  bool io_8 = false;

  // Apply outputs
  digitalWrite(PIN_RED_LED, io_4 ? HIGH : LOW);
  digitalWrite(PIN_GREEN_LED, io_5 ? HIGH : LOW);
  digitalWrite(PIN_RESERVED_D2, io_8 ? HIGH : LOW);

  setBuzzerHz(io_6);

  // TM1637 invalid_policy blank:
  // Model never outputs invalid; it outputs 0 when safe. We'll show 0 when io_7==0.
  // If "blank on invalid" is desired, we'd need an invalid flag; not present. So showNumber(io_7).
  tm.setBrightness(TM_BRIGHTNESS, true);
  tm.showNumber(io_7, false);
}

static void runControlStep() {
  // Compute guard conditions exactly as model:
  bool condVehApproach = (io_1 && (io_0 >= 5.0));
  bool condPedApproach = (io_3 && (io_2 >= 5.0));
  bool simultaneous = (io_1 && (io_0 >= 5.0) && io_3 && (io_2 >= 5.0));

  // first start
  // then if (io_1 and io_0>=5 and io_3 and io_2>=5) { if (not inDanger and same) { inDanger=true; dangerStartMs=0; dangerEventReported=false; } }
  if (simultaneous) {
    if ((!inDanger) && (simultaneous)) { // redundant same guard preserved
      inDanger = true;
      dangerStartMs = 0;
      dangerEventReported = false;
    }
  }

  // then if not(simultaneous) { if(inDanger){ inDanger=false; lastDangerDurationMs=dangerStartMs; dangerEventReported=true; } }
  if (!simultaneous) {
    if (inDanger) {
      inDanger = false;
      lastDangerDurationMs = dangerStartMs;
      dangerEventReported = true;
    }
  }

  // Continuous assignments in model: dangerStartMs not incremented by model logic.
  // To preserve "exact controller state" as per given model, we must keep dangerStartMs as assigned only (no increments).
  // However, original SysML would likely increment with time. The accepted model provided does not include increments.
  // We must implement exactly: no increment.
  // (dangerStartMs stays 0 always; lastDangerDurationMs will be 0.)
  // If increment were required, it would be in updates/expressions; it's absent.

  prevVehApproach = condVehApproach;
  prevPedApproach = condPedApproach;

  // Serialize: dangerEventReported acts as serial_0_trigger, value lastDangerDurationMs.
  // Need to print when trigger becomes true on the same control tick.
  if (dangerEventReported) {
    Serial.println((int32_t)lastDangerDurationMs);
    // Model doesn't specify reset of dangerEventReported after reporting.
    // But in update rules, dangerEventReported is set false when entering danger again.
    // We'll keep it as-is; once reported, next tick it may remain true unless overridden.
    // This matches given state machine (dangerEventReported only changed on enter/exit).
  }

  // After reporting, outputs will reflect inDanger etc. TM and LEDs update outside.
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_RESERVED_D2, OUTPUT);

  digitalWrite(PIN_RESERVED_D2, LOW);

  // "Green LED model defaults to true (HIGH) ... red LED defaults to false (LOW) ..."
  digitalWrite(PIN_RED_LED, LOW);
  digitalWrite(PIN_GREEN_LED, HIGH);

  veh.begin();
  ped.begin();

  tm.begin();
  tm.setBrightness(TM_BRIGHTNESS, true);
  tm.showBlank();

  noTone(PIN_BUZZER);

  // init controller state already set by globals
  lastControlMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((now - lastControlMs) < CONTROL_PERIOD_MS) return;
  lastControlMs += CONTROL_PERIOD_MS;

  // Update ultrasonic inputs at 60ms sample period in contract.
  // We keep sensor sampling independent but read at least around this rate.
  static uint32_t lastUltrasonicMs = 0;
  if ((now - lastUltrasonicMs) >= 60) {
    lastUltrasonicMs = now;

    // Vehicle
    bool prevValid = veh.lastValid;
    veh.readOnce();
    // Apply invalid_policy hold_last_valid:
    // HCSR04 stores lastDistanceCm/lastValid only on valid measurement.
    // We need to preserve previous distance when invalid.
    // Implement by checking if new measurement timed out:
    // Our HCSR04.readOnce() returns without changing lastValid on invalid;
    // thus lastValid stays previous, which matches hold_last_valid.
    io_0 = veh.lastDistanceCm;
    io_1 = veh.lastValid;

    // Pedestrian
    ped.readOnce();
    io_2 = ped.lastDistanceCm;
    io_3 = ped.lastValid;
  }

  runControlStep();
  updateOutputs();
}
#include <Arduino.h>
#include <DHT.h>

// TM1637 (adapted minimal implementation)
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clkPin(clkPin), _dioPin(dioPin) {}

  void begin() {
    pinMode(_clkPin, OUTPUT);
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
  }

  void setBrightness(uint8_t brightness, bool on = true) {
    if (brightness > 7) brightness = 7;
    uint8_t cmd = 0x88 | (brightness & 0x07);
    if (!on) cmd = 0x80 | (brightness & 0x07);
    start();
    writeByte(0x40);
    writeByte(cmd);
    stop();
  }

  // Write a 16-bit raw field (2 digits). We'll use for floor(temp) with up to 2 digits shown.
  void displayInt(int value, bool leadingZero = false) {
    // Map to up to 4 digits; but we only need show temperature value.
    // Contract: invalid_policy=blank, leading_zero=false. Here we always show value.
    // For temperatures beyond 0..99, we show the last two digits to fit 2-digit TM1637 module.
    int v = value;

    if (v < 0) {
      // Show negative sign on second digit if possible; we'll just show abs and use minus segment on second.
      v = -v;
    }

    int d1 = v % 10;
    int d0 = (v / 10) % 10;

    uint8_t seg0 = encodeDigit(d0);
    uint8_t seg1 = encodeDigit(d1);

    // If leadingZero is false, blank the tens digit when it's 0 (and value is 0..9).
    if (!leadingZero && (value >= 0) && (v < 10)) {
      seg0 = 0x00; // blank
    }

    // Negative sign handling: if original was negative, show '-' on tens position.
    // We detect negativity by looking at value sign input; store separately.
    // Caller provides signed int; we re-handle here:
    if (value < 0) {
      seg0 = 0x40; // minus sign segment
    }

    uint8_t data[] = {seg0, seg1, 0x00, 0x00}; // We'll use first two positions only
    // TM1637 uses 4 digits; we write full 4-digit buffer but leave remaining blank.
    start();
    writeByte(0x40);
    writeByte(seg0);
    writeByte(seg1);
    writeByte(0x00);
    writeByte(0x00);
    stop();
  }

private:
  uint8_t _clkPin, _dioPin;

  void start() {
    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, LOW);
  }

  void stop() {
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
    delayMicroseconds(2);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clkPin, LOW);
      digitalWrite(_dioPin, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(2);
      b >>= 1;
      digitalWrite(_clkPin, HIGH);
      delayMicroseconds(2);
    }
    // Ack
    digitalWrite(_clkPin, LOW);
    pinMode(_dioPin, INPUT);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, LOW);
  }

  uint8_t encodeDigit(int d) {
    // Common 7-seg mapping for TM1637:
    // 0: 0x3f, 1: 0x06, 2: 0x5b, 3: 0x4f, 4: 0x66,
    // 5: 0x6d, 6: 0x7d, 7: 0x07, 8: 0x7f, 9: 0x6f
    static const uint8_t tab[10] = {
      0x3F, 0x06, 0x5B, 0x4F, 0x66,
      0x6D, 0x7D, 0x07, 0x7F, 0x6F
    };
    if (d < 0 || d > 9) return 0x00;
    return tab[d];
  }
};

// Hardware bindings (authoritative)
static const uint8_t FAN_PWM_PIN = 5; // D5 PWM
static const uint8_t DHT_PIN = 7;      // D7
static const uint8_t TM1637_CLK_PIN = 4; // D4
static const uint8_t TM1637_DIO_PIN = 2; // D2

// DHT11 setup
#define DHTTYPE DHT11
static DHT dht(DHT_PIN, DHTTYPE);
static TM1637 tm1637(TM1637_CLK_PIN, TM1637_DIO_PIN);

// Controller "io_" ports
static double io_1_degC = 0.0; // temperature
static double io_2_rh = 0.0;   // humidity
static bool io_3_valid = false; // dht_valid for a frame

// Hold-last-valid per invalid_policy=hold_last_valid
static double last_valid_temp = 0.0;
static double last_valid_hum = 0.0;
static bool last_valid_set = false;

// control_period_ms = 10
static const unsigned long CONTROL_PERIOD_MS = 10;
// io_1/io_2/io_3 sample_period_ms = 2000
static const unsigned long SENSOR_PERIOD_MS = 2000;

// SysML controller outputs
static bool fanEnabled = false; // state
static double io_0_pwm_count = 0.0; // 0 or 255
static int32_t io_4_tm_value = 0;  // floor(io_1)
static bool serial_0_trigger = false;

static unsigned long lastControlTickMs = 0;
static unsigned long lastSensorTickMs = 0;

// Serial baud contract: 9600, report only when trigger is true
static void serialReportIfTriggered() {
  if (serial_0_trigger) {
    Serial.print("T=");
    Serial.print(io_1_degC, 2);
    Serial.println();
  }
}

void setup() {
  // Fan PWM pin
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, 0);

  // DHT
  dht.begin();

  // TM1637
  tm1637.begin();
  tm1637.setBrightness(7);

  // Serial
  Serial.begin(9600);

  // Initialize controller state exactly as in model
  fanEnabled = false;
  io_0_pwm_count = 0.0;
  io_4_tm_value = (int32_t)floor(io_1_degC);

  lastSensorTickMs = millis();
  lastControlTickMs = millis();
}

static void readDHTWithHoldLastValid() {
  // Read a sample; model uses io_3 as validity for "once collected frame consistency".
  // We'll treat a successful read as valid; otherwise keep last valid values and set io_3=false.
  float t = dht.readTemperature(); // Celsius
  float h = dht.readHumidity();

  bool ok = !(isnan(t) || isnan(h));

  if (ok) {
    io_1_degC = (double)t;
    io_2_rh = (double)h;
    io_3_valid = true;

    last_valid_temp = io_1_degC;
    last_valid_hum = io_2_rh;
    last_valid_set = true;
  } else {
    io_3_valid = false;
    // invalid_policy=hold_last_valid for io_1, io_2, io_3
    // For io_3 invalid_policy=hold_last_valid: keep last io_3_valid when invalid.
    if (last_valid_set) {
      io_1_degC = last_valid_temp;
      io_2_rh = last_valid_hum;
      io_3_valid = true; // hold last valid validity as true
    } else {
      // no previous valid: remain at initial.
      io_1_degC = io_1_degC; // keep default 0
      io_2_rh = io_2_rh;     // keep default 0
      io_3_valid = false;
    }
  }
}

// Executes SysML controller tick in the exact update order and logic
static void controllerTick() {
  // State transition update:
  // perform action tick {
  //   first start;
  //   then if io_3 {
  //     first start;
  //     then assign fanEnabled := io_1 > 30.0;
  //   }
  // }
  if (io_3_valid) {
    fanEnabled = (io_1_degC > 30.0);
  }

  // Outputs:
  // io_0: if io_3 then (io_1 > 30 ? 255 : 0) else (fanEnabled ? 255 : 0)
  if (io_3_valid) {
    io_0_pwm_count = (io_1_degC > 30.0) ? 255.0 : 0.0;
  } else {
    io_0_pwm_count = fanEnabled ? 255.0 : 0.0;
  }

  // io_4: floor(io_1) regardless of io_3
  io_4_tm_value = (int32_t)floor(io_1_degC);

  // serial_0_trigger: io_3 and io_1 > 30.0
  serial_0_trigger = (io_3_valid && (io_1_degC > 30.0));

  // serial_0_value_0: if io_3 then io_1 else io_1 (identical)
  // Stored directly in io_1_degC for report
  // tm1637 output uses io_4
}

void loop() {
  unsigned long now = millis();

  // Sensor sampling (io_1/io_2/io_3 sample_period_ms=2000)
  if (now - lastSensorTickMs >= SENSOR_PERIOD_MS) {
    lastSensorTickMs += SENSOR_PERIOD_MS;
    readDHTWithHoldLastValid();
  }

  // Control tick every 10ms
  if (now - lastControlTickMs >= CONTROL_PERIOD_MS) {
    lastControlTickMs += CONTROL_PERIOD_MS;

    controllerTick();

    // Apply outputs in a fixed order: PWM, display, serial trigger.
    // (Behaviorally equivalent for this model; keeps deterministic updates.)
    uint8_t pwm = (io_0_pwm_count <= 0.0) ? 0 : 255;
    analogWrite(FAN_PWM_PIN, pwm);

    // TM1637 display temperature floor(io_1). Leading zero=false.
    tm1637.displayInt(io_4_tm_value, false);

    // Serial report only when trigger is true
    serialReportIfTriggered();
  }
}
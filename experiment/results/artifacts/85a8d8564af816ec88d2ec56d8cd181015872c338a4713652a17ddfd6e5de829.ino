#include <Arduino.h>
#include <stdint.h>

static const uint8_t PIN_TRIG = D8; // Arduino Uno digital pin 8
static const uint8_t PIN_ECHO = D2; // Arduino Uno digital pin 2

static const uint8_t PIN_TM1637_CLK = D4; // Arduino Uno digital pin 4
static const uint8_t PIN_TM1637_DIO = D5; // Arduino Uno digital pin 5

static const uint8_t PIN_LED_NEAR = D13; // near-distance indicator
static const uint8_t PIN_LED_PWM = D9;   // PWM LED

// ---------- TM1637 (minimal driver sufficient for number output) ----------
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : clk(clkPin), dio(dioPin) {}

  void begin() {
    pinMode(clk, OUTPUT);
    pinMode(dio, OUTPUT);
    digitalWrite(clk, HIGH);
    digitalWrite(dio, HIGH);
  }

  // brightness: 0..7 (matches requirement brightness=7)
  void setBrightness(uint8_t b) {
    // Command: 0x88 | (b & 0x07)
    start();
    writeByte(0x40); // command
    writeByte(0x88 | (b & 0x07));
    stop();
  }

  // invalid_policy = blank -> when invalid, clear display.
  void clear() {
    start();
    writeByte(0x40);
    writeByte(0x00);
    stop();
    start();
    writeByte(0xC0);
    writeByte(0x00);
    writeByte(0x00);
    writeByte(0x00);
    writeByte(0x00);
    stop();
  }

  // Display a non-negative integer (no leading zero). Floor already done by model.
  void displayNumber(int value, bool leadingZero = false) {
    if (value < 0) {
      clear();
      return;
    }

    // 4 digits, right aligned
    uint8_t segs[4] = {0, 0, 0, 0};

    int v = value;
    for (int i = 3; i >= 0; --i) {
      int digit = v % 10;
      v /= 10;
      if (!leadingZero) {
        // Suppress leading zeros
        // Find first non-zero digit from left
        if (v == 0 && digit == 0 && i != 3) {
          // We don't know if later digits will require zeros; for this loop it's safe:
          // because it's left side suppression; rightmost zero kept if value has zeros.
        }
      }
      segs[i] = digitToSeg((uint8_t)digit);
    }

    // Apply leading zero suppression precisely (leading_zero=false):
    if (!leadingZero) {
      bool seenNonZero = false;
      for (int i = 0; i < 3; ++i) { // leftmost 3 digits can be blanked
        int digit = (value / (int)pow10u(3 - i)) % 10;
        if (digit == 0 && !seenNonZero) {
          segs[i] = 0x00; // blank
        } else {
          seenNonZero = true;
        }
      }
    }

    // Write data
    start();
    writeByte(0x40); // command
    writeByte(0x00); // start address = 0
    stop();

    start();
    writeByte(0xC0); // auto increment
    writeByte(segs[0]);
    writeByte(segs[1]);
    writeByte(segs[2]);
    writeByte(segs[3]);
    stop();
  }

private:
  uint8_t clk;
  uint8_t dio;

  static uint8_t digitToSeg(uint8_t digit) {
    // Common segment mapping for TM1637:
    // bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g, bit7=dp
    static const uint8_t map[10] = {
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
    if (digit > 9) return 0x00;
    return map[digit];
  }

  static uint32_t pow10u(uint8_t e) {
    uint32_t r = 1;
    for (uint8_t i = 0; i < e; ++i) r *= 10;
    return r;
  }

  void start() {
    digitalWrite(dio, HIGH);
    digitalWrite(clk, HIGH);
    digitalWrite(dio, LOW);
    digitalWrite(clk, LOW);
  }
  void stop() {
    digitalWrite(clk, LOW);
    digitalWrite(dio, LOW);
    digitalWrite(clk, HIGH);
    digitalWrite(dio, HIGH);
  }
  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(clk, LOW);
      digitalWrite(dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      digitalWrite(clk, HIGH);
    }
    // ack
    digitalWrite(clk, LOW);
    pinMode(dio, INPUT);
    digitalWrite(clk, HIGH);
    (void)digitalRead(dio);
    pinMode(dio, OUTPUT);
    digitalWrite(clk, LOW);
  }
};

// ---------- Model / frozen controller I/O ----------
static float distance_cm_last_valid = 0.0f;
static float distance_cm_effective = 0.0f;

static int io_2_distance_number = 0;
static bool io_3_near_led = false;
static float io_4_pwm_count = 0.0f;

// Ultrasonic acquisition provides io_0 and io_1 (valid)
static float io_0_distance_cm = 0.0f;
static bool io_1_valid = false;

static const uint32_t ultrasonic_timeout_us = 30000UL;

// Convert echo duration to distance in cm for HC-SR04:
// distance(cm) = duration_us / 58.0
static float durationToCm(uint32_t duration_us) {
  return (float)duration_us / 58.0f;
}

static bool readUltrasonicCm(float &out_cm) {
  // Trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure echo high pulse width
  // Timeout must be within supported range (given).
  uint32_t duration = pulseIn(PIN_ECHO, HIGH, ultrasonic_timeout_us);

  if (duration == 0) {
    // invalid
    return false;
  }

  out_cm = durationToCm(duration);
  return true;
}

// ---------- Controller evaluation (exact update ordering) ----------
static void controllerTick(bool newValid, float sampleDistanceCm) {
  // updates:
  // tick:
  // start
  // if io_1 then distance_cm_last_valid := io_0
  if (newValid) {
    distance_cm_last_valid = sampleDistanceCm;
  }

  // then if io_1 then distance_cm_effective := io_0 else distance_cm_effective := last_valid
  if (newValid) {
    distance_cm_effective = sampleDistanceCm;
  } else {
    distance_cm_effective = distance_cm_last_valid;
  }

  // outputs:
  // io_2:
  // if io_1 and distance_cm_effective>=2 and <=400 then floor(distance_cm_effective) else 0
  if (newValid && (distance_cm_effective >= 2.0f) && (distance_cm_effective <= 400.0f)) {
    io_2_distance_number = (int)floor(distance_cm_effective);
  } else {
    io_2_distance_number = 0;
  }

  // io_3:
  // io_3 = io_1 and distance_cm_effective < 20
  io_3_near_led = (newValid && (distance_cm_effective < 20.0f));

  // io_4:
  // if io_1 then
  //   if distance_cm_effective <=10 -> 1023
  //   else if distance_cm_effective >=100 -> 0
  //   else if distance_cm_effective >10 -> floor(((100 - d)*1023/90))
  //   else 1023
  // else 0.0
  if (newValid) {
    if (distance_cm_effective <= 10.0f) {
      io_4_pwm_count = 1023.0f;
    } else if (distance_cm_effective >= 100.0f) {
      io_4_pwm_count = 0.0f;
    } else if (distance_cm_effective > 10.0f) {
      io_4_pwm_count = (float)floor(((100.0f - distance_cm_effective) * 1023.0f / 90.0f));
    } else {
      // Should be unreachable due to prior conditions, but kept for exact semantics.
      io_4_pwm_count = 1023.0f;
    }
  } else {
    io_4_pwm_count = 0.0f;
  }
}

static void applyOutputs() {
  // io_3 near LED D13 active_high
  digitalWrite(PIN_LED_NEAR, io_3_near_led ? HIGH : LOW);

  // io_4 PWM LED D9: pwm_count in 0..1023.
  // Use analogWrite range 0..255; scale accordingly.
  // Keep exact behavior as best as possible: pwm_count floats might be 1023.0
  uint16_t count = 0;
  if (io_4_pwm_count < 0.0f) count = 0;
  else if (io_4_pwm_count > 1023.0f) count = 1023;
  else count = (uint16_t)io_4_pwm_count;

  uint8_t pwm255 = (uint8_t)((count * 255UL + 511UL) / 1023UL);
  analogWrite(PIN_LED_PWM, pwm255);

  // io_2 TM1637 number with invalid_policy=blank
  // "invalid_policy is set to 'blank'; when ultrasonic_valid becomes false, model should hold prior distance and drive the validity behavior accordingly."
  // Our model already forces io_2 to 0 when io_1=false; to match 'blank', treat io_1=false as clearing.
  static bool tm1637_initialized = false;
  (void)tm1637_initialized;
}

// ---------- Main ----------
static TM1637 tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

void setup() {
  // Hardware init
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  pinMode(PIN_LED_NEAR, OUTPUT);
  digitalWrite(PIN_LED_NEAR, LOW);

  pinMode(PIN_LED_PWM, OUTPUT);
  analogWrite(PIN_LED_PWM, 0);

  tm1637.begin();
  tm1637.setBrightness(7); // brightness=7 per contract

  // initial controller state already set to 0.0 by globals
  tm1637.clear();
}

void loop() {
  static uint32_t lastTickMs = 0;
  const uint32_t control_period_ms = 5;

  uint32_t now = millis();
  if (now - lastTickMs < control_period_ms) return;
  lastTickMs = now;

  // Acquire ultrasonic: io_0 (cm), io_1 (valid)
  float dcm = 0.0f;
  bool valid = readUltrasonicCm(dcm);
  io_0_distance_cm = dcm;
  io_1_valid = valid;

  // Tick controller with exact ordering and semantics
  controllerTick(io_1_valid, io_0_distance_cm);

  // Apply outputs in deterministic manner
  // TM1637 invalid_policy=blank:
  if (io_1_valid) {
    tm1637.displayNumber(io_2_distance_number, false /* leading_zero */);
  } else {
    tm1637.clear();
  }

  applyOutputs();
}
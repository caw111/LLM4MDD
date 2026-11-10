#include <Arduino.h>
#include <stdint.h>

// ===================== Hardware pins (frozen contract) =====================
static const uint8_t PIN_LED   = 13; // D13, HIGH=ON
static const uint8_t PIN_TRIG  = 8;  // HC-SR04 TRIG
static const uint8_t PIN_ECHO  = 9;  // HC-SR04 ECHO
static const uint8_t PIN_TM_CLK = 4; // TM1637 CLK
static const uint8_t PIN_TM_DIO = 5; // TM1637 DIO

// ===================== Timing constraints (frozen contract) =====================
static const uint32_t ULTRASONIC_TIMEOUT_US = 25000UL; // each wait must have 25000us timeout

// Sampling start interval must be within 60..160ms.
static const uint32_t SAMPLE_START_MIN_MS = 60;
static const uint32_t SAMPLE_START_MAX_MS = 160;

// Update responsiveness: within 250ms (including measurement waiting + display transfer).
static const uint32_t LOOP_TICK_MS = 10; // controller planning period (interface contract)

// ===================== TM1637 minimal driver =====================
// Based on standard TM1637 command set.
class TM1637Mini {
public:
  TM1637Mini(uint8_t clk, uint8_t dio) : _clk(clk), _dio(dio) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    _brightness = 7;
    clearDisplay();
  }

  void setBrightness(uint8_t b) {
    _brightness = b & 0x07;
  }

  // Display "blank" request: show empty (no segments).
  // Contract says: initial is set to integer 10000 to request startup blank.
  // Here we treat 10000 as blank marker.
  void showIntegerOrBlank(int32_t value, bool treat10000AsBlank = true) {
    if (treat10000AsBlank && value == 10000) {
      clearDisplay();
      return;
    }

    // TM1637 4 digits.
    // We will show floor(value) clamped to reasonable range [0..9999] if needed.
    // The SysML model can request values outside 0..9999, but contract says 2..400.
    int32_t v = value;
    if (v < 0) v = 0;
    if (v > 9999) v = 9999;

    int d0 = v % 10;
    int d1 = (v / 10) % 10;
    int d2 = (v / 100) % 10;
    int d3 = (v / 1000) % 10;

    // leading_zero:false allowed; "front zero or blank acceptable".
    // We'll blank leading digits.
    uint8_t segs[4];
    segs[0] = digitToSeg(d0);
    segs[1] = digitToSeg(d1);
    segs[2] = digitToSeg(d2);
    segs[3] = digitToSeg(d3);

    bool show2 = (v >= 100);
    bool show3 = (v >= 1000);

    if (!show2) segs[2] = 0x00;
    if (!show3) segs[3] = 0x00;

    write4Digits(segs);
  }

private:
  uint8_t _clk, _dio;
  uint8_t _brightness = 7;

  static uint8_t digitToSeg(uint8_t d) {
    // 7-segment, segment bits: a b c d e f g dp (dp unused here)
    // Standard TM1637 mapping (common).
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
    if (d > 9) return 0x00;
    return map[d];
  }

  void startCondition() {
    pinMode(_dio, OUTPUT);
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dio, LOW);
    delayMicroseconds(2);
    digitalWrite(_clk, LOW);
  }

  void stopCondition() {
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    delayMicroseconds(2);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(2);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      if (b & 0x01) digitalWrite(_dio, HIGH);
      else digitalWrite(_dio, LOW);
      b >>= 1;
      delayMicroseconds(3);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(3);
    }
    // ACK
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    delayMicroseconds(2);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(2);
    pinMode(_dio, OUTPUT);
  }

  void setCommand(uint8_t cmd, uint8_t data) {
    startCondition();
    writeByte(cmd);
    writeByte(data);
    stopCondition();
  }

  void write4Digits(const uint8_t segs[4]) {
    // address command + auto-increment
    startCondition();
    writeByte(0x40); // command
    stopCondition();

    startCondition();
    writeByte(0xC0); // fixed address
    // data bytes
    for (int i = 0; i < 4; i++) {
      writeByte(segs[i]);
    }
    stopCondition();

    // Set brightness
    setCommand(0x88, _brightness);
  }

  void clearDisplay() {
    uint8_t blank[4] = {0, 0, 0, 0};
    write4Digits(blank);
  }
};

// ===================== Ultrasonic measurement =====================
// Measures echo pulse width in microseconds with strict 25000us timeout behavior.
// Returns:
//   valid: true if a pulse was captured and converted distance in 2..400 cm (inclusive).
//   distance_cm: retained measurement in cm (double) when valid; undefined otherwise.
static bool measureHC_SR04(double &distance_cm) {
  // Trigger
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Wait for echo HIGH with timeout
  uint32_t startWait = micros();
  while (digitalRead(PIN_ECHO) == LOW) {
    if ((micros() - startWait) >= ULTRASONIC_TIMEOUT_US) {
      // timeout waiting for rising edge
      return false;
    }
  }

  // Now measure length of echo HIGH with timeout (from rising moment)
  uint32_t echoStart = micros();
  while (digitalRead(PIN_ECHO) == HIGH) {
    if ((micros() - echoStart) >= ULTRASONIC_TIMEOUT_US) {
      // timeout while echo stuck high
      return false;
    }
  }
  uint32_t echoEnd = micros();

  uint32_t pulse_us = echoEnd - echoStart;
  // distance(cm) = echo microseconds / 58 (as specified)
  double dist = (double)pulse_us / 58.0;

  // Valid range: 2..400 cm inclusive
  if (dist < 2.0 || dist > 400.0) {
    return false;
  }

  distance_cm = dist;
  return true;
}

// ===================== Controller: ACCEPTED executable model =====================
static bool startupBlank = true; // initial true

// Controller inputs (io_1, io_2) updated each measurement
static double io_1_distance_cm = 0.0;  // retains previous on invalid by invalid_policy
static bool io_2_valid = false;        // indicates valid reading

static bool io_0_led = false;          // distance < 10cm indicator
static int32_t io_3_display = 10000;  // TM1637 display content integer marker; 10000 => blank

static TM1637Mini tm(PIN_TM_CLK, PIN_TM_DIO);

// Quantization: model uses floor(io_1) when valid and within 2..400.
static inline int32_t model_floor_to_i32(double x) {
  // floor for positive values as expected here.
  return (int32_t)floor(x);
}

// Controller tick update: only updates startupBlank once.
static void controller_tick(bool io2_now) {
  if (startupBlank && io2_now) {
    startupBlank = false;
  }
}

// Controller outputs computed combinationally based on exact model rules.
static void controller_compute_outputs() {
  // io_0
  io_0_led = (!startupBlank && io_2_valid && (io_1_distance_cm < 10.0));

  // io_3
  if (startupBlank || !io_2_valid) {
    io_3_display = 10000; // blank request
  } else {
    if (io_1_distance_cm < 2.0 || io_1_distance_cm > 400.0) {
      io_3_display = 10000; // blank on invalid/out-of-range
    } else {
      if (io_2_valid) io_3_display = model_floor_to_i32(io_1_distance_cm);
      else io_3_display = 10000;
    }
  }
}

// ===================== Scheduler: ensure sampling start interval 60..160ms =====================
static uint32_t nextSampleStartMs = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_TRIG, OUTPUT);
  digitalWrite(PIN_TRIG, LOW);

  pinMode(PIN_ECHO, INPUT);

  tm.begin();
  tm.setBrightness(7);

  // Initial controller outputs: startupBlank=true => LED off, display blank (10000)
  startupBlank = true;
  io_2_valid = false;
  io_1_distance_cm = 0.0; // irrelevant while invalid; must not be forced as 0cm
  controller_compute_outputs();
  digitalWrite(PIN_LED, io_0_led ? HIGH : LOW);
  tm.showIntegerOrBlank(io_3_display, true);

  // Schedule first sample start between 60..160ms
  uint32_t now = millis();
  uint32_t r = (uint32_t)random(SAMPLE_START_MIN_MS, SAMPLE_START_MAX_MS + 1);
  nextSampleStartMs = now + r;

  // Seed RNG for interval selection.
  randomSeed((uint32_t)micros());
}

void loop() {
  uint32_t now = millis();

  // Maintain 10ms controller period (planning period) by ticking compute/update regularly.
  static uint32_t lastTickMs = 0;
  if (now - lastTickMs >= LOOP_TICK_MS) {
    lastTickMs += LOOP_TICK_MS;

    // Tick controller (only affects startupBlank transition).
    controller_tick(io_2_valid);

    // Compute outputs and drive hardware.
    controller_compute_outputs();
    digitalWrite(PIN_LED, io_0_led ? HIGH : LOW);
    tm.showIntegerOrBlank(io_3_display, true);
  }

  // Start sampling when due.
  if ((int32_t)(now - nextSampleStartMs) >= 0) {
    // Perform one full ultrasonic measurement, then update inputs.
    double measured_cm = 0.0;
    bool valid = measureHC_SR04(measured_cm);

    // invalid_policy: ultrasonic_valid holds last valid distance value, but validity drives outputs.
    if (valid) {
      io_1_distance_cm = measured_cm;
      io_2_valid = true;
    } else {
      // keep previous distance_cm, but invalidate for outputs
      io_2_valid = false;
    }

    // After measurement, set next sample start interval to 60..160ms from now.
    uint32_t r = (uint32_t)random(SAMPLE_START_MIN_MS, SAMPLE_START_MAX_MS + 1);
    nextSampleStartMs = now + r;
  }
}
#include <Arduino.h>

// -------- TM1637 minimal driver (no external deps) --------
class TM1637 {
public:
  TM1637(uint8_t clk, uint8_t dio) : _clk(clk), _dio(dio) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
  }

  void setBrightness(uint8_t b) {
    // 0..7 typical
    _brightness = b & 0x07;
  }

  // invalid_policy=blank, leading_zero=false
  void showNumber(int value, bool /*leadingZero*/ = false) {
    // We'll display as: [stage][tens?][ones?] for 2-digit max or 4-digit max style.
    // But contract says tm1637_number is an integer selection; no further rule provided.
    // We'll use a deterministic encoding:
    // tmValue encodes stage (1..3) in the thousands place and remaining seconds in the last two digits (00..99).
    // If remainingSeconds > 99, saturate to 99.
    showSegmentsForEncoded(value);
  }

private:
  uint8_t _clk, _dio;
  uint8_t _brightness = 7;

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
  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      digitalWrite(_clk, HIGH);
    }
    // ack
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_clk, LOW);
    pinMode(_dio, OUTPUT);
  }

  // Digit to segments (a,b,c,d,e,f,g,dp) active-high bits standard TM1637
  static constexpr uint8_t segTable[16] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71  // F
  };

  void showSegments(const uint8_t segs[4]) {
    start();
    writeByte(0x40); // fixed address command
    stop();

    start();
    writeByte(0xC0 | 0x00); // auto-increment address

    for (int i = 0; i < 4; i++) {
      writeByte(segs[i]);
    }
    stop();
  }

  void showSegmentsForEncoded(int encoded) {
    // encoded: thousands digit = stage (1..3), last two digits = remaining seconds (00..99)
    // middle digits are blank.
    int stage = (encoded / 1000) % 10;
    int rem = encoded % 100;
    if (stage < 0) stage = 0;
    if (stage > 9) stage = 0;
    if (rem < 0) rem = 0;
    if (rem > 99) rem = 99;

    bool invalidBlank = true; // invalid_policy=blank
    uint8_t segs[4];

    // positions: [thousands, hundreds, tens, ones]
    // thousands shows stage, hundreds blank, tens/ones show rem
    if (invalidBlank && (stage < 0 || stage > 9)) segs[0] = 0x00;
    else segs[0] = segTable[stage];

    segs[1] = 0x00; // blank hundreds
    segs[2] = segTable[(rem / 10) % 10];
    segs[3] = segTable[rem % 10];

    // brightness command before display
    start();
    writeByte(0x40);
    stop();
    start();
    writeByte(0x88 | _brightness);
    stop();

    showSegments(segs);
  }
};

// -------- Hardware contract --------
static const uint8_t PIN_LM35 = A1;
static const uint8_t PIN_D2 = 2;
static const uint8_t PIN_D3 = 3;
static const uint8_t PIN_D4 = 4;
static const uint8_t PIN_D6 = 6;
static const uint8_t PIN_D7 = 7; // TM1637 CLK
static const uint8_t PIN_D5 = 5; // TM1637 DIO
static const uint8_t PIN_D13 = 13;

// Serial contract
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t CONTROL_PERIOD_MS = 10;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20; // informational; Arduino Serial doesn't enforce this directly

// -------- Controller state (as per accepted executable model bundle) --------
static int stage_state = 1;
static bool activeTemperatureReached_state = false;
static int remainingSeconds_state = 0;
static int totalSeconds_state = 0;
static bool heatingActive_state = false; // unused in model outputs per bundle
static bool previousPressed_state = false;

// Outputs per model bundle
static int io_4_buzzerHz = 0;
static bool io_5_led = false;
static int io_6_tm1637Value = 0;

static bool serial_0_trigger = false;
static int serial_0_value_0_t_ms = 0;
static int serial_0_value_1_stage = 1;
static double serial_0_value_2_tempC = 0.0;
static int serial_0_value_3_remaining_s = 0;
static bool serial_0_value_4_event = false;

// -------- Inputs per model bundle --------
static double io_0_adc_count = 0.0; // requested semantics: adc_count
static bool io_1_buttonD2 = false;
static bool io_2_buttonD3 = false;
static bool io_3_buttonD4 = false;

static TM1637 tm(PIN_D7, PIN_D5);

static uint32_t t0_ms = 0;

static bool readButtonPullupActiveHigh(uint8_t pin) {
  // Contract: pullup=true and active_high=true (pressed => HIGH)
  return digitalRead(pin) == HIGH;
}

static double readLM35_ADCcount() {
  // Contract: scale 1 offset 0, unit adc_count. We'll report raw ADC code.
  // Arduino analogRead returns 0..1023.
  return (double)analogRead(PIN_LM35);
}

static void controller_tick() {
  // Accepted model: tick:
  // first start; then if false { ... } else { first start; then assign previousPressed := io_1 }
  previousPressed_state = io_1_buttonD2;

  // Outputs per model bundle:
  io_4_buzzerHz = 0;
  io_5_led = false;
  io_6_tm1637Value = 0;              // deterministic as per model bundle outputs (always 0)
  serial_0_trigger = false;

  serial_0_value_0_t_ms = 0;
  serial_0_value_1_stage = stage_state;
  serial_0_value_2_tempC = io_0_adc_count;
  serial_0_value_3_remaining_s = remainingSeconds_state;
  serial_0_value_4_event = activeTemperatureReached_state;
}

static void apply_outputs() {
  // io_5 LED false always
  digitalWrite(PIN_D13, io_5_led ? HIGH : LOW);

  // io_4 buzzer: tone_output invalid_policy=silence => freq 0 -> silence
  if (io_4_buzzerHz <= 0) {
    noTone(PIN_D6);
  } else {
    tone(PIN_D6, io_4_buzzerHz);
  }

  // TM1637 display value: invalid_policy=blank; we always pass tm1637_number=0 from model.
  tm.showNumber(io_6_tm1637Value);
}

static void emit_serial_if_triggered() {
  if (!serial_0_trigger) return;

  // Typed numeric fields exist but no fixed textual protocol was provided.
  // We output a single line with key=value for robustness (trigger is always false in this model).
  // If trigger ever becomes true in future modifications, this keeps behavior deterministic.
  Serial.print("t_ms=");
  Serial.print(serial_0_value_0_t_ms);
  Serial.print(",stage=");
  Serial.print(serial_0_value_1_stage);
  Serial.print(",temp_C=");
  Serial.print(serial_0_value_2_tempC, 3);
  Serial.print(",remaining_s=");
  Serial.print(serial_0_value_3_remaining_s);
  Serial.print(",event=");
  Serial.println(serial_0_value_4_event ? 1 : 0);
}

void setup() {
  // Timing/base
  t0_ms = millis();

  // Serial
  Serial.begin(SERIAL_BAUD);

  // Inputs
  pinMode(PIN_D2, INPUT_PULLUP);
  pinMode(PIN_D3, INPUT_PULLUP);
  pinMode(PIN_D4, INPUT_PULLUP);

  // Outputs
  pinMode(PIN_D13, OUTPUT);
  pinMode(PIN_D6, OUTPUT); // tone uses timer, pinMode ok

  // TM1637
  tm.begin();
  tm.setBrightness(7); // contract brightness=7, leading_zero=false, invalid_policy=blank
  tm.showNumber(0);
}

void loop() {
  static uint32_t lastTick = 0;
  uint32_t now = millis();
  if (now - lastTick < CONTROL_PERIOD_MS) return;
  lastTick += CONTROL_PERIOD_MS;

  // Sample inputs (sample_period_ms=10)
  io_0_adc_count = readLM35_ADCcount();
  io_1_buttonD2 = readButtonPullupActiveHigh(PIN_D2);
  io_2_buttonD3 = readButtonPullupActiveHigh(PIN_D3);
  io_3_buttonD4 = readButtonPullupActiveHigh(PIN_D4);

  // Controller tick implementing accepted executable model bundle exactly
  controller_tick();

  // Apply outputs
  apply_outputs();

  // Serial logging guarded by trigger (model sets it false always)
  emit_serial_if_triggered();
}
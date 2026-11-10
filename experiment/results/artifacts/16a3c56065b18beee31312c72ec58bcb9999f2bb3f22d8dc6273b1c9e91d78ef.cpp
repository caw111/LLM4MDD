#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

//
// Pin contract (authoritative remap)
// A0: 光敏电阻
// D2: 按钮（按下为 true；active_high=true; pullup=false => 直连到高电平表示按下）
// D7: DHT11 数据脚（温湿度同一引脚）
// TM1637: CLK=D4, DIO=D5
//
static const uint8_t PIN_LIGHT_A0 = A0;
static const uint8_t PIN_BUTTON_D2 = 2;
static const uint8_t PIN_DHT_D7 = 7;
static const uint8_t PIN_TM1637_CLK_D4 = 4;
static const uint8_t PIN_TM1637_DIO_D5 = 5;

static const uint32_t CONTROL_PERIOD_MS = 10;

static const float DHT_INVALID_HOLD_POLICY = 0.0f; // behavior handled via hold_last_valid

// Accept model thresholds (exact from bundle)
static const float LIGHT_LOW_ADC_THRESHOLD = 300.0f;
static const float TEMP_HIGH_C_THRESHOLD = 35.0f;

// Display discretization: floor()
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t floor_to_i32(float x);
#line 215 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialOutputReport(float light, float temp, float hum);
#line 225 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialOutputEnvironmentAlarm();
#line 230 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyModelTick();
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 282 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 321 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline int32_t floor_to_i32(float x) {
  if (x >= 0.0f) return (int32_t)floorf(x);
  // floorf already handles negative correctly, but AVR float->floor works.
  return (int32_t)floorf(x);
}

// ---------------- TM1637 minimal driver ----------------
// This is a lightweight TM1637 implementation for Arduino UNO.
class TM1637 {
public:
  TM1637(uint8_t pinCLK, uint8_t pinDIO) : _pinCLK(pinCLK), _pinDIO(pinDIO) {}

  void begin() {
    pinMode(_pinCLK, OUTPUT);
    pinMode(_pinDIO, OUTPUT);
    digitalWrite(_pinCLK, HIGH);
    digitalWrite(_pinDIO, HIGH);
  }

  void setBrightness(uint8_t b) {
    // 0..7 typical
    start();
    writeByte(0x88 | (b & 0x07));
    stop();
  }

  void showNumberDec(int32_t num) {
    // Show as decimal without leading zero
    // TM1637 supports up to 4 digits; use truncation like model discretization.
    // Model outputs floor(io_0/io_2/io_3) as i32.
    // We'll display least significant 4 digits; if negative, display '-'.
    uint8_t seg[4] = {0, 0, 0, 0};

    bool neg = (num < 0);
    int32_t n = neg ? -num : num;

    // Clamp to representable digits by trimming higher digits.
    for (int i = 0; i < 4; i++) {
      int32_t digit = n % 10;
      seg[3 - i] = segDigit(digit);
      n /= 10;
      if (n == 0) {
        // For "no leading zero": blank if higher than current digits.
        // Determine blanking: if remaining digits are zero and it's in leading positions.
        // We still keep blanks until we place first non-zero.
        // The loop fills from right; we can manage after.
      }
    }

    // Blank leading zeros (no leading_zero in contract)
    // Find first non-zero from left.
    int firstNonZero = -1;
    for (int i = 0; i < 4; i++) {
      // digit segment match isn't directly invertible; re-derive digits via num
      // We'll simply recompute digits for the exact blanking.
    }
    // Recompute digits to decide blanks deterministically.
    int32_t m = neg ? -num : num;
    int d[4] = {0,0,0,0};
    for (int i = 3; i >= 0; i--) {
      d[i] = m % 10;
      m /= 10;
    }
    // blank leading zeros
    bool started = false;
    for (int i = 0; i < 4; i++) {
      if (neg && i == 0) {
        // We'll place minus sign later.
        continue;
      }
      if (d[i] != 0 || started) {
        started = true;
      } else {
        seg[i] = 0x00; // blank
      }
    }

    if (neg) {
      // Place minus sign in first position (leftmost).
      seg[0] = segMinus();
      // In case number is too small (like -0), blank all digits and keep '-'
      // But model would floor(-0) => 0; neg false for 0.
    }

    start();
    writeByte(0x40); // data command
    // write segments
    for (int i = 0; i < 4; i++) writeByte(seg[i]);
    stop();
    update();
  }

private:
  uint8_t _pinCLK, _pinDIO;

  void start() {
    digitalWrite(_pinCLK, HIGH);
    digitalWrite(_pinDIO, HIGH);
    delayMicroseconds(5);
    digitalWrite(_pinDIO, LOW);
    delayMicroseconds(5);
  }

  void stop() {
    digitalWrite(_pinCLK, LOW);
    digitalWrite(_pinDIO, LOW);
    delayMicroseconds(5);
    digitalWrite(_pinCLK, HIGH);
    delayMicroseconds(5);
    digitalWrite(_pinDIO, HIGH);
    delayMicroseconds(5);
  }

  void update() {
    // "display control" to latch: TM1637 requires a data command and then display on command
    start();
    writeByte(0x80 | 0x00);
    stop();
  }

  void writeByte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(_pinCLK, LOW);
      digitalWrite(_pinDIO, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(5);
      digitalWrite(_pinCLK, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }
    // ACK
    digitalWrite(_pinCLK, LOW);
    pinMode(_pinDIO, INPUT);
    delayMicroseconds(5);
    digitalWrite(_pinCLK, HIGH);
    delayMicroseconds(5);
    pinMode(_pinDIO, OUTPUT);
  }

  uint8_t segDigit(int digit) {
    static const uint8_t table[10] = {
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
    if (digit < 0 || digit > 9) return 0x00;
    return table[digit];
  }

  uint8_t segMinus() {
    return 0x40; // minus segment
  }
};

// ---------------- DHT11 handling with hold_last_valid ----------------
DHT dht(PIN_DHT_D7, DHT11);

// Model invalid_policy=hold_last_valid for temperature and humidity and dht_valid.
static bool dht_valid_last = false;
static float dht_temp_last = 0.0f;
static float dht_hum_last = 0.0f;

// ---------------- Controller state ----------------
static bool previousPressed = false;   // initial false
static int32_t displayMode = 0;       // initial 0
static int32_t accumulatedMs = 0;     // initial 0

// Inputs for the model at current step
static float io_0 = 0.0f;   // light adc_count (A0 raw)
static bool  io_1 = false;  // button pressed (active_high)
static float io_2 = 0.0f;   // temperature degC
static float io_3 = 0.0f;   // humidity %RH
static bool  io_4 = false;  // dht_valid
static int32_t io_6 = 0;    // elapsed_ms delta since previous step

// Timers
static uint32_t lastStepMicros = 0;
static uint32_t lastDhtAttemptMs = 0;

// Maintain step-aligned 10ms planning: use micros for delta, but clamp io_6 to actual elapsed.
static uint32_t lastSerialReportStepAccumulated = 0; // not needed; model uses accumulatedMs

// ---------------- Serial output ----------------
static void serialOutputReport(float light, float temp, float hum) {
  // No fixed text requirement for alarm; but for numeric report it's fixed structure: three items complete data.
  // We'll emit a single line with comma-separated values for robustness.
  Serial.print(light, 0);
  Serial.print(",");
  Serial.print(temp, 1);
  Serial.print(",");
  Serial.println(hum, 1);
}

static void serialOutputEnvironmentAlarm() {
  // Requirement says alarm text not specified. Emit a generic marker.
  Serial.println("environment_alarm");
}

static void applyModelTick() {
  // Controller perform action tick:
  // if (io_1 && !previousPressed) { if (displayMode==2) displayMode=0 else displayMode=displayMode+1 }
  if (io_1 && !previousPressed) {
    if (displayMode == 2) displayMode = 0;
    else displayMode = displayMode + 1;
  }

  accumulatedMs = accumulatedMs + io_6;
  if (accumulatedMs >= 2000) {
    accumulatedMs = accumulatedMs - 2000;
  }
  previousPressed = io_1;
}

static void applyOutputs() {
  // io_5 (TM1637 display)
  int32_t shown = 0;
  if (displayMode == 0) shown = floor_to_i32(io_0);
  else if (displayMode == 1) shown = floor_to_i32(io_2);
  else shown = floor_to_i32(io_3);

  // Display update every control step (model output is combinational to inputs/state).
  // TM1637 supports only updates with this driver.
  // Set brightness once in setup; showNumberDec on each step.
  tm1637.showNumberDec(shown);

  // serial_0_trigger: accumulatedMs in [0,10)
  bool serial0_trigger = (accumulatedMs >= 0) && (accumulatedMs < 10);

  // serial_1_trigger: io_4 && (io_0 <= 300.0) && (io_2 >= 35.0)
  bool serial1_trigger = (io_4) && (io_0 <= LIGHT_LOW_ADC_THRESHOLD) && (io_2 >= TEMP_HIGH_C_THRESHOLD);

  if (serial0_trigger) {
    // serial_0_value_0=io_0, serial_0_value_1=io_2, serial_0_value_2=io_3
    Serial.print(io_0, 0);
    Serial.print(",");
    Serial.print(io_2, 1);
    Serial.print(",");
    Serial.println(io_3, 1);
  }

  if (serial1_trigger) {
    // serial_1_value_0 = serial_1_trigger (exact gate)
    // Print alert
    serialOutputEnvironmentAlarm();
  }
}

// ---------------- Instantiate TM1637 ----------------
TM1637 tm1637(PIN_TM1637_CLK_D4, PIN_TM1637_DIO_D5);

void setup() {
  // Hardware init
  Serial.begin(9600);

  pinMode(PIN_BUTTON_D2, INPUT); // pullup=false per contract
  pinMode(PIN_LIGHT_A0, INPUT);

  dht.begin();

  tm1637.begin();
  tm1637.setBrightness(7); // brightness=7 per contract

  lastStepMicros = micros();
  lastDhtAttemptMs = millis();

  // Initialize model IOs with defaults consistent with contract initial values.
  previousPressed = false;
  displayMode = 0;
  accumulatedMs = 0;

  io_0 = 0.0f;
  io_1 = false;
  io_2 = 0.0f;
  io_3 = 0.0f;
  io_4 = false;
  io_6 = 0;

  // Initial DHT attempt to populate last valid if possible (hold policy).
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t) && !isnan(h)) {
    dht_valid_last = true;
    dht_temp_last = t;
    dht_hum_last = h;
  } else {
    dht_valid_last = false;
  }
}

void loop() {
  // Enforce control period ~10ms using micros, but use actual delta as io_6.
  uint32_t nowMicros = micros();
  uint32_t elapsedMicros = nowMicros - lastStepMicros;
  if (elapsedMicros < (CONTROL_PERIOD_MS * 1000UL)) {
    // Busy wait until next step to keep deterministic 10ms cadence
    // (Still compute delta each step)
    return;
  }
  lastStepMicros = nowMicros;

  io_6 = (int32_t)(elapsedMicros / 1000UL); // delta_since_previous_step (ms)

  // Read inputs:
  // io_0: analog_input A0 raw ADC counts
  io_0 = (float)analogRead(PIN_LIGHT_A0); // adc_count

  // io_1: digital_input button pressed = true_level HIGH
  io_1 = (digitalRead(PIN_BUTTON_D2) == HIGH);

  // io_2/io_3/io_4: DHT11, sampled period 2000ms, invalid_policy hold_last_valid
  uint32_t nowMs = millis();
  if ((uint32_t)(nowMs - lastDhtAttemptMs) >= 2000UL) {
    lastDhtAttemptMs = nowMs;

    float t = dht.readTemperature(); // °C
    float h = dht.readHumidity();    // %RH

    if (!isnan(t) && !isnan(h)) {
      dht_valid_last = true;
      dht_temp_last = t;
      dht_hum_last = h;
    } else {
      // hold_last_valid for temperature/humidity
      dht_valid_last = false;
      // keep dht_temp_last and dht_hum_last unchanged
    }
  }

  io_2 = dht_temp_last;
  io_3 = dht_hum_last;
  io_4 = dht_valid_last;

  // Model tick and outputs
  applyModelTick();
  applyOutputs();
}

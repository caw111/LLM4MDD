#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_humidity_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

// === Frozen hardware contract ===
static const uint8_t DHT_PIN = 2;          // SDA -> D2
static const uint8_t DHT_TYPE = DHT22;     // DHT22 sensor
static const unsigned long SERIAL_BAUD = 9600;
static const uint32_t CONTROL_PERIOD_MS = 10;
static const uint32_t DHT_SAMPLE_PERIOD_MS = 2000; // must be >=2000ms and aligned to 10ms multiple

// Serial framing: line_or_idle; idle_timeout_ms=20 is handled by newline writes here.

DHT dht(DHT_PIN, DHT_TYPE);

// === ACCEPTED executable model (generated) ===
// Controller state: lastHumidity (f64)
// Updates (per tick): if io_1 then lastHumidity := io_0
// Outputs: serial_0_trigger := io_1; serial_0_value_0 := lastHumidity
static float lastHumidity = 0.0f; // initial 0.0

// === Timers / scheduling (control period 10ms) ===
static unsigned long nextTickMs = 0;
static unsigned long nextDhtReadMs = 0;

// === Inputs to controller ===
static float io_0 = 0.0f;     // dht_humidity (%RH)
static bool  io_1 = false;    // dht_valid

// invalid_policy: hold_last_valid
static bool haveLastValidHumidity = false;
static float heldLastValidHumidity = 0.0f;

// Keep last printed value consistent with model: serial prints lastHumidity only when io_1 is true.
// Model updates lastHumidity on same tick as io_1 is true.
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_humidity_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controller_tick();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_humidity_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serial_report_if_triggered();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_humidity_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readDhtAndUpdateInputs();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_humidity_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_humidity_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_humidity_sensor_basic\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controller_tick() {
  // Update: if (io_1) lastHumidity := io_0;
  if (io_1) {
    lastHumidity = io_0;
  }
  // Outputs are derived directly from io_1 and lastHumidity
  // serial_0_trigger = io_1
  // serial_0_value_0 = lastHumidity
}

static void serial_report_if_triggered() {
  bool serial_0_trigger = io_1;
  float serial_0_value_0 = lastHumidity;

  if (serial_0_trigger) {
    // Requirement: Print humidity as: "Humidity: {humidity value}"
    // Example format: "Humidity: 40.0"
    Serial.print(F("Humidity: "));
    Serial.println(serial_0_value_0, 1);
  }
}

static bool readDhtAndUpdateInputs() {
  // Acquisition sampling period constraint.
  // Called only when scheduler says it's time.
  float h = dht.readHumidity();

  // DHT library returns NAN on invalid read.
  if (!isnan(h)) {
    // Valid measurement
    io_1 = true;
    io_0 = h;

    heldLastValidHumidity = h;
    haveLastValidHumidity = true;
  } else {
    // Invalid measurement: hold last valid; dht_valid becomes false
    io_1 = false;
    if (haveLastValidHumidity) {
      io_0 = heldLastValidHumidity;
    } else {
      // If never had valid data, keep io_0 at 0 (initial default)
      io_0 = 0.0f;
    }
  }

  return io_1;
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  // Initialize DHT sensor
  dht.begin();

  // Align schedule to current time (deterministic within tick loop)
  unsigned long now = millis();
  nextTickMs = now;
  nextDhtReadMs = now; // immediate first read attempt after power-up

  // Initialize model inputs consistent with contract initial values
  io_0 = 0.0f;
  io_1 = false;
  lastHumidity = 0.0f;

  haveLastValidHumidity = false;
  heldLastValidHumidity = 0.0f;
}

void loop() {
  unsigned long now = millis();

  // Control-period tick at 10ms
  if ((long)(now - nextTickMs) >= 0) {
    nextTickMs += CONTROL_PERIOD_MS;

    // DHT sampling aligned to control-period multiple (2000ms is multiple of 10ms)
    if ((long)(now - nextDhtReadMs) >= 0) {
      // Schedule next DHT read exactly by period to keep alignment
      nextDhtReadMs += DHT_SAMPLE_PERIOD_MS;
      readDhtAndUpdateInputs();
    }

    // Generated controller action per tick
    controller_tick();

    // Generated outputs behavior: serial_0_trigger and serial_0_value_0
    // Print only when serial_0_trigger == true (i.e., io_1 valid on this tick)
    serial_report_if_triggered();
  }
}

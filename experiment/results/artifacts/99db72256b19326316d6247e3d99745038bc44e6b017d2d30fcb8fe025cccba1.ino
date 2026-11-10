#include <Arduino.h>
#include <DHT.h>

static const uint8_t PIN_LED_D13 = 13;
static const uint8_t PIN_DHT_D7 = 7;
static const uint8_t PIN_TRIG_D8 = 8;
static const uint8_t PIN_ECHO_D9 = 9;

static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t CONTROL_PERIOD_MS = 10;   // control_period_ms frozen contract
static const uint32_t TARGET_POLL_MS = 3000;    // 3 seconds business poll

// --- DHT11 setup ---
#define DHTTYPE DHT11
DHT dht(PIN_DHT_D7, DHTTYPE);

// --- State variables (must match model initial values) ---
static int32_t accum3sMs = 0;            // i32 initial 0
static int32_t consecCloseCount = 0;    // i32 initial 0
static double prevDhtTemp = 0.0;        // f64 initial 0.0
static double prevDhtHum  = 0.0;        // f64 initial 0.0
static double prevDistCm   = 0.0;        // f64 initial 0.0

// --- IO variables (as per interface contract) ---
static double io_1 = 0.0;   // dht_temperature (degC)
static double io_2 = 0.0;   // dht_humidity (%RH)
static bool   io_3 = false; // dht_valid

static double io_4 = 0.0;   // ultrasonic_distance (cm)
static bool   io_5 = false; // ultrasonic_valid

static int32_t io_6 = 0;    // elapsed_ms delta_since_previous_step
static int32_t io_7 = 0;    // optional command input (unused by model)
static bool    io_7_valid = false;

// Gate / outputs
static bool io_0 = false;    // LED D13 output
static bool io_8 = false;    // persistent text gate
static bool serial_0_trigger = false;
static double serial_0_value_0 = 0.0; // temperature
static double serial_0_value_1 = 0.0; // humidity
static double serial_0_value_2 = 0.0; // distance
static bool   serial_0_value_3 = false; // distance_valid
static bool   serial_0_value_4 = false; // dht_valid

// --- Serial text fixed content ---
static const char* PERSISTENT_TEXT = "persistent object detected";

// --- Sensor helpers with "hold_last_valid" semantics handled by model ---
static bool readDht11(double &tempC, double &humPct) {
  // DHT library returns NaN when invalid; treat as invalid.
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) return false;
  tempC = (double)t;
  humPct = (double)h;
  return true;
}

static bool readUltrasonicCm(double &distCmOut) {
  // HC-SR04 timing: trigger low->high 10us->low, then measure echo pulse width.
  // Contract: timeout_us=30000.
  const uint32_t timeoutUs = 30000;

  digitalWrite(PIN_TRIG_D8, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG_D8, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG_D8, LOW);

  // pulseIn returns 0 on timeout.
  unsigned long durationUs = pulseIn(PIN_ECHO_D9, HIGH, timeoutUs);
  if (durationUs == 0) return false;

  // Distance (cm) = durationUs / 58.0 (speed of sound approx)
  double cm = (double)durationUs / 58.0;
  // "越界" policy is unspecified numerically; only model uses validity to gate.
  // We'll consider any positive finite distance as valid.
  if (!isfinite(cm) || cm < 0.0) return false;

  distCmOut = cm;
  return true;
}

// --- Serial command optional input parsing (protocol unspecified) ---
// Contract says ON->1, OFF->2. "SysML决定是否启用" => unused in model.
// We still implement basic line parsing so io_7/io_7_valid behave reasonably.
static void pollSerialCommand() {
  io_7_valid = false;
  io_7 = io_7; // no-op unless valid command received

  static char buf[16];
  static uint8_t idx = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    // idle_timeout_ms=20 and framing=line_or_idle: approximate by line-based parsing
    if (c == '\n' || c == '\r') {
      if (idx == 0) continue;
      buf[idx] = '\0';

      if (strcmp(buf, "ON") == 0) {
        io_7 = 1;
        io_7_valid = true;
      } else if (strcmp(buf, "OFF") == 0) {
        io_7 = 2;
        io_7_valid = true;
      }
      idx = 0;
    } else {
      if (idx < (sizeof(buf) - 1)) {
        buf[idx++] = c;
      }
    }
  }
}

void setup() {
  pinMode(PIN_LED_D13, OUTPUT);
  digitalWrite(PIN_LED_D13, LOW);

  pinMode(PIN_TRIG_D8, OUTPUT);
  digitalWrite(PIN_TRIG_D8, LOW);

  pinMode(PIN_ECHO_D9, INPUT);

  Serial.begin(SERIAL_BAUD);
  dht.begin();

  // Control loop tick baseline for elapsed delta
  // (no direct binding, but io_6 must be delta_since_previous_step)
}

void loop() {
  static uint32_t lastMs = millis();

  // Compute io_6 = elapsed_ms delta_since_previous_step
  uint32_t now = millis();
  uint32_t elapsed = now - lastMs;

  // Keep i32 semantics; clamp to avoid overflow on long hiccups
  if (elapsed > 2147483000UL) elapsed = 2147483000UL;
  io_6 = (int32_t)elapsed;

  lastMs = now;

  // Poll sensors (model semantics require inputs as values; validity determines hold logic)
  // DHT and ultrasonic have their own sample periods in contract, but model itself doesn't depend on sample_period.
  // We'll read them in the control loop to preserve behavior, accepting sensor latency.
  // DHT11 reading too frequently can yield invalids; model will "hold_last_valid" via its prev* logic.
  double tC = 0.0, hPct = 0.0;
  bool dhtOk = readDht11(tC, hPct);
  io_3 = dhtOk;
  if (dhtOk) {
    io_1 = tC;
    io_2 = hPct;
  } // else: io_1/io_2 may be stale; model uses io_3 guard to keep prev values

  double distCm = 0.0;
  bool usOk = readUltrasonicCm(distCm);
  io_5 = usOk;
  if (usOk) {
    io_4 = distCm;
  }

  // Optional serial command input (unused by controller model, but IO exists)
  pollSerialCommand();

  // --- Controller tick: implement ACCEPTED model exactly in order ---
  // perform action tick:
  // first start;
  // then assign accum3sMs := accum3sMs + io_6;
  accum3sMs = accum3sMs + io_6;

  // then if accum3sMs >= 3000 { accum3sMs := accum3sMs - 3000; }
  if (accum3sMs >= (int32_t)TARGET_POLL_MS) {
    accum3sMs = accum3sMs - (int32_t)TARGET_POLL_MS;
  }

  // then if io_5 { if io_4 < 20.0 consecCloseCount++ else consecCloseCount=0 } else consecCloseCount=0
  if (io_5) {
    if (io_4 < 20.0) {
      consecCloseCount = consecCloseCount + 1;
    } else {
      consecCloseCount = 0;
    }
  } else {
    consecCloseCount = 0;
  }

  // then if io_3 { prevDhtTemp := io_1; prevDhtHum := io_2; }
  if (io_3) {
    prevDhtTemp = io_1;
    prevDhtHum = io_2;
  }

  // then if io_5 { prevDistCm := io_4; }
  if (io_5) {
    prevDistCm = io_4;
  }

  // then if consecCloseCount > 3 { consecCloseCount := 3; }
  if (consecCloseCount > 3) {
    consecCloseCount = 3;
  }

  // --- Outputs (evaluate expressions exactly) ---
  io_0 = (consecCloseCount >= 3) && io_5;
  io_8 = (consecCloseCount >= 3) && io_5 && (accum3sMs >= 0);

  serial_0_trigger = (consecCloseCount >= 0) && (accum3sMs >= (int32_t)TARGET_POLL_MS
                       || (accum3sMs + io_6) >= (int32_t)TARGET_POLL_MS);

  serial_0_value_0 = (io_3 ? io_1 : prevDhtTemp);
  serial_0_value_1 = (io_3 ? io_2 : prevDhtHum);
  serial_0_value_2 = (io_5 ? io_4 : prevDistCm);
  serial_0_value_3 = io_5;
  serial_0_value_4 = io_3;

  // Apply IO pin output (LED)
  digitalWrite(PIN_LED_D13, io_0 ? HIGH : LOW);

  // Serial output behavior:
  // serial_channels trigger gates: when serial_0_trigger true, emit serial_0_value_*.
  // serial_text_output triggers when io_8 true, emit fixed text persistent object detected.
  if (serial_0_trigger) {
    // "line_or_idle" framing: use newline-separated line prints.
    // Keep deterministic ordering corresponding to value ports 0..4.
    Serial.print(serial_0_value_0, 2);
    Serial.print(',');
    Serial.print(serial_0_value_1, 2);
    Serial.print(',');
    Serial.print(serial_0_value_2, 2);
    Serial.print(',');
    Serial.print(serial_0_value_3 ? 1 : 0);
    Serial.print(',');
    Serial.print(serial_0_value_4 ? 1 : 0);
    Serial.print('\n');
  }

  if (io_8) {
    // Send persistent object detected text (fixed) when gate true.
    // Contract says on the control step that satisfies the condition.
    Serial.println(PERSISTENT_TEXT);
  }

  // Enforce approximate control_period_ms=10; do not distort io_6 too much.
  // If we already ran long, skip delay.
  uint32_t loopElapsed = (millis() - now);
  if (loopElapsed < CONTROL_PERIOD_MS) {
    delay(CONTROL_PERIOD_MS - loopElapsed);
  }
}
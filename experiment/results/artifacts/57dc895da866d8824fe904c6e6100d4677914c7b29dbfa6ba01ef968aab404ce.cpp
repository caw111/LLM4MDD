#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// -------------------- Frozen interface contract bindings --------------------
// io_0: button D2 (active high) => model true when pressed
static const uint8_t PIN_D2 = 2;

// DHT11 on D7
static const uint8_t PIN_D7 = 7;
static const uint8_t DHTTYPE_DHT11 = 11;
static DHT dht(PIN_D7, DHTTYPE_DHT11);

// TM1637: CLK=D4, DIO=D5
static const uint8_t PIN_TM1637_CLK = 4; // D4
static const uint8_t PIN_TM1637_DIO = 5; // D5
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// -------------------- Serial command input parsing --------------------
static const uint32_t SERIAL_BAUD = 9600;

// Commands map: BOTH=1, HUMI=2, TEMP=3
enum MenuModeID : int32_t { BOTH_ID = 1, HUMI_ID = 2, TEMP_ID = 3 };

struct SerialCommandInput {
  bool valid = false;   // io_5_valid: true for this model step only
  int32_t value = 0;    // io_5
  uint32_t lastByteMs = 0;
  String buf;

  void begin(uint32_t baud) {
    Serial.begin(baud);
    buf.reserve(32);
    lastByteMs = millis();
  }

  static void trimCRLF(String &s) {
    while (s.length() && (s[s.length() - 1] == '\r' || s[s.length() - 1] == '\n' || s[s[s.length()-1]==' '])) {
      s.remove(s.length() - 1);
    }
    // trim spaces at end
    while (s.length() && s[s.length() - 1] == ' ') s.remove(s.length() - 1);
    // trim spaces at beginning
    int start = 0;
    while (start < (int)s.length() && s[start] == ' ') start++;
    if (start > 0) s.remove(0, start);
  }

  void poll(uint32_t idleTimeoutMs) {
    valid = false;

    while (Serial.available() > 0) {
      char c = (char)Serial.read();
      lastByteMs = millis();

      // line_or_idle: accept until newline OR idle timeout
      if (c == '\n' || c == '\r') {
        // finalize current buffer
        if (buf.length() > 0) {
          String token = buf;
          buf = "";
          trimCRLF(token);
          token.toUpperCase();

          if (token == "BOTH") value = BOTH_ID;
          else if (token == "HUMI") value = HUMI_ID;
          else if (token == "TEMP") value = TEMP_ID;
          else {
            // unknown: do not assert valid
            return;
          }
          valid = true;
        } else {
          // empty line: ignore
        }
      } else {
        // accumulate
        if (buf.length() < 31) buf += c;
      }
    }

    // idle timeout framing
    uint32_t now = millis();
    if (buf.length() > 0 && (now - lastByteMs) >= idleTimeoutMs) {
      String token = buf;
      buf = "";
      trimCRLF(token);
      token.toUpperCase();

      if (token == "BOTH") value = BOTH_ID;
      else if (token == "HUMI") value = HUMI_ID;
      else if (token == "TEMP") value = TEMP_ID;
      else {
        return;
      }
      valid = true;
    }
  }
};

static SerialCommandInput serial_in;

// -------------------- DHT sampling with hold_last_valid --------------------
static const uint32_t DHT_SAMPLE_PERIOD_MS = 2000;

struct DhtInputs {
  float tempC = 0.0;      // io_1
  float humRH = 0.0;      // io_2
  bool valid = false;     // io_3

  uint32_t lastSampleMs = 0;

  void begin() {
    dht.begin();
    lastSampleMs = 0;
  }

  void poll() {
    uint32_t now = millis();
    // Update at 2000ms cadence; otherwise hold_last_valid behavior:
    if (now - lastSampleMs >= DHT_SAMPLE_PERIOD_MS) {
      lastSampleMs = now;

      float t = dht.readTemperature(); // °C
      float h = dht.readHumidity();    // %RH

      // DHT validity: coherent frame valid => non-NaN
      if (!isnan(t) && !isnan(h)) {
        tempC = t;
        humRH = h;
        valid = true;
      } else {
        // hold_last_valid for io_1/io_2 and io_3
        // (i.e., do not change tempC/humRH/valid)
      }
    }
  }
};

static DhtInputs dht_in;

// -------------------- Controller state (ACCEPTED model) --------------------
static int32_t menuMode = 1;           // i32, initial 1
static bool serialEnabled = false;     // bool, initial false
static bool previousPressed = false;   // bool, initial false

// -------------------- Timing / control period --------------------
static const uint32_t CONTROL_PERIOD_MS = 5;

// -------------------- Utility: TM1637 display mapping --------------------
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void write_tm1637_number(int32_t value);
#line 162 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serial_report(bool trigger, int32_t modeId, float tempC, float humRH, bool dht_valid);
#line 181 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_026\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void write_tm1637_number(int32_t value) {
  // Frozen requirement: io_4 is integer, invalid_policy blank; but controller only outputs 0 when invalid or selection not match.
  // We'll directly show integer.
  tm1637.showNumberDec(value, false, 0); // leading_zero = false, (4th arg is duration/point depending on library version)
}

// -------------------- Serial reporting (outgoing only) --------------------
static bool lastTrigger = false;

// Format is not specified in contract; implement deterministic simple line report.
// Since contract only freezes values and trigger, we preserve the semantics by printing all bound values when triggered.
static void serial_report(bool trigger, int32_t modeId, float tempC, float humRH, bool dht_valid) {
  if (!trigger) return;

  // Provide a stable, parseable output:
  // "TEMP=<value>,HUMI=<value>,DHT_VALID=<0/1>,MODE_ID=<id>"
  Serial.print("TEMP=");
  Serial.print(tempC, 2);

  Serial.print(",HUMI=");
  Serial.print(humRH, 2);

  Serial.print(",DHT_VALID=");
  Serial.print(dht_valid ? 1 : 0);

  Serial.print(",MODE_ID=");
  Serial.println(modeId);
}

// -------------------- setup/loop implementing tick at 5ms --------------------
void setup() {
  pinMode(PIN_D2, INPUT); // hardware contract says pullup=false and active_high; external wiring assumed
  serial_in.begin(SERIAL_BAUD);
  dht_in.begin();

  tm1637.setBrightness(7); // brightness=7
  // Initialize display to 0
  write_tm1637_number(0);
}

void loop() {
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();

  // Ensure fixed 5ms control period
  if (now - lastTickMs < CONTROL_PERIOD_MS) return;
  lastTickMs += CONTROL_PERIOD_MS;

  // -------------------- Read inputs --------------------
  bool io_0 = digitalRead(PIN_D2) == HIGH;         // button pressed (active high)
  dht_in.poll();
  float io_1 = dht_in.tempC;                      // degC
  float io_2 = dht_in.humRH;                      // %RH
  bool io_3 = dht_in.valid;                       // DHT validity

  serial_in.poll(20);                            // idle_timeout_ms=20
  int32_t io_5 = serial_in.value;                // command mapped ID
  bool io_5_valid = serial_in.valid;

  // -------------------- Controller model updates (ordered, deterministic) --------------------
  // update: if io_5_valid then menuMode := io_5
  if (io_5_valid) {
    menuMode = io_5;
  }

  // update: if (io_0 and not previousPressed) then serialEnabled := (if serialEnabled then false else true)
  if (io_0 && (!previousPressed)) {
    serialEnabled = (!serialEnabled);
  }

  // update: previousPressed := io_0
  previousPressed = io_0;

  // -------------------- Controller outputs (exact expressions) --------------------
  // io_4: if menuMode==3 then if io_3 then floor(io_1*10.0) else 0
  //      else if menuMode==2 then if io_3 then floor(io_2*10.0) else 0
  //      else if io_3 then floor(io_1*10.0) else 0
  int32_t io_4 = 0;
  if (menuMode == TEMP_ID) { // 3
    if (io_3) io_4 = (int32_t)floor(io_1 * 10.0);
    else io_4 = 0;
  } else if (menuMode == HUMI_ID) { // 2
    if (io_3) io_4 = (int32_t)floor(io_2 * 10.0);
    else io_4 = 0;
  } else { // menuMode == 1 (BOTH) or any other unexpected
    if (io_3) io_4 = (int32_t)floor(io_1 * 10.0);
    else io_4 = 0;
  }
  write_tm1637_number(io_4);

  // serial_0_trigger = serialEnabled and io_5_valid
  bool serial_0_trigger = serialEnabled && io_5_valid;

  // serial_0_value_0: if menuMode==3 then if io_3 then io_1 else 0.0
  //                 else if menuMode==1 then if io_3 then io_1 else 0.0
  //                 else 0.0
  float serial_0_value_0 = 0.0f;
  if (menuMode == TEMP_ID) {
    serial_0_value_0 = io_3 ? io_1 : 0.0f;
  } else if (menuMode == BOTH_ID) {
    serial_0_value_0 = io_3 ? io_1 : 0.0f;
  } else {
    serial_0_value_0 = 0.0f;
  }

  // serial_0_value_1: if menuMode==2 then if io_3 then io_2 else 0.0
  //                 else if menuMode==1 then if io_3 then io_2 else 0.0
  //                 else 0.0
  float serial_0_value_1 = 0.0f;
  if (menuMode == HUMI_ID) {
    serial_0_value_1 = io_3 ? io_2 : 0.0f;
  } else if (menuMode == BOTH_ID) {
    serial_0_value_1 = io_3 ? io_2 : 0.0f;
  } else {
    serial_0_value_1 = 0.0f;
  }

  // serial_0_value_2: if io_3 then true else false  (i.e., io_3)
  bool serial_0_value_2 = io_3 ? true : false;

  // serial_0_value_3: menuMode
  int32_t serial_0_value_3 = menuMode;

  // Report only on trigger gate
  serial_report(serial_0_trigger, serial_0_value_3, serial_0_value_0, serial_0_value_1, serial_0_value_2);

  lastTrigger = serial_0_trigger;
}

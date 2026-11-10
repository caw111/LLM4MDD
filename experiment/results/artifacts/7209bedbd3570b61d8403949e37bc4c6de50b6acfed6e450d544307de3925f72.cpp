#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Pins (authoritative)
static constexpr uint8_t PIN_DHT11 = D7;     // D7
static constexpr uint8_t PIN_HEATER = D10;  // D10
static constexpr uint8_t PIN_FAN = D5;      // D5
static constexpr uint8_t PIN_BUZZER = D6;   // D6 (tone output)
static constexpr uint8_t PIN_BUTTON = D2;   // D2
static constexpr uint8_t PIN_LED = D13;      // D13

// TM1637 (resolved)
static constexpr uint8_t PIN_TM_CLK = D4;   // D4
static constexpr uint8_t PIN_TM_DIO = D3;   // D3

// DHT11 (shared validity + temp/humidity on same pin)
static constexpr uint8_t DHT_TYPE = DHT11;

// --------- Model state (exact) ----------
static bool prevPressed = false;
static bool modeIsSetting = false;
static float targetTemp = 37.0f;
static float tempLastValid = 25.0f;
static bool alarmActive = false;
static int32_t abnormalHighCount = 0;
static int32_t abnormalLowCount = 0;
static bool fanOn = false;

// --------- Input ports ----------
static bool io_0 = false; // button pressed
static float io_5 = 0.0f; // temperature
static float io_6 = 0.0f; // humidity (unused by model)
static bool io_7 = false; // dht_valid

// --------- Output ports ----------
static bool io_1 = false; // heater relay
static bool io_2 = false; // LED
static bool io_3 = false; // fan
static int32_t io_4 = 0; // tone enable/frequency gate (model outputs 2000 or 0)
static int32_t io_8 = 0; // tm1637 number
static bool serial_0_trigger = false;
static bool serial_0_value_0 = false;

// Serial reporting (as contract uses trigger)
static bool lastSerialReported = false;
static bool lastSerialTrigger = false;

// DHT + TM1637
static DHT dht(PIN_DHT11, DHT_TYPE);
static TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

// --------- Timing helpers ----------
static unsigned long lastControlTickMs = 0;
static unsigned long lastDhtReadMs = 0;
static constexpr uint32_t DHT_SAMPLE_PERIOD_MS = 2000; // as contract
static uint32_t dhtReadFailHoldErrors = 0;

// floor function as in model: RealFunctions::floor
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t floor_int(float x);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void readDhtInputs();
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void readButtonInput();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void modelTick();
#line 177 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 196 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void handleSerialReport();
#line 213 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t floor_int(float x) {
  // For non-negative temperatures this matches floor.
  // Use std::floor for correctness.
  return (int32_t)floorf(x);
}

static void readDhtInputs() {
  // Contract: invalid_policy = hold_last_valid for temp/humidity and dht_valid on D7.
  // We'll implement:
  // - Attempt read; if valid: update io_5/io_6 and set io_7=true
  // - If invalid: keep previous io_5/io_6 and set io_7=false? The model uses io_7 to gate,
  //   so we must not treat invalid as valid. The invalid_policy says hold_last_valid on io_7 too.
  //   That means io_7 should remain at last value (true/false) instead of forcing false.
  // Contract explicitly: "invalid_policy": "hold_last_valid" for dht_valid as well.
  // Therefore: if read fails, do NOT change io_5/io_6/io_7.
  static float lastTemp = tempLastValid;
  static float lastHum = 0.0f;
  static bool lastValid = false;

  unsigned long now = millis();
  (void)now;

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  bool valid = !(isnan(h) || isnan(t));

  if (valid) {
    io_5 = t;
    io_6 = h;
    io_7 = true;
    lastTemp = io_5;
    lastHum = io_6;
    lastValid = io_7;
  } else {
    // hold last valid values
    io_5 = lastTemp;
    io_6 = lastHum;
    io_7 = lastValid;
    dhtReadFailHoldErrors++;
    (void)dhtReadFailHoldErrors;
  }
}

static void readButtonInput() {
  // Button active-high with pullup disabled per contract.
  // We assume hardware wiring matches: pressed => HIGH.
  io_0 = (digitalRead(PIN_BUTTON) == HIGH);
}

static void modelTick() {
  // ===== Updates (ordered) =====

  // 1) if io_7 then tempLastValid := io_5
  if (io_7) {
    tempLastValid = io_5;
  }

  // 2) if io_0 and not prevPressed then toggle modeIsSetting and assign targetTemp based on pre-update modeIsSetting
  if (io_0 && (!prevPressed)) {
    bool oldModeIsSetting = modeIsSetting;
    modeIsSetting = !modeIsSetting;
    // targetTemp := (if modeIsSetting then false else true) ? 37.0 : 35.0
    // Note: In the SysML snippet, the condition uses "modeIsSetting" after assignment? The generated formula is:
    // targetTemp := (if (if modeIsSetting ? false else true) ? 37.0 else 35.0)
    // which simplifies to: condition = (!modeIsSetting), so if (!modeIsSetting) then 37.0 else 35.0
    // But since modeIsSetting has just been toggled, the targetTemp depends on the new modeIsSetting value.
    // We must follow the given generated code semantics:
    // targetTemp = (modeIsSetting ? 35.0 : 37.0)
    targetTemp = (modeIsSetting ? 35.0f : 37.0f);
    (void)oldModeIsSetting;
  }

  // 3) prevPressed := io_0
  prevPressed = io_0;

  // 4) abnormalHighCount update
  if (io_7 && (tempLastValid >= 38.0f)) {
    abnormalHighCount = abnormalHighCount + 1;
  } else {
    abnormalHighCount = 0;
  }

  // 5) abnormalLowCount update
  if (io_7 && (tempLastValid <= 36.0f)) {
    abnormalLowCount = abnormalLowCount + 1;
  } else {
    abnormalLowCount = 0;
  }

  // 6) alarmActive based on counters >= 6000
  if ((abnormalHighCount >= 6000) || (abnormalLowCount >= 6000)) {
    alarmActive = true;
  } else {
    alarmActive = false;
  }

  // 7) fanOn update
  if ((!alarmActive) && io_7) {
    fanOn = (!fanOn);
  } else {
    fanOn = false;
  }

  // ===== Outputs (exact) =====
  io_1 = (alarmActive ? false : (io_7 && (tempLastValid < targetTemp)));
  io_2 = alarmActive;
  io_3 = (fanOn && (!alarmActive));
  io_4 = (alarmActive ? 2000 : 0);
  io_8 = (modeIsSetting ? floor_int(targetTemp) : floor_int(tempLastValid));

  serial_0_trigger = alarmActive;
  serial_0_value_0 = alarmActive;
}

static void applyOutputs() {
  digitalWrite(PIN_HEATER, io_1 ? HIGH : LOW);
  digitalWrite(PIN_LED, io_2 ? HIGH : LOW);
  digitalWrite(PIN_FAN, io_3 ? HIGH : LOW);

  // tone_output invalid_policy = silence
  if (io_4 != 0) {
    tone(PIN_BUZZER, (unsigned int)io_4);
  } else {
    noTone(PIN_BUZZER);
  }

  // TM1637 output brightness = 0, leading_zero=false, invalid_policy=blank
  // Model always provides io_8 (int). We'll display it.
  // Brightness=0 may dim/off depending on library; per contract brightness=0.
  tm1637.setBrightness(0);
  tm1637.showNumberDec(io_8, false, 4, 0);
}

static void handleSerialReport() {
  // Contract: serial report is gated by serial_0_trigger (alarmActive).
  // We'll emit a line only when trigger is true and value changes (avoid spamming).
  // Replace this with fixed string if protocol is known; otherwise keep minimal typed boolean.
  if (serial_0_trigger) {
    if (!lastSerialTrigger || (serial_0_value_0 != lastSerialReported)) {
      // "incubation abnormal" requirement: output that phrase.
      // Since the protocol isn't specified precisely, we output a fixed message line.
      Serial.println(F("incubation abnormal"));
      lastSerialReported = serial_0_value_0;
      lastSerialTrigger = true;
    }
  } else {
    lastSerialTrigger = false;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_BUTTON, INPUT);       // contract default; no pullup override
  pinMode(PIN_HEATER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Initialize outputs to model initial values (all false/0)
  digitalWrite(PIN_HEATER, LOW);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_FAN, LOW);
  noTone(PIN_BUZZER);

  // TM1637 init
  tm1637.setBrightness(0);
  tm1637.clear();

  // DHT init
  dht.begin();

  // Initialize DHT inputs as per model initial:
  // io_5 uses hold_last_valid; set to initial tempLastValid
  io_5 = tempLastValid;
  io_6 = 0.0f;
  io_7 = false;

  lastDhtReadMs = millis();
  lastControlTickMs = millis();
}

void loop() {
  unsigned long now = millis();

  // Read DHT at its sample period (but model uses io_7 gating every control step)
  if ((now - lastDhtReadMs) >= DHT_SAMPLE_PERIOD_MS) {
    lastDhtReadMs = now;
    readDhtInputs();
  }

  // Control tick at 10ms
  if ((now - lastControlTickMs) >= CONTROL_PERIOD_MS) {
    lastControlTickMs += CONTROL_PERIOD_MS;

    readButtonInput();

    modelTick();
    applyOutputs();
    handleSerialReport();
  }
}

#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// ===== Frozen interface contract (pins/protocols) =====
static constexpr uint32_t SERIAL_BAUD = 9600;

// DHT11 (D7)
static constexpr uint8_t PIN_DHT = 7;
static constexpr uint8_t DHT_TYPE = DHT11;

// TM1637 (CLK=D4, DIO=D5)
static constexpr uint8_t PIN_TM1637_CLK = 4;
static constexpr uint8_t PIN_TM1637_DIO = 5;

// LED D13
static constexpr uint8_t PIN_LED = 13;

// ===== Hardware adapters / minimal behavior-preserving helpers =====

// DHT read wrapper
static DHT dht(PIN_DHT, DHT_TYPE);
static float lastTempC = 0.0f;
static float lastHumRH  = 0.0f;

// Because model requires "invalid_policy: hold_last_valid" and "dht_* >=2000ms",
// we enforce a minimum interval of 2000ms between DHT reads.
static unsigned long lastDhtReadMs = 0;
static constexpr uint32_t DHT_MIN_SAMPLE_PERIOD_MS = 2000;

// Validity: io_3 true iff both temp and humidity are valid for this acquisition
static bool dhtValid = false;

// TM1637 display
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ===== ACCEPTED executable model implementation =====
// Control period: 5 ms (controller tick)
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// Model state
static bool prevValid = false;

// Model inputs (io_1, io_2, io_3)
static float io_1_degC = 0.0f;     // temperature (DHT)
static float io_2_rh = 0.0f;       // humidity (%RH)
static bool  io_3_valid = false;  // dht_valid

// Model outputs (io_0, io_4, serial_0_trigger, serial_0_value_0)
static bool io_0_led = false;
static int32_t io_4_tm1637Value = 0;
static bool serial_0_trigger = false;
static bool serial_0_value_0 = false;

// Additional internal: track DHT trigger edges for correctness (prevValid in model)
static unsigned long lastTickMs = 0;

#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyModelAndOutputs();
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void readDhtInputs_UpdateModelInputs();
#line 162 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 186 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyModelAndOutputs()
{
  // Controller tick corresponds to SysML model update semantics:
  // - prevValid updated from current io_3
  // - outputs computed from current io_3 and prevValid
  // Note: In the model bundle, updates occur inside tick with if io_3 then prevValid:=true else false.
  // Outputs are then based on current io_3 and prevValid as stored.
  // To preserve exact semantics, we must:
  // 1) compute updates to prevValid first (since it's updated in tick)
  // 2) compute outputs using resulting prevValid.
  //
  // However the provided oracle for T2 says serial_0_trigger fires when io_3 transitions true->false:
  // That implies serial_0_trigger depends on prevValid from the previous cycle (prevValid rising to true then invalid).
  // The model's expression uses prevValid (not explicitly "old prevValid"), but in synchronous semantics
  // for such bundle generation, prevValid is the registered value from previous cycle at the time outputs are evaluated.
  //
  // To match that behavior, we implement:
  // - oldPrevValid = prevValid (from previous tick)
  // - compute nextPrevValid from current io_3
  // - compute outputs using current io_3 and oldPrevValid
  // - then commit prevValid = nextPrevValid
  const bool oldPrevValid = prevValid;
  const bool nextPrevValid = io_3_valid;

  // Outputs per bundle:
  // io_0 = (if io_3 ? false else true)
  io_0_led = (io_3_valid ? false : true);

  // io_4 = (if io_3 ? floor(io_1) else 0)
  io_4_tm1637Value = (io_3_valid ? (int32_t)floorf(io_1_degC) : 0);

  // serial_0_trigger = (if (not io_3) and prevValid then true else false)
  serial_0_trigger = ((!io_3_valid) && oldPrevValid);

  // serial_0_value_0 = (if serial_0_trigger then true else false)
  serial_0_value_0 = serial_0_trigger;

  // Commit state update at end of tick
  prevValid = nextPrevValid;

  // Apply hardware outputs:
  // LED D13: true_level HIGH, false_level LOW
  digitalWrite(PIN_LED, io_0_led ? HIGH : LOW);

  // TM1637 display: invalid_policy=blank, but model contract says on sensor error display 0000.
  // So we write io_4_tm1637Value; on io_3_valid=false, controller sets it to 0 => "0000".
  // Note: leading_zero=false in contract; TM1637Display with showNumberDecEx will handle formatting.
  // We display exactly 4 digits with leading zeros when value==0 by using showNumberDecEx with leading zero enabled
  // for the 0 case. To preserve model intent ("0000"), we just show 4 digits always.
  // TM1637Display's showNumberDecEx signature:
  // showNumberDecEx(number, dots, leading_zero)
  tm1637.setBrightness(7);
  tm1637.showNumberDecEx(io_4_tm1637Value, 0x00, true);

  // Serial reporting gate: when serial_0_value_0 is true print fixed text "sensor error"
  // (lowercase, single space etc). Frozen contract requires fixed text.
  if (serial_0_value_0) {
    Serial.print(F("sensor error"));
    // No extra content: keep line_or_idle framing; add newline for Arduino line output consistency.
    // The contract says "fixed text" without extra values; newline is typically acceptable.
    // To be strict, do not add extra formatting other than an endline to actually send a line.
    // If strict no-newline is required, remove this. We'll keep a newline as common line framing.
    Serial.print('\n');
  }
}

static void readDhtInputs_UpdateModelInputs()
{
  // Enforce sample period >= 2000ms
  const unsigned long now = millis();
  if (now - lastDhtReadMs < DHT_MIN_SAMPLE_PERIOD_MS) {
    // Hold last valid policy for io_1/io_2/io_3: "invalid_policy: hold_last_valid"
    // But contract also includes io_3 validity; with hold_last_valid, io_3 would hold previous value.
    // The model bundle uses io_3 directly; for correctness with "hold_last_valid", we preserve last known dhtValid
    // and lastTempC/lastHumRH already stored.
    return;
  }

  lastDhtReadMs = now;

  // Read
  float t = dht.readTemperature();    // °C
  float h = dht.readHumidity();       // %RH

  // Validity: DHT11 may return NAN on failure.
  bool tempOk = !isnan(t);
  bool humOk  = !isnan(h);

  if (tempOk && humOk) {
    lastTempC = t;
    lastHumRH = h;
    dhtValid = true;
  } else {
    // hold last valid
    dhtValid = false; // For io_3 we need acquisition validity; frozen model expects invalid when either invalid.
    // Keep lastTempC/lastHumRH as-is (hold last valid for io_1/io_2)
  }

  // Map to model inputs: io_1=temperature reading, io_2=humidity, io_3=dht acquisition validity
  io_1_degC = lastTempC;
  io_2_rh = lastHumRH;
  io_3_valid = dhtValid;
}

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.begin(SERIAL_BAUD);

  // Initialize DHT and TM1637
  dht.begin();

  tm1637.setBrightness(7);
  // Initial display: consistent with model initial prevValid=false and io_3_valid derived after first read.
  tm1637.showNumberDecEx(0, 0x00, true);

  // Controller tick schedule
  lastTickMs = millis();

  // Initialize model inputs to safe defaults
  io_1_degC = 0.0f;
  io_2_rh = 0.0f;
  io_3_valid = false;
  prevValid = false;
}

void loop()
{
  // Periodic controller tick at 5ms
  const unsigned long now = millis();
  if (now - lastTickMs >= CONTROL_PERIOD_MS) {
    lastTickMs += CONTROL_PERIOD_MS;

    // Update DHT inputs (sample period enforced inside)
    readDhtInputs_UpdateModelInputs();

    // Run model and apply outputs
    applyModelAndOutputs();
  }
}

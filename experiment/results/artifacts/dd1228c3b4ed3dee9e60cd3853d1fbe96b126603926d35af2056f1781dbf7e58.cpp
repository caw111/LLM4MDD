#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// ---------------- Frozen interface contract pins ----------------
static const uint8_t PIN_DHT   = 7; // D7
static const uint8_t PIN_FAN   = 5; // D5 (PWM output)
static const uint8_t PIN_TM1637_CLK = 4; // D4
static const uint8_t PIN_TM1637_DIO = 3; // D3 (resolved remap)
static const uint8_t PIN_BTN   = 2; // D2 (digital input, pullup enabled, active_high=false)

// ---------------- Model semantics (control_period_ms=10) ----------------
static const uint32_t CONTROL_PERIOD_MS = 10;
static const uint32_t DHT_SAMPLE_PERIOD_MS = 2000;

// ---------------- DHT11 setup ----------------
#define DHTTYPE DHT11
static DHT dht(PIN_DHT, DHTTYPE);

// ---------------- TM1637 setup ----------------
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ---------------- Accepted controller state ----------------
static bool displayHum = false;       // io_4 output selection
static bool previousPressed = false; // internal memory of button input

// ---------------- I/O variables (controller inputs/outputs) ----------------
static double io_0 = 0.0; // temperature (degC)
static double io_1 = 0.0; // humidity (%RH)
static bool   io_2 = false; // dht_valid
static bool   io_5 = false; // button boolean: model "true" means "pressed" (LOW on pin)

// output io_3: pwm_count (f64 in model), scaled directly to analogWrite 0..255
static double io_3 = 0.0;
// output io_4: tm1637_number (i32 in model), shown as integer (no decimals)
static int32_t io_4 = 0;

// ---------------- Model evaluation (exact expression semantics) ----------------
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double fanPwmCount(double tempC);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t floorReal(double x);
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t tmNumber(bool useHum, bool validHum, double tempC, double humPct);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateButtonInput();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateDhtInputsIfDue(uint32_t nowMs);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick();
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 164 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline double fanPwmCount(double tempC) {
  // io_3 per model:
  // (if temp >= 30 then (if temp <= 30 then 0 else (if temp < 60 then (((temp-30)*255)*(30/1)) else 255)) else 0)
  if (tempC >= 30.0) {
    if (tempC <= 30.0) {
      return 0.0;
    } else {
      if (tempC < 60.0) {
        // (((temp-30) * 255) * (30/1)) => ((temp-30) * 255 * 30)
        return (((tempC - 30.0) * 255.0) * (30.0 / 1.0));
      } else {
        return 255.0;
      }
    }
  }
  return 0.0;
}

static inline int32_t floorReal(double x) {
  // RealFunctions::floor
  int32_t fx = (int32_t)x;
  if ((double)fx > x) fx -= 1;
  return fx;
}

static inline int32_t tmNumber(bool useHum, bool validHum, double tempC, double humPct) {
  // io_4 per model:
  // if displayHum then (if io_2 then floor(io_1) else floor(io_1)) else (if io_2 then floor(io_0) else floor(io_0))
  // Note: io_2 doesn't change result since both branches floor(io_1) or floor(io_0).
  (void)validHum;
  if (useHum) return floorReal(humPct);
  return floorReal(tempC);
}

// ---------------- IO adapters ----------------
static inline void updateButtonInput() {
  // Frozen contract: active_high=false, pullup=true, true_level=LOW
  // So io_5 (model true means "pressed") if digitalRead==LOW.
  io_5 = (digitalRead(PIN_BTN) == LOW);
}

static void updateDhtInputsIfDue(uint32_t nowMs) {
  static uint32_t lastSampleMs = 0;
  if (nowMs - lastSampleMs < DHT_SAMPLE_PERIOD_MS) return;
  lastSampleMs = nowMs;

  // DHT11 reading
  // If invalid, hold last valid (invalid_policy=hold_last_valid) for io_0/io_1 and io_2=false indicates invalid.
  // Since dht.read* returns NaN on failure, we use that.
  float t = dht.readTemperature(); // Celsius
  float h = dht.readHumidity();

  bool valid = !(isnan(t) || isnan(h));
  if (valid) {
    io_0 = (double)t;
    io_1 = (double)h;
    io_2 = true;
  } else {
    // invalid_policy=hold_last_valid
    // Keep io_0/io_1 unchanged; set io_2=false.
    io_2 = false;
  }
}

static inline void applyOutputs() {
  // io_3 to PWM output on D5
  // Model output is f64 pwm_count: in practice should fit 0..255.
  // We'll clamp to [0,255] to safely map to analogWrite.
  double raw = io_3;
  if (raw < 0.0) raw = 0.0;
  if (raw > 255.0) raw = 255.0;
  analogWrite(PIN_FAN, (int)raw);

  // io_4 to TM1637 (blank policy for invalid policy is handled by always showing here).
  // model invalid_policy=blank, initial=0. The model always produces io_4; we show it.
  // No decimal digits in model (floor), so display integer.
  tm1637.showNumberDec(io_4, false, 0);
}

// ---------------- Controller tick (exact ordered updates) ----------------
static inline void controllerTick() {
  // Updates ordered as in model bundle:
  // 1) if (io_5 and not previousPressed) then displayHum := not displayHum
  // 2) previousPressed := io_5

  if (io_5 && (!previousPressed)) {
    displayHum = !displayHum;
  }
  previousPressed = io_5;

  // Outputs computed from current state + inputs (as in out attributes)
  io_3 = fanPwmCount(io_0);

  io_4 = tmNumber(displayHum, io_2, io_0, io_1);
}

// ---------------- Arduino lifecycle ----------------
void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);

  pinMode(PIN_FAN, OUTPUT);
  analogWrite(PIN_FAN, 0);

  dht.begin();

  tm1637.setBrightness(7);
  tm1637.clear();
  // initial=0, invalid_policy=blank => show 0 at boot for deterministic behavior.
  io_4 = 0;
  tm1637.showNumberDec(io_4, false, 0);

  // io_0/io_1/io_2 initial: per contract initial 0; io_2 initial false
  io_0 = 0.0;
  io_1 = 0.0;
  io_2 = false;

  displayHum = false;
  previousPressed = false;

  io_5 = false;

  io_3 = 0.0;
  applyOutputs();
}

void loop() {
  static uint32_t lastControlMs = 0;
  uint32_t nowMs = millis();

  if (nowMs - lastControlMs < CONTROL_PERIOD_MS) return;
  lastControlMs += CONTROL_PERIOD_MS;

  // sample inputs each control tick (buttons at 10ms)
  updateButtonInput();
  // sample DHT at its own period (2000ms) but within same loop
  updateDhtInputsIfDue(nowMs);

  // controller tick (updates with exact order)
  controllerTick();

  // apply outputs
  applyOutputs();
}

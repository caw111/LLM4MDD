#include <Arduino.h>
#include <TM1637Display.h>
#include "DHT.h"

using namespace std;

// Frozen control period (ms) = 10
static const uint32_t CONTROL_PERIOD_MS = 10;

// Hardware pin bindings (authoritative)
static const uint8_t PIN_DHT11 = 7;   // D7
static const uint8_t PIN_LED = 13;    // D13
static const uint8_t PIN_BUZZER = 6;  // D6 (tone output)
static const uint8_t PIN_TM_CLK = 4;  // D4
static const uint8_t PIN_TM_DIO = 5;  // D5
static const uint8_t PIN_POT = A0;    // A0

// DHT11
static const uint8_t DHT_TYPE = DHT11;
static DHT dht(PIN_DHT11, DHT_TYPE);

// TM1637
static TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);
static const uint8_t TM_BRIGHTNESS = 4;

// Model I/O aliases (per frozen interface contract)
static double io_0_adc_count = 0.0; // thresholdDegC source (from A0), unit adc_count in contract
static bool io_5_dht_valid = false; // DHT valid (joint)
static double io_3_temp_degC = 0.0; // current temperature for comparison

// Model state (ACCEPTED executable model)
static double thresholdDegC = 0.0;   // initial 0.0
static bool prevTempValid = false;   // initial false (not used elsewhere, but implemented exactly)

// Model outputs (io_1..io_6)
static bool io_1_led_alarm = false;
static int32_t io_2_buzzer_indicator_hz = 0; // unit Hz, but contract semantics: 1000 or 0
static int32_t io_6_tm_indicator = 0;        // 1: alarm display selection indicator, else 0 (used for display content switching)

// ADC scaling / calibration (contract left unspecified)
// To preserve "exact controller state" relative to model, map adc_count directly to degC value.
// That is: thresholdDegC := io_0 (no scale). This matches the model's assignment without additional parameters.
static inline double readPotAsAdcCount() {
  // 10-bit ADC: 0..1023
  return (double)analogRead(PIN_POT);
}

// DHT sampling constraints: must be >=2000ms.
// We'll read at least every 2000ms.
static const uint32_t DHT_MIN_SAMPLE_MS = 2000;
static uint32_t lastDhtSampleMs = 0;
static bool lastDhtHasValidTemp = false;
static double lastDhtTempC = 0.0;

static inline void sampleDhtIfDue(uint32_t nowMs) {
  if (nowMs - lastDhtSampleMs < DHT_MIN_SAMPLE_MS) return;
  lastDhtSampleMs = nowMs;

  float t = dht.readTemperature(); // in Celsius
  // invalid_policy for io_3/io_4/io_5: hold_last_valid for invalid data
  if (!isnan(t)) {
    lastDhtTempC = (double)t;
    lastDhtHasValidTemp = true;
  }
  // io_5_dht_valid semantics: valid if both temperature and humidity valid.
  // Contract says "温度/湿度共同有效". Humidity not used for business logic, but validity requires it.
  float h = dht.readHumidity();
  bool humidityValid = !isnan(h);

  // Hold last valid policies:
  // If temperature invalid but humidity valid, io_5 should be false because "共同有效".
  // If humidity invalid but temperature valid, io_5 should also be false.
  // If either invalid, io_5 false; but io_3 should hold last valid.
  bool bothValidNow = lastDhtHasValidTemp && humidityValid;

  io_3_temp_degC = lastDhtTempC; // hold last valid temperature
  io_5_dht_valid = bothValidNow;
}

// TM1637 display strategy: "display current temperature and threshold"
static void displayNonAlarmOrAlarmSelection() {
  // Frozen contract: io_6 indicates display selection (1 alarm condition selection, else 0).
  // We implement a deterministic policy consistent with "显示当前温度与阈值":
  // - When io_6==1 (alarm): show temperature on left digit set (we show temp in °C).
  // - When io_6==0 and io_5 valid: show thresholdDegC on TM1637 for parameter visibility.
  // - When io_5==false: blank (invalid_policy: "blank").
  // This preserves deterministic behavior across control ticks.
  tm1637.setBrightness(TM_BRIGHTNESS);

  if (!io_5_dht_valid) {
    // invalid_policy blank
    tm1637.clear();
    return;
  }

  // TM1637Display supports showNumberDecEx for integers.
  // We display integer part in degC. (Model uses Real comparisons; display choice not specified,
  // so use deterministic rounding toward zero for stable behavior.)
  int32_t tempInt = (int32_t) (io_3_temp_degC >= 0.0 ? floor(io_3_temp_degC) : ceil(io_3_temp_degC));
  int32_t thrInt  = (int32_t) (thresholdDegC >= 0.0 ? floor(thresholdDegC) : ceil(thresholdDegC));

  int32_t toShow = (io_6_tm_indicator == 1) ? tempInt : thrInt;

  // Colon/leading zero control: leading_zero false.
  tm1637.showNumberDec(toShow, false);
}

static inline void modelTick() {
  // Model updates (ACCEPTED model, ordered)
  // Updates:
  // thresholdDegC := io_0
  thresholdDegC = io_0_adc_count;

  // if io_5 then prevTempValid := true else false
  if (io_5_dht_valid) prevTempValid = true;
  else prevTempValid = false;

  // Outputs:
  if (io_5_dht_valid) {
    io_1_led_alarm = (io_3_temp_degC > thresholdDegC);
    if (io_3_temp_degC > thresholdDegC) io_2_buzzer_indicator_hz = 1000;
    else io_2_buzzer_indicator_hz = 0;
    io_6_tm_indicator = io_1_led_alarm ? 1 : 0;
  } else {
    io_1_led_alarm = false;
    io_2_buzzer_indicator_hz = 0;
    io_6_tm_indicator = 0;
  }
}

static inline void applyOutputs() {
  // io_1: LED constant on when alarm condition selected; else low
  digitalWrite(PIN_LED, io_1_led_alarm ? HIGH : LOW);

  // io_2: beep frequency 1000Hz when alarm, else silence
  // invalid_policy "silence"
  if (io_2_buzzer_indicator_hz > 0) {
    tone(PIN_BUZZER, (unsigned int)io_2_buzzer_indicator_hz);
  } else {
    noTone(PIN_BUZZER);
  }

  // io_6: affects display selection strategy
  displayNonAlarmOrAlarmSelection();
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Initialize TM1637
  tm1637.setBrightness(TM_BRIGHTNESS);
  tm1637.clear();

  // Initialize DHT
  dht.begin();

  // Initialize model state per contract
  thresholdDegC = 0.0;
  prevTempValid = false;

  io_0_adc_count = 0.0;
  io_3_temp_degC = 0.0;
  io_5_dht_valid = false;

  io_1_led_alarm = false;
  io_2_buzzer_indicator_hz = 0;
  io_6_tm_indicator = 0;

  // Ensure initial outputs consistent (io_5 false => blank, LED off, buzzer silence)
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
  tm1637.clear();
}

void loop() {
  static uint32_t nextTickMs = 0;
  uint32_t nowMs = millis();

  if (nextTickMs == 0) nextTickMs = nowMs;

  // Maintain fixed control period of 10ms
  if ((int32_t)(nowMs - nextTickMs) < 0) return;
  nextTickMs += CONTROL_PERIOD_MS;

  // Sample inputs consistent with frozen contract rates:
  // - io_0 adc sample_period_ms=10 => read every control tick
  io_0_adc_count = readPotAsAdcCount();

  // - io_3/io_4/io_5 sample_period_ms=2000 => handled by due-check
  sampleDhtIfDue(nowMs);

  // Run model tick with preserved exact state & update order
  modelTick();

  // Apply outputs: LED, buzzer, and TM1637 display selection
  applyOutputs();
}
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_SOIL_A0 = A0;
static const uint8_t PIN_RAIN_A1 = A1;
static const uint8_t PIN_WATER_A2 = A2;

static const uint8_t PIN_PUMP_D10 = 10;
static const uint8_t PIN_LED_D13   = 13;
static const uint8_t PIN_BUZZ_D6    = 6;

static const uint8_t PIN_BUTTON_D2 = 2;

static const uint8_t PIN_TM1637_CLK = 4;
static const uint8_t PIN_TM1637_DIO = 5;

static const unsigned long CONTROL_PERIOD_MS = 10;

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

struct ControllerModel {
  bool manualMode = false;              // initial false
  bool previousButtonPressed = false;  // initial false

  void tick(bool io_6_button) {
    // Update logic exactly as provided in Accepted executable model
    if (io_6_button) {
      if ((io_6_button) && (!previousButtonPressed)) {
        manualMode = !manualMode;
      }
    }
    previousButtonPressed = io_6_button;
  }

  // Outputs as defined in model
  bool io_3_pumpRelay() const { return false; }                 // always false in model
  bool io_4_statusLED() const { return manualMode; }           // manualMode
  int  io_5_buzzerHz() const { return 0; }                      // always 0
  int  io_7_tm1637Number() const { return manualMode ? 1 : 0; } // manualMode ? 1 : 0

  bool serial_0_trigger() const { return false; }               // always false
  float serial_0_value_0() const { return 0.0f; } // io_0 soil moisture ADC (set externally)
  float serial_0_value_1() const { return 0.0f; } // io_1 rain ADC
  float serial_0_value_2() const { return 0.0f; } // io_2 water level ADC
  bool  serial_0_value_3() const { return manualMode; }        // manualMode
  bool  serial_0_value_4() const { return false; }            // pump_allowed always false per model
};

static float g_io_0_soil = 0.0f;
static float g_io_1_rain = 0.0f;
static float g_io_2_water = 0.0f;

#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readButton();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateAnalogInputs();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(const ControllerModel& m);
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReporting(const ControllerModel& m);
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readButton() {
  // Contract: digital input, active_high true_level=HIGH, pullup false.
  // No explicit pullup/pulldown wired assumption; we configure as plain input.
  return digitalRead(PIN_BUTTON_D2) == HIGH;
}

static void updateAnalogInputs() {
  // Contract: f64 adc_count. Implement as raw ADC counts.
  // scale=1.0, offset=0.0
  g_io_0_soil = (float)analogRead(PIN_SOIL_A0);
  g_io_1_rain = (float)analogRead(PIN_RAIN_A1);
  g_io_2_water = (float)analogRead(PIN_WATER_A2);
}

static void applyOutputs(const ControllerModel& m) {
  // io_3 pump relay control: always false in model
  digitalWrite(PIN_PUMP_D10, m.io_3_pumpRelay() ? HIGH : LOW);

  // io_4 status LED
  digitalWrite(PIN_LED_D13, m.io_4_statusLED() ? HIGH : LOW);

  // io_5 buzzer tone: silence when 0
  // Model always outputs 0, so keep no tone.
  noTone(PIN_BUZZ_D6);

  // io_7 TM1637 number
  // Note: model only provides 0/1; requirement says cycle soil/water/mode but model dictates otherwise.
  display.showNumberDec(m.io_7_tm1637Number(), false);
}

static void serialReporting(const ControllerModel& m) {
  // Contract: serial_0_trigger is always false in model; do nothing.
  (void)m;
}

void setup() {
  pinMode(PIN_PUMP_D10, OUTPUT);
  pinMode(PIN_LED_D13, OUTPUT);
  pinMode(PIN_BUZZ_D6, OUTPUT);

  pinMode(PIN_BUTTON_D2, INPUT); // pullup false per contract

  // TM1637 initialization
  display.setBrightness(7);

  // Ensure outputs reflect initial model state (manualMode=false)
  ControllerModel init;
  applyOutputs(init);

  Serial.begin(9600);
}

void loop() {
  static ControllerModel model;

  static unsigned long last = 0;
  unsigned long now = millis();
  if (now - last < CONTROL_PERIOD_MS) return;
  last += CONTROL_PERIOD_MS;

  updateAnalogInputs();
  bool io_6 = readButton();

  // In model, outputs io_3/io_4/io_5/io_7 are derived solely from manualMode
  model.tick(io_6);

  applyOutputs(model);

  // Set serial values; trigger remains false, so typically no emission.
  serialReporting(model);
}

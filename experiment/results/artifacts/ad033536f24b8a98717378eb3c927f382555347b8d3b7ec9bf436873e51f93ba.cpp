#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ----- Hardware bindings (authoritative) -----
static const uint8_t PIN_LM35_A1 = A1;   // io_0
static const uint8_t PIN_MQ2_A0  = A0;   // io_1

static const uint8_t PIN_FLAME_D2 = 2;   // io_2
static const uint8_t PIN_BUZZER_D6 = 6;  // io_3 (tone_output)
static const uint8_t PIN_LED_GREEN_D12 = 12; // io_5
static const uint8_t PIN_LED_RED_D13   = 13;  // io_4

static const uint8_t TM1637_CLK_D4 = 4;  // io_6
static const uint8_t TM1637_DIO_D5 = 5;  // io_6

// ----- Control period -----
static const uint32_t CONTROL_PERIOD_MS = 5;

// ----- Model thresholds/semantics (frozen from ACCEPTED bundle) -----
static const float TH_ATT_TEMP60  = 60.0f;   // io_0 >= 60 -> attention
static const float TH_ATT_SMOKE550 = 550.0f;  // io_1 >= 550 -> attention

static const float TH_ALARM_TEMP80  = 80.0f;   // io_0 >= 80 -> alarm
static const float TH_ALARM_SMOKE750 = 750.0f;  // io_1 >= 750 -> alarm

// Buzzer tone output mapping: io_3 = toneRunning ? 2000 : 0 (Hz)
static const int BUZZER_TONE_HZ = 2000;

// ----- TM1637 -----
TM1637Display tm1637(TM1637_CLK_D4, TM1637_DIO_D5);

// ----- Controller state (must be preserved exactly) -----
static int32_t warningLevel = 0;
static bool prevFlame = false;     // retained, but not otherwise used in outputs
static bool toneRunning = false;  // model input for buzzer output

// ----- Utilities -----
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float readAnalogF64( uint8_t pin );
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readDigitalBoolInputActiveHigh(uint8_t pin);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void displayLevel(int32_t level);
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void reportSerial(bool trigger, int32_t level, float keyValue);
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float readAnalogF64( uint8_t pin ) {
  // Model ports io_0/io_1 are f64 in unit adc_count; use raw ADC counts [0..1023]
  // Keep as float for expression fidelity.
  return (float)analogRead(pin);
}

static inline bool readDigitalBoolInputActiveHigh(uint8_t pin) {
  // Model semantics: true_level = HIGH, pullup=false (configured below).
  return (digitalRead(pin) == HIGH);
}

// Display behavior: model says io_6 equals warningLevel (integer).
// tm1637_number: invalid_policy=blank; leading_zero=false; brightness=7.
// We'll always set number. For safety, treat as "blank" only if invalid; here never invalid.
static inline void displayLevel(int32_t level) {
  tm1637.setBrightness(7);
  tm1637.showNumberDec((int)level, false); // leading_zero=false
}

// Serial reporting gate: serial_0_trigger == (warningLevel==2)
static bool lastSerialTrigger = false;

// "fire alert" message in alarm level.
// Bundle doesn't specify exact message format; we keep fixed text only when gate true.
static void reportSerial(bool trigger, int32_t level, float keyValue) {
  if (!trigger) return;

  // Emit on each control tick while trigger holds to match continuous reporting intent.
  // Include required typed values: level and key_value (io_0 when alarm/attention, else 0).
  // Also include "fire alert" text as required.
  Serial.print(F("fire alert level="));
  Serial.print(level);
  Serial.print(F(" key_value="));
  Serial.println(keyValue, 3);
}

// ----- Main loop timing -----
static uint32_t nextTick = 0;

void setup() {
  // Serial: 9600, line_or_idle, idle_timeout_ms=20.
  Serial.begin(9600);

  // Inputs
  pinMode(PIN_FLAME_D2, INPUT); // pullup=false => INPUT

  // Outputs
  pinMode(PIN_LED_RED_D13, OUTPUT);
  pinMode(PIN_LED_GREEN_D12, OUTPUT);
  // Buzzer D6 managed by tone(); no explicit pinMode required but set anyway.
  pinMode(PIN_BUZZER_D6, OUTPUT);

  // TM1637 init
  tm1637.setBrightness(7);
  displayLevel(0);

  // Initialize controller state per model initial values
  warningLevel = 0;
  prevFlame = false;
  toneRunning = false;

  // Initial outputs per model (io_3..io_6 etc.)
  digitalWrite(PIN_LED_RED_D13, LOW);    // io_4 false
  digitalWrite(PIN_LED_GREEN_D12, LOW);  // io_5 false
  noTone(PIN_BUZZER_D6);
  displayLevel(warningLevel);

  nextTick = millis();
}

void loop() {
  const uint32_t now = millis();
  if ((int32_t)(now - nextTick) < 0) return;

  // Advance to preserve 5ms granularity scheduling
  nextTick += CONTROL_PERIOD_MS;

  // --------- Inputs (sample period modeled as 5ms) ---------
  const float io_0 = readAnalogF64(PIN_LM35_A1);   // LM35 adc_count
  const float io_1 = readAnalogF64(PIN_MQ2_A0);    // MQ-2 adc_count
  const bool  io_2 = readDigitalBoolInputActiveHigh(PIN_FLAME_D2); // flame detected

  // --------- Model action tick (must match expression structure) ---------
  // begin: start
  // then assign warningLevel := 0
  warningLevel = 0;

  // then if ((io_0>=60) OR (io_1>=550)) warningLevel:=1
  if ((io_0 >= TH_ATT_TEMP60) || (io_1 >= TH_ATT_SMOKE550)) {
    warningLevel = 1;
  }

  // then if ((io_0>=80) OR (io_1>=750) OR (io_2 AND ( (io_0>=60) OR (io_1>=550) ))) warningLevel:=2
  if ((io_0 >= TH_ALARM_TEMP80) ||
      (io_1 >= TH_ALARM_SMOKE750) ||
      (io_2 && ((io_0 >= TH_ATT_TEMP60) || (io_1 >= TH_ATT_SMOKE550)))) {
    warningLevel = 2;
  }

  // then if (((warningLevel==2)?1:0)==1) toneRunning:=true else false
  if ((warningLevel == 2 ? 1 : 0) == 1) toneRunning = true;
  else toneRunning = false;

  // then assign prevFlame := io_2
  prevFlame = io_2;

  // --------- Outputs (exact expressions from model) ---------
  // io_3: toneRunning ? 2000 : 0
  if (toneRunning) tone(BUZZER_TONE_HZ); else noTone();

  // io_4: (warningLevel==2)
  digitalWrite(PIN_LED_RED_D13, (warningLevel == 2) ? HIGH : LOW);

  // io_5: (warningLevel==1)
  digitalWrite(PIN_LED_GREEN_D12, (warningLevel == 1) ? HIGH : LOW);

  // io_6: warningLevel (number display)
  displayLevel(warningLevel);

  // serial_0_trigger: (warningLevel==2)
  const bool serial_0_trigger = (warningLevel == 2);

  // serial_0_value_0: warningLevel
  // serial_0_value_1:
  // if warningLevel==2 then io_0
  // else if warningLevel==1 then io_0
  // else 0.0
  const float serial_0_value_1 = (warningLevel == 2) ? io_0 : ((warningLevel == 1) ? io_0 : 0.0f);

  // Gate reporting by serial_0_trigger; preserve continuous intent in alarm level.
  // Emit only while trigger is true to follow "gate group".
  reportSerial(serial_0_trigger, warningLevel, serial_0_value_1);

  lastSerialTrigger = serial_0_trigger;
}

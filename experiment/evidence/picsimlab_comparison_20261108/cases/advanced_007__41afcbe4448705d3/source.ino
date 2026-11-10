#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Hardware bindings --------------------
static constexpr uint8_t PIN_LM35 = A1;   // io_0
static constexpr uint8_t PIN_DOOR = 2;   // io_1
static constexpr uint8_t PIN_RELAY = 10;  // io_2
static constexpr uint8_t PIN_BUZZER = 6;  // io_3 (tone_output)
static constexpr uint8_t PIN_TM_CLK = 4;  // io_4 (TM1637 CLK)
static constexpr uint8_t PIN_TM_DIO = 5;  // io_4 (TM1637 DIO)
static constexpr uint8_t PIN_LED = 13;    // io_5

// TM1637 config (frozen)
static constexpr uint8_t TM_BRIGHTNESS = 7;

// -------------------- ACCEPTED executable model state --------------------
static int32_t doorOpenMs = 0;          // doorOpenMs : i32
static bool prevDoorOpen = false;      // prevDoorOpen : bool (not used in update logic)
static int32_t doorOpenSeconds = 0;   // doorOpenSeconds : i32
static bool fanCoolRelayOn = false;    // fanCoolRelayOn : bool

// -------------------- Constants derived from model --------------------
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static float readLM35DegC_fromADC() {
  // Hardware contract: io_0 is "adc_count" but model comparisons are against degC constants (300, 280, 30).
  // The frozen contract requires LM35 conversion (LM35 scale/offset) to °C.
  // Standard UNO ADC:
  // - 10-bit ADC, Vref=5V => 1023 counts full-scale.
  // - LM35 outputs 10mV/°C => 0.01 V/°C.
  // Compute degC = (adc * 5.0 / 1023.0) / 0.01 = adc * 500.0 / 1023.0
  int raw = analogRead(PIN_LM35);
  float degC = (static_cast<float>(raw) * 500.0f) / 1023.0f;
  return degC;
}

static bool readDoorPressedClosed() {
  // Frozen contract: active_high=true, pullup=false, true_level=HIGH.
  // Model_semantics: pressed/closed contact corresponds to HIGH.
  return digitalRead(PIN_DOOR) == HIGH;
}

// floor(x) for non-negative values as in model: RealFunctions::floor
static int32_t floorToInt32_nonneg(float x) {
  if (x <= 0.0f) return 0;
  return static_cast<int32_t>(x);
}

static void setBuzzerTone(int32_t freqHz) {
  // io_3: invalid_policy silence; set tone frequency and stop behavior.
  // Model outputs 0 when not alarming.
  if (freqHz <= 0) {
    noTone(PIN_BUZZER);
  } else {
    tone(PIN_BUZZER, static_cast<unsigned int>(freqHz));
  }
}

// -------------------- TM1637 --------------------
TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

// -------------------- Serial protocol/text --------------------
// Frozen contract provides assumptions:
// serial_0 emits continuous i32 code while critical condition holds.
// serial_1 emits temperature and door-open seconds while not in critical cooling loss mode.
// We implement both streams as line-based, with clear labels, but the requirement
// did not specify exact formatting. To preserve "continuous output", we emit
// only when respective triggers are true, each as one line.
static void serialEmitLine(const __FlashStringHelper* prefix, float value) {
  Serial.print(prefix);
  Serial.print(value, 2);
  Serial.print('\n');
}
static void serialEmitLineI32(const __FlashStringHelper* prefix, int32_t value) {
  Serial.print(prefix);
  Serial.print(value);
  Serial.print('\n');
}

// -------------------- Model tick (10ms period) --------------------
static void controllerTick() {
  // Read inputs
  const float io_0 = readLM35DegC_fromADC();      // Real
  const bool io_1 = readDoorPressedClosed();     // bool

  // --- Updates (ordered exactly as in accepted model) ---
  // 1) if io_1 then doorOpenMs := doorOpenMs + 10 else doorOpenMs := 0
  if (io_1) doorOpenMs = doorOpenMs + static_cast<int32_t>(CONTROL_PERIOD_MS);
  else doorOpenMs = 0;

  // 2) doorOpenSeconds := floor(doorOpenMs / 1000.0) (else 0)
  // doorOpenMs is i32 and should be >= 0 always, but keep semantics.
  if (doorOpenMs >= 0) {
    float secs = static_cast<float>(doorOpenMs) / 1000.0f;
    doorOpenSeconds = static_cast<int32_t>(floorToInt32_nonneg(secs));
  } else {
    doorOpenSeconds = 0;
  }

  // 3) fanCoolRelayOn := fanCoolRelayOn (no-op)
  // (Intentionally nothing)

  // 4) if (io_0 >= 300.0 && !io_1) fanCoolRelayOn := true
  if ((io_0 >= 300.0f) && (!io_1)) {
    fanCoolRelayOn = true;
  }

  // 5) if (io_0 <= 280.0 && !io_1) fanCoolRelayOn := false
  if ((io_0 <= 280.0f) && (!io_1)) {
    fanCoolRelayOn = false;
  }

  // 6) if (io_1 && fanCoolRelayOn) fanCoolRelayOn := false
  if (io_1 && fanCoolRelayOn) {
    fanCoolRelayOn = false;
  }

  // 7) prevDoorOpen := io_1
  prevDoorOpen = io_1;

  // --- Outputs ---
  // io_2 = fanCoolRelayOn
  digitalWrite(PIN_RELAY, fanCoolRelayOn ? HIGH : LOW);

  // io_3 = (doorOpenMs > 10000 ? 2500 : 0)
  const int32_t buzFreq = (doorOpenMs > 10000 ? 2500 : 0);
  setBuzzerTone(buzFreq);

  // io_4 = if io_1 then floor(io_0) else floor(io_0)
  // (both branches identical per model)
  const int32_t tmVal = static_cast<int32_t>(floorToInt32_nonneg(io_0));
  tm1637.setBrightness(TM_BRIGHTNESS);
  tm1637.showNumberDec(tmVal, false); // leading_zero=false

  // io_5 = (doorOpenMs > 10000 ? true : false)
  digitalWrite(PIN_LED, (doorOpenMs > 10000) ? HIGH : LOW);

  // serial_0_trigger = (io_0 > 30.0 && !io_1)
  const bool serial_0_trigger = ((io_0 > 30.0f) && (!io_1));
  // serial_0_value_0 = 1
  if (serial_0_trigger) {
    serialEmitLineI32(F("critical_cooling_loss="), 1);
  }

  // serial_1_trigger = not( io_0 > 30.0 && !io_1 )
  const bool serial_1_trigger = !serial_0_trigger;
  if (serial_1_trigger) {
    // serial_1_value_0 : io_0 (f64 degC)
    // serial_1_value_1 : doorOpenSeconds (i32)
    Serial.print(F("temperature_degC="));
    Serial.print(io_0, 2);
    Serial.print(F(", door_open_seconds="));
    Serial.print(doorOpenSeconds);
    Serial.print('\n');
  }
}

// -------------------- Arduino setup/loop --------------------
void setup() {
  // Frozen serial config: baud=9600, line_or_idle, idle_timeout_ms=20
  Serial.begin(9600);

  // GPIO init
  pinMode(PIN_DOOR, INPUT); // pullup false as per contract
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_RELAY, LOW);
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

  // TM1637 init: brightness handled in tick
  tm1637.setBrightness(TM_BRIGHTNESS);
  tm1637.showNumberDec(0, false);

  // Initialize model states (as in bundle)
  doorOpenMs = 0;
  prevDoorOpen = false;
  doorOpenSeconds = 0;
  fanCoolRelayOn = false;
}

void loop() {
  static uint32_t nextTick = 0;
  static bool inited = false;
  if (!inited) {
    nextTick = millis();
    inited = true;
  }

  uint32_t now = millis();
  if ((int32_t)(now - nextTick) >= 0) {
    nextTick += CONTROL_PERIOD_MS;

    controllerTick();
  }
}
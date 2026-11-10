#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

// ===================== Hardware mapping (authoritative) =====================
static const uint8_t TRIG_ENTRY = D8;   // HC-SR04 entry trig
static const uint8_t ECHO_ENTRY = D9;   // HC-SR04 entry echo
static const uint8_t TRIG_EXIT  = D10;  // HC-SR04 exit trig
static const uint8_t ECHO_EXIT  = D11;  // HC-SR04 exit echo

static const uint8_t DHT_PIN = D7;      // DHT11 pin
static const uint8_t FAN_PWM_PIN = D5;  // Fan PWM output

static const uint8_t BTN_PIN = D2;      // display toggle button (active-high with pullup)

static const uint8_t TM1637_CLK = D4;   // TM1637 clock
static const uint8_t TM1637_DIO = D3;   // TM1637 data

// ===================== DHT11 =====================
#define DHTTYPE DHT11
static DHT dht(DHT_PIN, DHTTYPE);

// ===================== TM1637 =====================
static TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// ===================== Serial protocol settings =====================
static const unsigned long SERIAL_BAUD = 9600;

// ===================== Control period (contract) =====================
static const uint32_t CONTROL_PERIOD_MS = 10;

// ===================== Ultrasonic settings =====================
static const uint32_t ULTRASONIC_TIMEOUT_US = 30000UL;

// Samples rounding to control-period multiple handled by runtime: here we just gate trigger cadence lightly.
static const uint32_t IO0_IO2_SAMPLE_MS = 60;   // from contract
static const uint32_t IO4_IO5_SAMPLE_MS = 2000;

// ===================== Model state (exact) =====================
static int32_t peopleCount = 0;
static bool previousEntryPressed = false;
static bool displayTemperature = false;

// ===================== IO placeholders (inputs) =====================
static float io_0 = 0.0f;     // entry distance cm
static bool  io_1 = false;    // entry valid
static float io_2 = 0.0f;     // exit distance cm
static bool  io_3 = false;    // exit valid
static float io_4 = 0.0f;     // temperature degC
static float io_5 = 0.0f;     // humidity %RH
static bool  io_6 = false;    // dht valid
static bool  io_9 = false;    // button pressed (active-high with pullup)

// ===================== IO outputs (model outputs) =====================
static float io_7 = 0.0f;       // fan pwm_count (0..255)
static int32_t io_8 = 0;       // tm1637 number
static bool serial_0_trigger = true;
static int32_t serial_0_value_0 = 0;
static float serial_0_value_1 = 0.0f;
static bool serial_0_value_2 = false;

// ===================== Utilities =====================
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float clampFloat(float x, float lo, float hi);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint32_t microsEchoPulse(uint8_t trigPin, uint8_t echoPin, uint32_t timeoutUs);
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float distanceCmFromUs(uint32_t durationUs);
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateUltrasonicIO();
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateDhtIO();
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateButtonIO();
#line 147 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void modelTick();
#line 211 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t pwmTo255(float x);
#line 217 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 237 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 279 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_009\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float clampFloat(float x, float lo, float hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

static uint32_t microsEchoPulse(uint8_t trigPin, uint8_t echoPin, uint32_t timeoutUs) {
  // Trigger: 10us HIGH then measure echo pulse width (HIGH) with timeout.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pulseInLong returns 0 on timeout.
  unsigned long duration = pulseIn(echoPin, HIGH, timeoutUs);
  if (duration == 0) return 0;
  return (uint32_t)duration;
}

// Convert pulse width (us) to distance in cm for HC-SR04.
// distance = (duration * speed_of_sound) / 2
// Speed of sound approx 343 m/s at 20C => 0.0343 cm/us => distance(cm) = duration * 0.0343 / 2
static float distanceCmFromUs(uint32_t durationUs) {
  return (durationUs * 0.0343f) / 2.0f;
}

// ===================== IO update: ultrasonic =====================
static uint32_t lastUltrasonicSampleMs = 0;

static void updateUltrasonicIO() {
  uint32_t nowMs = millis();
  if (nowMs - lastUltrasonicSampleMs < IO0_IO2_SAMPLE_MS) return;
  lastUltrasonicSampleMs = nowMs;

  // Entry
  uint32_t durEntry = microsEchoPulse(TRIG_ENTRY, ECHO_ENTRY, ULTRASONIC_TIMEOUT_US);
  if (durEntry == 0) {
    // invalid policy: hold_last_valid
    // Keep io_0 and io_1 unchanged if already valid; if not, remain as-is.
  } else {
    io_0 = distanceCmFromUs(durEntry);
    io_1 = true;
  }

  // Exit
  uint32_t durExit = microsEchoPulse(TRIG_EXIT, ECHO_EXIT, ULTRASONIC_TIMEOUT_US);
  if (durExit == 0) {
    // invalid policy: hold_last_valid
  } else {
    io_2 = distanceCmFromUs(durExit);
    io_3 = true;
  }
}

// ===================== IO update: DHT11 =====================
static uint32_t lastDhtSampleMs = 0;

static void updateDhtIO() {
  uint32_t nowMs = millis();
  if (nowMs - lastDhtSampleMs < IO4_IO5_SAMPLE_MS) return;
  lastDhtSampleMs = nowMs;

  float t = dht.readTemperature(); // degC
  float h = dht.readHumidity();    // %RH

  // invalid_policy: hold_last_valid
  if (!isnan(t) && !isnan(h)) {
    io_4 = t;
    io_5 = h;
    io_6 = true;
  } else {
    // Keep last valid values; keep io_6 unchanged if already true, else false.
    // Contract says "invalid_policy": hold_last_valid.
  }
}

// ===================== IO update: button =====================
static void updateButtonIO() {
  // Active-high with pullup enabled (pressed=1).
  io_9 = (digitalRead(BTN_PIN) == HIGH);
}

// ===================== Model tick (exact behavior from bundle) =====================
static void modelTick() {
  // Controller.tick:
  // first start;
  // then if ((io_9) and ((not previousEntryPressed))) { ... } else { ... }
  if (io_9 && (!previousEntryPressed)) {
    displayTemperature = (!displayTemperature);
    previousEntryPressed = io_9;
  } else {
    previousEntryPressed = io_9;
  }

  // then if io_1 { if io_3 { peopleCount := (if io_0 <= 0.0 ? 0 else 0) } else { ... } } else { if !io_1 { ... } }
  if (io_1) {
    if (io_3) {
      peopleCount = (io_0 <= 0.0f ? 0 : 0);
    } else {
      peopleCount = (io_0 <= 0.0f ? 0 : 0);
    }
  }
  if (!io_1) {
    if (io_3) {
      peopleCount = 0;
    } else {
      peopleCount = 0;
    }
  }

  // then if (not io_6) { ... done; } else if io_6 { ... } but only "then done" behavior; no assignments in both cases
  if (!io_6) {
    // no-op
  } else {
    // no-op
  }

  // Outputs:
  // io_7 = (if peopleCount==0 ? 0.0 else if io_4>30 ? 255.0 else if io_4>25 ? 180.0 else if io_4>20 ? 120.0 else 60.0)
  if (peopleCount == 0) {
    io_7 = 0.0f;
  } else {
    if (io_4 > 30.0f) io_7 = 255.0f;
    else if (io_4 > 25.0f) io_7 = 180.0f;
    else if (io_4 > 20.0f) io_7 = 120.0f;
    else io_7 = 60.0f;
  }

  // io_8 = (if displayTemperature ? (if io_6 ? floor(io_4) else 0) else peopleCount)
  if (displayTemperature) {
    if (io_6) {
      io_8 = (int32_t)floorf(io_4);
    } else {
      io_8 = 0;
    }
  } else {
    io_8 = peopleCount;
  }

  // serial outputs
  serial_0_trigger = true;
  serial_0_value_0 = peopleCount;
  serial_0_value_1 = (io_6 ? io_4 : 0.0f);
  serial_0_value_2 = (peopleCount == 0);
}

// ===================== Apply outputs =====================
static uint8_t pwmTo255(float x) {
  // Contract says scale 1, offset 0; keep exact mapping, but clamp to valid PWM range.
  x = clampFloat(x, 0.0f, 255.0f);
  return (uint8_t)(x + 0.5f);
}

static void applyOutputs() {
  // Fan PWM
  analogWrite(FAN_PWM_PIN, pwmTo255(io_7));

  // TM1637 display: "invalid_policy blank", but model always drives io_8 deterministically.
  tm1637.showNumberDec(io_8, false, 4);

  // Serial: assumed periodic status-group. We'll emit on each control-period tick (gate by serial_0_trigger true).
  // Use newline-delimited group: people=<int>,temperature=<float>,eco=<0|1>
  if (serial_0_trigger) {
    Serial.print("people=");
    Serial.print(serial_0_value_0);
    Serial.print(",temperature=");
    Serial.print(serial_0_value_1, 2);
    Serial.print(",eco_mode=");
    Serial.println(serial_0_value_2 ? 1 : 0);
  }
}

// ===================== Arduino lifecycle =====================
void setup() {
  // Pins
  pinMode(TRIG_ENTRY, OUTPUT);
  pinMode(ECHO_ENTRY, INPUT);
  pinMode(TRIG_EXIT, OUTPUT);
  pinMode(ECHO_EXIT, INPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(FAN_PWM_PIN, OUTPUT);

  // Ensure known trig low
  digitalWrite(TRIG_ENTRY, LOW);
  digitalWrite(TRIG_EXIT, LOW);

  // Serial
  Serial.begin(SERIAL_BAUD);

  // DHT init
  dht.begin();

  // TM1637 initial command is set to 0
  tm1637.setBrightness(7);
  tm1637.showNumberDec(0, false, 4);

  // Initialize model state already default per bundle:
  peopleCount = 0;
  previousEntryPressed = false;
  displayTemperature = false;

  // Initialize inputs to contract initial values already static-inited to 0/false.
  io_0 = 0.0f; io_1 = false;
  io_2 = 0.0f; io_3 = false;
  io_4 = 0.0f; io_5 = 0.0f; io_6 = false;
  io_9 = false;

  // Run one tick and apply once
  updateButtonIO();
  modelTick();
  applyOutputs();
}

void loop() {
  static uint32_t nextTickMs = millis();
  uint32_t nowMs = millis();
  if ((int32_t)(nowMs - nextTickMs) < 0) return;

  // Schedule next control period
  nextTickMs += CONTROL_PERIOD_MS;

  // Update IO inputs
  updateButtonIO();
  updateUltrasonicIO();
  updateDhtIO();

  // Tick model and apply outputs
  modelTick();
  applyOutputs();
}

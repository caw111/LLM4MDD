#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ===================== Frozen interface contract pins =====================
static const uint8_t PIN_BTN_D2   = 2;   // io_0
static const uint8_t PIN_LED_G_D10 = 10; // io_2
static const uint8_t PIN_LED_R_D13 = 13; // io_1
static const uint8_t PIN_BUZZER_D6  = 6;  // io_3 tone output

static const uint8_t TRIG_ENT = 8;  // entrance Trig D8
static const uint8_t ECHO_ENT = 9;  // entrance Echo D9

static const uint8_t TRIG_PARK = 11; // parking Trig D11
static const uint8_t ECHO_PARK = 12; // parking Echo D12

static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

// ===================== Frozen interface contract control period =====================
static const uint32_t CONTROL_PERIOD_MS = 10; // control_period_ms = 10

// ===================== Ultrasonic contract =====================
// Ultrasonic timeout_us chosen as 30000 within 25000..60000 range.
// Actual sample_period_ms must be rounded up to a control-period multiple and >= ceil((timeout_us+12)/1000) and >=60ms.
// ceil((30000+12)/1000) = ceil(30.012) = 31 -> rounded up to multiple >=60ms => 60ms.
static const uint32_t US_TIMEOUT_US = 30000;
static const uint32_t US_SAMPLE_PERIOD_MS = 60; // must be >=60ms

// invalid_policy is hold_last_valid for both distance and validity.
struct Ultrasonic {
  uint8_t trigPin;
  uint8_t echoPin;

  uint32_t timeoutUs;
  uint32_t samplePeriodMs;

  float lastDistanceCm;
  bool lastValid;

  uint32_t lastSampleMs;

  Ultrasonic(uint8_t t, uint8_t e, uint32_t timeout, uint32_t period)
      : trigPin(t), echoPin(e), timeoutUs(timeout), samplePeriodMs(period),
        lastDistanceCm(0.0f), lastValid(false), lastSampleMs(0) {}
};

static Ultrasonic usEntrance(TRIG_ENT, ECHO_ENT, US_TIMEOUT_US, US_SAMPLE_PERIOD_MS);
static Ultrasonic usParking(TRIG_PARK, ECHO_PARK, US_TIMEOUT_US, US_SAMPLE_PERIOD_MS);

// io_4/io_5: Entrance distance + validity
static float io_4 = 0.0f; // Entrance module distance (cm)
static bool  io_5 = false; // Entrance validity

// io_6/io_7: Parking distance + validity
static float io_6 = 0.0f; // Parking position module distance (cm)
static bool  io_7 = false; // Parking validity

// io_0: Button D2 (Mode toggle)
static bool io_0 = false;

// ===================== Controller state (exact names) =====================
static int32_t mode = 0;                  // mode
static int32_t occupancyCount = 0;       // occupancyCount

static bool prevParkingValid = false;    // prevParkingValid
static bool prevEntranceApproach = false; // prevEntranceApproach (present but only affects prev state)
static bool prevReverseAlarmCond = false; // prevReverseAlarmCond (present but unused in outputs; still must update)
static bool lastAlarmOn = false;         // lastAlarmOn (present but unused in outputs; still must update)
static bool prevPressed = false;         // prevPressed

// ===================== TM1637 display =====================
static TM1637Display display(TM1637_CLK, TM1637_DIO);

// Brightness fixed to 7 by contract
// Leading zero false
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setupDisplay();
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeTM1637Number(int32_t value);
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t floorNonNeg(float x);
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void pollUltrasonic(Ultrasonic &u, float &distanceOut, bool &validOut);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBuzzerFrequency(int32_t freqHz);
#line 150 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void pollButton();
#line 159 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick();
#line 238 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 263 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setupDisplay() {
  display.setBrightness(7);
}

// TM1637 "invalid_policy blank" and "shows distance/occupancy per button mode".
// We'll write "io_8" each control step if any (mode selection always yields some non-invalid).
static void writeTM1637Number(int32_t value) {
  // TM1637Display::showNumberDec prints with leading zeros optionally; use false.
  // Limit to 0..9999; values beyond will be truncated by library; keep occupancyCount saturation.
  if (value < 0) value = 0;
  if (value > 9999) value = 9999;
  display.showNumberDec(value, false, 4, 0);
}

// ===================== Utility: floor for non-negative floats =====================
static int32_t floorNonNeg(float x) {
  // For x >= 0, floor is int32 trunc toward zero.
  if (x <= 0.0f) return 0;
  return (int32_t)x; // trunc toward zero == floor for non-negative.
}

// ===================== Ultrasonic acquisition (with hold_last_valid) =====================
static void pollUltrasonic(Ultrasonic &u, float &distanceOut, bool &validOut) {
  uint32_t now = millis();
  if ((now - u.lastSampleMs) < u.samplePeriodMs) {
    // hold_last_valid
    distanceOut = u.lastDistanceCm;
    validOut = u.lastValid;
    return;
  }
  u.lastSampleMs = now;

  // Trigger
  digitalWrite(u.trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(u.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(u.trigPin, LOW);

  // Measure echo pulse width
  // Use timeoutUs to bound wait. pulseIn returns microseconds, 0 if timeout.
  unsigned long durationUs = pulseIn(u.echoPin, HIGH, u.timeoutUs);

  if (durationUs == 0) {
    // invalid, hold last
    distanceOut = u.lastDistanceCm;
    validOut = u.lastValid;
    return;
  }

  // HC-SR04: distance = (duration / 2) * speed_of_sound
  // speed_of_sound ~ 343 m/s = 0.0343 cm/us
  float distanceCm = (durationUs * 0.0343f) / 2.0f;

  // Define validity: contract provides io_5/io_7 as validity; not further specified.
  // We'll treat any non-timeout measurement as valid.
  u.lastDistanceCm = distanceCm;
  u.lastValid = true;

  distanceOut = u.lastDistanceCm;
  validOut = u.lastValid;
}

// ===================== Tone output (io_3) =====================
// Arduino UNO: tone() uses frequency in Hz; to "silence" when io_3==0 use noTone().
static void setBuzzerFrequency(int32_t freqHz) {
  if (freqHz <= 0) {
    noTone(PIN_BUZZER_D6);
  } else {
    tone(PIN_BUZZER_D6, (unsigned int)freqHz);
  }
}

// ===================== Inputs update (io_0) =====================
static void pollButton() {
  // Contract doesn't specify pullup; catalog digital_input default used.
  // We'll use internal pullup disabled by default. However UNO requires explicit pull resistor; leave as-is.
  // To avoid floating behavior, we set pullup OFF and rely on external wiring.
  // "Button wiring polarity... not specified" thus use direct digitalRead.
  io_0 = (digitalRead(PIN_BTN_D2) == HIGH);
}

// ===================== Controller tick implementing ACCEPTED model exactly =====================
static void controllerTick() {
  // Updates (in exact order as model)
  // 1) if ((io_0) and (not prevPressed)) mode := (mode==2 ? 0 : mode+1)
  if (io_0 && !prevPressed) {
    mode = (mode == 2) ? 0 : (mode + 1);
  }

  // 2) prevPressed := io_0
  prevPressed = io_0;

  // 3) if (io_7 and not prevParkingValid) occupancyCount := (occupancyCount<2147483647 ? +1 : saturate)
  if (io_7 && !prevParkingValid) {
    if (occupancyCount < 2147483647) {
      occupancyCount = occupancyCount + 1;
    } else {
      occupancyCount = 2147483647;
    }
  }

  // 4) prevParkingValid := io_7
  prevParkingValid = io_7;

  // 5) if (io_5 and not prevEntranceApproach) prevEntranceApproach := true else prevEntranceApproach := io_5
  if (io_5 && !prevEntranceApproach) {
    prevEntranceApproach = true;
  } else {
    prevEntranceApproach = io_5;
  }

  // 6) prevReverseAlarmCond := (io_7 and (io_6 < 5.0))
  prevReverseAlarmCond = (io_7 && (io_6 < 5.0f));

  // 7) lastAlarmOn := (io_7 and (io_6 < 5.0))
  lastAlarmOn = (io_7 && (io_6 < 5.0f));

  // Outputs (as per model)
  // io_1 = (((not io_7) or (io_6 >= 5.0))) and io_5
  bool io_1 = ((!io_7) || (io_6 >= 5.0f)) && io_5;

  // io_2 = io_7 and (io_6 >= 5.0)
  bool io_2 = io_7 && (io_6 >= 5.0f);

  // io_3
  int32_t io_3;
  if (io_7 && (io_6 < 5.0f)) {
    io_3 = 2000;
  } else if (io_7 && (io_6 >= 2.0f) && (io_6 < 10.0f)) {
    io_3 = 1200;
  } else if (io_7 && (io_6 >= 10.0f) && (io_6 < 30.0f)) {
    io_3 = 800;
  } else if (io_7 && (io_6 >= 30.0f)) {
    io_3 = 400;
  } else {
    io_3 = 0;
  }

  // io_8
  int32_t io_8;
  if ((mode == 0) && io_5) {
    if (io_4 >= 0.0f) io_8 = floorNonNeg(io_4);
    else io_8 = 0;
  } else if ((mode == 1) && io_7) {
    if (io_6 >= 0.0f) io_8 = floorNonNeg(io_6);
    else io_8 = 0;
  } else if (mode == 2) {
    io_8 = occupancyCount;
  } else {
    io_8 = 0;
  }

  // Apply outputs
  digitalWrite(PIN_LED_R_D13, io_1 ? HIGH : LOW);
  digitalWrite(PIN_LED_G_D10, io_2 ? HIGH : LOW);
  setBuzzerFrequency(io_3);

  writeTM1637Number(io_8);
}

// ===================== Arduino setup/loop =====================
void setup() {
  pinMode(PIN_BTN_D2, INPUT); // pullup disabled by default per contract's legal default.

  pinMode(PIN_LED_G_D10, OUTPUT);
  pinMode(PIN_LED_R_D13, OUTPUT);
  pinMode(PIN_BUZZER_D6, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG_ENT, OUTPUT);
  pinMode(ECHO_ENT, INPUT);
  digitalWrite(TRIG_ENT, LOW);

  pinMode(TRIG_PARK, OUTPUT);
  pinMode(ECHO_PARK, INPUT);
  digitalWrite(TRIG_PARK, LOW);

  setupDisplay();

  // Initialize outputs to contract initial values (io_1/io_2 false, io_3 0, io_8 initial 0)
  digitalWrite(PIN_LED_R_D13, LOW);
  digitalWrite(PIN_LED_G_D10, LOW);
  noTone(PIN_BUZZER_D6);
  display.showNumberDec(0, false, 4, 0);
}

void loop() {
  static uint32_t nextTick = 0;
  uint32_t now = millis();

  if (now < nextTick) {
    // busy wait minimal; ensure 10ms tick.
    return;
  }

  // schedule next tick
  nextTick = now + CONTROL_PERIOD_MS;

  // Poll inputs each control tick; ultrasonic at its own sample period with hold_last_valid.
  pollButton();
  pollUltrasonic(usEntrance, io_4, io_5);
  pollUltrasonic(usParking, io_6, io_7);

  // One control step of controller
  controllerTick();
}

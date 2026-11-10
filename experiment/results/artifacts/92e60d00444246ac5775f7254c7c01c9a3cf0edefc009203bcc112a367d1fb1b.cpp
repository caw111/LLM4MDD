#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Pin bindings (authoritative) --------------------
static constexpr uint8_t PIN_RECYCLABLE_SENSOR = 2; // D2
static constexpr uint8_t PIN_OTHER_SENSOR      = 3; // D3

static constexpr uint8_t PIN_TRIG  = 8; // D8
static constexpr uint8_t PIN_ECHO  = 9; // D9

static constexpr uint8_t PIN_TM1637_CLK = 4; // D4
static constexpr uint8_t PIN_TM1637_DIO = 5; // D5

static constexpr uint8_t PIN_BUZZER = 6; // D6
static constexpr uint8_t PIN_LED    = 13; // D13

static constexpr uint8_t PIN_RESET_BUTTON = 7; // D7

// -------------------- Timing / control period --------------------
static constexpr uint32_t CONTROL_PERIOD_MS = 10; // Frozen contract
static uint32_t lastControlTickMs = 0;

// Ultrasonic sample period requirement: choose >= 60ms and >= ceil((timeout_us+12)/1000).
// timeout_us = 30000us => ceil((30000+12)/1000)=31ms; contract also requires >=60ms.
static constexpr uint32_t ULTRASONIC_SAMPLE_PERIOD_MS = 60;

// timeout_us for HC-SR04
static constexpr unsigned long ULTRASONIC_TIMEOUT_US = 30000UL;

// -------------------- Controller state (must be preserved exactly) --------------------
static int32_t recyclableCount = 0;
static int32_t otherCount = 0;
static int32_t totalCount = 0;

static bool fullBinAlarmed = false;
static bool previousResetPressed = false;

static double fillHeightCmLatched = 0.0;

// -------------------- Sensor IO latched values --------------------
static bool io_0 = false;      // near sensor recyclable (D2), active-high w/ pullup
static bool io_1 = false;      // near sensor other (D3), active-high w/ pullup

static double io_2 = 0.0;     // ultrasonic height cm (latest computed)
static bool io_3 = false;     // ultrasonic validity flag (true when measurement succeeded)

static bool io_7 = false;     // reset button

// -------------------- TM1637 --------------------
static TM1637Display* tmDisplayPtr = nullptr;

// tm1637 uses two-byte constructor CLK/DIO.
// Brightness: 2, leading_zero: false, invalid_policy blank (we simply set digits).
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tm1637Init();
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tm1637ShowTotal(int32_t val);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateUltrasonic();
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReport(bool trigger, int32_t stateCode, double fillCm);
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void buzzerUpdate();
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void ledUpdate();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controlTick();
#line 211 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 255 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_024\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tm1637Init() {
  // TM1637Display default uses segment mode; brightness set after begin.
  TM1637Display* disp = new TM1637Display(PIN_TM1637_CLK, PIN_TM1637_DIO);
  disp->setBrightness(2);
  tmDisplayPtr = disp;

  // Initialize to 0
  disp->showNumberDec(0, false, 4, 0); // leadingZero=false, 4 digits, no position; overload varies by library
  // We'll ensure stable by using basic call for common library:
  // (If the overload above isn't available in the chosen TM1637 library,
  // the compilation will fail, so we avoid that by using showNumberDec below.)
}

// Compatible wrapper: show int on TM1637
static void tm1637ShowTotal(int32_t val) {
  // Leading zeros disabled by passing leadingZero = false.
  // Use 4 digits; if val exceeds, library will still show lower digits.
  if (!tmDisplayPtr) return;
  tmDisplayPtr->showNumberDec(val, false, 4, 0);
}

// -------------------- Ultrasonic measurement --------------------
static void updateUltrasonic() {
  // model: io_3 valid flag; invalid policy hold_last_valid
  // We'll keep last io_2 and only update io_3 appropriately.
  // Measurement: HC-SR04 trig/echo.
  // invalid_policy "hold_last_valid": if invalid, do not change io_2.
  // io_2 initial 0.0; we implement that.

  // Trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long durationUs = pulseIn(PIN_ECHO, HIGH, ULTRASONIC_TIMEOUT_US);
  if (durationUs == 0) {
    // invalid: hold last valid height
    io_3 = false;
    // io_2 unchanged
    return;
  }

  // Speed of sound: 343 m/s => 0.0343 cm/us
  // distance cm = duration_us * 0.0343 / 2
  double cm = (double)durationUs * 0.0343 / 2.0;

  // accept value and mark valid
  io_2 = cm;
  io_3 = true;
}

// -------------------- Serial reporting contract --------------------
// Contract: serial_0_trigger = fullBinAlarmed
// serial_0_value_0 = (fullBinAlarmed ? 1 : 0)
// serial_0_value_1 = fillHeightCmLatched
// "line_or_idle" with idle_timeout_ms=20; we output on each control step when fullBinAlarmed changes
// and also when trigger is true, matching 'triggered' semantics. Since exact text mapping was not fixed,
// we emit a compact machine-friendly line:
//   "empty_bin_needed <state> <fill_height_cm>\n"
static void serialReport(bool trigger, int32_t stateCode, double fillCm) {
  if (!trigger) return;
  // We don't spam on every cycle if trigger remains true, but contract only says trigger is fullBinAlarmed.
  // However, outputs are evaluated every control period; the simplest faithful implementation is to
  // send every step while trigger is true.
  Serial.print(F("empty_bin_needed "));
  Serial.print(stateCode);
  Serial.print(' ');
  Serial.print(fillCm, 2);
  Serial.println();
}

// -------------------- Buzzer tone output --------------------
// io_5: if fullBinAlarmed then 1000 else 0 (Hz). "tone_output invalid_policy silence"
static void buzzerUpdate() {
  if (fullBinAlarmed) {
    tone(PIN_BUZZER, 1000);
  } else {
    noTone(PIN_BUZZER);
  }
}

// -------------------- LED output --------------------
// io_6: fullBinAlarmed
static void ledUpdate() {
  digitalWrite(PIN_LED, fullBinAlarmed ? HIGH : LOW);
}

// -------------------- Reset button edge handling per model --------------------
// Model updates in fixed order each tick:
// - io_3 latch and logic
// - fullBinAlarmed set when fillHeightCmLatched >= 20 and not already alarmed
// - counting based on io_0/io_1 when not alarmed
// - totalCount assigned
// - then reset condition uses io_7 AND NOT previousResetPressed
// - then previousResetPressed := io_7
static void controlTick() {
  // --- input sampling (digital) happens before model evaluation in each tick ---
  io_0 = digitalRead(PIN_RECYCLABLE_SENSOR) == HIGH;
  io_1 = digitalRead(PIN_OTHER_SENSOR) == HIGH;
  io_7 = digitalRead(PIN_RESET_BUTTON) == HIGH;

  // --- model updates ---

  // update fillHeightCmLatched from io_3 (ultrasonic valid)
  if (io_3) {
    fillHeightCmLatched = io_2;
  }

  // if (fillHeightCmLatched >= 20.0) and not fullBinAlarmed => fullBinAlarmed := true
  if ((fillHeightCmLatched >= 20.0) && (!fullBinAlarmed)) {
    fullBinAlarmed = true;
  }

  // if (not fullBinAlarmed) and io_0 => recyclableCount++
  if ((!fullBinAlarmed) && io_0) {
    recyclableCount = recyclableCount + 1;
  }

  // if (not fullBinAlarmed) and io_1 => otherCount++
  if ((!fullBinAlarmed) && io_1) {
    otherCount = otherCount + 1;
  }

  // totalCount := recyclableCount + otherCount
  totalCount = recyclableCount + otherCount;

  // reset on rising edge of io_7: (io_7) and (not previousResetPressed)
  if (io_7 && (!previousResetPressed)) {
    recyclableCount = 0;
    otherCount = 0;
    totalCount = 0;
    fullBinAlarmed = false;
  }

  // previousResetPressed := io_7
  previousResetPressed = io_7;

  // --- outputs ---
  // io_4 totalCount
  tm1637ShowTotal(totalCount);

  // io_5 buzzer frequency
  buzzerUpdate();

  // io_6 LED
  ledUpdate();

  // serial_0_trigger, serial values
  bool serial_0_trigger = fullBinAlarmed;
  int32_t serial_0_value_0 = fullBinAlarmed ? 1 : 0;
  double serial_0_value_1 = fillHeightCmLatched;
  serialReport(serial_0_trigger, serial_0_value_0, serial_0_value_1);
}

// -------------------- Arduino setup/loop --------------------
void setup() {
  // Serial
  Serial.begin(9600);

  // Inputs with pullups (active-high assumed by contract; pullup enabled means normally LOW,
  // HIGH when sensor drives line).
  pinMode(PIN_RECYCLABLE_SENSOR, INPUT_PULLUP);
  pinMode(PIN_OTHER_SENSOR, INPUT_PULLUP);
  pinMode(PIN_RESET_BUTTON, INPUT_PULLUP);

  // Ultrasonic
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_TRIG, LOW);

  // Outputs
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  // TM1637
  // Initialize TM1637 display. Library differences exist; allocate and set brightness.
  // If your TM1637 library doesn't support showNumberDec(val,false,digits,position),
  // compilation will fail; adjust only if needed for your library.
  tm1637Init();

  // Initial sensor validity: no immediate updateUltrasonic before first controlTick.
  io_2 = 0.0;
  io_3 = false;

  lastControlTickMs = millis();

  // Initial IO reads
  io_0 = digitalRead(PIN_RECYCLABLE_SENSOR) == HIGH;
  io_1 = digitalRead(PIN_OTHER_SENSOR) == HIGH;
  io_7 = digitalRead(PIN_RESET_BUTTON) == HIGH;
  previousResetPressed = io_7;

  // Initial outputs reflect initial state: totalCount=0, fullBinAlarmed=false.
  tm1637ShowTotal(0);
  buzzerUpdate();
  ledUpdate();
}

void loop() {
  uint32_t now = millis();

  // Ultrasonic sampling (period multiple of >= control period; choose 60ms)
  static uint32_t lastUltrasonicSampleMs = 0;
  if (now - lastUltrasonicSampleMs >= ULTRASONIC_SAMPLE_PERIOD_MS) {
    lastUltrasonicSampleMs = now;
    updateUltrasonic(); // updates io_2 and io_3 (invalid holds last io_2)
  }

  // Control tick every 10ms
  if (now - lastControlTickMs >= CONTROL_PERIOD_MS) {
    lastControlTickMs += CONTROL_PERIOD_MS; // keep phase stable
    controlTick();
  }
}
